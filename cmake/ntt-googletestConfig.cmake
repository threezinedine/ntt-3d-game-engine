set(LIB_NAME ntt-googletest)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET gtest)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
        SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/googletest
    )

    FetchContent_MakeAvailable(googletest)
endif()

add_library(
    ${LIB_NAME}
    INTERFACE
)

target_link_libraries(
    ${LIB_NAME}
    INTERFACE
    gtest
    gtest_main
    gmock
    gmock_main
)