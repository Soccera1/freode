cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME TestDependency)

project(${PROJECT_NAME} VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

add_compile_definitions(EXPORTING_MOD)

set(FREOD_LINK_SOURCE ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mod.json.in ${CMAKE_CURRENT_SOURCE_DIR}/mod.json)
create_freod_file(${PROJECT_NAME} DONT_INSTALL)
