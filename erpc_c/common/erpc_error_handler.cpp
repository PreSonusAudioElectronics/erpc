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
            ERPC_PRINTF("\r\nGeneric failure.\n");
            break;

        case (erpc_status_t)kErpcStatus_InvalidArgument:
            ERPC_PRINTF("\r\nArgument is an invalid value.\n");
            break;

        case (erpc_status_t)kErpcStatus_Timeout:
            ERPC_PRINTF("\r\nOperated timed out.\n");
            break;

        case (erpc_status_t)kErpcStatus_InvalidMessageVersion:
            ERPC_PRINTF("\r\nMessage header contains an unknown version.\n");
            break;

        case (erpc_status_t)kErpcStatus_ExpectedReply:
            ERPC_PRINTF("\r\nExpected a reply message but got another message type.\n");
            break;

        case (erpc_status_t)kErpcStatus_CrcCheckFailed:
            ERPC_PRINTF("\r\nMessage is corrupted.\n");
            break;

        case (erpc_status_t)kErpcStatus_BufferOverrun:
            ERPC_PRINTF("\r\nAttempt to read or write past the end of a buffer.\n");
            break;

        case (erpc_status_t)kErpcStatus_UnknownName:
            ERPC_PRINTF("\r\nCould not find host with given name.\n");
            break;

        case (erpc_status_t)kErpcStatus_ConnectionFailure:
            ERPC_PRINTF("\r\nFailed to connect to host.\n");
            break;

        case (erpc_status_t)kErpcStatus_ConnectionClosed:
            ERPC_PRINTF("\r\nConnected closed by peer.\n");
            break;

        case (erpc_status_t)kErpcStatus_MemoryError:
            ERPC_PRINTF("\r\nMemory allocation error.\n");
            break;

        case (erpc_status_t)kErpcStatus_ServerIsDown:
            ERPC_PRINTF("\r\nServer is stopped.\n");
            break;

        case (erpc_status_t)kErpcStatus_InitFailed:
            ERPC_PRINTF("\r\nTransport layer initialization failed.\n");
            break;

        case (erpc_status_t)kErpcStatus_ReceiveFailed:
            ERPC_PRINTF("\r\nFailed to receive data.\n");
            break;

        case (erpc_status_t)kErpcStatus_SendFailed:
            ERPC_PRINTF("\r\nFailed to send data.\n");
            break;

        /* unhandled error */
        default:
            ERPC_PRINTF("\r\nUnhandled error occurred.\n");
            break;
    }

    /* When error occurred on client side. */
    if (functionID != 0U)
    {
        ERPC_PRINTF("Function id '%u'.", (unsigned int)functionID);
    }
    ERPC_PRINTF("\r\n");

    /* error occurred */
    g_erpc_error_occurred = true;
}
