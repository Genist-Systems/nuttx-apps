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
#include "esp32_peripherals/esp32_peripherals.hpp"



/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * test_main
 ****************************************************************************/
// ESP #1 info
// wlan0	Link encap:Ethernet HWaddr 30:c9:22:ff:73:b4 at UP mtu 1504
// 	inet addr:10.0.0.2 DRaddr:10.0.0.1 Mask:255.255.255.0
// 	inet6 addr: fc00::2/112
// 	inet6 DRaddr: fc00::1

// ESP #2 info
// wlan0	Link encap:Ethernet HWaddr 2c:bc:bb:6e:b6:3c at UP mtu 1504
// 	inet addr:10.0.0.2 DRaddr:10.0.0.1 Mask:255.255.255.0
// 	inet6 addr: fc00::2/112
// 	inet6 DRaddr: fc00::1



using namespace ESP32::GPIO;
// using namespace ESP32::PWM;
// using namespace ESP32::I2C;
// using namespace ESP32::SPI;
// using namespace ESP32::WiFi::TCP;


// const uint8_t trusted_mac[6] = { 0x30, 0xC9, 0x22, 0xFF, 0x73, 0xB4 };
// const uint8_t peer_mac0[6] = { 0x2C, 0xBC, 0xBB, 0x6E, 0xB6, 0x3C };

void myInterruptHandler() {
    printf("GPIO interrupt fired!\n");
}

extern "C" int test_main(int argc, char *argv[])
{
    
    printf("test_main\r\n");


    // GPIO gpio0;
    // // PinStatus val0, val1, val2;

    // const char* dev0 = "/dev/gpio0";


    // // Set all as output
    // if (!gpio0.setPinType(dev0, GPIO_OUTPUT_PIN)) {
    //     printf("Failed to initialize GPIOs\r\n");
    //     return 1;
    // }

    // // Turn them on

    // bool no_problem = true;

    // no_problem &= gpio0.writePin(PinStatus::GPIO_HIGH);

    // printf("Write result: %s\r\n", no_problem ? "SUCCESS" : "FAILURE");

     GPIO gpio;

    if (!gpio.setPinType("/dev/gpio2", GPIO_INTERRUPT_PIN)) {
        fprintf(stderr, "Failed to set pin type\n");
        return 1;
    }

    if (!gpio.attachInterrupt(SIGUSR1, myInterruptHandler)) {
        fprintf(stderr, "Failed to attach interrupt\n");
        return 1;
    }

    while (true) {
        pause();  // Wait for signal
    }
    // // Block forever — actual ISR does the work
    // while (true) {
    //     sleep(1);
    // }



    // PWM pwm;

    // pwm_info_s pwm_config;
    // pwm_config.frequency = 1000; // 1 kHz
    // pwm_config.duty = b16divi(uitoub16(30), 100); // 50%

    // if (!pwm.setup("/dev/pwm0", &pwm_config))
    // {
    //     printf("PWM setup failed\n");
    //     return 1;
    // }

    // if (!pwm.start())
    // {
    //     printf("PWM start failed\n");
    //     return 1;
    // }

    // printf("PWM running at 1 kHz, 50%% duty cycle\n");




    // // Step 1: I2C config for TCS3472
    // i2c_config_s i2c_config = {
    //     .frequency = 400000,  // Max supported by the TCS3472
    //     .address = 0x29,      // TCS3472 I2C address
    //     .addrlen = 7
    // };

    // I2C_Master i2c;
    // const char* devName = "/dev/i2c0";
    // if (!i2c.setup(devName, &i2c_config))
    // {
    //     printf("I2C0 setup failed\n");
    // }

    // // Step 2: Read the ID register (0x12)
    // uint8_t id = 0;
    // if (!i2c.readRegister(0x12, &id, 1))
    // {
    //     printf("I2C0: Failed to read TCS3472 ID register\n");
    // }
    // else
    // {
    //     printf("I2C0: TCS3472 ID register = 0x%02X\n", id);
    // }



    // const char* spiDevice = "/dev/spislv2";  // Change if needed
    // SPI_Slave spi;

    // if (!spi.setup(spiDevice, 5 /* timeout in seconds */, false /* blocking */)) {
    //     printf("Failed to set up SPI slave.\n");
    //     return 1;
    // }

    // const size_t bufferSize = 64;
    // uint8_t buffer[bufferSize];
    // size_t receivedBytes = 0;

    // printf("Waiting to receive SPI data...\n");

    // while (true) {
    //     if (spi.receive(buffer, bufferSize, receivedBytes)) {
    //         printf("Received %zu bytes: ", receivedBytes);
    //         for (size_t i = 0; i < receivedBytes; ++i) {
    //             printf("%02X ", buffer[i]);
    //         }
    //         printf("\n");
    //     } else {
    //         printf("Failed to receive data or timeout occurred.\n");
    //     }

    //     // Optional: add delay or exit condition here
    // }

    // spi.shutdown();
    
    // while (1); // Block forever

//     if (argc != 2)
//   {
//     printf("Usage: %s <server_ip>\n", argv[0]);
//     return 1;
//   }

//   TCPClient client(argv[1], 5000); // port must match server

//   if (!client.connectToServer())
//   {
//     printf("Failed to connect to server.\n");
//     return 1;
//   }

//   while (true)
//   {
//     protocolo_ipc msg;
//     msg.opcode = 'U';
//     msg.msg_size = 4;
//     snprintf((char*)msg.msg, sizeof(msg.msg), "0102");

//     if (!client.sendMessage(msg))
//     {
//       printf("Failed to send message.\n");
//       break;
//     }

//     protocolo_ipc response;
//     if (!client.receiveMessage(response))
//     {
//       printf("Failed to receive response.\n");
//       break;
//     }

//     sleep(5);
//   }

//   client.closeSocket();
//   return 0;

    // const char* ip = "10.0.0.3";
    // const char* ifname = "wlan0";
    // const char* ssid = "MyTestAP";
    // const char* password = "12345678";

    // TCPServer server(5000, ip, ifname, ssid, password);

    // if (!server.init())
    // {
    //     printf("Server init failed\n");
    //     return -1;
    // }

    // if (!server.acceptClient())
    // {
    //     printf("Client accept failed\n");
    //     return -1;
    // }

    // int received_number;

    // while (1)
    // {
    //     if (!server.receiveMessage(received_number))
    //     {
    //         printf("Failed to receive integer\n");
    //         return -1;
    //     }

    //     printf("Server received: %d\n", received_number);

    //     received_number += 1;

    //     if (!server.sendMessage(received_number))
    //     {
    //         printf("Failed to send response\n");
    //         return -1;
    //     }

    //     printf("Server sent: %d\n", received_number);
    // }
    

    

    // server.closeAll();
    // return 0;

    //  const char* server_ip = "10.0.0.3";
    //  const char* client_ip = "10.0.0.4";
    // const char* ifname = "wlan0";
    // const char* ssid = "MyTestAP";
    // const char* password = "12345678";

    // TCPClient client(5000, server_ip, client_ip, ifname, ssid, password);

    // if (!client.connectToServer())
    // {
    //     printf("Client connection failed\n");
    //     return -1;
    // }

    // int num = 123;
    // while(1)
    // {
    // if (!client.sendMessage(num))
    //     {
    //         printf("Failed to send\n");
    //         return -1;
    //     }

    //     printf("Client sent: %d\n", num);

    //     int response = 0;
    //     if (!client.receiveMessage(response))
    //     {
    //         printf("Failed to receive\n");
    //         return -1;
    //     }

    //     printf("Client received: %d\n", response);
    // }
    

    

    return 0;

    
    


}

