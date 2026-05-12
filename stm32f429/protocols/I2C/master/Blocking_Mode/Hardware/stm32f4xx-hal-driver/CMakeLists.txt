project(i2c_master_blocking_hw_hal C)

add_firmware(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE 
    "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/stm32f4xx-hal-driver/Inc"
)

file(GLOB AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES 
    CONFIGURE_DEPENDS 
    "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/stm32f4xx-hal-driver/Src/*.c"
)
target_sources(${PROJECT_NAME} PRIVATE 
    ${AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES}
)

set(CBMC_FLAGS "${CBMC_FLAGS} --unwindset I2C_WaitOnFlagUntilTimeout.1:3,I2C_WaitOnMasterAddressFlagUntilTimeout.2:3,I2C_WaitOnTXEFlagUntilTimeout.1:3,I2C_WaitOnBTFFlagUntilTimeout.1:3")