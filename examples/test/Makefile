############################################################################
# apps/examples/test/Makefile
############################################################################

include $(APPDIR)/Make.defs

PROGNAME  = $(CONFIG_EXAMPLES_TEST_PROGNAME)
PRIORITY  = $(CONFIG_EXAMPLES_TEST_PRIORITY)
STACKSIZE = $(CONFIG_EXAMPLES_TEST_STACKSIZE)
MODULE    = $(CONFIG_EXAMPLES_TEST)

# Source files
CSRCS = 
CXXSRCS = test_main.cxx \
          ../../../nuttx/boards/xtensa/esp32/esp32-devkitc/peripheral_namespaces/esp32_gpio.cxx \
		  ../../../nuttx/boards/xtensa/esp32/esp32-devkitc/peripheral_namespaces/esp32_pwm.cxx \
		  ../../../nuttx/boards/xtensa/esp32/esp32-devkitc/peripheral_namespaces/esp32_i2c.cxx \
		  ../../../nuttx/boards/xtensa/esp32/esp32-devkitc/peripheral_namespaces/esp32_spi.cxx \
		  ../../../nuttx/boards/xtensa/esp32/esp32-devkitc/peripheral_namespaces/esp32_tcp.cxx

# Include paths

CFLAGS +=


CXXFLAGS += \
  -I../../../nuttx/boards/xtensa/esp32/esp32-devkitc/peripheral_namespaces

include $(APPDIR)/Application.mk
