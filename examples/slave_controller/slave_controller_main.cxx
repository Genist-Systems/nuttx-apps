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
#include "SlaveController.hpp"
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



extern "C" int slave_controller_main(int argc, FAR char *argv[])
{
    const struct MQueue_Settings settings =
    {
        .flags = O_CREAT | O_RDWR | O_NONBLOCK,
        .mode = 0644,
        .maxMsg = CONFIG_PREALLOC_MQ_MSGS,
        .msgSize = CONFIG_MQ_MAXMSGSIZE
    };
    
    SlaveController controller = SlaveController(CONFIG_EXAMPLES_MASTER_AND_SLAVE_CONTROLLER_MQUEUE_DRONE_TO_STRETCHER_NAME, 
                                                    CONFIG_EXAMPLES_MASTER_AND_SLAVE_CONTROLLER_MQUEUE_STRETCHER_TO_DRONE_NAME, 
                                                    CONFIG_EXAMPLES_SLAVE_CONTROLLER_MQUEUE_IMU_NAME, 
                                                    CONFIG_EXAMPLES_SLAVE_CONTROLLER_MQUEUE_PROXIMITY_SENSOR_NAME, 
                                                    CONFIG_EXAMPLES_SLAVE_CONTROLLER_MQUEUE_TEMPERATURE_SENSOR_NAME,
                                                    settings);

    controller.run();

    return 0;
  
}
