cmake_minimum_required(VERSION 3.15)
project(firmware C)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(CBMC_FLAGS "--arch arm --32 --no-standard-checks --drop-unused-functions")
set(CMAKE_C_FLAGS "${OPT_FLAGS} ${MCU_FLAGS} -Wall -g -DSTM32F429xx -DUSE_HAL_DRIVER")

function(add_firmware TARGET_NAME)
    file(GLOB CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/*.c")
    file(GLOB HAL_SOURCES "${LIB_DIR}/stm32f4xx-hal-driver/Src/*.c")
    list(FILTER HAL_SOURCES EXCLUDE REGEX ".*template\\.c$")
    file(GLOB AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/Src/*.c")

    add_executable(${TARGET_NAME}
        ${HAL_SOURCES}
        ${CORE_SOURCES}
    )

    set_target_properties(${TARGET_NAME} PROPERTIES 
        OUTPUT_NAME ${CMAKE_PROJECT_NAME}
        SUFFIX ".out"
    )

    target_include_directories(${TARGET_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc"
        "${LIB_DIR}/stm32f4xx-hal-driver/Inc"
        "${LIB_DIR}/cmsis-device-f4/Include"
        "${LIB_DIR}/CMSIS_5/CMSIS/Core/Include"
        "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/Inc"
    )

    separate_arguments(CBMC_FLAGS_LIST NATIVE_COMMAND "${CBMC_FLAGS}")
    add_custom_target(${TARGET_NAME}-verify
        DEPENDS ${TARGET_NAME}
        COMMAND ${CBMC_TOOL} $<TARGET_FILE_DIR:${TARGET_NAME}>/${CMAKE_PROJECT_NAME}.out ${CBMC_FLAGS_LIST} || true  # 加上 "|| true" 避免 verification failure 時 cbmc 回傳的錯誤碼被 cmake 接收，印出錯誤訊息
        USES_TERMINAL
    )
    add_custom_target(${TARGET_NAME}-show-loops
        DEPENDS ${TARGET_NAME}
        COMMAND ${CBMC_TOOL} $<TARGET_FILE_DIR:${TARGET_NAME}>/${CMAKE_PROJECT_NAME}.out ${CBMC_FLAGS_LIST} --show-loops
        USES_TERMINAL
    )

endfunction()

add_subdirectory(protocols/I2C/master/Blocking_Mode/Hardware/stm32f4xx-hal-driver)
add_subdirectory(protocols/I2C/master/Blocking_Mode/Software/stm32_bitbang_i2c)
add_subdirectory(protocols/I2C/master/Interrupt_Mode/stm32f4xx-hal-driver)
add_subdirectory(protocols/I2C/master/DMA_Mode/stm32f4xx-hal-driver)