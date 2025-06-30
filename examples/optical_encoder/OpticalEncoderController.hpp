#pragma once

#include "esp32_peripherals/esp32_peripherals.hpp"
#include <stdint.h>

using namespace ESP32::GPIO;




class OpticalEncoderController
{
public:
    enum class VelocityUnits
    {
        RADIANS_PER_SECOND, 
        DEGREES_PER_SECOND, 
        REVOLUTIONS_PER_MINUTE, 
        METERS_PER_SECOND
    };

    OpticalEncoderController(const char* pinA_devName, const char* pinB_devName,  const int PPR, const int signo, const float initial_length, const float drum_radius);
    ~OpticalEncoderController();

    bool init();
    void resetTimer();

    float getLength() const;
    float getVelocity(VelocityUnits unit) const;
    
    void run();

private:
    static OpticalEncoderController* _instance;    
    static void _staticISR();             
    void _handleInterrupt();   
    
    

    GPIO _pinA, _pinB;
    const char* _pinA_devName;
    const char* _pinB_devName;

    volatile int _prevTimeMicroSeconds = 0;
    volatile int _microSecondsBetweenPulses = 0;

    const int _ppr;
    const int _signo;

    volatile bool _interruptTriggered = false;

    volatile float _length;
    const float _drum_radius;
};

