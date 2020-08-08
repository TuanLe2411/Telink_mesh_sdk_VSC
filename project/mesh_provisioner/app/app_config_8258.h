#pragma once
/* 
    This section define constants of function
    ----------writer by LeTuan------------
*/

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#include "vendor/common/version.h" 

#include "vendor/common/default_config.h"

/* Clock */
#define	CLOCK_SYS_CLOCK_HZ	        16000000
#define CLOCK_SYS_CLOCK_1S          CLOCK_SYS_CLOCK_HZ,
#define CLOCK_SYS_CLOCK_1MS         (CLOCK_SYS_CLOCK_1S / 1000)
#define CLOCK_SYS_CLOCK_1US         (CLOCK_SYS_CLOCK_1S / 1000000)

/* Extern Crystal Type */
#define CRYSTAL_TYPE			    XTAL_12M		//  extern 12M crystal

/* watchdog */
#define MODULE_WATCHDOG_ENABLE		1
#define WATCHDOG_INIT_TIMEOUT		20000  //ms

/* set mesh */
#define DONGLE_PROVISION_EN			1

/* uart config */

#define HCI_USE_UART                2
#define HCI_USE_NONE                0
#define HCI_ACCESS                  HCI_USE_UART

/* define led pin */
#define Led_R                       GPIO_PC4 // red led
#define Led_G                       GPIO_PC3 // green led
#define Led_B                       GPIO_PC2 // blue led

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif