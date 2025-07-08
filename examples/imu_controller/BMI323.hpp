#pragma once

#include "esp32_peripherals/esp32_peripherals.hpp"

using namespace ESP32::I2C;

class BMI323 {
public:
    BMI323(const char* devPath, struct i2c_config_s* config,
               const BMI323_Config& accelCfg, const BMI323_Config& gyroCfg);

    bool init();
    void softReset();

    bool readAccelAndGyro();
private:
    
    uint16_t _getAccelConfigRegister();
    uint16_t _getGyroConfigRegister();
    bool _writeRegister16(uint8_t reg, uint16_t value);
    uint16_t _readRegister16(uint8_t reg);
    float _lsbToM2S(int16_t rawData);
    float _lsbToDPS(int16_t rawData);

    const char* _devPath;
    struct i2c_config_s* _config = nullptr;
    I2C_Master _i2c;

    BMI323_Config _accelCfg, _gyroCfg;

    int16_t _x, _y, _z;
    int16_t _gx, _gy, _gz;
    float _accelX_m_s2, _accelY_m_s2, _accelZ_m_s2 = 0;
    float _gyroX_dps, _gyroY_dps, _gyroZ_dps = 0;
    float _accelSensitivity, _gyroSensitivity = 0;
};

