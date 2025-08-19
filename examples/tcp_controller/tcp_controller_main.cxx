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
#include "TCPController.hpp"
#include "tcp_controller_main.hpp"
#include "master_controller_main.hpp"
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



extern "C" int tcp_controller_main(int argc, FAR char *argv[])
{
    struct MQueue_Settings mq_settings =
    {
        .flags = O_CREAT | O_RDWR | O_NONBLOCK,
        .mode = 0644,
        .maxMsg = 8,
        .msgSize = 32
    };

    struct TCP_Settings tcp_settings = 
    {
        .port = CONFIG_EXAMPLES_TCPCONTROLLER_PORT_NUM,
        .server_ip = CONFIG_EXAMPLES_TCPCONTROLLER_SERVER_IP,
        .client_ip = CONFIG_EXAMPLES_TCPCONTROLLER_CLIENT_IP,
        .ifname = CONFIG_EXAMPLES_TCPCONTROLLER_IFNAME,
        .ssid = CONFIG_EXAMPLES_TCPCONTROLLER_SSID,
        .password = CONFIG_EXAMPLES_TCPCONTROLLER_PASSWORD
    };

    TCPController controller = TCPController(CONFIG_EXAMPLES_MASTER_AND_SLAVE_CONTROLLER_MQUEUE_DRONE_TO_STRETCHER_NAME, 
                                            CONFIG_EXAMPLES_MASTER_AND_SLAVE_CONTROLLER_MQUEUE_STRETCHER_TO_DRONE_NAME, 
                                            mq_settings, 
                                            tcp_settings);
    controller.run();

    return 0;


  
}
