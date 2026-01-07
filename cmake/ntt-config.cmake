macro(ntt_configure)
    option(CMAKE_BUILD_TYPE "Build type" "Debug") 
    set(TARGET_COMPILE_OPTIONS)
    set(TARGET_DEFINITIONS)

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        list(APPEND TARGET_COMPILE_OPTIONS "-DDEBUG=0" "-O3" "-DNDEBUG")
        list(APPEND TARGET_DEFINITIONS "NDEBUG=1")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Test")
        list(APPEND TARGET_DEFINITIONS "TEST=1")
        list(APPEND TARGET_COMPILE_OPTIONS "-DDEBUG=1" "-O0" "-g" "-DRUN_TESTS=1")
    else()
        list(APPEND TARGET_COMPILE_OPTIONS "-DDEBUG=1" "-O0" "-g")
        list(APPEND TARGET_DEFINITIONS "DEBUG=1")
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
