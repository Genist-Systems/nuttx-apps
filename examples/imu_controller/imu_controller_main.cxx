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
#include "MQueue.hpp"
#include "imu_controller_main.hpp"
#include "IMUController.hpp"
#include "slave_controller_main.hpp"

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



extern "C" int imu_controller_main(int argc, FAR char *argv[])
{
    const struct MQueue_Settings mq_settings =
    {
        .flags = O_CREAT | O_RDWR | O_NONBLOCK,
        .mode = 0644,
        .maxMsg = CONFIG_PREALLOC_MQ_MSGS,
        .msgSize = CONFIG_MQ_MAXMSGSIZE
    };

    struct i2c_config_s i2c_config =
    {
        .frequency = 400000,
        .address = 0x68,
        .addrlen = 7
    };

    const BMI323_Config accelCfg = 
    {
        .mode = 0x7,
        .average = 0x0, 
        .bw = 0x1,
        .range = 0x0,
        .odr = 0xB 
    };

    const BMI323_Config gyroCfg =
    {
        .mode = 0x7,
        .average = 0x0, 
        .bw = 0x1,
        .range = 0x0,
        .odr = 0xB 
    };
    
    IMUController controller = IMUController(CONFIG_EXAMPLES_SLAVE_CONTROLLER_MQUEUE_IMU_NAME, 
                                            mq_settings, 
                                            CONFIG_EXAMPLES_IMU_I2C_FD, 
                                            i2c_config,
                                            accelCfg, 
                                            gyroCfg);
   


    return 0;


  
}
