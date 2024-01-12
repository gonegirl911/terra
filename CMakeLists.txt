cmake_minimum_required(VERSION 3.5)

project(terra LANGUAGES C CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE include)
add_subdirectory(extern/glfw)
add_subdirectory(extern/webgpu)
add_subdirectory(extern/glfw3webgpu)
add_subdirectory(extern/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw webgpu glfw3webgpu glm::glm)
add_custom_target(assets COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
                                                                    ${CMAKE_BINARY_DIR}/assets)
add_dependencies(${PROJECT_NAME} assets)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

target_copy_webgpu_binaries(${PROJECT_NAME})
