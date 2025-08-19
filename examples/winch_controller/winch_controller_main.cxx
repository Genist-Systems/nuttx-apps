/****************************************************************************
 * apps/examples/optical_encoder/optical_encoder_main.cxx
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

#include "esp32_peripherals/esp32_peripherals.hpp"
#include "winch_controller_main.hpp"
#include "WinchController.hpp"
#include "MQueue.hpp"


/****************************************************************************
 * Defines
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * main
 ****************************************************************************/

extern "C" int winch_controller_main(int argc, FAR char *argv[])
{
    const char* mq_name = CONFIG_EXAMPLES_WINCH_CONTROLLER_MQUEUE_NAME;
    const char* pwm_dev = CONFIG_EXAMPLES_WINCH_CONTROLLER_PWM_DEVNAME;

    MQueue_Settings mq_settings = {
        .flags = O_CREAT | O_RDWR | O_NONBLOCK,
        .mode = 0644,
        .maxMsg = 8,
        .msgSize = sizeof(WinchControlData)
    };

    pwm_info_s pwm_cfg = 
    {
        .frequency = 50,
        .duty = 0
    };
     

    constexpr uint8_t max_reel_speed = CONFIG_EXAMPLES_WINCH_CONTROLLER_MAX_REEL_SPEED_PERCENT; 

    WinchController winch(mq_name, mq_settings, pwm_dev, &pwm_cfg, max_reel_speed);

    winch.run();

    return 0;
    


  
}