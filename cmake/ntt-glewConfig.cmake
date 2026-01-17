set(LIB_NAME ntt-glew)

if (TARGET ${LIB_NAME})
    return()
endif()

set(glew-cmake_BUILD_SHARED OFF)
set(glew-cmake_BUILD_STATIC ON)
set(ONLY_LIBS ON)

if (NOT TARGET glew)
    include(FetchContent)

    FetchContent_Declare(
        glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/glew
    )

    FetchContent_MakeAvailable(glew)
endif()

if (NOT TARGET OpenGL::GL)
    find_package(OpenGL REQUIRED)
endif()

add_library(${LIB_NAME} INTERFACE)
target_link_libraries(${LIB_NAME} INTERFACE libglew_static OpenGL::GL)
