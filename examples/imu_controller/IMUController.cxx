#include "IMUController.hpp"

IMUController::IMUController(const char* mq_imu_name, const struct MQueue_Settings& mq_settings, const char* i2c_devPath, struct i2c_config_s* config,
               const BMI323_Config& accelCfg, const BMI323_Config& gyroCfg) : _mq(mq_imu_name, mq_settings), _imu(i2c_devPath, config, accelCfg, gyroCfg)
{}

IMUController::~IMUController() = default;

bool IMUController::init()
{
    if (!_imu.init())
    {
        return false;
    }        

}

void IMUController::run()
{
    for(;;)
    {

    }
}