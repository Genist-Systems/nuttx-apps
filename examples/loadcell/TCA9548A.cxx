#include "TCA9548A.hpp"

TCA9548A::TCA9548A(const char* sel2, const char* sel1, const char* sel0)
{
    _sel_pins[0] = GPIO();
    _sel_pins[1] = GPIO();
    _sel_pins[2] = GPIO();

    
    _pinSetupStatus &= _sel_pins[0].setPinType(sel0, GPIO_OUTPUT_PIN);
    _pinSetupStatus &= _sel_pins[1].setPinType(sel1, GPIO_OUTPUT_PIN);
    _pinSetupStatus &= _sel_pins[2].setPinType(sel2, GPIO_OUTPUT_PIN);

    setSelect(0);
}

TCA9548A::~TCA9548A(){}

bool TCA9548A::setSelect(uint8_t newSelect)
{
    bool noError = true;

    if (!_pinSetupStatus)
    return false;

    for (int i = 0; i < 3; ++i)
    {
        bool level = (newSelect >> i) & 0x01;
        if (!_sel_pins[i].writePin(static_cast<PinStatus>(level)))
        {
            noError = false;
        }
    }

    if (noError)
        _select = newSelect;

    return noError;
}

uint8_t TCA9548A::getSelect()
{
    return _select;
}
