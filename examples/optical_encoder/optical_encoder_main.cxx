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

#include "optical_encoder_main.hpp"
#include "OpticalEncoderController.hpp"

/****************************************************************************
 * Defines
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * main
 ****************************************************************************/

extern "C" int optical_encoder_main(int argc, FAR char *argv[])
{

    OpticalEncoderController controller = OpticalEncoderController(CONFIG_EXAMPLES_OPTICAL_ENCODER_GPIOA, 
                                                                    CONFIG_EXAMPLES_OPTICAL_ENCODER_GPIOB, 
                                                                    CONFIG_EXAMPLES_OPTICAL_ENCODER_PPR,
                                                                    CONFIG_EXAMPLES_OPTICAL_ENCODER_SIGNO, 
                                                                    CONFIG_EXAMPLES_OPTICAL_ENCODER_INITIAL_LENGTH_METERS, 
                                                                    CONFIG_EXAMPLES_OPTICAL_ENCODER_AVERAGE_DRUM_RADIUS_CM);


    controller.init();

    controller.run();


    return 0;


  
}