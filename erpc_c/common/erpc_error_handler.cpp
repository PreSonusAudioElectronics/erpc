/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_error_handler.h"
#include "erpc_debug.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

bool g_erpc_error_occurred = false;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void erpc_error_handler(erpc_status_t err, uint32_t functionID)
{
    if (err == (erpc_status_t)kErpcStatus_Success)
    {
        return;
    }

    switch (err)
    {
        case (erpc_status_t)kErpcStatus_Fail:
            ERPC_TRACEF("\r\nGeneric failure.");
            break;

        case (erpc_status_t)kErpcStatus_InvalidArgument:
            ERPC_TRACEF("\r\nArgument is an invalid value.");
            break;

        case (erpc_status_t)kErpcStatus_Timeout:
            ERPC_TRACEF("\r\nOperated timed out.");
            break;

        case (erpc_status_t)kErpcStatus_InvalidMessageVersion:
            ERPC_TRACEF("\r\nMessage header contains an unknown version.");
            break;

        case (erpc_status_t)kErpcStatus_ExpectedReply:
            ERPC_TRACEF("\r\nExpected a reply message but got another message type.");
            break;

        case (erpc_status_t)kErpcStatus_CrcCheckFailed:
            ERPC_TRACEF("\r\nMessage is corrupted.");
            break;

        case (erpc_status_t)kErpcStatus_BufferOverrun:
            ERPC_TRACEF("\r\nAttempt to read or write past the end of a buffer.");
            break;

        case (erpc_status_t)kErpcStatus_UnknownName:
            ERPC_TRACEF("\r\nCould not find host with given name.");
            break;

        case (erpc_status_t)kErpcStatus_ConnectionFailure:
            ERPC_TRACEF("\r\nFailed to connect to host.");
            break;

        case (erpc_status_t)kErpcStatus_ConnectionClosed:
            ERPC_TRACEF("\r\nConnected closed by peer.");
            break;

        case (erpc_status_t)kErpcStatus_MemoryError:
            ERPC_TRACEF("\r\nMemory allocation error.");
            break;

        case (erpc_status_t)kErpcStatus_ServerIsDown:
            ERPC_TRACEF("\r\nServer is stopped.");
            break;

        case (erpc_status_t)kErpcStatus_InitFailed:
            ERPC_TRACEF("\r\nTransport layer initialization failed.");
            break;

        case (erpc_status_t)kErpcStatus_ReceiveFailed:
            ERPC_TRACEF("\r\nFailed to receive data.");
            break;

        case (erpc_status_t)kErpcStatus_SendFailed:
            ERPC_TRACEF("\r\nFailed to send data.");
            break;

        /* unhandled error */
        default:
            ERPC_TRACEF("\r\nUnhandled error occurred.");
            break;
    }

    /* When error occurred on client side. */
    if (functionID != 0U)
    {
        ERPC_TRACEF("Function id '%u'.", (unsigned int)functionID);
    }
    ERPC_TRACEF("\r\n");

    /* error occurred */
    g_erpc_error_occurred = true;
}
