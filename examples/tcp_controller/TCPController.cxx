#include "TCPController.hpp"

#if !defined(CONFIG_EXAMPLES_MASTERCONTROLLER) && !defined(CONFIG_EXAMPLES_SLAVECONTROLLER)
#error "You must define either CONFIG_EXAMPLES_MASTERCONTROLLER or CONFIG_EXAMPLES_SLAVECONTROLLER"
#endif

#if defined(CONFIG_EXAMPLES_MASTERCONTROLLER) && defined(CONFIG_EXAMPLES_SLAVECONTROLLER)
#error "Cannot define both CONFIG_EXAMPLES_MASTERCONTROLLER and CONFIG_EXAMPLES_SLAVECONTROLLER"
#endif

TCPController::TCPController(const char* mqueue_drone_to_stretcher_name, 
                              const char* mqueue_stretcher_to_drone_name, 
                              struct MQueue_Settings& mq_settings, 
                              struct TCP_Settings& tcp_settings)
#if defined(CONFIG_EXAMPLES_MASTERCONTROLLER)
    : _mq_drone_to_stretcher(mqueue_drone_to_stretcher_name, mq_settings), 
      _mq_stretcher_to_drone(mqueue_stretcher_to_drone_name, mq_settings),
      _server(tcp_settings) 
#elif defined(CONFIG_EXAMPLES_SLAVECONTROLLER)
    : _mq_drone_to_stretcher(mqueue_drone_to_stretcher_name, mq_settings), 
      _mq_stretcher_to_drone(mqueue_stretcher_to_drone_name, mq_settings),
      _client(tcp_settings)
#endif
{
#if defined(CONFIG_EXAMPLES_MASTERCONTROLLER)
    _server.acceptClient();
#elif defined(CONFIG_EXAMPLES_SLAVECONTROLLER)
    _client.connectToServer();
#endif
}



TCPController::~TCPController() = default;

void TCPController::run()
{
    struct StretcherToDroneData std_data{};
    struct DroneToStretcherData dts_data{};

    for (;;)
    {
        #ifdef CONFIG_EXAMPLES_MASTERCONTROLLER
            _mq_drone_to_stretcher.receive(dts_data);
            _server.receiveMessage(std_data);

            _mq_stretcher_to_drone.send(std_data);
            _server.sendMessage(dts_data);
        #elif defined(CONFIG_EXAMPLES_SLAVECONTROLLER)
            _mq_stretcher_to_drone.receive(std_data);
            _client.receiveMessage(dts_data);

            _mq_drone_to_stretcher.send(dts_data);
            _client.sendMessage(std_data);
        #endif
    }
}



