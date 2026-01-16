set(LIB_NAME ntt-imgui)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET imgui)
    include(FetchContent)

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/threezinedine/imgui.git
        GIT_TAG v1.89.9-docking
    )

    FetchContent_MakeAvailable(imgui)
endif()

add_library(
    ${LIB_NAME}
    INTERFACE
    imgui
)