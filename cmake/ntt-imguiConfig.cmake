set(LIB_NAME ntt-imgui)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET imgui)
    include(FetchContent)

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/threezinedine/imgui.git
        GIT_TAG d463a66762e4fc0cf4ffb8bb7333e175739d0daf
        SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/imgui
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