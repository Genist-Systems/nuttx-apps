#pragma once

#include "NAU7802.hpp"
#include "TCA9548A.hpp"

#include <cstdint>
#include <vector>
#include <memory>
#include <cmath>

#define GRAVITY 9.81
#define MAX_24_BIT 16777216

struct NAU7802Config
{
    // I2C needs to be added
    bool enable = true;
    uint8_t channel = 0;
    NAU7802_LDOVoltage ldoVoltage = NAU7802_3V3;
    NAU7802_Gain gain = NAU7802_GAIN_128;
    NAU7802_SampleRate sampleRate = NAU7802_RATE_80SPS;
    NAU7802_Calibration calibrationMode = NAU7802_CALMOD_INTERNAL;
};

struct TCA9548AConfig
{
    uint8_t sel2;
    uint8_t sel1;
    uint8_t sel0;
};

class LoadCellController
{
    public:
        LoadCellController(struct NAU7802Config* load_cell_config, struct TCA9548AConfig* i2c_mux_config, uint8_t num_channels, uint16_t load_cell_rated_kg);
        ~LoadCellController();

        bool begin();
        void run();
        std::shared_ptr<std::vector<float>> getLoadForces() const;
    private:
        TCA9548A _mux;
        NAU7802 _adc;

        struct NAU7802Config* _adcConfig;
        struct TCA9548AConfig* _muxConfig;
    
        uint8_t _numChannels;
        uint16_t _loadCellRatedKg;


        std::shared_ptr<std::vector<float>> _loadForces;
};