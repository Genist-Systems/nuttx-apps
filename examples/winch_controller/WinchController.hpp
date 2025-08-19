#pragma once

#include <stdint.h>

#include "MQueue.hpp"
#include "esp32_peripherals/esp32_peripherals.hpp"
#include "master_controller_main.hpp"

using namespace ESP32::PWM;

class WinchController
{
    public:
        WinchController(const char* mq_name, struct MQueue_Settings& mq_settings, const char* pwm_devName, struct pwm_info_s* pwm_config, uint8_t max_speed_percentage);
        ~WinchController();

        void run();
    private:

        MQueue<WinchControlData> _mq;
        PWM _pwm;
        const uint8_t _maxSpeedPercent = 100;

        uint8_t _normalizeDuty(uint8_t percentage);

        
        
};