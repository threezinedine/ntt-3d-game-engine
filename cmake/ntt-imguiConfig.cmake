set(LIB_NAME ntt-imgui)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET imgui)
    include(FetchContent)

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/threezinedine/imgui.git
        GIT_TAG 6886265fbd939539fcef5c15092f587ddb9ac6b5
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