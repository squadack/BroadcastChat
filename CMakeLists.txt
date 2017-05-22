cmake_minimum_required (VERSION 2.8)

project (BroadcastChat)

find_package (Qt5 COMPONENTS Core Network Widgets)

set (EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set (CMAKE_CXX_FLAGS "-std=c++17 -Wall -ggdb")
set (CMAKE_AUTOMOC ON)

add_definitions (${QT_DEFINITIONS})

add_subdirectory (src)
