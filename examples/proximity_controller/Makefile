############################################################################
# apps/examples/PROXIMITYSENSOR/Makefile
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

PROGNAME  = $(CONFIG_EXAMPLES_PROXIMITYSENSOR_PROGNAME)
PRIORITY  = $(CONFIG_EXAMPLES_PROXIMITYSENSOR_PRIORITY)
STACKSIZE = $(CONFIG_EXAMPLES_PROXIMITYSENSOR_STACKSIZE)
MODULE    = $(CONFIG_EXAMPLES_PROXIMITYSENSOR)

# Source files
CSRCS =
CXXSRCS = proximity_sensor_controller_main.cxx \
		ProximitySensorController.cxx \
		VL53L4ED/VL53L4ED.cxx \
		VL53L4ED/VL53L4ED_calibration.cxx \
		VL53L4ED/platform.cxx \
		$(TOPDIR)/drivers/esp32_peripherals/esp32_i2c.cxx \
		$(TOPDIR)/drivers/esp32_peripherals/esp32_gpio.cxx 



CXXFLAGS += -I$(APPDIR)/cpputils/MQueue




include $(APPDIR)/Application.mk