/****************************************************************************
 * apps/examples/loadcell/loadcell_main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "MasterController.hpp"
#include "master_controller_main.hpp"

extern "C" {
    #include <nuttx/config.h>
}
/****************************************************************************
 * Defines
 ****************************************************************************/



/****************************************************************************
 * Public Functions
 ****************************************************************************/


/****************************************************************************
 * main
 ****************************************************************************/


extern "C" int master_controller_main(int argc, FAR char *argv[])
{
    
    const struct MQueue_Settings mq_settings =
    {
        .flags = O_CREAT | O_RDWR | O_NONBLOCK,
        .mode = 0644,
        .maxMsg = CONFIG_PREALLOC_MQ_MSGS,
        .msgSize = CONFIG_MQ_MAXMSGSIZE
    };
    
    MasterController controller = MasterController(CONFIG_EXAMPLES_MASTER_AND_SLAVE_CONTROLLER_MQUEUE_DRONE_TO_STRETCHER_NAME, 
                                                    CONFIG_EXAMPLES_MASTER_AND_SLAVE_CONTROLLER_MQUEUE_STRETCHER_TO_DRONE_NAME, 
                                                    CONFIG_EXAMPLES_MASTER_CONTROLLER_MQUEUE_DRONE_TO_FLIGHT_CONTROLLER_NAME, 
                                                    CONFIG_EXAMPLES_MASTER_CONTROLLER_MQUEUE_FLIGHT_CONTROLLER_TO_DRONE_NAME,
                                                    CONFIG_EXAMPLES_MASTER_CONTROLLER_MQUEUE_WINCHCONTROL_NAME, 
                                                    CONFIG_EXAMPLES_MASTER_CONTROLLER_MQUEUE_LOADCELL_NAME, 
                                                    CONFIG_EXAMPLES_MASTER_CONTROLLER_MQUEUE_OPTICALENCODER_NAME, 
                                                    mq_settings);

    controller.run();

    return 0;
  
}
