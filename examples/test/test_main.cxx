/****************************************************************************
 * apps/examples/hello/hello_main.c
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

#include <nuttx/config.h>
#include "esp32_peripherals.hpp"


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * test_main
 ****************************************************************************/

using namespace ESP32::GPIO;
using namespace ESP32::PWM;
using namespace ESP32::I2C;



extern "C" int test_main(int argc, char *argv[])
{
    
    printf("test_main\r\n");


    GPIO gpio0;
    // PinStatus val0, val1, val2;

    const char* dev0 = "/dev/gpio0";


    // Set all as output
    if (!gpio0.setPinType(dev0, GPIO_OUTPUT_PIN)) {
        printf("Failed to initialize GPIOs\r\n");
        return 1;
    }

    // Turn them on

    bool no_problem = true;

    no_problem &= gpio0.writePin(PinStatus::GPIO_HIGH);

    printf("Write result: %s\r\n", no_problem ? "SUCCESS" : "FAILURE");

    PWM pwm;

    pwm_info_s pwm_config;
    pwm_config.frequency = 1000; // 1 kHz
    pwm_config.duty = b16divi(uitoub16(30), 100); // 50%

    if (!pwm.setup("/dev/pwm0", pwm_config))
    {
        printf("PWM setup failed\n");
        return 1;
    }

    if (!pwm.start())
    {
        printf("PWM start failed\n");
        return 1;
    }

    printf("PWM running at 1 kHz, 50%% duty cycle\n");




    // Step 1: I2C config for TCS3472
    i2c_config_s i2c_config = {
        .frequency = 400000,  // Max supported by the TCS3472
        .address = 0x29,      // TCS3472 I2C address
        .addrlen = 7
    };

    I2C_Master i2c;
    const char* devName = "/dev/i2c0";
    if (!i2c.setup(devName, i2c_config))
    {
        printf("I2C0 setup failed\n");
    }

    // Step 2: Read the ID register (0x12)
    uint8_t id = 0;
    if (!i2c.readRegister(0x12, &id, 1))
    {
        printf("I2C0: Failed to read TCS3472 ID register\n");
    }
    else
    {
        printf("I2C0: TCS3472 ID register = 0x%02X\n", id);
    }

    
    while (1); // Block forever

    return 0;

}

