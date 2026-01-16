set(LIB_NAME ntt-imgui)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET imgui)
    include(FetchContent)

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/threezinedine/imgui.git
        GIT_TAG 8bb872af95fa3a2043c1f49863854f49f26e6f55
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