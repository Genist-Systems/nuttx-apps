#include "WinchController.hpp"

WinchController::WinchController(const char* mq_name, struct MQueue_Settings& mq_settings, const char* pwm_devName, struct pwm_info_s* pwm_config, uint8_t max_speed_percentage) 
                                    : _mq(mq_name, mq_settings),
                                    _pwm(),
                                    _maxSpeedPercent(max_speed_percentage > 100 ? 100 : max_speed_percentage)
                                    
{
    if (!_pwm.setup(pwm_devName, pwm_config)) {
        printf("Winch Controller PWM error\n");
    }
}

WinchController::~WinchController() = default;

void WinchController::run()
{
    _pwm.editDuty(0);
    _pwm.start();
    
    for (;;)
    {
        WinchControl msg{};
        MqResult res = _mq.receiveMostRecent(msg);

        if (res == MqResult::Success) {
            if (msg.direction == WINCH_DIRECTION::DOWN)
            {
                _pwm.editDuty(_normalizeDuty(msg.new_duty));
                // _pwm2.editDuty(0);
            }
                
            else   
            {
                _pwm.editDuty(0);
                // _pwm2.editDuty(_normalizeDuty(msg.new_duty));
            } 

        }
    }
    
}

uint8_t WinchController::_normalizeDuty(uint8_t percentage)
{
    // Scale input percentage to be limited by _maxSpeedPercent
    return static_cast<uint8_t>((percentage * _maxSpeedPercent) / 100);
}

