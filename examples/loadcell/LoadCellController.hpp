#pragma once

#include "NAU7802.hpp"
#include "TCA9548A.hpp"
#include "loadcell_main.hpp"

#include <cstdint>

#define GRAVITY 9.81
#define MAX_24_BIT 16777216

struct NAU7802Config
{
    bool enable = true;
    NAU7802_LDOVoltage ldoVoltage = NAU7802_3V3;
    NAU7802_Gain gain = NAU7802_GAIN_128;
    NAU7802_SampleRate sampleRate = NAU7802_RATE_10SPS;
    NAU7802_Calibration calibrationMode = NAU7802_CALMOD_INTERNAL;
};

struct TCA9548APins
{
    const char* sel2;
    const char* sel1;
    const char* sel0;
};

class LoadCellController {
    public:
        LoadCellController(struct NAU7802Config* load_cell_config, 
                        const char* i2c_devName, 
                        struct i2c_config_s* i2c_config,
                        struct TCA9548APins* i2c_mux_pins, 
                        uint16_t load_cell_rated_kg);

        ~LoadCellController();

        bool init();
        void run();

    private:
        struct NAU7802Config* _adcConfig;

        NAU7802 _adc;
        TCA9548A _mux;

        const uint8_t _numLoadCells; 
        const uint16_t _loadCellRatedKg;

        float _loadForces[CONFIG_EXAMPLES_NUM_LOADCELLS];

};
