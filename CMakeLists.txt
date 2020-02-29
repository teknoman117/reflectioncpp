cmake_minimum_required(VERSION 2.8)
project (reflectioncpp)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11 -g -pg")

include_directories(${PROJECT_SOURCE_DIR}/include)

# Compile the ReflectionCpp library
#add_library(reflectioncpp   common.cpp
#							primitive.cpp)

add_executable(reflectioncpp_test src/test/test.cpp)
#target_link_libraries(reflectioncpp_test reflectioncpp boost_system boost_thread)
