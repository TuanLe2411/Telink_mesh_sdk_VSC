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
#define GATEWAY_ENABLE			    1
#define USER_INTERFACE              0
/* uart config */
#define HCI_USE_UART                1
#define HCI_USE_NONE                0
#define HCI_ACCESS                  HCI_USE_UART

/* define led pin */
#define PWM_R                       GPIO_PC4 // red led
#define PWM_G                       GPIO_PC3 // green led
#define PWM_B                       GPIO_PC2 // blue led
#define PWM_W                       GPIO_PWM4B1		//white

#define PWM_FUNC_R  AS_PWM  // AS_PWM_SECOND
#define PWM_FUNC_G  AS_PWM  // AS_PWM_SECOND
#define PWM_FUNC_B  AS_PWM  // AS_PWM_SECOND
#define PWM_FUNC_W  AS_PWM  // AS_PWM_SECOND

#define PWMID_R     (GET_PWMID(PWM_R, PWM_FUNC_R))
#define PWMID_G     (GET_PWMID(PWM_G, PWM_FUNC_G))
#define PWMID_B     (GET_PWMID(PWM_B, PWM_FUNC_B))
#define PWMID_W     (GET_PWMID(PWM_W, PWM_FUNC_W))
                    
#define PWM_INV_R   (GET_PWM_INVERT_VAL(PWM_R, PWM_FUNC_R))
#define PWM_INV_G   (GET_PWM_INVERT_VAL(PWM_G, PWM_FUNC_G))
#define PWM_INV_B   (GET_PWM_INVERT_VAL(PWM_B, PWM_FUNC_B))
#define PWM_INV_W   (GET_PWM_INVERT_VAL(PWM_W, PWM_FUNC_W))

#define GPIO_LED	PWM_R

#define UART_TX_PIN         UART_TX_PD7
#define UART_RX_PIN         UART_RX_PA0

//----------------------- GPIO for UI --------------------------------
//---------------  Button 
#if (PCBA_8258_SEL == PCBA_8258_DONGLE_48PIN)
#define PULL_WAKEUP_SRC_PD6     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PD5     PM_PIN_PULLUP_1M	//btn
#define PD6_INPUT_ENABLE		1
#define PD5_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PD6
#define	SW2_GPIO				GPIO_PD5
#else   // PCBA_8258_DEVELOPMENT_BOARD
#define PULL_WAKEUP_SRC_PD2     PM_PIN_PULLUP_1M	//btn
#define PULL_WAKEUP_SRC_PD1     PM_PIN_PULLUP_1M	//btn
#define PD2_INPUT_ENABLE		1
#define PD1_INPUT_ENABLE		1
#define	SW1_GPIO				GPIO_PD2
#define	SW2_GPIO				GPIO_PD1
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif