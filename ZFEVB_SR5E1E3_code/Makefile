################################################################################
# Project makefile.
################################################################################

# 'Stellar SDK' project values
PROJECTNAME := fireware
SDKID := StellarESDK-1.7.0

# Please Update it if you move your workspace
STELLAR_E_SDK_RELATIVE_PATH := ../../SDKS/$(SDKID)
TEST_ROOT_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)
PROJECT_COMMON_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)/Projects/SDKTests/CommonBuild
CONFIG_BOARD := Config

include $(PROJECT_COMMON_DIR)/make/test_defs.mk

################################################################################
# Project builds
################################################################################
BUILD_OS_OSAL                  := 1

BUILD_DRIVERS_GPIO             := 1
BUILD_DRIVERS_EXTI             := 1
BUILD_DRIVERS_SARADC           := 1
BUILD_DRIVERS_CORDIC           := 1
BUILD_DRIVERS_FLASH            := 1

BUILD_DRIVERS_DMA              := 1
BUILD_DRIVERS_UART             := 1
BUILD_DRIVERS_SPI              := 1
BUILD_DRIVERS_CAN              := 1

BUILD_DRIVERS_TIM              := 1
BUILD_DRIVERS_HRTIM            := 1
BUILD_DRIVERS_TIM_TS           := 1

################################################################################
# Add project files
################################################################################

# Application name
APP_NAME := $(PROJECTNAME)

# C sources
C_SRCS += \
	src/main.c \
	\
	libraries/zf_common/zf_common_debug.c \
	libraries/zf_common/zf_common_fifo.c \
	libraries/zf_common/zf_common_function.c \
	libraries/zf_common/zf_common_memory.c \
	\
	libraries/zf_driver/zf_driver_adc.c \
	libraries/zf_driver/zf_driver_can.c \
	libraries/zf_driver/zf_driver_delay.c \
	libraries/zf_driver/zf_driver_encoder.c \
	libraries/zf_driver/zf_driver_exti.c \
	libraries/zf_driver/zf_driver_flash.c \
	libraries/zf_driver/zf_driver_gpio.c \
	libraries/zf_driver/zf_driver_hrtim.c \
	libraries/zf_driver/zf_driver_interrupt.c \
	libraries/zf_driver/zf_driver_pit.c \
	libraries/zf_driver/zf_driver_pwm.c \
	libraries/zf_driver/zf_driver_rcc.c \
	libraries/zf_driver/zf_driver_soft_iic.c \
	libraries/zf_driver/zf_driver_soft_spi.c \
	libraries/zf_driver/zf_driver_spi.c \
	libraries/zf_driver/zf_driver_system.c \
	libraries/zf_driver/zf_driver_timer.c \
	libraries/zf_driver/zf_driver_uart.c \
	\
	libraries/zf_device/zf_device_imu_interface.c \
	libraries/zf_device/zf_device_imu660rb.c \
	libraries/zf_device/zf_device_imu963ra.c \
	\
	libraries/zf_device/zf_device_wireless_interface.c \
	libraries/zf_device/zf_device_wireless_uart.c \
	libraries/zf_device/zf_device_bluetooth_ch9141.c \
	\
	libraries/zf_device/zf_device_display_interface.c \
	libraries/zf_device/zf_device_display_font.c \
	libraries/zf_device/zf_device_ips200.c \
	\
	libraries/zf_device/zf_device_gnss.c \
	libraries/zf_device/zf_device_key.c \
	libraries/zf_device/zf_device_uart_receiver.c \
	\
	libraries/zf_components/seekfree_assistant/seekfree_assistant.c \
	libraries/zf_components/seekfree_assistant/seekfree_assistant_interface.c \
	\

# Add entries to the files in user_code, as if they were files above
	
# C includes
# PLEASE UPDATE IT FOR GENERATED CODE
# Open when an .gpio file has been added to open this include
# Add src-gen/... when an .Sr5e1_clk file has been added to open this include
# Add cfg/ when any driver config file has been added to open this include
C_INCS += \
	src-gen/ \
	src-gen/sr5e1 \
	\
	libraries/ \
	libraries/zf_common/ \
	libraries/zf_driver/ \
	libraries/zf_device/ \
	libraries/zf_components/\
	libraries/zf_components/seekfree_assistant\
	\
	user_code/ \

LIBS = -lm

################################################################################
# Include 'Stellar SDK' top level makefile
################################################################################
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/StellarESDK.mk

################################################################################
# Define 'Stellar E SDK' compiling target
################################################################################
all: default-all


