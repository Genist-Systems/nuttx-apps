#include "BMI323.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstring>

#define BMI323_ADDRESS 0x69
#define ACC_CONF  0x20
#define GYR_CONF  0x21
#define CMD       0x7E
#define DEVICE_ID_REG 0x00
#define DEVICE_ID 0x32

BMI323::BMI323(const char* devPath, struct i2c_config_s* config,
               const BMI323_Config& accelCfg, const BMI323_Config& gyroCfg)
    : _devPath(devPath), _config(config), _accelCfg(accelCfg), _gyroCfg(gyroCfg)
{
    _i2c = I2C_Master();
}

bool BMI323::init()
{
    if (!_i2c.setup(_devPath, _config)) {
        printf("Failed to initialize I2C master\n");
        return false;
    }

    uint8_t buffer[2];
    if (!_i2c.readRegister(static_cast<uint8_t>(DEVICE_ID_REG), buffer, sizeof(buffer)))
    {
        printf("Failed to read BMI device ID\n");
        return false;
    }

    if (buffer[1] != DEVICE_ID)
    {
        printf("BMI incorrect device ID");
        return false;
    }

    softReset();


    if (!_i2c.writeRegister16(ACC_CONF, _getAccelConfigRegister())) return false;

    if (!_i2c.writeRegister16(GYR_CONF, _getGyroConfigRegister())) return false;

    return true;
}

uint16_t BMI323::_getAccelConfigRegister()
{
    uint16_t acc_conf = 0;
    
    if (_accelCfg.mode == 0x0 || _accelCfg.mode == 0x3 || _accelCfg.mode == 0x4 || _accelCfg.mode == 0x7)
        acc_conf |= _accelCfg.mode << 12;
    else 
        acc_conf |= 0x7 << 12;
    
    if (_accelCfg.average <= 0x6)
        acc_conf |= _accelCfg.average << 8;
    
    if (_accelCfg.bw == 0x1)
        acc_conf |= _accelCfg.bw << 7;

    if (_accelCfg.range <= 0x3)
        acc_conf |= _accelCfg.range << 4;

    switch (_accelCfg.range)
    {
        case 0x0: // ±2g
            _accelSensitivity = 16.38f;
            break;

        case 0x1: // ±4g
            _accelSensitivity = 8.19f;
            break;

        case 0x2: // ±8g
            _accelSensitivity = 4.1f;
            break;

        case 0x3: // ±16g
            _accelSensitivity = 2.05f;
            break;

        default:
            _accelSensitivity = 16.38f;  // Fallback to safest low range
            break;
    }
    
    if (_accelCfg.odr >= 0x1 && _accelCfg.odr <= 0xE)
        acc_conf |= _accelCfg.odr;
    else 
        acc_conf |= 0xB;

    return acc_conf;

    
}

uint16_t BMI323::_getGyroConfigRegister()
{
    uint16_t gyro_conf = 0;
    
    if (_gyroCfg.mode == 0x0 || _gyroCfg.mode == 0x3 || _gyroCfg.mode == 0x4 || _gyroCfg.mode == 0x7)
        gyro_conf |= _gyroCfg.mode << 12;
    else 
        gyro_conf |= 0x7 << 12;
    
    if (_gyroCfg.average <= 0x6)
        gyro_conf |= _gyroCfg.average << 8;
    
    if (_gyroCfg.bw == 0x1)
        gyro_conf |= _gyroCfg.bw << 7;

    if (_gyroCfg.range <= 0x4)
        gyro_conf |= _gyroCfg.range << 4;

    switch (_gyroCfg.range)
    {
        case 0x0: // ±125 °/s
            _gyroSensitivity = 262.144f;
            break;

        case 0x1: // ±250 °/s
            _gyroSensitivity = 131.072f;
            break;

        case 0x2: // ±500 °/s
            _gyroSensitivity = 65.536f;
            break;

        case 0x3: // ±1000 °/s
            _gyroSensitivity = 32.768f;
            break;

        case 0x4: // ±2000 °/s
            _gyroSensitivity = 16.4f;
            break;

        default:
            _gyroSensitivity = 262.144f;  // Fallback to safest low range
            break;
    }
    
    if (_gyroCfg.odr >= 0x1 && _gyroCfg.odr <= 0xE)
        gyro_conf |= _gyroCfg.odr;
    else 
        gyro_conf |= 0xB;

    return gyro_conf;
}


void BMI323::softReset()
{
    _i2c.writeRegister16(CMD, 0xDEAF);
    usleep(50000);
}

bool BMI323::readAccelAndGyro()
{
    uint8_t buffer[14] = {};
    if (!_i2c.readRegister(0x03, buffer, sizeof(buffer)))
        return false;

    _x = buffer[0] | (buffer[1] << 8);
    _y = buffer[2] | (buffer[3] << 8);
    _z = buffer[4] | (buffer[5] << 8);

    _gx = buffer[6] | (buffer[7] << 8);
    _gy = buffer[8] | (buffer[9] << 8);
    _gz = buffer[10] | (buffer[11] << 8);

    _accelX_m_s2 = _lsbToM2S(_x);
    _accelY_m_s2 = _lsbToM2S(_y);
    _accelZ_m_s2 = _lsbToM2S(_z);

    _gyroX_dps = _lsbToDPS(_gx);
    _gyroY_dps = _lsbToDPS(_gy);
    _gyroZ_dps = _lsbToDPS(_gz);

    return true;
}






float BMI323::_lsbToM2S(int16_t rawData)
{

    return (static_cast<float>(rawData) / _accelSensitivity) * 9.80665;
}

float BMI323::_lsbToDPS(int16_t rawData)
{   

    return static_cast<float>(rawData) / _gyroSensitivity;
}





