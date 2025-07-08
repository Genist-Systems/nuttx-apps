#pragma once

#include "BMI323.hpp"
#include "MQueue.hpp"


class IMUController
{
    public:
        IMUController(const char* mq_imu_name, const struct MQueue_Settings& mq_settings, const char* i2c_devPath, struct i2c_config_s* config,
               const BMI323_Config& accelCfg, const BMI323_Config& gyroCfg) ;
        ~IMUController();

        bool init();
        void run();
    private:
        MQueue _mq<IMUData>;
        BMI323 _imu;
};