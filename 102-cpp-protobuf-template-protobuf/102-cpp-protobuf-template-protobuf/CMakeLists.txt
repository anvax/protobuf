cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0016 NEW)

project(protobuf_parser)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/src)



find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS message.proto)

protobuf_generate_python(PROTO_PY pr.proto)
add_custom_target(pyProto ALL
        DEPENDS ${PROTO_PY})
add_executable(protobuf_parser
        src/main.cpp
        src/protobuf_parser/DelimitedMessagesStreamParser.hpp
        src/protobuf_parser/helpers.hpp
        ${PROTO_HDRS}
        ${PROTO_SRCS}
        src/protobuf_parser/DelimitedMessagesStreamParser.cpp)

if(WIN32 AND Protobuf_VERSION VERSION_GREATER_EQUAL 4.22.0)
    if (MSVC)
        target_link_libraries(protobuf_parser PRIVATE abseil_dll protobuf)
    else()
        target_link_libraries(protobuf_parser PRIVATE absl_log_internal_message absl_log_internal_check_op protobuf)
    endif()
endif()

enable_testing()
add_subdirectory(test)