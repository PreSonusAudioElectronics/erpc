/*
*   Throwaway redirection of NXP erpc example to zephyr
*/
#ifndef _fsl_redirect_h
#define _fsl_redirect_h

#include <kernel.h>

#define PRINTF (printk)

#endif // _fsl_redirect_h
