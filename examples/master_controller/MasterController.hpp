#pragma once

#include "MQueue.hpp"

class MasterController
{
    public:
        MasterController(const char* mqueue_wifi_name, 
                        const char* mqueue_spi_name, 
                        const char* mqueue_winchcontrol_name, 
                        const char* mqueue_loadcell_name, 
                        const char* mqueue_opticalencoder_name);

        ~MasterController();

        void run();
    private:
        float _tetherLength = 0;
};