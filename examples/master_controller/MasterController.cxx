#include "MasterController.hpp"

MasterController::MasterController(const char* mqueue_wifi_name, 
                        const char* mqueue_spi_name, 
                        const char* mqueue_winchcontrol_name, 
                        const char* mqueue_loadcell_name, 
                        const char* mqueue_opticalencoder_name)
                        {
                            MQueue mq_wifi(mqueue_wifi_name, O_CREAT | O_RDWR | O_NONBLOCK, 0644, 8, sizeof(MyMessage));
                        }