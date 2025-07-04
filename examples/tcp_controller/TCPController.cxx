#include "TCPController.hpp"

#if !defined(CONFIG_EXAMPLES_MASTER_CONTROLLER) && !defined(CONFIG_EXAMPLES_SLAVE_CONTROLLER)
#error "You must define either CONFIG_EXAMPLES_MASTER_CONTROLLER or CONFIG_EXAMPLES_SLAVE_CONTROLLER"
#endif

#if defined(CONFIG_EXAMPLES_MASTER_CONTROLLER) && defined(CONFIG_EXAMPLES_SLAVE_CONTROLLER)
#error "Cannot define both CONFIG_EXAMPLES_MASTER_CONTROLLER and CONFIG_EXAMPLES_SLAVE_CONTROLLER"
#endif

TCPController::TCPController(const char* mqueue_drone_to_stretcher_name, 
                        const char* mqueue_stretcher_to_drone_name, 
                        struct MQueue_Settings& mq_settings, 
                        struct TCPSettings& tcp_settings) :
                        _mq_drone_to_stretcher(mqueue_drone_to_stretcher_name, mq_settings), 
                        _mq_stretcher_to_drone(mqueue_stretcher_to_drone_name, mq_settings)
                        #ifdef(CONFIG_EXAMPLES_MASTER_CONTROLLER)
                        , _server(tcp_settings)
                        #elif(CONFIG_EXAMPLES_SLAVE_CONTROLLER)
                        , _client(tcp_settings)
                        #endif

                        {
                            #ifdef(CONFIG_EXAMPLES_MASTER_CONTROLLER)
                                _server.acceptClient();
                            #elif(CONFIG_EXAMPLES_SLAVE_CONTROLLER)
                                _client.connectToServer()
                            #endif
                        }

TCPController::~TCPController() = default;

void TCPController::run()
{

    struct StretcherToDroneData std_data{};
    struct DroneToStretcherData dts_data{};

    for (;;)
    {
        #ifdef CONFIG_EXAMPLES_MASTER_CONTROLLER
            _mq_drone_to_stretcher.receive(dts_data);
            _server.receiveMessage(std_data);

            _mq_stretcher_to_drone.send(std_data);
            _server.sendMessage(dts_data);
        #elif defined(CONFIG_EXAMPLES_SLAVE_CONTROLLER)
            _mq_stretcher_to_drone.receive(std_data);
            _client.receiveMessage(dts_data);

            _mq_drone_to_stretcher.send(dts_data);
            _client.sendMessage(std_data);
        #endif
    }
}



