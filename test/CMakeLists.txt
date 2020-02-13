set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

aux_source_directory(. SRC_LIST)

add_executable(hparser_test ${SRC_LIST})

target_link_libraries(hparser_test hparser)

add_test(NAME parser_test COMMAND hparser_test)

