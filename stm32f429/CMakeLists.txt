cmake_minimum_required(VERSION 3.15)
project(firmware C)

include(ExternalProject)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LIB_DIR "${ROOT_DIR}/lib")
set(MCU_FLAGS --native-compiler ${GOTO_CC_NATIVE_COMPILER} -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs nosys.specs)
set(CBMC_DEFAULT_ARGS --no-standard-checks --drop-unused-functions --object-bits 13)

add_library(cortex_m4_flags INTERFACE)
target_compile_options(cortex_m4_flags INTERFACE ${MCU_FLAGS} -Wall -g)
target_link_options(cortex_m4_flags INTERFACE ${MCU_FLAGS})

add_library(cmsis INTERFACE)
target_include_directories(cmsis INTERFACE
    "${LIB_DIR}/CMSIS_5/CMSIS/Core/Include"
    "${LIB_DIR}/cmsis-device-f4/Include"
)
target_compile_definitions(cmsis INTERFACE STM32F429xx)

add_library(common_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE)
file(GLOB COMMON_AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/common/*.c")
target_sources(common_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE ${COMMON_AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES})
target_include_directories(common_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE
    "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/common"
)
target_link_libraries(common_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE 
    cmsis
)

add_library(stm32f4xx_hal INTERFACE)
file(GLOB STM32F4XX_HAL_SOURCES "${LIB_DIR}/stm32f4xx-hal-driver/Src/*.c")
list(FILTER STM32F4XX_HAL_SOURCES EXCLUDE REGEX ".*template\\.c$")
target_sources(stm32f4xx_hal INTERFACE ${STM32F4XX_HAL_SOURCES})
target_include_directories(stm32f4xx_hal INTERFACE
    "${LIB_DIR}/stm32f4xx-hal-driver/Inc"
)
target_compile_definitions(stm32f4xx_hal INTERFACE USE_HAL_DRIVER)
target_link_libraries(stm32f4xx_hal INTERFACE cortex_m4_flags cmsis)

add_library(stm32f4xx_hal_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE)
file(GLOB STM32F4XX_HAL_AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/stm32f4xx-hal-driver/*.c")
target_sources(stm32f4xx_hal_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE ${STM32F4XX_HAL_AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES})
target_include_directories(stm32f4xx_hal_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE
    "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/stm32f4xx-hal-driver"
)
target_link_libraries(stm32f4xx_hal_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE 
    common_automated_firmware_verification_using_firmware_hardware_interaction_patterns
    stm32f4xx_hal
)

add_library(stm32_bitbang_i2c INTERFACE)
target_include_directories(stm32_bitbang_i2c INTERFACE "${LIB_DIR}/stm32_bitbang_i2c")
file(GLOB STM32_BITBANG_I2C_SOURCES CONFIGURE_DEPENDS "${LIB_DIR}/stm32_bitbang_i2c/*.c")
target_sources(stm32_bitbang_i2c INTERFACE ${STM32_BITBANG_I2C_SOURCES})
target_compile_definitions(stm32_bitbang_i2c INTERFACE STM32F4)
target_link_libraries(stm32_bitbang_i2c INTERFACE stm32f4xx_hal)

add_library(stm32_bitbang_i2c_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE)
file(GLOB STM32_BITBANG_I2C_AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/stm32_bitbang_i2c/*.c")
target_sources(stm32_bitbang_i2c_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE ${STM32_BITBANG_I2C_AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES})
target_include_directories(stm32_bitbang_i2c_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE
    "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/stm32_bitbang_i2c"
)
target_link_libraries(stm32_bitbang_i2c_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE 
    common_automated_firmware_verification_using_firmware_hardware_interaction_patterns
    stm32_bitbang_i2c
)

add_library(libopencm3 INTERFACE)
set(LIBOPENCM3_SOURCES
    "${LIB_DIR}/libopencm3/lib/stm32/common/gpio_common_all.c"
    "${LIB_DIR}/libopencm3/lib/stm32/common/gpio_common_f0234.c"
    "${LIB_DIR}/libopencm3/lib/stm32/common/i2c_common_v1.c"
    "${LIB_DIR}/libopencm3/lib/stm32/common/rcc_common_all.c"
    "${LIB_DIR}/libopencm3/lib/stm32/f4/rcc.c"
)
target_sources(libopencm3 INTERFACE ${LIBOPENCM3_SOURCES})
target_include_directories(libopencm3 INTERFACE "${LIB_DIR}/libopencm3/include")
target_compile_definitions(libopencm3 INTERFACE STM32F4)
target_link_libraries(libopencm3 INTERFACE cortex_m4_flags)

add_library(libopencm3_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE)
file(GLOB LIBOPENCM3_AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/libopencm3/*.c")
target_sources(libopencm3_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE ${LIBOPENCM3_AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_SOURCES})
target_include_directories(libopencm3_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE
    "${LIB_DIR}/automated-firmware-verification-using-firmware-hardware-interaction-patterns/libopencm3"
)
target_link_libraries(libopencm3_automated_firmware_verification_using_firmware_hardware_interaction_patterns INTERFACE 
    common_automated_firmware_verification_using_firmware_hardware_interaction_patterns
    libopencm3
)

function(add_firmware TARGET_NAME)
    cmake_parse_arguments(PARSE_ARGV 1 FIRMWARE "" "" "CBMC_ARGS")

    set_target_properties(${TARGET_NAME} PROPERTIES 
        OUTPUT_NAME ${CMAKE_PROJECT_NAME}
        SUFFIX ".out"
    )

    set(CBMC_TARGET_ARGS ${CBMC_DEFAULT_ARGS} ${FIRMWARE_CBMC_ARGS})

    add_custom_target(${TARGET_NAME}-verify
        DEPENDS ${TARGET_NAME}
        COMMAND ${CMAKE_COMMAND}
            "-DCBMC_TOOL=${CBMC_TOOL}"
            "-DTARGET_FILE=$<TARGET_FILE:${TARGET_NAME}>"
            "-DCBMC_ARGS=${CBMC_TARGET_ARGS}"
            -P "${ROOT_DIR}/cmake/scripts/cbmc_verify.cmake"
        USES_TERMINAL
        VERBATIM
    )

    add_custom_target(${TARGET_NAME}-show-loops
        DEPENDS ${TARGET_NAME}
        COMMAND "${CBMC_TOOL}" "$<TARGET_FILE:${TARGET_NAME}>" ${CBMC_TARGET_ARGS} --show-loops
        USES_TERMINAL
        VERBATIM
    )

endfunction()

add_subdirectory(protocols/I2C/master/Blocking_Mode/Hardware/stm32f4xx-hal-driver)
add_subdirectory(protocols/I2C/master/Blocking_Mode/Hardware/libopencm3)
add_subdirectory(protocols/I2C/master/Blocking_Mode/Software/stm32_bitbang_i2c)
add_subdirectory(protocols/I2C/master/Interrupt_Mode/stm32f4xx-hal-driver)
add_subdirectory(protocols/I2C/master/DMA_Mode/stm32f4xx-hal-driver)
