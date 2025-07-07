#include "BMI323.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstring>

#define BMI323_ADDRESS 0x69
#define ACC_CONF  0x20
#define GYR_CONF  0x21
#define CMD       0x7E

BMI323::BMI323(const char* devPath, struct i2c_config_s* config)
    : _devPath(devPath), _config(config)
{
    _i2c = I2C_Master();
    _i2c.setup(_devPath, _config);
}

bool BMI323::init()
{
    if (!_i2c.setup(_devPath, _config)) {
        printf("Failed to initialize I2C master\n");
        return false;
    }

    softReset();

    /*
   * Acc_Conf P.91
   * mode:        0x7000  -> High
   * average:     0x0000  -> No
   * filtering:   0x0080  -> ODR/4
   * range:       0x0000  -> 2G
   * ODR:         0x000B  -> 800Hz
   * Total:       0x708B
   */
    if (!writeRegister16(ACC_CONF, 0x708B)) return false;
    // if (!writeRegister16(ACC_CONF, 0x753D)) return false;

    /*
   * Gyr_Conf P.93
   * mode:        0x7000  -> High
   * average:     0x0000  -> No
   * filtering:   0x0080  -> ODR/4
   * range:       0x0000  -> 125kdps
   * ODR:         0x000B  -> 800Hz
   * Total:       0x708B
   */
    _gyro_range_setting = 0x0;  // Default to ±125 °/s
    uint16_t gyro_conf = (0x7000) | (0x0080) | (_gyro_range_setting << 4) | 0x0B;
    if (!writeRegister16(GYR_CONF, 0x708B)) return false;
    // if (!writeRegister16(GYR_CONF, 0x758D)) return false;

    return true;
}

void BMI323::softReset()
{
    writeRegister16(CMD, 0xDEAF);
    usleep(50000);
}

bool BMI323::writeRegister16(uint8_t reg, uint16_t value)
{
    uint8_t payload[3] = { reg, static_cast<uint8_t>(value & 0xFF), static_cast<uint8_t>((value >> 8) & 0xFF) };
    return _i2c.write(payload, sizeof(payload));
}

uint16_t BMI323::readRegister16(uint8_t reg)
{
    uint8_t buffer[4] = {};
    if (!_i2c.read(reg, buffer, sizeof(buffer)))
        return 0xFFFF;
    return (buffer[3]   | buffer[2] << 8);
}

bool BMI323::readAccel()
{
    uint8_t buffer[14] = {};
    if (!_i2c.read(0x03, buffer, sizeof(buffer)))
        return false;

    int offset = 2;
    _x = buffer[offset + 0] | (buffer[offset + 1] << 8);
    _y = buffer[offset + 2] | (buffer[offset + 3] << 8);
    _z = buffer[offset + 4] | (buffer[offset + 5] << 8);

    _accelX_m_s2 = lsbToM2S(_x);
    _accelY_m_s2 = lsbToM2S(_y);
    _accelZ_m_s2 = lsbToM2S(_z);

    return true;
}

bool BMI323::readGyro()
{
    uint8_t buffer[14] = {};
    if (!_i2c.read(0x03, buffer, sizeof(buffer)))
        return false;

    int offset = 2;
    _gx = buffer[offset + 6] | (buffer[offset + 7] << 8);
    _gy = buffer[offset + 8] | (buffer[offset + 9] << 8);
    _gz = buffer[offset + 10] | (buffer[offset + 11] << 8);

    _gyroX_dps = lsbToDPS(_gx);
    _gyroY_dps = lsbToDPS(_gy);
    _gyroZ_dps = lsbToDPS(_gz);

    return true;
}


float BMI323::lsbToM2S(int16_t rawData)
{
    constexpr float sensitivity = 2048.0;
    constexpr float gToM2S = 9.80665;
    return (rawData / sensitivity) * gToM2S;
}

float BMI323::lsbToDPS(int16_t rawData)
{
    float sensitivity;

    switch (_gyro_range_setting)  // _gyro_range_setting must be set according to GYR_CONF register (bits 7:4)
    {
        case 0x0: // ±125 °/s
            sensitivity = 262.144f;
            break;

        case 0x1: // ±250 °/s
            sensitivity = 131.072f;
            break;

        case 0x2: // ±500 °/s
            sensitivity = 65.536f;
            break;

        case 0x3: // ±1000 °/s
            sensitivity = 32.768f;
            break;

        case 0x4: // ±2000 °/s
            sensitivity = 16.4f;
            break;

        default:
            sensitivity = 262.144f;  // Fallback to safest low range
            break;
    }

    return static_cast<float>(rawData) / sensitivity;
}


