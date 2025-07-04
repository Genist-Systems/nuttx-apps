#pragma once

#include "MQueue.hpp"
#include "esp32_peripherals/esp32_peripherals.hpp"
#include "tcp_controller_main.hpp"

using namespace ESP32::WiFi::TCP;

class TCPController
{
    public:
        TCPController(const char* mqueue_drone_to_stretcher_name, 
                        const char* mqueue_stretcher_to_drone_name,
                        struct MQueue_Settings& settings);

        ~TCPController();

        void run();

    private:
        MQueue _mq_drone_to_stretcher, 
        _mq_stretcher_to_drone;

        #ifdef CONFIG_EXAMPLES_MASTER_CONTROLLER
        TCPServer _server;
        #endif
        
        
        

};