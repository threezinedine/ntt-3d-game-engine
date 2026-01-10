cmake_minimum_required(VERSION 3.20)
project(EngineCore)

set(CMAKE_MODULE "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_MODULE}")

include(ntt-config)

ntt_project(EngineCore)

ntt_configure()

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

set(TARGET_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(TARGET_PRECOMPILED_HEADER
    "${CMAKE_CURRENT_SOURCE_DIR}/include/platforms/common.h"
)

ntt_find_package(ntt-glm "Dependencies")

set(TARGET_LINK_LIBRARIES
    ntt-glm
)

if (NTT_USE_GLFW)
    ntt_find_package(ntt-glfw "Dependencies")
    list(APPEND TARGET_LINK_LIBRARIES ntt-glfw)
endif()

add_library(
    ${PROJECT_NAME}
    STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${TARGET_INCLUDE_DIRS}
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

target_precompile_headers(
    ${PROJECT_NAME}
    PUBLIC
    ${TARGET_PRECOMPILED_HEADER}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    ${TARGET_LINK_LIBRARIES}
)

if (NTT_ENGINE_BINDING AND NTT_PLATFORM_LINUX)
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE 
        -fPIC
    )
endif()

if (NTT_ENGINE_BINDING)
    ntt_find_package(ntt-pybind "Dependencies")
    set(BINDING_PROJECT_NAME EngineCoreBinding)
    pybind11_add_module(
        ${BINDING_PROJECT_NAME}
        "temp/bindings.cpp"
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

    set(SRC_FILE ${CMAKE_BINARY_DIR}/${BINDING_PROJECT_NAME}${BINDING_OUTPUT_SUFFIX})
    set(DST_FILE ${BASE_DIRECTORY}/editor/Engine/${BINDING_PROJECT_NAME}${BINDING_OUTPUT_SUFFIX})
    set(STAMP_FILE ${SRC_FILE}.stamp)

    add_custom_command(
        OUTPUT ${STAMP_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SRC_FILE} ${DST_FILE}
        COMMAND ${CMAKE_COMMAND} -E echo "" > ${STAMP_FILE}
        COMMENT "Copying ${BINDING_PROJECT_NAME}${BINDING_OUTPUT_SUFFIX} to editor folder"
    )

    add_custom_target(copy_binding_to_editor ALL DEPENDS ${STAMP_FILE})

    set(AUTOGEN_STAMP ${CMAKE_BINARY_DIR}/autogen.stamp)

    find_program(PYTHON_EXECUTABLE NAMES Python PATHS ${BASE_DIRECTORY}/.venv)
    message("Using Python interpreter at: ${PYTHON_EXECUTABLE}")

    add_custom_command(
        OUTPUT ${AUTOGEN_STAMP}
        COMMAND cd ${BASE_DIRECTORY} && ${PYTHON_EXECUTABLE} helper.py run autogen
        COMMAND ${CMAKE_COMMAND} -E touch ${AUTOGEN_STAMP}
        DEPENDS ${HEADER_FILES}
        COMMENT "Running autogen script"
    )

    add_custom_target(run_autogen ALL DEPENDS ${HEADER_FILES} ${AUTOGEN_STAMP})
endif()

if (NTT_ENGINE_CORE_TESTS_ENABLE)
    ntt_find_package(ntt-googletest "Dependencies")
    file(
        GLOB_RECURSE 
        TEST_SOURCE_FILES
        "tests/*.cpp"
    )

    set(TEST_PROJECT_NAME ${PROJECT_NAME}Tests)

    add_executable(
        ${TEST_PROJECT_NAME}
        ${TEST_SOURCE_FILES}
        ${HEADER_FILES}
        ${SOURCE_FILES}
    )

    target_link_libraries(
        ${TEST_PROJECT_NAME}
        PRIVATE
        ntt-googletest
        ${TARGET_LINK_LIBRARIES}
    )

    target_include_directories(
        ${TEST_PROJECT_NAME}
        PRIVATE
        ${TARGET_INCLUDE_DIRS}
    )

    target_compile_definitions(
        ${TEST_PROJECT_NAME}
        PRIVATE 
        ${TARGET_DEFINITIONS}
    )

    target_compile_options(
        ${TEST_PROJECT_NAME}
        PRIVATE
        ${TARGET_COMPILE_OPTIONS}
    )

    target_precompile_headers(
        ${TEST_PROJECT_NAME}
        PRIVATE
        ${TARGET_PRECOMPILED_HEADER}
    )
endif()