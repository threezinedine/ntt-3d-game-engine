cmake_minimum_required(VERSION 3.20)
project(EngineCore)

set(CMAKE_MODULE "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_MODULE}")

include(ntt-config)

ntt_configure()

ntt_print_configuration()

file(
    GLOB_RECURSE 
    SOURCE_FILES
    "src/*.cpp"
    "include/*.h"
)

file(
    GLOB_RECURSE 
    HEADER_FILES
    "include/*.h"
)

add_library(
    ${PROJECT_NAME}
    STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    ${TARGET_DEFINITIONS}
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    ${TARGET_COMPILE_OPTIONS}
)

ntt_find_package(ntt-pybind "Dependencies")
set(BINDING_PROJECT_NAME EngineCoreBinding)
pybind11_add_module(
    ${BINDING_PROJECT_NAME}
    "temp/main.cpp"
)

target_link_libraries(
    ${BINDING_PROJECT_NAME}
    PUBLIC
    ${PROJECT_NAME}
)

if (NTT_PLATFORM_WINDOWS)
    set(BINDING_OUTPUT_SUFFIX ".pyd")
elseif(NTT_PLATFORM_LINUX)
    set(BINDING_OUTPUT_SUFFIX ".so")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

set_target_properties(
    ${BINDING_PROJECT_NAME}
    PROPERTIES
    SUFFIX ${BINDING_OUTPUT_SUFFIX}
)

add_custom_command(
    TARGET ${BINDING_PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/${BINDING_PROJECT_NAME}${BINDING_OUTPUT_SUFFIX}
    ${BASE_DIRECTORY}/editor/Engine/${BINDING_PROJECT_NAME}${BINDING_OUTPUT_SUFFIX}
    COMMENT "Copying ${BINDING_PROJECT_NAME}${BINDING_OUTPUT_SUFFIX} to editor folder"
)
