#include "OpticalEncoderController.hpp"

#define PI 3.14159265358979323846


OpticalEncoderController::OpticalEncoderController(const char* pinA_devName, const char* pinB_devName,  const int PPR, const int signo, const int initial_length, const int drum_radius) :
                                                    _pinA(), _pinB(), _pinA_devName(pinA_devName), _pinB_devName(pinB_devName), _ppr(PPR), _signo(signo), _length(initial_length), _drum_radius(drum_radius / 1000) {}

OpticalEncoderController::~OpticalEncoderController() = default;

OpticalEncoderController* OpticalEncoderController::_instance = nullptr;


bool OpticalEncoderController::init()
{
    OpticalEncoderController::_instance = this;

    
    if (_ppr <= 0 || _drum_radius <= 0)
        return false;
    
    if (!_pinA.setPinType(_pinA_devName, GPIO_INTERRUPT_PIN))
    {
        return false;
    }

    if (!_pinA.attachInterrupt(_signo, _staticISR))
    {
        return false;
    }
        
    if (!_pinB.setPinType(_pinB_devName, GPIO_INPUT_PIN))
    {
        return false;
    }

    resetTimer();
        
    return true;
}

void OpticalEncoderController::resetTimer()
{
    
    _prevTimeMicroSeconds = 0;
    _microSecondsBetweenPulses = 0;

}


void OpticalEncoderController::_staticISR()
{
    if (_instance) {
        _instance->_handleInterrupt();
    }
}

void OpticalEncoderController::_handleInterrupt()
{
    _interruptTriggered = true;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int time = ts.tv_sec * 1000000 + ts.tv_nsec / 1000;

    _microSecondsBetweenPulses = time - _prevTimeMicroSeconds;
    _prevTimeMicroSeconds = time;

    PinStatus value;
    if (_pinB.readPin(value))
    {
        if (static_cast<bool>(value))
            _length += 2.0 * PI *_drum_radius / (_ppr);
        else
            _length -= 2.0 * PI *_drum_radius / (_ppr);
    }

    // _length += 2.0 * PI *_drum_radius / (_ppr);
}


float OpticalEncoderController::getVelocity(VelocityUnits unit) const
{
    switch (unit)
    {
        case VelocityUnits::RADIANS_PER_SECOND: 
            if (_microSecondsBetweenPulses == 0) return 0.0;
            return (2.0 * PI / _ppr) * (1e6 / _microSecondsBetweenPulses);

        case VelocityUnits::DEGREES_PER_SECOND:
            if (_microSecondsBetweenPulses == 0) return 0.0;
            return (360.0 / _ppr) * (1e6 / _microSecondsBetweenPulses);

        case VelocityUnits::REVOLUTIONS_PER_MINUTE:
            if (_microSecondsBetweenPulses == 0) return 0.0;
                // return (60.0 / _ppr) / (1e6 * _microSecondsBetweenPulses);
                return (1e6 / _microSecondsBetweenPulses) * (60.0 / _ppr);

        case VelocityUnits::METERS_PER_SECOND:
            if (_microSecondsBetweenPulses == 0) return 0.0;
            return ((2.0 * PI / _ppr) * (1e6 / _microSecondsBetweenPulses)) * (_drum_radius);

        default:
            return 0.0;
    }

}

float OpticalEncoderController::getLength() const
{
    return _length;
}

void OpticalEncoderController::run()
{ 
    
    for (;;)
    {
        
        if (_interruptTriggered)
        {
            float rad_s = getVelocity(VelocityUnits::RADIANS_PER_SECOND);
            float deg_s = getVelocity(VelocityUnits::DEGREES_PER_SECOND);
            float rpm   = getVelocity(VelocityUnits::REVOLUTIONS_PER_MINUTE);
            float m_s   = getVelocity(VelocityUnits::METERS_PER_SECOND);
            float len   = getLength();

            printf("Angular Velocity:\n");
            printf("  Radians/s : %.3f\n", rad_s);
            printf("  Degrees/s : %.3f\n", deg_s);
            printf("  RPM       : %.3f\n", rpm);
            printf("Linear Velocity:\n");
            printf("  Meters/s  : %.3f\n", m_s);
            printf("Cable Length: %.3f m\n", len);

            printf("Microseconds since last interrupt: %d\r\n", _microSecondsBetweenPulses);


            _interruptTriggered = false;

        }

        
    }
}

