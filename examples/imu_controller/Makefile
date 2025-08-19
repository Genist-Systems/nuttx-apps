############################################################################
# apps/examples/IMUCONTROLLER/Makefile
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.  The
# ASF licenses this file to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#
############################################################################/


include $(APPDIR)/Make.defs

PROGNAME  = $(CONFIG_EXAMPLES_IMUCONTROLLER_PROGNAME)
PRIORITY  = $(CONFIG_EXAMPLES_IMUCONTROLLER_PRIORITY)
STACKSIZE = $(CONFIG_EXAMPLES_IMUCONTROLLER_STACKSIZE)
MODULE    = $(CONFIG_EXAMPLES_IMUCONTROLLER)

# Source files
CSRCS =
CXXSRCS = BMI323.cxx \
          IMUController.cxx \
          imu_controller_main.cxx \
          $(TOPDIR)/drivers/esp32_peripherals/esp32_i2c.cxx



CXXFLAGS += -I$(APPDIR)/cpputils/MQueue
CXXFLAGS += -I$(APPDIR)/examples/slave_controller


include $(APPDIR)/Application.mk

