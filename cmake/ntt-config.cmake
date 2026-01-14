macro(ntt_project PROJECT)
    if (NOT TOP_LEVEL_PROJECT)
        set(TOP_LEVEL_PROJECT ${PROJECT})
        message(STATUS "Top level project set to: ${TOP_LEVEL_PROJECT}")
    endif()

    if (${PROJECT_NAME} STREQUAL TOP_LEVEL_PROJECT)
        set(IS_TOP_LEVEL_PROJECT TRUE)
    else()
        set(IS_TOP_LEVEL_PROJECT FALSE)
    endif()
endmacro()

macro(ntt_configure)
    if (IS_TOP_LEVEL_PROJECT)
        set(BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")
        cmake_path(SET NTT_ENGINE_DIRECTORY NORMALIZE "${BASE_DIRECTORY}")
        ntt_log_var("NTT_ENGINE_DIRECTORY")
    endif()

    if (NTT_ENGINE_EDITOR_BINDING)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    else()
        set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
    endif()

    option(CMAKE_BUILD_TYPE "Build type" "Debug") 
    set(TARGET_COMPILE_OPTIONS)
    set(TARGET_DEFINITIONS "NTT_ENGINE_DIRECTORY=${NTT_ENGINE_DIRECTORY}")

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        list(APPEND TARGET_COMPILE_OPTIONS "-DDEBUG=0" "-O3" "-DNDEBUG")
        list(APPEND TARGET_DEFINITIONS "NTT_DEBUG=0")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Test")
        list(APPEND TARGET_DEFINITIONS "NTT_TEST=1")
        list(APPEND TARGET_COMPILE_OPTIONS "-DDEBUG=1" "-O0" "-g" "-DRUN_TESTS=1")
    else()
        list(APPEND TARGET_COMPILE_OPTIONS "-DDEBUG=1" "-O0" "-g")
        list(APPEND TARGET_DEFINITIONS "NTT_DEBUG=1")
    endif()

    ntt_platform_detect()

    ntt_option("NTT_NO_ANALYZE" "OFF")
    ntt_option("NTT_ENGINE_CORE_TESTS_ENABLE" "ON")
    ntt_option("NTT_ENGINE_EDITOR_BINDING" "OFF")
    ntt_option("NTT_BOOTAPP_COPY" "ON")

    ntt_detect_graphics_api()

    if (MSVC)
    else()
        add_compile_options(-Wall -Werror -Wswitch)
    endif()

    ntt_remove_duplicates()

    if (IS_TOP_LEVEL_PROJECT)
        ntt_print_configuration()
    endif()
endmacro()

macro(ntt_log_var VAR_NAME)
    message(STATUS "${VAR_NAME}: ${${VAR_NAME}}")
    
endmacro()


macro(ntt_detect_graphics_api)
    ntt_option("NTT_USE_GRAPHICS_OPENGL" OFF)
    ntt_option("NTT_USE_GRAPHICS_VULKAN" ON)
    ntt_option("NTT_USE_GLFW" OFF)

    if (NTT_USE_GRAPHICS_VULKAN)
        list(APPEND TARGET_DEFINITIONS "NTT_USE_GRAPHICS_OPENGL=0")
        list(APPEND TARGET_DEFINITIONS "NTT_USE_GRAPHICS_VULKAN=1")

        if (NTT_USE_GRAPHICS_OPENGL)
            message(WARNING "Both OpenGL and Vulkan graphics APIs are enabled. Vulkan will be used as the primary graphics API.")
            list(REMOVE_ITEM TARGET_DEFINITIONS "NTT_USE_GRAPHICS_OPENGL=1")
            set(NTT_USE_GRAPHICS_OPENGL OFF)
        endif()
    else()
        list(APPEND TARGET_DEFINITIONS "NTT_USE_GRAPHICS_OPENGL=1")
        list(APPEND TARGET_DEFINITIONS "NTT_USE_GRAPHICS_VULKAN=0")
        list(REMOVE_ITEM TARGET_DEFINITIONS "NTT_USE_GLFW=0")
        list(APPEND TARGET_DEFINITIONS "NTT_USE_GLFW=1")

        if (NOT NTT_USE_GLFW)
            message(WARNING "OpenGL forces the use of GLFW. Enabling NTT_USE_GLFW.")
            set(NTT_USE_GLFW ON)
        endif()
    endif()
endmacro()


macro(ntt_platform_detect)
    set(NTT_PLATFORM_WINDOWS FALSE)
    set(NTT_PLATFORM_LINUX FALSE)
    set(NTT_PLATFORM_MACOS FALSE)
    set(NTT_PLATFORM_WEB FALSE)

    if (WIN32)
        list(APPEND TARGET_DEFINITIONS "NTT_PLATFORM_WINDOWS=1")
        set(NTT_PLATFORM_WINDOWS TRUE)
    elseif(APPLE)
        list(APPEND TARGET_DEFINITIONS "NTT_PLATFORM_MACOS=1")
        set(NTT_PLATFORM_MACOS TRUE)
    elseif(UNIX)
        set(NTT_PLATFORM_LINUX TRUE)
        list(APPEND TARGET_DEFINITIONS "NTT_PLATFORM_LINUX=1")
    elseif(EMSCRIPTEN)
        set(NTT_PLATFORM_WEB TRUE)
        list(APPEND TARGET_DEFINITIONS "NTT_PLATFORM_WEB=1")
    endif() 

    if (MSVC)
        list(APPEND TARGET_DEFINITIONS "NTT_COMPILER_MSVC=1")
    endif()
endmacro()


macro(ntt_option OPTION_NAME OPTION_DEFAULT)
    option(${OPTION_NAME} "${OPTION_NAME} value" ${OPTION_DEFAULT}) 

    if(${OPTION_NAME} STREQUAL "ON" OR ${OPTION_NAME} STREQUAL "1" OR ${OPTION_NAME} STREQUAL "TRUE")
        list(APPEND TARGET_DEFINITIONS ${OPTION_NAME}=1)
    elseif(${OPTION_NAME} STREQUAL "OFF" OR ${OPTION_NAME} STREQUAL "0" OR ${OPTION_NAME} STREQUAL "FALSE")
        list(APPEND TARGET_DEFINITIONS ${OPTION_NAME}=0)
    else()
        list(APPEND TARGET_DEFINITIONS ${OPTION_NAME}=${${OPTION_NAME}})
    endif()
endmacro()

macro(ntt_print_configuration)
    message(STATUS "CMake Build Type: ${CMAKE_BUILD_TYPE}")
    message(STATUS "Target Compile Options: ${TARGET_COMPILE_OPTIONS}")

    foreach(DEFINITION ${TARGET_DEFINITIONS})
        message(STATUS " - ${DEFINITION}")
    endforeach()
endmacro()

macro(ntt_find_package PACKAGE_NAME FOLDER)
    if (MSVC)
        set(CMAKE_FOLDER ${FOLDER})
    endif()

    set(${PACKAGE_NAME}_DIR ${BASE_DIRECTORY}/cmake)
    find_package(${PACKAGE_NAME} REQUIRED)

    if (MSVC)
        unset(CMAKE_FOLDER)
    endif()
endmacro()

macro(ntt_remove_duplicates)
    list(REMOVE_DUPLICATES TARGET_DEFINITIONS)
endmacro()

macro(ntt_run_autogen)
    if (IS_TOP_LEVEL_PROJECT)
        set(AUTOGEN_STAMP ${CMAKE_BINARY_DIR}/autogen.stamp)

        file(
            GLOB_RECURSE 
            TEMPLATE_FILES
            "${BASE_DIRECTORY}/autogen/templates/*.j2"
        )

        file(
            GLOB_RECURSE
            AUTOGEN_TRACKING_HEADERS
            "${BASE_DIRECTORY}/core/include/*.h"
        )

        if (NTT_PLATFORM_WINDOWS)
            set(PYTHON_EXECUTABLE "${BASE_DIRECTORY}\\.venv\\Scripts\\python.exe")
        else()
            set(PYTHON_EXECUTABLE "${BASE_DIRECTORY}/.venv/bin/python")
        endif()

        add_custom_command(
            OUTPUT ${AUTOGEN_STAMP}
            COMMAND cd ${BASE_DIRECTORY} && ${PYTHON_EXECUTABLE} helper.py run autogen
            COMMAND ${CMAKE_COMMAND} -E touch ${AUTOGEN_STAMP}
            DEPENDS ${AUTOGEN_TRACKING_HEADERS} ${TEMPLATE_FILES}
            COMMENT "Running autogen script"
        )

        add_custom_target(run_autogen ALL DEPENDS ${AUTOGEN_TRACKING_HEADERS} ${AUTOGEN_STAMP} ${TEMPLATE_FILES})
        message(STATUS "Setup autogen target")
    endif()
endmacro()
