#include "LoadCellController.hpp"


LoadCellController::LoadCellController(struct NAU7802Config* load_cell_config, struct TCA9548AConfig* i2c_mux_config, uint8_t num_channels, uint16_t load_cell_rated_kg) 
: _adcConfig(load_cell_config),
  _muxConfig(i2c_mux_config), 
  _numChannels(num_channels), 
  _loadCellRatedKg(load_cell_rated_kg), 
  _adc(),
  _mux(_muxConfig->sel2, _muxConfig->sel1, _muxConfig->sel0), 
  {}

LoadCellController::~LoadCellController() {}

std::shared_ptr<std::vector<float>> LoadCellController::getLoadForces() const
{
    return _loadForces;
}

bool LoadCellController::begin()
{
    bool noError = true;
    for (uint8_t i = 0; i < _numChannels; i++)
    {
        
        noError &= _mux.setSelect(i);
        noError &= _adc.begin();
        noError &= _adc.enable(_adcConfig->enable);
        noError &= _adc.setChannel(_adcConfig->channel);
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
        for (uint8_t i = 0; i < _numChannels; i++)
        {
            _mux.setSelect(i);
            if (_adc.available())
            {
                float forceN = static_cast<float>(_adc.read() / MAX_24_BIT) * _loadCellRatedKg * GRAVITY;

                (*_loadForces)[i] = forceN; 
            }
        
        }
    }
    
    
}
