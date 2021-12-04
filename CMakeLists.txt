
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
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/linux/erpcsetupserialext.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/linux/erpcserialtransportext.cpp  
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

        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos/erpcrpmsgmanager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos/erpcexttransportsetup.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos/erpcrpmsgttyrtosmultiepttransport.cpp
    )
    target_include_directories(pep PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/common/zephyr
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos
        )
    target_compile_definitions(pep PUBLIC ERPC_BASE_TRANSPORT_NOT_STATIC)
elseif("${TARGET_ENVIRONMENT}" STREQUAL "lk")
    message("Littlekernel eRPC adding sources relative to: ${CMAKE_CURRENT_LIST_DIR}")
    target_sources(pep PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/port/erpc_port_littlekernel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/port/erpc_threading_littlekernel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/infra/erpc_framed_transport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/port/erpc_port_littlekernel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/transports/erpc_transport_callback.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/setup/erpc_setup_callback.cpp
    )
    target_include_directories(pep PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/erpc_c/common/littlekernel
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/rtos
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/transports
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/setup
        ${CMAKE_CURRENT_LIST_DIR}/../erpcextensions/port
    )

    target_compile_definitions(pep PUBLIC
        ERPC_ENV_IS_LITTLEKERNEL=1
    )
else()
    message(FATAL_ERROR"Invalid TARGET_ENVIRONMENT specified!")
endif()

message("done erpc CMakeLists.txt")
