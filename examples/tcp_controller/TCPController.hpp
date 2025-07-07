#pragma once

#include "MQueue.hpp"
#include "esp32_peripherals/esp32_peripherals.hpp"
#include "tcp_controller_main.hpp"
#include "master_controller_main.hpp"



using namespace ESP32::WiFi::TCP;


class TCPController
{
    public:
        TCPController(const char* mqueue_drone_to_stretcher_name, 
                        const char* mqueue_stretcher_to_drone_name, 
                        struct MQueue_Settings& mq_settings, 
                        struct TCP_Settings& tcp_settings);

        ~TCPController();

        void run();

    private:
        MQueue<DroneToStretcherData> _mq_drone_to_stretcher;
        MQueue<StretcherToDroneData> _mq_stretcher_to_drone;


        #ifdef CONFIG_EXAMPLES_MASTERCONTROLLER
        TCPServer _server;
        #elif CONFIG_EXAMPLES_SLAVECONTROLLER
        TCPClient _client;
        #endif
        
        
        

};