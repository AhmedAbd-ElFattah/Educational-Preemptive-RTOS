/*
 * File Name --> EduRTOS.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Include this file and configure RTOS through "EduRTOS_Config.h" file and
 * based on configuration it will include all other files and APIs.
 * */

#ifndef EDURTOS_H_
#define EDURTOS_H_

/*
==================================================
  Start Section --> File Includes
==================================================
 */

#include "EduRTOS_Config.h"
#include "EduRTOS_defines.h"

#include "Task/task.h"


#if EduRTOS_Config_USE_MUTEX
#include "Mutex/mutex.h"
#endif


#if EduRTOS_Config_USE_SEMAPHORE
#include "Semaphore/semaphore.h"
#endif


#if EduRTOS_Config_USE_MAILBOX
#include "Mailbox/mailbox.h"
#endif

/*
==================================================
  End Section --> File Includes
==================================================
 */

/*
==================================================
  Start Section --> APIs
==================================================
 */
void EduRTOS_Init(void);
void EduRTOS_ErrorHandler(void);
/*
==================================================
  End Section --> APIs
==================================================
 */



#endif /* EDURTOS_H_ */
