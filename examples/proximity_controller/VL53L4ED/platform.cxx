#include "VL53L4ED.hpp"
#include "VL53L4ED_api.hpp"
#include "esp32_peripherals/esp32_peripherals.hpp"

uint8_t VL53L4ED::VL53L4ED_WrByte(uint16_t index, uint8_t data)
{
    // Only supports 8-bit register addresses
    if (index > 0xFF) return VL53L4ED_ERROR_INVALID_ARGUMENT;
    return _i2c.writeRegister(static_cast<uint8_t>(index), data)
           ? VL53L4ED_ERROR_NONE
           : VL53L4ED_ERROR_TIMEOUT;
}

uint8_t VL53L4ED::VL53L4ED_WrWord(uint16_t index, uint16_t data)
{
    // Only supports 8-bit register addresses
    if (index > 0xFF) return VL53L4ED_ERROR_INVALID_ARGUMENT;
    return _i2c.writeRegister16(static_cast<uint8_t>(index), data)
           ? VL53L4ED_ERROR_NONE
           : VL53L4ED_ERROR_TIMEOUT;
}

uint8_t VL53L4ED::VL53L4ED_WrDWord(uint16_t index, uint32_t data)
{
    if (index > 0xFF) return VL53L4ED_ERROR_INVALID_ARGUMENT;

    uint8_t buffer[5];
    buffer[0] = static_cast<uint8_t>(index);
    buffer[1] = (data >> 0) & 0xFF; 
    buffer[2] = (data >> 8) & 0xFF;
    buffer[3] = (data >> 16) & 0xFF;
    buffer[4] = (data >> 24) & 0xFF; 


    return _i2c.write(buffer, 5)
           ? VL53L4ED_ERROR_NONE
           : VL53L4ED_ERROR_TIMEOUT;
}


uint8_t VL53L4ED::VL53L4ED_RdByte(uint16_t index, uint8_t *data)
{
    uint8_t reg[2] = { static_cast<uint8_t>(index >> 8), static_cast<uint8_t>(index & 0xFF) };
    return _i2c.readRegister(reg, data, 1) ? VL53L4ED_ERROR_NONE : VL53L4ED_ERROR_TIMEOUT;
}

uint8_t VL53L4ED::VL53L4ED_RdWord(uint16_t index, uint16_t *data)
{
    uint8_t reg[2] = { static_cast<uint8_t>(index >> 8), static_cast<uint8_t>(index & 0xFF) };
    uint8_t buffer[2];

    if (!_i2c.readRegister(reg, buffer, 2))
        return VL53L4ED_ERROR_TIMEOUT;

    *data = (static_cast<uint16_t>(buffer[0]) << 8) | buffer[1];
    return VL53L4ED_ERROR_NONE;
}

uint8_t VL53L4ED::VL53L4ED_RdDWord(uint16_t index, uint32_t *data)
{
    uint8_t reg[2] = { static_cast<uint8_t>(index >> 8), static_cast<uint8_t>(index & 0xFF) };
    uint8_t buffer[4];

    if (!_i2c.readRegister(reg, buffer, 4))
        return VL53L4ED_ERROR_TIMEOUT;

    *data = (static_cast<uint32_t>(buffer[0]) << 24) |
            (static_cast<uint32_t>(buffer[1]) << 16) |
            (static_cast<uint32_t>(buffer[2]) << 8)  |
            (static_cast<uint32_t>(buffer[3]));

    return VL53L4ED_ERROR_NONE;
}