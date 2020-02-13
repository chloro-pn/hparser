set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory(. SRC_LIST)

add_library(hparser ${SRC_LIST})

