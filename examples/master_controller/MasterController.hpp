#pragma once

#include "MQueue.hpp"
#include "master_controller_main.hpp"

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
        MQueue _mq_drone_to_stretcher, 
        _mq_stretcher_to_drone, 
        _mq_drone_to_flight_controller, 
        _mq_flight_controller_to_drone, 
        _mq_winch, 
        _mq_loadcell, 
        _mq_opticalencoder;
        
        
        template <typename T>
        bool _sendData(const MQueue& mq, const T& data)
        {
            return mq.send(data) == MqResult::Success;
        }

        template <typename T>
        bool _receiveData(const MQueue& mq, T& data)
        {
            return mq.receive(data) == MqResult::Success;
        }

        template <typename T>
        bool _receiveMostRecentData(const MQueue& mq, T& data)
        {
            return mq.receiveMostRecent(data) == MqResult::Success;
        }   

        bool _calculateWinchControlData(struct LoadCellData& loadcell_data, struct OpticalEncoderData& opticalencoder_data, struct StretcherToDroneData& std_data, struct FlightControllerToDroneData& fctd_data);
        bool _calculateDroneToStretcherData(struct LoadCellData& loadcell_data, struct OpticalEncoderData& opticalencoder_data, struct StretcherToDroneData& std_data, struct FlightControllerToDroneData& fctd_data);
        bool _calculateDroneToFlightControllerData(struct LoadCellData& loadcell_data, struct OpticalEncoderData& opticalencoder_data, struct StretcherToDroneData& std_data, struct FlightControllerToDroneData& fctd_data);

};