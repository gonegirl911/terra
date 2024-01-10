cmake_minimum_required(VERSION 3.5)

project(terra LANGUAGES CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE include)
add_subdirectory(extern/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF COMPILE_WARNING_AS_ERROR ON)
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()