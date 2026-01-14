set(LIB_NAME ntt-vulkan)

if (TARGET ${LIB_NAME})
    return()
endif()

if (NOT TARGET Vulkan)
    find_package(glslang CONFIG REQUIRED)
    find_package(Vulkan REQUIRED)
endif()

add_library(
    ${LIB_NAME}
    INTERFACE
)

target_link_libraries(
    ${LIB_NAME}
    INTERFACE
    Vulkan::Vulkan
    glslang::glslang
    glslang::SPIRV
    glslang::glslang-default-resource-limits
)