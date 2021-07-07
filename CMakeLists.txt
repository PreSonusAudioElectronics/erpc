
CMAKE_MINIMUM_REQUIRED (VERSION 3.1)

# Shared across all erpc instances
target_include_directories( pep PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/config
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/port
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/setup
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/transports
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/common
)

target_sources(pep PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_crc16.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_client_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_message_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_basic_codec.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/setup/erpc_client_setup.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/setup/erpc_setup_mbf_dynamic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/common/erpc_error_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/setup/erpc_server_setup.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_simple_server.cpp
    ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_server.cpp 
)

if("${TARGET_ENVIRONMENT}" STREQUAL "linux_pepdev" OR "linux")
    target_sources(pep PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/port/erpc_serial.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/port/erpc_threading_pthreads.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_framed_transport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/setup/erpc_setup_serial.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/transports/erpc_serial_transport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/linux/erpc_setup_serial_ext.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/linux/erpc_serial_transport_ext.cpp  
    )

    target_include_directories(pep PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/common/linux
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/linux
    )
elseif("${TARGET_ENVIRONMENT}" STREQUAL "zephyr")
    message("Zephyr eRPC adding sources relative to: ${CMAKE_CURRENT_LIST_DIR}")
    target_sources(pep PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/port/erpc_port_zephyr.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/port/erpc_threading_zephyr.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/setup/erpc_setup_rpmsg_tty_rtos_remote.cpp 
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/transports/erpc_rpmsg_tty_rtos_transport.cpp        
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/setup/erpc_setup_mbf_rpmsg.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_client_manager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_framed_transport.cpp

        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos/erpc_rpmsg_manager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos/erpc_ext_transport_setup.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos/erpc_rpmsg_tty_rtos_multiept_transport.cpp
    )
    target_include_directories(pep PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/common/zephyr
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos
        )
    target_compile_definitions(pep PUBLIC ERPC_BASE_TRANSPORT_NOT_STATIC)
else()
    message(FATAL_ERROR"Invalid TARGET_ENVIRONMENT specified!")
endif()

message("done erpc CMakeLists.txt")
