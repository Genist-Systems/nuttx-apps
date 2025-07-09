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

#include "VL53L4ED/VL53L4ED.hpp"

/****************************************************************************
 * Defines
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * main
 ****************************************************************************/

extern "C" int proximity_sensor_main(int argc, FAR char *argv[])
{
    struct i2c_config_s i2c_config = 
    {
        .frequency = 400000,
        .address = 0x52, 
        .addrlen = 7
    };
    
    // just a test
    VL53L4ED thing = VL53L4ED("/dev/i2c0", &i2c_config, "/dev/gpio0");
    
    


    thing.init();

    thing.run();


    return 0;


  
}