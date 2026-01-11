cmake_minimum_required(VERSION 3.20)
project(EngineRuntime)

set(CMAKE_MODULE "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_MODULE}")

include(ntt-config)

ntt_project(EngineRuntime)

ntt_configure()
ntt_run_autogen()

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

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
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

if (NTT_BOOTAPP_COPY)
    set(SRC_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/bootapp")
    set(DEST_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/appdata")
    set(COPY_BOOTAPP_STAMP "${CMAKE_CURRENT_BINARY_DIR}/appdata/bootapp_copied.stamp")
    file(
        GLOB_RECURSE
        BOOTAPP_FILES
        "${SRC_FOLDER}/*"
    )

    add_custom_command(
        OUTPUT ${COPY_BOOTAPP_STAMP}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SRC_FOLDER} ${DEST_FOLDER}
        COMMAND ${CMAKE_COMMAND} -E touch ${COPY_BOOTAPP_STAMP}
        DEPENDS ${BOOTAPP_FILES}
        COMMENT "Copying bootapp files..."
    )

    add_custom_target(copy_bootapp_directory ALL DEPENDS ${BOOTAPP_FILES} ${COPY_BOOTAPP_STAMP})
endif()