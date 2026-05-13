project(i2c_master_blocking_hw_hal C)

set(CBMC_FLAGS "${CBMC_FLAGS} --unwindset I2C_WaitOnFlagUntilTimeout.1:3,I2C_WaitOnMasterAddressFlagUntilTimeout.2:3,I2C_WaitOnTXEFlagUntilTimeout.1:3,I2C_WaitOnBTFFlagUntilTimeout.1:3")

add_firmware(${PROJECT_NAME})