/*
 * File Name --> task.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Provides API for tasks.
 * */

#ifndef EDUCATIONALRTOS_TASK_H_
#define EDUCATIONALRTOS_TASK_H_

/*
==================================================
  Start Section --> File Includes
==================================================
 */
#include <stdint.h>
#include "stm32f401xc.h"
#include "EduRTOS_Config.h"
#include "EduRTOS_defines.h"
#include "EduRTOS.h"

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
typedef enum{
	EduRTOS_TaskStatus_Running,
	EduRTOS_TaskStatus_Ready,
	EduRTOS_TaskStatus_Blocked,
	EduRTOS_TaskStatus_Suspended,
}EduTask_Status;

typedef Edu_u32 TaskId;

typedef enum {
	EduTask_BlockingReason_WaitingTickTime,
	EduTask_BlockingReason_MutexLocked,
	EduTask_BlockingReason_SemaphoreLocked,
}EduTask_BlockingReason;

typedef struct {
	/*Pushed by software*/
	Edu_u32 r4;
	Edu_u32 r5;
	Edu_u32 r6;
	Edu_u32 r7;
	Edu_u32 r8;
	Edu_u32 r9;
	Edu_u32 r10;
	Edu_u32 r11;

	/*Pushed by hardware*/
	Edu_u32 r0;
	Edu_u32 r1;
	Edu_u32 r2;
	Edu_u32 r3;
	Edu_u32 r12;
	Edu_u32 lr;
	Edu_u32 pc;
	Edu_u32 psr;
}StackFrameM4WithoutFPU;

typedef struct {
	EduTask_BlockingReason BlockingReason;
	Edu_u32  ValidTask;
	Edu_u32  WaitingTillTick;

#if EduRTOS_Config_USE_MUTEX
	Mutex *BlockingMutex;
#endif

#if EduRTOS_Config_USE_SEMAPHORE
	Semaphore *BlockingSemaphore;
#endif

#if EduRTOS_Config_USE_MAILBOX
	MailBox *BlockingMailBox;
#endif

	TaskId	  TaskIdentifier;
	Edu_u32  Priority;
	Edu_u32  TotalTimeSlots;
	Edu_u32  FinishedTimeSlots;
	EduTask_Status TaskStatus;
	Edu_u32 pStackPointer;
	void (*Function) () ;
} EduRTOS_TCB;


/* Examples are documented in detail please visit github repository of EduRTOS
 * */
typedef union{
	struct{
		Edu_u8 arg1_8bit;
		Edu_u8 reserved_1[3];

		Edu_u8 arg2_8bit;
		Edu_u8 reserved_2[3];

		Edu_u8 arg3_8bit;
		Edu_u8 reserved_3[3];

		Edu_u8 arg4_8bit;
		Edu_u8 reserved_4[3];
	};

	struct{
		Edu_u16 arg1_16bit;
		Edu_u16 reserved_5;

		Edu_u16 arg2_16bit;
		Edu_u16 reserved_6;

		Edu_u16 arg3_16bit;
		Edu_u16 reserved_7;

		Edu_u16 arg4_16bit;
		Edu_u16 reserved_8;
	};

	struct{
		Edu_u32 arg1_32bit;
		Edu_u32 arg2_32bit;
		Edu_u32 arg3_32bit;
		Edu_u32 arg4_32bit;

	};
	struct{
		void 	*arg1_pointer;
		void 	*arg2_pointer;
		void 	*arg3_pointer;
		void 	*arg4_pointer;

	};
	struct{
		Edu_u64 arg1_64bit;
		Edu_u64 arg2_64bit;

	};

	Edu_u32 parameters[4];
} TaskParameters;

TaskId EduRTOS_CreateTask(
		void (*TaskFunction) (),
		Edu_u32 TaskPriority,
		Edu_u32 TaskStackSize,
		TaskParameters const * const TaskParameters
);

void RTOS_Start_Scheduler(void);
void EduRTOS_TaskDelayUntil(const Edu_u32 ticks);

void EduRTOS_TaskBlock_Mutex(Mutex * const Arg_Mutex);
void EduRTOS_TaskBlock_Semaphore(Semaphore * const Arg_Semaphore);

void EduRTOS_TaskUnBlock_Mutex(Mutex * const Arg_Mutex);
void EduRTOS_TaskUnBlock_Semaphore(Semaphore * const Arg_Semaphore);

Edu_u32 EduRTOS_GetCurrentTickNumber(void);
Edu_u32 EduRTOS_GetCurrentRunningTask(void);
Edu_u32 EduRTOS_GetNumberOfCreatedTasks(void);

#endif /* EDUCATIONALRTOS_TASK_H_ */
