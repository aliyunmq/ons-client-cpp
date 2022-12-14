cmake_minimum_required(VERSION 3.19)
project(rocketmq)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(gRPC_DEBUG ON)

# Assume gRPC is installed $HOME/grpc
list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/grpc)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(absl REQUIRED)
find_package(OpenSSL REQUIRED)

add_subdirectory(proto)

add_library(api INTERFACE)
target_include_directories(api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/api)

add_subdirectory(third_party)

add_subdirectory(src/main/cpp)

option(BUILD_EXAMPLES "Build example programs or not" ON)
if (BUILD_EXAMPLES)
    message("Would build examples")
    find_package(ZLIB REQUIRED)
    add_subdirectory(examples)
endif ()