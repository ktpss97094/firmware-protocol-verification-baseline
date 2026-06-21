project(i2c_master_blocking_hw_opencm3 C)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

target_link_libraries(${PROJECT_NAME} PRIVATE
    libopencm3_automated_firmware_verification_using_firmware_hardware_interaction_patterns
)

add_firmware(${PROJECT_NAME}
    CBMC_ARGS
        --unwindset i2c_write7_v1.0:1,i2c_write7_v1.1:1,i2c_write7_v1.2:1,i2c_write7_v1.3:1
)
