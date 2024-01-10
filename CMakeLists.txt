cmake_minimum_required(VERSION 3.5)

project(terra LANGUAGES CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
