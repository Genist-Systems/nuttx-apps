#pragma once

#include "MQueue.hpp"
#include "slave_controller_main.hpp"
#include "master_controller_main.hpp"


class SlaveController
{
    public:
        SlaveController(const char* mqueue_drone_to_stretcher_name, 
                        const char* mqueue_stretcher_to_drone_name, 
                        const char* mqueue_imu_name, 
                        const char* mqueue_proximity_sensor_name, 
                        const char* mqueue_temperature_sensor_name, 
                        const struct MQueue_Settings& settings);

        ~SlaveController();

        void run();
    private:
        MQueue<DroneToStretcherData> _mq_drone_to_stretcher;
        MQueue<StretcherToDroneData> _mq_stretcher_to_drone;
        MQueue<IMUData> _mq_imu;
        MQueue<ProximitySensorData> _mq_proximity;
        MQueue<TemperatureSensorData> _mq_temperature;
   

        template <typename T>
        bool _sendData(const MQueue<T>& mq, const T& data)
        {
            return mq.send(data) == MqResult::Success;
        }

        template <typename T>
        bool _receiveData(const MQueue<T>& mq, T& data)
        {
            return mq.receive(data) == MqResult::Success;
        }

        template <typename T>
        bool _receiveMostRecentData(const MQueue<T>& mq, T& data)
        {
            return mq.receiveMostRecent(data) == MqResult::Success;
        } 

        bool _calculateStretcherToDroneData(const struct IMUData& imu_data, 
                                            const struct ProximitySensorData& proximity_data, 
                                            const struct TemperatureSensorData& temperature_data, 
                                            const struct DroneToStretcherData& dts_data, 
                                            struct StretcherToDroneData std_data);

};