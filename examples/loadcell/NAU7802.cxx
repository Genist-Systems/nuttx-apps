#include "NAU7802.hpp"

NAU7802::NAU7802(const char* devPath, struct i2c_config_s* config)
    : _devPath(devPath), _config(config)
{
    _i2c = I2C_Master();
    _i2c.setup(_devPath, _config);
}

NAU7802::~NAU7802() {}


bool NAU7802::begin()
{
    if (!_i2c.setup(_devPath, _config))
    {
        perror("NAU7802: I2C setup failed");
        return false;
    }

    // Device ID check — ID register should return 0x0F in low nibble
    uint8_t id_val = 0;
    if (!_i2c.readRegister(NAU7802_REVISION_ID, &id_val, 1))
    {
        perror("NAU7802: Failed to read device ID register");
        return false;
    }

    if ((id_val & 0x0F) != 0x0F)
    {
        fprintf(stderr, "NAU7802: Unexpected device ID 0x%02X (expected 0x0F in low nibble)\n", id_val);
        return false;
    }

    if (!reset())
    {
        perror("NAU7802: Reset failed");
        return false;
    }

    if (!enable(true))
    {
        perror("NAU7802: Power-up failed");
        return false;
    }

    // Set LDO to 3.0V
    if (!setLDO(NAU7802_3V0))
    {
        perror("NAU7802: setLDO failed");
        return false;
    }

    if (!setGain(NAU7802_GAIN_128))
    {
        perror("NAU7802: setGain failed");
        return false;
    }

    if (!setRate(NAU7802_RATE_10SPS))
    {
        perror("NAU7802: setRate failed");
        return false;
    }

    // Disable ADC chopper clock — bits [5:4] of ADC register
    uint8_t adc_val;
    if (!_i2c.readRegister(NAU7802_ADC, &adc_val, 1))
    {
        perror("NAU7802: Failed to read ADC register");
        return false;
    }

    adc_val &= ~(0x3 << 4);
    adc_val |= (0x3 << 4);

    if (!_i2c.writeRegister(NAU7802_ADC, adc_val))
    {
        perror("NAU7802: Failed to write ADC register");
        return false;
    }

    // Use low ESR caps — clear bit 6 in PGA register
    uint8_t pga_val;
    if (!_i2c.readRegister(NAU7802_PGA, &pga_val, 1))
    {
        perror("NAU7802: Failed to read PGA register");
        return false;
    }

    pga_val &= ~(1 << 6);
    if (!_i2c.writeRegister(NAU7802_PGA, pga_val))
    {
        perror("NAU7802: Failed to write PGA register");
        return false;
    }

    // Enable PGA stabilizer cap — set bit 7 in POWER register
    uint8_t pwr_val;
    if (!_i2c.readRegister(NAU7802_POWER, &pwr_val, 1))
    {
        perror("NAU7802: Failed to read POWER register");
        return false;
    }

    pwr_val |= (1 << 7);
    if (!_i2c.writeRegister(NAU7802_POWER, pwr_val))
    {
        perror("NAU7802: Failed to write POWER register");
        return false;
    }

    return true;
}


bool NAU7802::enable(bool flag)
{
    uint8_t val;
    const uint8_t REG = NAU7802_PU_CTRL;

    // Read current PU_CTRL state
    if (!_i2c.readRegister(REG, &val, 1)) {
        perror("NAU7802: Failed to read PU_CTRL register");
        return false;
    }

    if (!flag) {
        // Shut down: clear PU_ANALOG (bit 2) and PU_DIGITAL (bit 1)
        val &= ~(1 << 2); // clear bit 2
        val &= ~(1 << 1); // clear bit 1

        if (!_i2c.writeRegister(REG, val)) {
            perror("NAU7802: Failed to power down");
            return false;
        }
        return true;
    }

    // Enable: set PU_DIGITAL and PU_ANALOG
    val |= (1 << 1); // set bit 1
    val |= (1 << 2); // set bit 2

    if (!_i2c.writeRegister(REG, val)) {
        perror("NAU7802: Failed to power up digital/analog");
        return false;
    }

    usleep(600 * 1000);  // wait for analog block to stabilize

    // Set PU_START (bit 4)
    if (!_i2c.readRegister(REG, &val, 1)) {
        perror("NAU7802: Failed to re-read PU_CTRL before setting START");
        return false;
    }

    val |= (1 << 4); // set PU_START
    if (!_i2c.writeRegister(REG, val) == false) {
        perror("NAU7802: Failed to set PU_START");
        return false;
    }

    // Wait until PU_READY (bit 3) is set
    if (!_i2c.readRegister(REG, &val, 1)) {
        perror("NAU7802: Failed to read PU_CTRL for PU_READY");
        return false;
    }

    return (val & (1 << 3)) != 0;
}



bool NAU7802::available()
{
    uint8_t val;
    if (!_i2c.readRegister(static_cast<uint8_t>(NAU7802_PU_CTRL), &val, 1))
    {
        perror("NAU7802: Failed to read PU_CTRL register in available()");
        return false;
    }

    // Bit 5 = CONV_READY
    return (val & (1 << 5)) != 0;
}


bool NAU7802::setChannel(uint8_t channel)
{
    if (channel > 1)
        channel = 1;

    uint8_t reg_val;
    if (!_i2c.readRegister(static_cast<uint8_t>(NAU7802_CTRL2), &reg_val, 1))
    {
        perror("NAU7802: Failed to read CTRL2 register");
        return false;
    }

    // Bit 7 = CHSEL (0 = Channel 1, 1 = Channel 2)
    reg_val &= ~(1 << 7);            // Clear bit 7
    reg_val |= (channel << 7);       // Set bit 7 if channel == 1

    if (!_i2c.writeRegister(static_cast<uint8_t>(NAU7802_CTRL2), reg_val))
    {
        perror("NAU7802: Failed to write CTRL2 register");
        return false;
    }

    return true;
}


int32_t NAU7802::read()
{
    uint8_t buffer[3] = {0};

    // Read 3 bytes from register 0x12 (NAU7802_ADCO_B2, MSB first)
    if (!_i2c.readRegister(static_cast<uint8_t>(NAU7802_ADCO_B2), buffer, 3))
    {
        perror("NAU7802: Failed to read ADC output registers");
        return 0;
    }

    // Assemble 24-bit value (MSB first)
    uint32_t val = ((uint32_t)buffer[0] << 16) |
                   ((uint32_t)buffer[1] << 8) |
                   buffer[2];

    // Sign-extend to 32 bits
    if (val & 0x800000)
    {
        val |= 0xFF000000;
    }

    return static_cast<int32_t>(val);
}


bool NAU7802::reset()
{
    const uint8_t REG = NAU7802_PU_CTRL;
    uint8_t val;

    // Set bit 0 (RR = 1) to request reset
    if (!_i2c.readRegister(REG, &val, 1)) {
        perror("NAU7802: Failed to read PU_CTRL for reset");
        return false;
    }

    val |= (1 << 0); // Set RR bit
    if (!_i2c.writeRegister(REG, val) == false) {
        perror("NAU7802: Failed to set RR bit");
        return false;
    }

    usleep(10 * 1000); // Reset pulse

    // Clear RR and set PU_DIGITAL (bit 1)
    val &= ~(1 << 0); // Clear RR
    val |= (1 << 1);  // Set PU_DIGITAL
    if (!_i2c.writeRegister(REG, val) == false) {
        perror("NAU7802: Failed to clear RR or set PU_DIGITAL");
        return false;
    }

    usleep(1 * 1000); // Wait 200 µs minimum — use 1 ms to be safe

    // Check if PU_READY (bit 3) is set
    if (!_i2c.readRegister(REG, &val, 1)) {
        perror("NAU7802: Failed to read PU_CTRL for PU_READY");
        return false;
    }

    return (val & (1 << 3)) != 0;
}


bool NAU7802::setLDO(NAU7802_LDOVoltage voltage)
{
    // First: Set or clear AVDDS bit (bit 7 of PU_CTRL)
    uint8_t pu_ctrl;
    if (!_i2c.readRegister(static_cast<uint8_t>(NAU7802_PU_CTRL), &pu_ctrl, 1)) {
        perror("NAU7802: Failed to read PU_CTRL register");
        return false;
    }

    if (voltage == NAU7802_EXTERNAL)
    {
        // Use external AVDD supply: clear bit 7
        pu_ctrl &= ~(1 << 7);
        if (!_i2c.writeRegister(NAU7802_PU_CTRL, pu_ctrl)) {
            perror("NAU7802: Failed to write AVDDS bit for external LDO");
            return false;
        }
        return true;
    }

    // Otherwise, use internal LDO: set bit 7
    pu_ctrl |= (1 << 7);
    if (!_i2c.writeRegister(NAU7802_PU_CTRL, pu_ctrl) == false) {
        perror("NAU7802: Failed to enable internal LDO");
        return false;
    }

    // Now write 3-bit LDO voltage value to bits [5:3] in CTRL1 (0x01)
    uint8_t ctrl1;
    if (!_i2c.readRegister(NAU7802_CTRL1, &ctrl1, 1)) {
        perror("NAU7802: Failed to read CTRL1 register");
        return false;
    }

    ctrl1 &= ~(0b111 << 3);             // Clear bits 5:3
    ctrl1 |= (voltage & 0b111) << 3;    // Set new voltage

    if (!_i2c.writeRegister(NAU7802_CTRL1, ctrl1)) {
        perror("NAU7802: Failed to write LDO voltage bits to CTRL1");
        return false;
    }

    return true;
}


NAU7802_LDOVoltage NAU7802::getLDO()
{
    uint8_t pu_ctrl;
    if (!_i2c.readRegister(static_cast<uint8_t>(NAU7802_PU_CTRL), &pu_ctrl, 1))
    {
        perror("NAU7802: Failed to read PU_CTRL register in getLDO()");
        return NAU7802_INVALID;
    }

    // Check bit 7: AVDDS
    bool using_internal_ldo = pu_ctrl & (1 << 7);
    if (!using_internal_ldo)
    {
        return NAU7802_EXTERNAL;
    }

    // Otherwise, read VLDO value from CTRL1 bits [5:3]
    uint8_t ctrl1;
    if (!_i2c.readRegister(NAU7802_CTRL1, &ctrl1, 1))
    {
        perror("NAU7802: Failed to read CTRL1 register in getLDO()");
        return NAU7802_INVALID;
    }

    uint8_t ldo_bits = (ctrl1 >> 3) & 0x07; // Extract bits 5:3
    return static_cast<NAU7802_LDOVoltage>(ldo_bits);
}


bool NAU7802::setGain(NAU7802_Gain gain)
{
    uint8_t ctrl1;

    // Read CTRL1 register (0x01)
    if (!_i2c.readRegister(NAU7802_CTRL1, &ctrl1, 1))
    {
        perror("NAU7802: Failed to read CTRL1 register in setGain()");
        return false;
    }

    // Clear bits [2:0] (gain select)
    ctrl1 &= ~0x07;

    // Set new gain value (3 bits)
    ctrl1 |= (gain & 0x07);

    // Write back
    if (!_i2c.writeRegister(NAU7802_CTRL1, ctrl1))
    {
        perror("NAU7802: Failed to write CTRL1 register in setGain()");
        return false;
    }

    return true;
}


NAU7802_Gain NAU7802::getGain()
{
    uint8_t ctrl1;

    // Read the CTRL1 register (0x01)
    if (!_i2c.readRegister(NAU7802_CTRL1, &ctrl1, 1))
    {
        perror("NAU7802: Failed to read CTRL1 register in getGain()");
        return NAU7802_GAIN_INVALID;  // You can define this value in your enum
    }

    // Extract bits [2:0] for gain setting
    uint8_t gain_bits = ctrl1 & 0x07;

    return static_cast<NAU7802_Gain>(gain_bits);
}


bool NAU7802::setRate(NAU7802_SampleRate rate)
{
    uint8_t ctrl2;

    // Read CTRL2 register (0x02)
    if (!_i2c.readRegister(NAU7802_CTRL2, &ctrl2, 1))
    {
        perror("NAU7802: Failed to read CTRL2 register in setRate()");
        return false;
    }

    // Clear bits [6:4] (3 bits for rate)
    ctrl2 &= ~(0b111 << 4);

    // Set new sample rate
    ctrl2 |= (static_cast<uint8_t>(rate) & 0b111) << 4;

    // Write back
    if (!_i2c.writeRegister(NAU7802_CTRL2, ctrl2))
    {
        perror("NAU7802: Failed to write CTRL2 register in setRate()");
        return false;
    }

    return true;
}


NAU7802_SampleRate NAU7802::getRate()
{
    uint8_t ctrl2;

    // Read the CTRL2 register (0x02)
    if (!_i2c.readRegister(NAU7802_CTRL2, &ctrl2, 1))
    {
        perror("NAU7802: Failed to read CTRL2 register in getRate()");
        return NAU7802_RATE_INVALID;  // You can define this in your enum
    }

    // Extract bits [6:4] (3-bit sample rate field)
    uint8_t rate_bits = (ctrl2 >> 4) & 0x07;

    return static_cast<NAU7802_SampleRate>(rate_bits);
}


bool NAU7802::calibrate(NAU7802_Calibration mode)
{
    uint8_t ctrl2;

    // Read CTRL2 register (0x02)
    if (!_i2c.readRegister(NAU7802_CTRL2, &ctrl2, 1))
    {
        perror("NAU7802: Failed to read CTRL2 register in calibrate()");
        return false;
    }

    // Set calibration mode bits [1:0]
    ctrl2 &= ~(0x03);                  // Clear bits 1:0
    ctrl2 |= (mode & 0x03);            // Set mode

    // Set CAL_START bit [2]
    ctrl2 |= (1 << 2);

    // Write modified CTRL2 back
    if (!_i2c.writeRegister(NAU7802_CTRL2, ctrl2))
    {
        perror("NAU7802: Failed to start calibration");
        return false;
    }

    // Wait for CAL_START to go low (bit 2 clears when done)
    int retries = 100;
    do
    {
        usleep(10 * 1000);
        if (!_i2c.readRegister(NAU7802_CTRL2, &ctrl2, 1))
        {
            perror("NAU7802: Failed to poll calibration status");
            return false;
        }
    } while ((ctrl2 & (1 << 2)) && --retries > 0);

    if (retries == 0)
    {
        fprintf(stderr, "NAU7802: Calibration timed out\n");
        return false;
    }

    // Check CAL_ERR bit [3]
    if (ctrl2 & (1 << 3))
    {
        fprintf(stderr, "NAU7802: Calibration error (CAL_ERR bit set)\n");
        return false;
    }

    return true;
}
