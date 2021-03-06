cmake_minimum_required(VERSION 3.8)
project(socketBuf)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/incl/nanopb/extra)
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS})

nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS amessage.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(generate_proto_sources DEPENDS ${PROTO_SRCS} ${PROTO_HDRS})
set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS}
		PROPERTIES GENERATED TRUE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g -O0")

include_directories(
		gen
		)

set(SOURCE_FILES_SEND
		send.c
		gen/amessage.pb.c
		incl/nanopb/pb_encode.c
		incl/nanopb/pb_common.c
		)
add_executable(send ${SOURCE_FILES_SEND})

set(SOURCE_FILES_RECV
		recv.c
		gen/amessage.pb.c
		incl/nanopb/pb_decode.c
		incl/nanopb/pb_common.c
		)

add_executable(recv ${SOURCE_FILES_RECV})
