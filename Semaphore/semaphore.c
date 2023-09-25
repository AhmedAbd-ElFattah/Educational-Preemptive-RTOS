#include "semaphore.h"
#include "Task/task.h"

EduRTOS_StdType_Status SemaphoreInit (Semaphore * const Arg_Semaphore_pSemaphore, const uint32_t Arg_Semaphore_InitVal){
	if (EduRTOS_NULL == Arg_Semaphore_pSemaphore){
		return EduRTOS_StdType_Status_Fail;
	}
	Arg_Semaphore_pSemaphore->Semaphore_Value = Arg_Semaphore_InitVal;
	return EduRTOS_StdType_Status_Succeed;
}


EduRTOS_StdType_Status SemaphoreTake (Semaphore * const Arg_Semaphore_pSemaphore, const Edu_u32 Arg_TicksToWait){
	if (EduRTOS_NULL == Arg_Semaphore_pSemaphore){
		return EduRTOS_StdType_Status_Fail;
	}

	/*If semaphore is already taken, block task till semaphore is available.*/
	if(0 == __LDREXW( (Edu_u32 *) &Arg_Semaphore_pSemaphore->Semaphore_Value)){
		EduRTOS_TaskBlock_Semaphore(Arg_Semaphore_pSemaphore);
	}
	/* Calculate Maximum number of ticks that can task wait, till it's unlocked.
	 * For example, assume current tick number is 50 and Arg_TicksToWait = 60,
	 * So Mutex will fail to lock when system ticks = (60+50) = 120
	 * */
	const Edu_u32 WaitToTicks = Arg_TicksToWait + EduRTOS_GetCurrentTickNumber();

	uint32_t Semaphore_temp_val;
	while(1){
		/*Check whether time to lock, exceed configured time or not.*/
		if ((Arg_TicksToWait != EduRTOS_INFINITE) && (WaitToTicks > EduRTOS_GetCurrentTickNumber())){
			return EduRTOS_StdType_Status_Timeout;
		}
		/*Exclusive load of semaphore value*/
		Semaphore_temp_val = __LDREXW(&Arg_Semaphore_pSemaphore->Semaphore_Value);
		if (Semaphore_temp_val > 0){
			/*Exclusive store semaphore value*/
			if (0 == __STREXW(Semaphore_temp_val - 1,&Arg_Semaphore_pSemaphore->Semaphore_Value)){
				__DMB();
				break;
			}
		}
	}
	return EduRTOS_StdType_Status_Succeed;
}


EduRTOS_StdType_Status SemaphoreGive(Semaphore * const Arg_Semaphore_pSemaphore, const Edu_u32 Arg_TicksToWait){
	if (EduRTOS_NULL == Arg_Semaphore_pSemaphore){
		return EduRTOS_StdType_Status_Fail;
	}
	/* Calculate Maximum number of ticks that can task wait, till it's unlocked.
	 * For example, assume current tick number is 50 and Arg_TicksToWait = 60,
	 * So Mutex will fail to lock when system ticks = (60+50) = 120
	 * */
	const Edu_u32 WaitToTicks = Arg_TicksToWait + EduRTOS_GetCurrentTickNumber();
	uint32_t Semaphore_temp_val;
	while(1){
		/*Check whether time to lock, exceed configured time or not.*/
		if ((Arg_TicksToWait != EduRTOS_INFINITE) && (WaitToTicks > EduRTOS_GetCurrentTickNumber())){
			return EduRTOS_StdType_Status_Timeout;
		}
		Semaphore_temp_val = __LDREXW(&Arg_Semaphore_pSemaphore->Semaphore_Value);
		++Semaphore_temp_val;

		if (0 == __STREXW(Semaphore_temp_val, &Arg_Semaphore_pSemaphore->Semaphore_Value)){
			__DMB();
			EduRTOS_TaskUnBlock_Semaphore(Arg_Semaphore_pSemaphore);
			return EduRTOS_StdType_Status_Succeed;
		}
	}
}
