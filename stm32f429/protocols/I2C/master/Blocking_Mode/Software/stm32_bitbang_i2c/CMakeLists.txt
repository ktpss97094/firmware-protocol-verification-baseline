project(i2c_master_blocking_sw_bitbang C)

set(CBMC_FLAGS "${CBMC_FLAGS} --unwindset DWT_Delay_us.0:2")

file(GLOB CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/*.c")
add_executable(${PROJECT_NAME}
    ${CORE_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    stm32f4xx_hal_automated_firmware_verification_using_firmware_hardware_interaction_patterns
    stm32_bitbang_i2c_automated_firmware_verification_using_firmware_hardware_interaction_patterns
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc"
)

add_firmware(${PROJECT_NAME})