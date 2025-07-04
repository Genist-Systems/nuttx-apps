#include "TCPController.hpp"

TCPController::TCPController(const char* mqueue_drone_to_stretcher_name, 
                        const char* mqueue_stretcher_to_drone_name, 
                        struct MQueue_Settings& settings, 
                        ) :
                        _mq_drone_to_stretcher(mqueue_drone_to_stretcher_name, settings), 
                        _mq_stretcher_to_drone(mqueue_stretcher_to_drone_name, settings)
                        {
                            
                        }

TCPController::~TCPController() = default;

void TCPController::run()
{
    
    struct StretcherToDroneData std_data{};


    struct DroneToStretcherData dts_data{};


    for (;;)
    {
        #ifdef CONFIG_EXAMPLES_MASTER_CONTROLLER
        
        _mq_stretcher_to_drone.send(std_data);
        #endif


    }
}




// bool TCPController::_sendDroneToStretcherData(struct DroneToStretcherData* data)
// {
//     return _mq_drone_to_stretcher.send(data) == MqResult::Success ? true : false;
// }

// bool TCPController::_recvStretcherToDroneData(struct StretcherToDroneData* data)
// {
//     return _mq_stretcher_to_drone.receive(data) == MqResult::Success ? true : false;
// }

// bool TCPController::_sendDroneToFlightControllerData(struct DroneToFlightControllerData* data)
// {
//     return _mq_drone_to_flight_controller.send(data) == MqResult::Success ? true : false;
// }

// bool TCPController::_recvFlightControllerToDroneData(struct FlightControllerToDroneData* data)
// {
//     return _mq_flight_controller_to_drone.receive(data) == MqResult::Success ? true : false;
// }

// bool TCPController::_sendWinchControlData(struct WinchControlData* data)
// {
//     return _mq_winch.send(data) == MqResult::Success ? true : false;
// }

// bool TCPController::_recvLoadCellData(struct LoadCellData* data)
// {
//     return _mq_loadcell.receiveMostRecent(data) == MqResult::Success ? true : false;
// }

// bool TCPController::_recvOpticalEncoderData(struct OpticalEncoderData* data)
// {
//     return _mq_opticalencoder.receiveMostRecent(data) == MqResult::Success ? true : false;
// }


