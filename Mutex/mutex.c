/*
 * File Name --> mutex.c
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Mutex implementation.
 * */

#include "mutex.h"
#include "Task/task.h"

EduRTOS_StdType_Status MutexInit (Mutex * const Arg_Mutex_pMutex){
	if (EduRTOS_NULL == Arg_Mutex_pMutex){
		return EduRTOS_StdType_Status_Fail;
	}
	/*Initialize in released state (unlocked)*/
	Arg_Mutex_pMutex->Mutex_State = EduRTOS_Mutex_State_Unlocked;
	/*Task that locks Mutex, It's the only task  that can release it*/
	return EduRTOS_StdType_Status_Succeed;
}


EduRTOS_StdType_Status MutexLock (Mutex * const Arg_Mutex_pMutex, const Edu_u32 Arg_TicksToWait){
	if (EduRTOS_NULL == Arg_Mutex_pMutex){
		return EduRTOS_StdType_Status_Fail;
	}
	/*If Mutex is already locked, block task till mutex is released.*/
	if(EduRTOS_Mutex_State_Locked == __LDREXW( (Edu_u32 *) &Arg_Mutex_pMutex->Mutex_State)){
		/*Block current*/
		EduRTOS_TaskBlock_Mutex(Arg_Mutex_pMutex);
	}

	/* Calculate Maximum number of ticks that can task wait, till it's unlocked.
	 * For example, assume current tick number is 50 and Arg_TicksToWait = 60,
	 * So Mutex will fail to lock when system ticks = (60+50) = 120
	 * */
	const Edu_u32 WaitToTicks = Arg_TicksToWait + EduRTOS_GetCurrentTickNumber();
	while(1){

		/*Check whether time to lock, exceed configured time or not.*/
		if ((Arg_TicksToWait != EduRTOS_INFINITE) && (WaitToTicks > EduRTOS_GetCurrentTickNumber())){
			return EduRTOS_StdType_Status_Timeout;
		}

		/*Exclusive load of mutex value*/
		if (EduRTOS_Mutex_State_Unlocked == __LDREXW( (Edu_u32 *) &Arg_Mutex_pMutex->Mutex_State)){
			/*Exclusive store mutex value*/
			if (0 == __STREXW(EduRTOS_Mutex_State_Locked,(Edu_u32 *)  &Arg_Mutex_pMutex->Mutex_State)){
				__DMB();
				/*Store task id that locked mutex.*/
				Arg_Mutex_pMutex->TaskLockId = EduRTOS_GetCurrentRunningTask();
				break;
			}
		}
	}
	return EduRTOS_StdType_Status_Succeed;
}


EduRTOS_StdType_Status MutexRelease(Mutex *const Arg_Mutex_pMutex, const Edu_u32 Arg_TicksToWait){
	if (EduRTOS_NULL == Arg_Mutex_pMutex){
		return EduRTOS_StdType_Status_Fail;
	}
	/* Calculate Maximum number of ticks that can task wait, till it's unlocked.
	 * For example, assume current tick number is 50 and Arg_TicksToWait = 60,
	 * So Mutex will fail to release when system ticks = (60+50) = 120
	 * */
	const Edu_u32 WaitToTicks = Arg_TicksToWait + EduRTOS_GetCurrentTickNumber();

	while(1){
		/*Check whether time to lock, exceed configured time or not.*/
		if ((Arg_TicksToWait != EduRTOS_INFINITE) && (WaitToTicks > EduRTOS_GetCurrentTickNumber())){
			return EduRTOS_StdType_Status_Timeout;
		}
		__DMB();
		/*It it was unlocked, no other operations needed so return EduRTOS_StdType_Status_Succeed*/
		if (EduRTOS_Mutex_State_Unlocked == Arg_Mutex_pMutex->Mutex_State ){
			/*Unblock tasks that was blocked due to mutex's lock*/
			EduRTOS_TaskUnBlock_Mutex(Arg_Mutex_pMutex);
			return EduRTOS_StdType_Status_Succeed;
		}

		if (EduRTOS_GetCurrentRunningTask() == __LDREXW(&Arg_Mutex_pMutex->TaskLockId)){
			if (EduRTOS_Mutex_State_Locked == __LDREXW(( Edu_u32 * )&Arg_Mutex_pMutex->Mutex_State)){
				/*Unlock mutex*/
				Arg_Mutex_pMutex->Mutex_State = EduRTOS_Mutex_State_Unlocked;
			}
			/*Unblock tasks that was blocked due to mutex's lock*/
			EduRTOS_TaskUnBlock_Mutex(Arg_Mutex_pMutex);
			return EduRTOS_StdType_Status_Succeed;
		}
	}
}

EduMutex_State GetMutexStatus(Mutex * const Arg_Mutex_pMutex){
	return __LDREXW(( Edu_u32 * )&Arg_Mutex_pMutex->Mutex_State);
}
