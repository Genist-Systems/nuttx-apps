#include "SlaveController.hpp"

SlaveController::SlaveController(const char* mqueue_drone_to_stretcher_name, 
                        const char* mqueue_stretcher_to_drone_name, 
                        const char* mqueue_imu_name, 
                        const char* mqueue_proximity_sensor_name, 
                        const char* mqueue_temperature_sensor_name, 
                        const struct MQueue_Settings& settings) :
                        _mq_drone_to_stretcher(mqueue_drone_to_stretcher_name, settings), 
                        _mq_stretcher_to_drone(mqueue_stretcher_to_drone_name, settings),
                        _mq_imu(mqueue_imu_name, settings), 
                        _mq_proximity(mqueue_proximity_sensor_name, settings), 
                        _mq_temperature(mqueue_temperature_sensor_name, settings)
                        {
                            
                        }

SlaveController::~SlaveController() = default;

void SlaveController::run()
{
    struct IMUData imu_data{};
    struct ProximitySensorData proximity_data{};
    struct TemperatureSensorData temperature_data{};

    struct DroneToStretcherData dts_data{};
    
    struct StretcherToDroneData std_data{};



    


    for (;;)
    {
        

        _receiveMostRecentData(_mq_imu, imu_data);
        _receiveMostRecentData(_mq_proximity, proximity_data);
        _receiveMostRecentData(_mq_temperature, temperature_data);

        _receiveData(_mq_drone_to_stretcher, dts_data);

        _calculateStretcherToDroneData(imu_data, proximity_data, temperature_data, dts_data, std_data);
        
        _sendData(_mq_stretcher_to_drone, std_data);



    }
}


bool SlaveController::_calculateStretcherToDroneData(   const struct IMUData& imu_data, 
                                                        const struct ProximitySensorData& proximity_data, 
                                                        const struct TemperatureSensorData& temperature_data, 
                                                        const struct DroneToStretcherData& dts_data, 
                                                        struct StretcherToDroneData std_data)
{
    return true;
}




