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
		$(TOPDIR)/drivers/esp32_peripherals/esp32_gpio.cxx \
		$(TOPDIR)/drivers/esp32_peripherals/esp32_pwm.cxx \
		$(TOPDIR)/drivers/esp32_peripherals/esp32_i2c.cxx \
		$(TOPDIR)/drivers/esp32_peripherals/esp32_spi.cxx \
        $(TOPDIR)/drivers/esp32_peripherals/esp32_tcp.cxx

# Include paths
CFLAGS +=
CXXFLAGS +=

include $(APPDIR)/Application.mk
