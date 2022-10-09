cmake_minimum_required(VERSION 3.7.0)

project(Communication)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# This one is important
SET(CMAKE_SYSTEM_NAME Linux)

# Specify the cross compiler

set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_BUILD_TYPE Debug)

include(GNUInstallDirs)

function(add_test_unit BIN_NAME BIN SOURCE)
  add_executable(${BIN} ${SOURCE})
  set_target_properties(${BIN}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_lib
    )
    add_test(NAME ${BIN_NAME} COMMAND ${BIN})
endfunction()

enable_testing()

set (sources 
        ${CMAKE_CURRENT_LIST_DIR}/src/udp/udp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/udp/client/udp_client.c
        ${CMAKE_CURRENT_LIST_DIR}/src/udp/server/udp_server.c
        )
set (includes
        ${CMAKE_CURRENT_LIST_DIR}/include/udp
        ${CMAKE_CURRENT_LIST_DIR}/src/udp/client
        ${CMAKE_CURRENT_LIST_DIR}/src/udp/server
        )

add_library(communication SHARED "")
target_sources(communication 
  PUBLIC 
  "${sources}"
  )
target_include_directories(communication
  PUBLIC 
  "${includes}"
  )

add_library(communication_static STATIC "")
target_sources(communication_static 
  PUBLIC 
  "${sources}"
  )
target_include_directories(communication_static
  PUBLIC 
  "${includes}"
  )

add_test_unit(client client_test tests/client_test.c)
target_link_libraries(client_test PUBLIC communication)

add_test_unit(server server_test tests/server_test.c)
target_link_libraries(server_test PUBLIC communication)

