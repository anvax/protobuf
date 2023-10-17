cmake_minimum_required(VERSION 3.16)

project(testProto)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
add_test(test ${EXECUTABLE_OUTPUT_PATH}/testProto)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(testProto
        Test_DelimitedMessagesStreamParser.cpp
        Test_Helpers.cpp
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ${CMAKE_SOURCE_DIR}/src/protobuf_parser/DelimitedMessagesStreamParser.hpp)
target_link_libraries(testProto protobuf gtest gmock pthread)
