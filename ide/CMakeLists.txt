cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_MODULE}")

include(ntt-config)

ntt_project(EngineIDE)

ntt_configure()
ntt_run_autogen()

file(
    GLOB_RECURSE 
    SOURCE_FILES
    "src/*.cpp"
    "src/*.h"
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    ${TARGET_DEFINITIONS}
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    ${TARGET_COMPILE_OPTIONS}
)

ntt_find_package(ntt-engine-core "Engine")
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    EngineCore
)