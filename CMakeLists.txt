cmake_minimum_required(VERSION 3.14)
project(terra LANGUAGES C CXX)
include(FetchContent)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE include)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_Declare(webgpu GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
                            GIT_TAG wgpu)
FetchContent_Declare(glfw3webgpu GIT_REPOSITORY https://github.com/eliemichel/glfw3webgpu.git
                                 GIT_TAG main)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git)
FetchContent_MakeAvailable(glfw webgpu glfw3webgpu glm)
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
