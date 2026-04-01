cmake_minimum_required(VERSION 3.16)
project(parametrization)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(libigl)
igl_include(glfw)

# Add your project files
include_directories("include/")
if(USE_SOLUTION)
  file(GLOB SRCFILES solution/*.cpp)
else()
  file(GLOB SRCFILES src/*.cpp)
endif()

add_library(core ${SRCFILES})
target_link_libraries(core
  igl::core
  igl::opengl
  igl::glfw
)

add_executable(parametrization "main.cpp")
target_link_libraries(parametrization
  core
  igl::core
  igl::opengl
  igl::glfw
)

# tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)
# Enable testing
enable_testing()
add_subdirectory(tests)
