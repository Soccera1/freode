cmake_minimum_required(VERSION 3.0.0)

project(cocos-headers CXX)

add_library(cocos2d INTERFACE)

target_include_directories(cocos2d INTERFACE 
  include
  extensions
)

target_link_libraries(cocos2d INTERFACE
  ${FREOD_LOADER_PATH}/include/link/libcocos2d.lib
  ${FREOD_LOADER_PATH}/include/link/libExtensions.lib
)
