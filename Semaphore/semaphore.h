/*
 * File Name --> semaphore.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Provides API for Semaphore.
 * */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_
#include "EduRTOS_Config.h"
#include "EduRTOS_defines.h"

typedef struct {
	uint32_t Semaphore_Value;
}Semaphore;

/* @brief  							--> Initialize Semaphore.
 * @param  Arg_Semaphore_pSemaphore --> Pointer of Semaphore.
 * @param  Arg_Semaphore_InitVal 	--> Initial value of semaphore.
 * @retval --> Status if it's initialized successfully or not.
 * */
EduRTOS_StdType_Status SemaphoreInit (Semaphore * const Arg_Semaphore_pSemaphore, const uint32_t Arg_Semaphore_InitVal);

/* @brief  							--> Take Semaphore.
 * @param  Arg_Semaphore_pSemaphore --> Pointer of Semaphore.
 * @param  Arg_TicksToWait 			--> Timeout value for taking semaphore.
 * @retval --> Status if it's taken successfully or not.
 * */
EduRTOS_StdType_Status SemaphoreTake (Semaphore * const Arg_Semaphore_pSemaphore, const Edu_u32 Arg_TicksToWait);
/* @brief  							--> Give Semaphore.
 * @param  Arg_Semaphore_pSemaphore --> Pointer of Semaphore.
 * @param  Arg_TicksToWait 			--> Timeout value for giving semaphore.
 * @retval --> Status if it's given successfully or not.
 * */
EduRTOS_StdType_Status SemaphoreGive(Semaphore * const Arg_Semaphore_pSemaphore, const Edu_u32 Arg_TicksToWait);

#endif /* SEMAPHORE_H_ */
