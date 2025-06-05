#include "TCA9548A.hpp"

TCA9548A::TCA9548A(uint8_t sel_pin2, uint8_t sel_pin1, uint8_t sel_pin0) : _sel_pins{sel_pin0, sel_pin1, sel_pin2} 
{
    setSelect(0);
}

TCA9548A::~TCA9548A(){}

bool TCA9548A::setSelect(uint8_t newSelect)
{
    bool noError = true;

    for (int i = 0; i < 3; ++i)
    {
        uint8_t pin = _sel_pins[i];
        bool level = (newSelect >> i) & 0x01;
        int ret = gpio_write(pin, level); 
        if (ret < 0)
        {
            noError = false;
        }
    }

    if (noError) _select = newSelect;

    return noError;
}

uint8_t TCA9548A::getSelect()
{
    return _select;
}

