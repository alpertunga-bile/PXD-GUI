cmake_minimum_required(VERSION 3.0.0)
project(gui VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB cpp_source_files "${CMAKE_SOURCE_DIR}/sources/*.cpp")
file(GLOB lib_files "${CMAKE_SOURCE_DIR}/lib/*.lib")
file(GLOB imgui_source_files "${CMAKE_SOURCE_DIR}/sources/imgui/*.cpp")

include_directories(includes)
add_executable(${PROJECT_NAME} main.cpp ${cpp_source_files} ${imgui_source_files})

target_link_libraries(${PROJECT_NAME} ${lib_files} opengl32)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)