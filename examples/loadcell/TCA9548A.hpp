#pragma once

#include <cstdint>
#include <array>

class TCA9548A
{
    public:
        TCA9548A(uint8_t sel2, uint8_t sel1, uint8_t sel0);
        ~TCA9548A();

        bool setSelect(uint8_t newSelect);
        uint8_t getSelect(void);
    
    private:
        std::array<uint8_t, 3> _sel_pins;
        uint8_t _select = 0;
};

