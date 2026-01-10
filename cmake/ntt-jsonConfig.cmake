set(LIB_NAME ntt-json)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET nlohmann)
    include(FetchContent)

    FetchContent_Declare(
        nlohmann
        GIT_REPOSITORY https://github.com/nlohmann/json.git
    )

    FetchContent_MakeAvailable(nlohmann)
endif()

add_library(${LIB_NAME} INTERFACE)
target_link_libraries(${LIB_NAME} INTERFACE nlohmann_json::nlohmann_json)