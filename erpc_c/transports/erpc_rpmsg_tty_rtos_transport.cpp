/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017-2020 NXP
 * Copyright 2019-2021 ACRIOS Systems s.r.o.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_rpmsg_tty_rtos_transport.h"

#include "erpc_config_internal.h"
#include "erpc_framed_transport.h"

#include "rpmsg_ns.h"

#include <cassert>

using namespace erpc;
using namespace std;

static const char * const kImxRpmsgTtyDriverMsg = "hello world!";

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#ifndef ERPC_BASE_TRANSPORT_NOT_STATIC
uint8_t RPMsgBaseTransport::s_initialized = 0U;
struct rpmsg_lite_instance *RPMsgBaseTransport::s_rpmsg;
#endif

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

RPMsgTTYRTOSTransport::RPMsgTTYRTOSTransport(void)
: RPMsgBaseTransport()
, m_dst_addr(0)
, m_rpmsg_queue(NULL)
, m_rpmsg_ept(NULL)
, m_crcImpl(NULL)
{
}

RPMsgTTYRTOSTransport::~RPMsgTTYRTOSTransport(void)
{
    bool skip = false;
    if (s_rpmsg != RL_NULL)
    {
        if (m_rpmsg_ept != RL_NULL)
        {
            if (RL_SUCCESS != rpmsg_lite_destroy_ept(s_rpmsg, m_rpmsg_ept))
            {
                skip = true;
            }
        }

        if (!skip && (m_rpmsg_queue != RL_NULL))
        {
            if (RL_SUCCESS != rpmsg_queue_destroy(s_rpmsg, m_rpmsg_queue))
            {
                skip = true;
            }
        }

        if (!skip && (RL_SUCCESS != rpmsg_lite_deinit(s_rpmsg)))
        {
            skip = true;
        }

        if (!skip)
        {
            s_initialized = 0U;
        }
    }
}

void RPMsgTTYRTOSTransport::setCrc16(Crc16 *crcImpl)
{
    assert(crcImpl);
    m_crcImpl = crcImpl;
}

erpc_status_t RPMsgTTYRTOSTransport::init(uint32_t src_addr, uint32_t dst_addr, void *base_address, uint32_t length,
                                          uint32_t rpmsg_link_id)
{
    erpc_status_t status = kErpcStatus_Success;

    if (0U == s_initialized)
    {
        s_rpmsg = rpmsg_lite_master_init(base_address, length, rpmsg_link_id, RL_NO_FLAGS);
        if (s_rpmsg == RL_NULL)
        {
            status = kErpcStatus_InitFailed;
        }

        if (status == kErpcStatus_Success)
        {
            m_rpmsg_queue = rpmsg_queue_create(s_rpmsg);
            if (m_rpmsg_queue == RL_NULL)
            {
                status = kErpcStatus_InitFailed;
            }
        }

        if (status == kErpcStatus_Success)
        {
            m_rpmsg_ept = rpmsg_lite_create_ept(s_rpmsg, src_addr, rpmsg_queue_rx_cb, m_rpmsg_queue);
            if (m_rpmsg_ept == RL_NULL)
            {
                status = kErpcStatus_InitFailed;
            }
            else
            {
                m_dst_addr = dst_addr;
                s_initialized = 1U;
            }
        }

        if (status != kErpcStatus_Success)
        {
            if (m_rpmsg_queue != RL_NULL)
            {
                if (RL_SUCCESS == rpmsg_queue_destroy(s_rpmsg, m_rpmsg_queue))
                {
                    m_rpmsg_queue = NULL;
                }
            }

            if (s_rpmsg != RL_NULL)
            {
                if (RL_SUCCESS == rpmsg_lite_deinit(s_rpmsg))
                {
                    s_rpmsg = NULL;
                }
            }
        }
    }

    return status;
}

erpc_status_t RPMsgTTYRTOSTransport::init(uint32_t src_addr, uint32_t dst_addr, void *base_address,
                                          uint32_t rpmsg_link_id, void (*ready_cb)(void), char *nameservice_name)
{
    erpc_status_t status = kErpcStatus_Success;

    if (0U == s_initialized)
    {
        s_rpmsg = rpmsg_lite_remote_init(base_address, rpmsg_link_id, RL_NO_FLAGS);
        if (s_rpmsg == RL_NULL)
        {
            status = kErpcStatus_InitFailed;
        }

        if (status == kErpcStatus_Success)
        {
            /* Signal the other core we are ready */
            if (ready_cb != NULL)
            {
                ready_cb();
            }

            while (0 == rpmsg_lite_is_link_up(s_rpmsg))
            {
            }

            m_rpmsg_queue = rpmsg_queue_create(s_rpmsg);
            if (m_rpmsg_queue == RL_NULL)
            {
                status = kErpcStatus_InitFailed;
            }
        }

        if (status == kErpcStatus_Success)
        {
            m_rpmsg_ept = rpmsg_lite_create_ept(s_rpmsg, src_addr, rpmsg_queue_rx_cb, m_rpmsg_queue);
            if (m_rpmsg_ept == RL_NULL)
            {
                status = kErpcStatus_InitFailed;
            }
        }

        if (status == kErpcStatus_Success)
        {
            if (NULL != nameservice_name)
            {
                if (RL_SUCCESS != rpmsg_ns_announce(s_rpmsg, m_rpmsg_ept, nameservice_name, (uint32_t)RL_NS_CREATE))
                {
                    status = kErpcStatus_InitFailed;
                }
            }
        }

        if (status == kErpcStatus_Success)
        {
            m_dst_addr = dst_addr;
            s_initialized = 1U;
        }
        else
        {
            if (m_rpmsg_ept != RL_NULL)
            {
                if (RL_SUCCESS == rpmsg_lite_destroy_ept(s_rpmsg, m_rpmsg_ept))
                {
                    m_rpmsg_ept = NULL;
                }
            }

            if (m_rpmsg_queue != RL_NULL)
            {
                if (RL_SUCCESS == rpmsg_queue_destroy(s_rpmsg, m_rpmsg_queue))
                {
                    m_rpmsg_queue = NULL;
                }
            }

            if (s_rpmsg != RL_NULL)
            {
                if (RL_SUCCESS == rpmsg_lite_deinit(s_rpmsg))
                {
                    s_rpmsg = NULL;
                }
            }
        }
    }

    return status;
}

erpc_status_t RPMsgTTYRTOSTransport::receive(MessageBuffer *message)
{
    erpc_status_t status = kErpcStatus_ReceiveFailed;
    FramedTransport::Header h;
    uint32_t headerLength = sizeof(h);
    char *buf = NULL;
    uint32_t lengthReceived = 0;
    int32_t ret_val;

    while( buf == NULL)
    {
        ret_val = rpmsg_queue_recv_nocopy(s_rpmsg, m_rpmsg_queue, &m_dst_addr, &buf, &lengthReceived, RL_DONT_BLOCK);
        env_yield();
    }
    uint16_t computedCrc;

    assert(m_crcImpl && "Uninitialized Crc16 object.");
    assert(buf);

    if (ret_val == RL_SUCCESS)
    {
        /* Here we have to diverge paths because if the client is Python on Linux, it will 
        send the whole message in one chunk, with the CRC calculated over the whole thing,
        whereas the C++ client will separate the header from the message body, send them in 
        two seperate transactions, with the CRC calculated on the message body only.  This is
        a bug that should be done one way or the other in all cases.
        */
        if ( lengthReceived == headerLength )
        {
            /*
            We've only received a header, so release the buffer and recieve the rest of the message
            */
            // copy the header and free the buffer
            memcpy(&h, buf, headerLength );
            int rc = rpmsg_queue_nocopy_free(s_rpmsg, buf);
            if ( rc != RL_SUCCESS )
            {
                return static_cast<erpc_status_t>(rc);
            }

            buf = NULL;
            lengthReceived = 0;
            uint32_t length = 0;

            while( buf == NULL)
            {
                rc = rpmsg_queue_recv_nocopy(s_rpmsg, m_rpmsg_queue, &m_dst_addr, &buf, &length, RL_DONT_BLOCK);
                env_yield();
            }
            if( rc != RL_SUCCESS )
            {
                return static_cast<erpc_status_t>(rc);
            }
            lengthReceived += length;

            message->set(reinterpret_cast<uint8_t*>(buf), h.m_messageSize);

            // buf now presumably contains the message body but not the header
            uint16_t computedCrc = m_crcImpl->computeCRC16(reinterpret_cast<uint8_t*>(buf), h.m_messageSize);

            if ( computedCrc != h.m_crc )
            {
                return static_cast<erpc_status_t>(kErpcStatus_CrcCheckFailed);
            }

            message->setUsed(h.m_messageSize);
            return ret_val != RL_SUCCESS ? (erpc_status_t)kErpcStatus_ReceiveFailed : (erpc_status_t)kErpcStatus_Success;

        }
        else if ( lengthReceived == (headerLength + h.m_messageSize ))
        {
            /* 
            We've alredy received the entire message, so calculate CRC on the whole thing.
            This was how this code came from NXP.
            */

            // not sure why we memcpy here instead of casting a pointer...
            memcpy((uint8_t *)&h, buf, sizeof(h));
            message->set(&((uint8_t *)buf)[sizeof(h)], lengthReceived - sizeof(h));

            /* Verify CRC. */
            computedCrc = m_crcImpl->computeCRC16(&((uint8_t *)buf)[sizeof(h)], h.m_messageSize);
            if (computedCrc != h.m_crc)
            {
                status = kErpcStatus_CrcCheckFailed;
            }
            else
            {
                message->setUsed(h.m_messageSize);
            }
        }
        else
        {
            if( strcmp(kImxRpmsgTtyDriverMsg, buf) == 0 )
            {
                /**
                 * This is the startup message from the Linux Rpmsg TTY
                 * driver.  We don't do anything with it, but we do need
                 * to free the buffer in came in.
                 */
                message->setUsed(0);
                int rc = rpmsg_queue_nocopy_free(s_rpmsg, buf);
                assert( rc == RL_SUCCESS );
            }
            status = kErpcStatus_ReceiveFailed;
        }
    }
    return status;
}

erpc_status_t RPMsgTTYRTOSTransport::send(MessageBuffer *message)
{
    erpc_status_t status = kErpcStatus_Success;
    FramedTransport::Header *header;
    uint32_t headerSize = sizeof(FramedTransport::Header);
    uint8_t *buf = message->get();
    uint32_t length = message->getLength();
    uint32_t used = message->getUsed();
    int32_t ret_val;

    assert(m_crcImpl && "Uninitialized Crc16 object.");
    message->set(NULL, 0);


    /**
     * 
     * Code from NXP was doing this:
     * (void)memcpy(&buf[-sizeof(h)], (uint8_t *)&h, sizeof(h));
     * 
     * Copying the header into memory starting before the start of the buffer.
     * I don't understand how that ever could be expected to work, and as it happens
     * it does not work (crashes the rpmsg system)
     * 
     * The Linux serial (aka tty) transport is a framed transport that expects a header
     * with CRC and message length fields to be sent ahead of the main message body.
     * 
     * I've modified this code to do that here.
     *
     * eRPC transport has already allocated a buffer for the message body and 
     * will later free it.  Here we locally allocate a buffer for
     * the header which is sent separately.  The receiver is responsible for freeing
     * the buffer.
     */

    uint32_t hBufSize;
    uint8_t *headerBuf = nullptr;
    while( headerBuf == nullptr )
    {
        headerBuf = static_cast<uint8_t*>(rpmsg_lite_alloc_tx_buffer(s_rpmsg, &hBufSize, RL_DONT_BLOCK));
        env_yield();
    }

    header = reinterpret_cast<FramedTransport::Header*>( headerBuf );

    // calculate CRC on the message body
    header->m_crc = m_crcImpl->computeCRC16(buf, used);
    header->m_messageSize = used;

    // send header
    int rc = rpmsg_lite_send_nocopy(s_rpmsg, m_rpmsg_ept, m_dst_addr, headerBuf, headerSize );
    if( rc != RL_SUCCESS )
    {
        status = kErpcStatus_ConnectionFailure;
    }

    // Now send the payload
    ret_val = rpmsg_lite_send_nocopy(s_rpmsg, m_rpmsg_ept, m_dst_addr, buf, used);
    if (ret_val == RL_SUCCESS)
    {
        status = kErpcStatus_Success;
    }
    else
    {
        message->set(buf, length);
        message->setUsed(used);
        status = kErpcStatus_SendFailed;
    }

    return status;
}
