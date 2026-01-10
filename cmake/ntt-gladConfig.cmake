set(LIB_NAME ntt-glad)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET glad)
    include(FetchContent)

    FetchContent_Declare(
        GLAD
        GIT_REPOSITORY https://github.com/threezinedine/glad.git
    )

    FetchContent_MakeAvailable(glad)
endif()

if (NOT TARGET OpenGL::GL)
    find_package(OpenGL REQUIRED)
endif()

add_library(${LIB_NAME} INTERFACE)
target_link_libraries(${LIB_NAME} INTERFACE glad OpenGL::GL)