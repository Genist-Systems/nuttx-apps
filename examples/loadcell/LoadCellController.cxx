#include "LoadCellController.hpp"


LoadCellController::LoadCellController(struct NAU7802Config* load_cell_config, 
                                       const char* i2c_devName, 
                                       struct i2c_config_s* i2c_config,
                                       struct TCA9548APins* i2c_mux_pins, 
                                       uint16_t load_cell_rated_kg)
: _adcConfig(load_cell_config),
  _adc(i2c_devName, i2c_config),
  _mux(i2c_mux_pins->sel2, i2c_mux_pins->sel1, i2c_mux_pins->sel0),
  _numLoadCells(CONFIG_EXAMPLES_NUM_LOADCELLS),
  _loadCellRatedKg(load_cell_rated_kg)
{}


LoadCellController::~LoadCellController() {}


bool LoadCellController::init()
{
    bool noError = true;

    // One ADC per 2 load cells, so select each mux channel once
    uint8_t numAdcs = (_numLoadCells + 1) / 2;

    for (uint8_t i = 0; i < numAdcs; i++)
    {
        noError &= _mux.setSelect(i);
        noError &= _adc.begin();
        noError &= _adc.enable(_adcConfig->enable);
        noError &= _adc.setLDO(_adcConfig->ldoVoltage);
        noError &= _adc.setGain(_adcConfig->gain);
        noError &= _adc.setRate(_adcConfig->sampleRate);
        noError &= _adc.calibrate(_adcConfig->calibrationMode);
    }

    return noError;
}


void LoadCellController::run()
{
    for (;;)
    {
        for (uint8_t i = 0; i < _numLoadCells; i++)
        {
            uint8_t muxIndex = i / 2;           // One mux setting per 2 load cells
            uint8_t adcChannel = i % 2;         // Alternate between channel 0 and 1

            _mux.setSelect(muxIndex);          // Only changes every 2 i’s
            _adc.setChannel(adcChannel);

            if (_adc.available())
            {
                float forceN = static_cast<float>(_adc.read()) / MAX_24_BIT 
                            * _loadCellRatedKg * GRAVITY;

                _loadForces[i] = forceN; 
            }
        }
    }   

    
    
}
