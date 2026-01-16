cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_MODULE}")

include(ntt-config)

ntt_project(EngineCore)

ntt_configure()
ntt_run_autogen()

file(
    GLOB_RECURSE 
    SOURCE_FILES
    "src/*.cpp"
    "include/*.h"
)

set(
    GENERATED_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/temp/generated_json_writer.cpp"
)


file(
    GLOB_RECURSE 
    HEADER_FILES
    "include/*.h"
)

set(PROJECT_SOURCES
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${GENERATED_SOURCE_FILES}
)

set(TARGET_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(TARGET_PRECOMPILED_HEADER
    "${CMAKE_CURRENT_SOURCE_DIR}/include/platforms/common.h"
)

ntt_find_package(ntt-glm "Dependencies")
ntt_find_package(ntt-json "Dependencies")
ntt_configure_imgui()

set(TARGET_LINK_LIBRARIES
    ntt-glm
    ntt-json
    ntt-imgui
)

if (NTT_USE_GLFW)
    ntt_find_package(ntt-glfw "Dependencies")
    list(APPEND TARGET_LINK_LIBRARIES ntt-glfw)

    if (NTT_USE_GRAPHICS_OPENGL)
        ntt_find_package(ntt-glew "Dependencies")
        list(APPEND TARGET_LINK_LIBRARIES ntt-glew)
    endif()
endif()

if (NTT_USE_GRAPHICS_VULKAN)
    ntt_find_package(ntt-vulkan "Dependencies")
    list(APPEND TARGET_LINK_LIBRARIES ntt-vulkan)
endif()

add_library(
    ${PROJECT_NAME}
    STATIC
    ${PROJECT_SOURCES}
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

if (NTT_ENGINE_EDITOR_BINDING AND NTT_PLATFORM_LINUX)
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE 
        -fPIC
    )
endif()

if (NTT_ENGINE_EDITOR_BINDING)
    ntt_find_package(ntt-pybind "Dependencies")
    set(BINDING_PROJECT_NAME EngineCoreBinding)
    pybind11_add_module(
        ${BINDING_PROJECT_NAME}
        "temp/generated_bindings.cpp"
    )

    target_link_libraries(
        ${BINDING_PROJECT_NAME}
        PUBLIC
        ${PROJECT_NAME}
    )

    target_compile_options(
        ${BINDING_PROJECT_NAME}
        PUBLIC
        -fPIC
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

    add_custom_command(
        TARGET ${BINDING_PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SRC_FILE} ${DST_FILE}
        COMMENT "Copying ${BINDING_PROJECT_NAME}${BINDING_OUTPUT_SUFFIX} to editor folder"
    )
endif()

if (NTT_ENGINE_CORE_TESTS_ENABLE AND NOT NTT_ENGINE_EDITOR_BINDING)
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
        ${PROJECT_SOURCES}
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
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
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