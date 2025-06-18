#pragma once

#include "esp32_peripherals.hpp"

#include <cstdint>
#include <array>

using namespace ESP32::GPIO;

class TCA9548A
{
    public:
        TCA9548A(const char* sel2, const char* sel1, const char* sel0);
        ~TCA9548A();

        bool setSelect(uint8_t newSelect);
        uint8_t getSelect(void);
    
    private:
        std::array<GPIO, 3> _sel_pins;
        bool _pinSetupStatus = true;

        uint8_t _select = 0;
};

