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
#include "loadcell_main.hpp"
#include "LoadCellController.hpp"
#include <nuttx/i2c/i2c_master.h>

/****************************************************************************
 * Defines
 ****************************************************************************/



/****************************************************************************
 * Public Functions
 ****************************************************************************/


/****************************************************************************
 * main
 ****************************************************************************/



extern "C" int loadcell_main(int argc, FAR char *argv[])
{
 
    struct NAU7802Config load_cell_config
    {
        .enable = true,
        .ldoVoltage = NAU7802_3V3,
        .gain = NAU7802_GAIN_128,
        .sampleRate = NAU7802_RATE_10SPS,
        .calibrationMode = NAU7802_CALMOD_INTERNAL
    };

    struct i2c_config_s i2c_config
    {
        .frequency = 400000,   
        .address = 0x2A,     
        .addrlen = 7,           
    };


    struct TCA9548APins i2c_mux_pins
    {
        .sel2 = CONFIG_EXAMPLES_LOADCELL_MUX_SELECT_PIN_BIT_2,
        .sel1 = CONFIG_EXAMPLES_LOADCELL_MUX_SELECT_PIN_BIT_1,
        .sel0 = CONFIG_EXAMPLES_LOADCELL_MUX_SELECT_PIN_BIT_0
    };

    LoadCellController controller = LoadCellController(&load_cell_config, 
                        CONFIG_EXAMPLES_LOADCELL_I2C_FD, 
                        &i2c_config,
                        &i2c_mux_pins, 
                        CONFIG_EXAMPLES_LOADCELL_RATED_KG);

    controller.begin();

    controller.run();


    return 0;


  
}
