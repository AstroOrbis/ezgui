cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

option(BUILD_DEMO "Build demo" OFF)

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
	set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()
include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")


project(ezgui)

file(GLOB_RECURSE SOURCES "src/*.cpp")
include_directories("src")
add_library(ezgui ${SOURCES})


find_package(imgui CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(ezgui PRIVATE SDL2::SDL2 imgui::imgui OpenGL::GL)

if(BUILD_DEMO)
	add_executable(demo "demo/main.cpp")
	target_link_libraries(demo PRIVATE ezgui)
endif()