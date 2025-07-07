#pragma once

#include "esp32_peripherals/esp32_peripherals.hpp"

using namespace ESP32::I2C;

class BMI323 {
public:
    BMI323(const char* devPath, struct i2c_config_s* config);

    bool init();
    void softReset();

    bool readAccel();
    bool readGyro();

private:
    bool writeRegister16(uint8_t reg, uint16_t value);
    uint16_t readRegister16(uint8_t reg);
    float lsbToM2S(int16_t rawData);
    float lsbToDPS(int16_t rawData);

    const char* _devPath;
    struct i2c_config_s* _config;
    I2C_Master _i2c;

    uint32_t _gyro_range_setting = 0x0;


    int16_t _x, _y, _z;
    int16_t _gx, _gy, _gz;
    float _accelX_m_s2, _accelY_m_s2, _accelZ_m_s2;
    float _gyroX_dps, _gyroY_dps, _gyroZ_dps;
};

