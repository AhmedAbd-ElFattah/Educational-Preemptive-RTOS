/*
 * File Name --> EduRTOS_defines.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Defines for EduRTOS like macro function ,data types , ... and so on.
 * */

#ifndef EDURTOS_DEFINES_H_
#define EDURTOS_DEFINES_H_

/*
==================================================
  Start Section --> File Includes
==================================================
 */
#include "EduRTOS_Config.h"
/*
==================================================
  End Section --> File Includes
==================================================
 */

/*
==================================================
  Start Section --> Macro functions
==================================================
 */

#define 	EduRTOS_SystickReloadTicks				((EduRTOS_Config_CLOCK_HZ*EduRTOS_Config_Systick_RATE_ms)/1000)
#define		EduRTOS_NULL							(void *)0

#define 	EduRTOS_Memory_Write(address, value)	(*((volatile uint32_t*)address) = value)
#define 	EduRTOS_Memory_Read(address)			(*((volatile uint32_t*)address))
#define 	EduRTOS_ArrayLength(arr) 				(sizeof(arr)/sizeof(arr[0]))

#define 	EduRTOS_TicksToMs(x)					(x/EduRTOS_Config_Systick_RATE_ms)
/*
==================================================
  End Section --> Macro functions
==================================================
 */


#define 	EduRTOS_TASKS_STACK_TOP				(EduRTOS_Config_OS_STACK_TOP-EduRTOS_Config_OS_STACK_SIZE)
#define		EduRTOS_IDLE_TASK					EduRTOS_Config_MAX_NUMBER_OF_TASKS
/*
==================================================
  Start Section --> Data Type Declaration
==================================================
 */
typedef enum EduRTOS_StdType_Status{
	EduRTOS_StdType_Status_Fail,
	EduRTOS_StdType_Status_Timeout,
	EduRTOS_StdType_Status_Succeed,

}EduRTOS_StdType_Status;

typedef enum Edu_Bool{
	Edu_False,
	Edu_True,
}Edu_Bool;

typedef unsigned char 			Edu_u8;
typedef unsigned short int 		Edu_u16;
typedef unsigned long int 		Edu_u32;
typedef unsigned long long int 	Edu_u64;


typedef signed char 			Edu_s8;
typedef signed short int 		Edu_s16;
typedef signed long int 		Edu_s32;
typedef signed long long int 	Edu_s64;

typedef double					Edu_f32;
typedef double					Edu_f64;

#define EduRTOS_INFINITE		(Edu_u32)0xFFFFFFFF

extern Edu_u32  volatile const * EduRTOS_Shared_CurrentTickNumber ;
extern Edu_u32  volatile const * EduRTOS_Shared_CurrentRunningTask ;
/*
==================================================
  End Section --> Data Type Declaration
==================================================
 */

#endif /* EDURTOS_DEFINES_H_ */
