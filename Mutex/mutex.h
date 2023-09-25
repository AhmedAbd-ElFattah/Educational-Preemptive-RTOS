/*
 * File Name --> mutex.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Provides API for Mutex.
 * */

#ifndef MUTEX_H_
#define MUTEX_H_
/*
==================================================
  Start Section --> File Includes
==================================================
 */
#include "EduRTOS_Config.h"
#include "EduRTOS_defines.h"
/*
==================================================
  End Section --> File Includes
==================================================
 */

/*
==================================================
  Start Section --> Mutex defines
==================================================
 */
typedef enum{
	EduRTOS_Mutex_State_Locked,
	EduRTOS_Mutex_State_Unlocked,
}EduMutex_State;

typedef struct {
	EduMutex_State Mutex_State;
	Edu_u32 TaskLockId;
}Mutex;
/*
==================================================
  End Section --> Mutex defines
==================================================
 */

/*
==================================================
  Start Section --> APIs
==================================================
 */

/* @brief  					--> Initialize Mutex.
 * @param  Arg_Mutex_pMutex --> Pointer of Mutex.
 * @retval --> Status if it's initialized successfully or not.
 * */
EduRTOS_StdType_Status MutexInit (Mutex * const Arg_Mutex_pMutex);

/* @brief  					--> Lock mutex.
 * @param  Arg_Mutex_pMutex --> Pointer of Mutex.
 * @param  Arg_TicksToWait  --> Timeout value for locking mutex.
 * @retval --> Status if mutex successfully locked or not.
 * */
EduRTOS_StdType_Status MutexLock (Mutex * const Arg_Mutex_pMutex, const Edu_u32 Arg_TicksToWait);

/* @brief  					--> Release mutex.
 * @param  Arg_Mutex_pMutex --> Pointer of Mutex.
 * @param  Arg_TicksToWait  --> Timeout value for releasing mutex.
 * @retval --> Status if mutex successfully released or not.
 * */
EduRTOS_StdType_Status MutexRelease(Mutex *const Arg_Mutex_pMutex, const Edu_u32 Arg_TicksToWait);

/* @brief  					--> Get status mutex whether it's locked or not.
 * @param  Arg_Mutex_pMutex --> Pointer of Mutex.
 * @retval --> Status if mutex is locked or not.
 * */
EduMutex_State GetMutexStatus(Mutex * const Arg_Mutex_pMutex);
/*
==================================================
  End Section --> APIs
==================================================
 */
#endif /* MUTEX_H_ */
