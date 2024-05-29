cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(SRC src/main.c src/bit_operations.c src/compress.c src/decompress.c src/huffman_tree.c src/printing_in_file.c src/scanning_file.c)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
