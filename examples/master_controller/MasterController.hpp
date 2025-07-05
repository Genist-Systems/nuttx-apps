#pragma once

#include "MQueue.hpp"
#include "master_controller_main.hpp"

#include <nuttx/clock.h>

class MasterController
{
    public:
        MasterController(const char* mqueue_drone_to_stretcher_name, 
                        const char* mqueue_stretcher_to_drone_name, 
                        const char* mqueue_drone_to_flight_controller_name, 
                        const char* mqueue_flight_controller_to_drone_name,
                        const char* mqueue_winchcontrol_name, 
                        const char* mqueue_loadcell_name, 
                        const char* mqueue_opticalencoder_name, 
                        struct MQueue_Settings& settings);

        ~MasterController();

        void run();
    private:
        MQueue<DroneToStretcherData> _mq_drone_to_stretcher;
        MQueue<StretcherToDroneData> _mq_stretcher_to_drone;
        MQueue<DroneToFlightControllerData> _mq_drone_to_flight_controller;
        MQueue<FlightControllerToDroneData> _mq_flight_controller_to_drone;
        MQueue<WinchControlData> _mq_winch;
        MQueue<LoadCellData> _mq_loadcell;
        MQueue<OpticalEncoderData> _mq_opticalencoder;
   

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

        bool _calculateWinchControlData(struct LoadCellData& loadcell_data, struct OpticalEncoderData& opticalencoder_data, struct StretcherToDroneData& std_data, struct FlightControllerToDroneData& fctd_data);
        bool _calculateDroneToStretcherData(struct LoadCellData& loadcell_data, struct OpticalEncoderData& opticalencoder_data, struct StretcherToDroneData& std_data, struct FlightControllerToDroneData& fctd_data);
        bool _calculateDroneToFlightControllerData(struct LoadCellData& loadcell_data, struct OpticalEncoderData& opticalencoder_data, struct StretcherToDroneData& std_data, struct FlightControllerToDroneData& fctd_data);

};