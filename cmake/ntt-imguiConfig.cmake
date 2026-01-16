set(LIB_NAME ntt-imgui)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET imgui)
    include(FetchContent)

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/threezinedine/imgui.git
        GIT_TAG 402784182f8c7633d366793c509d6a6a1dbb8b6c
    )

    FetchContent_MakeAvailable(imgui)
endif()

add_library(
    ${LIB_NAME}
    INTERFACE
)

target_link_libraries(
    ${LIB_NAME}
    INTERFACE
    imgui
)