#include "MasterController.hpp"

MasterController::MasterController(const char* mqueue_drone_to_stretcher_name, 
                        const char* mqueue_stretcher_to_drone_name, 
                        const char* mqueue_drone_to_flight_controller_name, 
                        const char* mqueue_flight_controller_to_drone_name,
                        const char* mqueue_winchcontrol_name, 
                        const char* mqueue_loadcell_name, 
                        const char* mqueue_opticalencoder_name, 
                        const struct MQueue_Settings& settings) :
                        _mq_drone_to_stretcher(mqueue_drone_to_stretcher_name, settings), 
                        _mq_stretcher_to_drone(mqueue_stretcher_to_drone_name, settings),
                        _mq_drone_to_flight_controller(mqueue_drone_to_flight_controller_name, settings), 
                        _mq_flight_controller_to_drone(mqueue_flight_controller_to_drone_name, settings),
                        _mq_winch(mqueue_winchcontrol_name, settings), 
                        _mq_loadcell(mqueue_loadcell_name, settings), 
                        _mq_opticalencoder(mqueue_opticalencoder_name, settings)
                        {
                            
                        }

MasterController::~MasterController() = default;

void MasterController::run()
{
    struct LoadCellData loadcell_data{};
    struct OpticalEncoderData opticalencoder_data{};
    
    struct StretcherToDroneData std_data{};
    struct FlightControllerToDroneData fctd_data{};

    struct WinchControlData winch_data{};

    struct DroneToStretcherData dts_data{};
    struct DroneToFlightControllerData dtfc_data{};

    for (;;)
    {
        

        _receiveMostRecentData(_mq_loadcell, loadcell_data);
        _receiveMostRecentData(_mq_opticalencoder, opticalencoder_data);

        _receiveData(_mq_stretcher_to_drone, std_data);
        _receiveData(_mq_flight_controller_to_drone, fctd_data); 

        _calculateWinchControlData(loadcell_data, opticalencoder_data, std_data, fctd_data, winch_data);
        _calculateDroneToStretcherData(loadcell_data, opticalencoder_data, std_data, fctd_data, dts_data);
        _calculateDroneToFlightControllerData(loadcell_data, opticalencoder_data, std_data, fctd_data, dtfc_data);

        _sendData(_mq_winch, winch_data);
        
        _sendData(_mq_drone_to_stretcher, dts_data);
        _sendData(_mq_drone_to_flight_controller, dtfc_data);


    }
}

bool MasterController::_calculateWinchControlData(const struct LoadCellData& loadcell_data, 
                                                const struct OpticalEncoderData& opticalencoder_data, 
                                                const struct StretcherToDroneData& std_data, 
                                                const struct FlightControllerToDroneData& fctd_data, 
                                                struct WinchControlData& winch_data)
{
    return true;
}

bool MasterController::_calculateDroneToStretcherData(const struct LoadCellData& loadcell_data, 
                                                    const struct OpticalEncoderData& opticalencoder_data, 
                                                    const struct StretcherToDroneData& std_data, 
                                                    const struct FlightControllerToDroneData& fctd_data, 
                                                    struct DroneToStretcherData dts_data)
{
    return true;
}

bool MasterController::_calculateDroneToFlightControllerData(const struct LoadCellData& loadcell_data, 
                                                            const struct OpticalEncoderData& opticalencoder_data, 
                                                            const struct StretcherToDroneData& std_data, 
                                                            const struct FlightControllerToDroneData& fctd_data, 
                                                            struct DroneToFlightControllerData& dtfc_data)
{
    return true;
}

// bool MasterController::_sendDroneToStretcherData(struct DroneToStretcherData* data)
// {
//     return _mq_drone_to_stretcher.send(data) == MqResult::Success ? true : false;
// }

// bool MasterController::_recvStretcherToDroneData(struct StretcherToDroneData* data)
// {
//     return _mq_stretcher_to_drone.receive(data) == MqResult::Success ? true : false;
// }

// bool MasterController::_sendDroneToFlightControllerData(struct DroneToFlightControllerData* data)
// {
//     return _mq_drone_to_flight_controller.send(data) == MqResult::Success ? true : false;
// }

// bool MasterController::_recvFlightControllerToDroneData(struct FlightControllerToDroneData* data)
// {
//     return _mq_flight_controller_to_drone.receive(data) == MqResult::Success ? true : false;
// }

// bool MasterController::_sendWinchControlData(struct WinchControlData* data)
// {
//     return _mq_winch.send(data) == MqResult::Success ? true : false;
// }

// bool MasterController::_recvLoadCellData(struct LoadCellData* data)
// {
//     return _mq_loadcell.receiveMostRecent(data) == MqResult::Success ? true : false;
// }

// bool MasterController::_recvOpticalEncoderData(struct OpticalEncoderData* data)
// {
//     return _mq_opticalencoder.receiveMostRecent(data) == MqResult::Success ? true : false;
// }


