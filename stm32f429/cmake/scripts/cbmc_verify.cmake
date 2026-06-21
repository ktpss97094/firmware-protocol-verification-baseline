string(REPLACE "@@" ";" CBMC_ARGS "${CBMC_ARGS_ENCODED}")
separate_arguments(EXTRA_ARGS NATIVE_COMMAND "$ENV{EXTRA_CBMC_FLAGS}")

execute_process(
    COMMAND ${CBMC_TOOL} ${TARGET_FILE} ${CBMC_ARGS} ${EXTRA_ARGS}
    RESULT_VARIABLE exit_code
)

# 0: Verification SUCCESSFUL
# 10: Verification FAILURE
if(NOT exit_code EQUAL 0 AND NOT exit_code EQUAL 10)
    message(FATAL_ERROR "exit code: ${exit_code}")
endif()
