cmake_minimum_required(VERSION 3.16)

project(testProto)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
add_test(test ${EXECUTABLE_OUTPUT_PATH}/testProto)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(testProto
        tests.cpp
        Test_DelimitedMessagesStreamParser.cpp
        Test_Helpers.cpp
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ${CMAKE_SOURCE_DIR}/src/protobuf_parser/DelimitedMessagesStreamParser.hpp
        )
if(WIN32 AND Protobuf_VERSION VERSION_GREATER_EQUAL 4.22.0)
        if (MSVC)
                target_link_libraries(testProto PRIVATE abseil_dll protobuf gtest gmock pthread)
        else()
                target_link_libraries(testProto PRIVATE absl_log_internal_message absl_log_internal_check_op protobuf gtest gmock pthread)
        endif()
endif()

