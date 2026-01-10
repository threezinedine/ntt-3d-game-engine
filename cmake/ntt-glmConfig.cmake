set(LIB_NAME ntt-glm)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET glm)
    include(FetchContent)

    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
    )

    FetchContent_MakeAvailable(glm)
endif()

add_library(${LIB_NAME} INTERFACE)
target_link_libraries(${LIB_NAME} INTERFACE glm)