/*
 * File Name --> EduRTOS_Config.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Configurations for EduRTOS.
 * */


#ifndef EDUCATIONALRTOS_EDURTOS_CONFIG_H_
#define EDUCATIONALRTOS_EDURTOS_CONFIG_H_

/*Include CMSIS Library here, replace "stm32f401xc.h" with other file is needed.*/
#include "stm32f401xc.h"

/*
==================================================
  Start Section --> Task Configurations
==================================================
 */

#define 	EduRTOS_Config_MAX_NUMBER_OF_TASKS			10

/*
 * To use Idle task callback function user defined, It must has the same following name
 * EduRTOS_IdleTaskCallback
 * */
#define 	EduRTOS_Config_USER_IDLE_TASK				0

/*
==================================================
  End Section --> Task Configurations
==================================================
 */

/*
==================================================
  Start Section --> Memory Configurations
==================================================
 */

#define		EduRTOS_Config_OS_STACK_TOP				0x20010000
#define 	EduRTOS_Config_OS_STACK_SIZE				(5*1024) // 5Kb


#define 	EduRTOS_Config_TASK_MIN_STACK_SIZE			(1024) 	// 1Kb
/*
==================================================
  End Section --> Memory Configurations
==================================================
 */

/*
==================================================
  Start Section --> Clock Configurations
==================================================
 */

#define 	EduRTOS_Config_CLOCK_HZ			16000000
#define 	EduRTOS_Config_Systick_RATE_ms	10
/*
==================================================
  End Section --> Clock Configurations
==================================================
 */


/*
==================================================
  Start Section --> Task Configurations
==================================================
 */

#define 	EduRTOS_Config_USE_MUTEX		1
#define 	EduRTOS_Config_USE_SEMAPHORE	1
#define 	EduRTOS_Config_USE_MAILBOX		1

/*As Mailbox uses semaphore*/
#if (EduRTOS_Config_USE_MAILBOX == 1) && (EduRTOS_Config_USE_SEMAPHORE == 0)
#define 	EduRTOS_Config_USE_SEMAPHORE	1
#endif
/*
==================================================
  End Section --> Task Configurations
==================================================
 */





#endif /* EDUCATIONALRTOS_EDURTOS_CONFIG_H_ */
