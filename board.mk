BOARDCPPSRC =  $(BOARD_DIR)/board_configuration.cpp \
    $(BOARD_DIR)/default_tune.cpp \


BOARDINC += $(BOARD_DIR)/generated/controllers/generated

# defines SHORT_BOARD_NAME
DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE
DDEFS += -DEFI_SOFTWARE_KNOCK=TRUE -DSTM32_ADC_USE_ADC3=TRUE
DDEFS += -DDIAG_5VP_PIN=Gpio::B12
include $(BOARD_DIR)/meta-info.env

# reduce memory usage monitoring
DDEFS += -DRAM_UNUSED_SIZE=100

# assign critical LED to a non-existent pin if you do not have it on your board
# good old PD14 is still the default value
DDEFS += -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::C4

