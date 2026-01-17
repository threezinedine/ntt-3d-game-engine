set(LIB_NAME ntt-glfw)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET glfw)
    include(FetchContent)

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/glfw
    )

    FetchContent_MakeAvailable(glfw)
endif()

add_library(${LIB_NAME} INTERFACE)
target_link_libraries(${LIB_NAME} INTERFACE glfw)
set(${LIB_NAME}_FOUND TRUE)