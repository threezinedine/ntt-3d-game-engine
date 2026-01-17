set(LIB_NAME ntt-pybind)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET pybind11)
    include(FetchContent)

    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.11
        SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/pybind11
    )

    FetchContent_MakeAvailable(pybind11)
endif()

if (NOT TARGET Python3)
    find_package(Python3 REQUIRED)
endif()