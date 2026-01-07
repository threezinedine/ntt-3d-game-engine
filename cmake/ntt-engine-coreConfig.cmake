set(LIB_NAME EngineCore)

if (TARGET ${LIB_NAME})
    return()
endif()

set(LIB_DIRECTORY "${BASE_DIRECTORY}/core")

add_subdirectory(
    ${LIB_DIRECTORY} ${CMAKE_BINARY_DIR}/packages/core
)