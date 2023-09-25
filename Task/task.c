/*
 * File Name --> task.c
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Provides implementation functions for tasks.
 * */

#include "task.h"

/*Holds tick number of system*/
volatile static Edu_u32 EduRTOS_CurrentTickNumber = 0;
/*
 * Array of TCB of each task,
 * Note last tcb of array holds Idle task tcb.
 * */
volatile static EduRTOS_TCB EduRTOS_TCBsArray [EduRTOS_Config_MAX_NUMBER_OF_TASKS + 1];

volatile static Edu_u32 EduRTOS_NumberOfCreatedTasks = 0;

volatile static Edu_u32 EduRTOS_CurrentRunningTask = 0;

volatile static Edu_u32 EduRTOS_NextTopTaskStackAddress = EduRTOS_TASKS_STACK_TOP;

#define EduRTOS_ContextSwitchingTrigger() (SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk)


void EduRTOS_TaskBlock_Mutex(Mutex * const Arg_Mutex){
	if (EduRTOS_NULL == Arg_Mutex){
		EduRTOS_ErrorHandler();
	}
	__disable_irq();
	TaskId TargetTaskId = EduRTOS_GetCurrentRunningTask();
	/*Switch task from running to blocked*/
	EduRTOS_TCBsArray[TargetTaskId].TaskStatus = EduRTOS_TaskStatus_Blocked;
	/*Assign blocking reason*/
	EduRTOS_TCBsArray[TargetTaskId].BlockingReason = EduTask_BlockingReason_MutexLocked;
	/*Pass mutex that blocks task.*/
	EduRTOS_TCBsArray[TargetTaskId].BlockingMutex = Arg_Mutex;
	__enable_irq();
	EduRTOS_ContextSwitchingTrigger();

}

void EduRTOS_TaskBlock_Semaphore(Semaphore * const Arg_Semaphore){
	if (EduRTOS_NULL == Arg_Semaphore){
		EduRTOS_ErrorHandler();
	}
	__disable_irq();
	TaskId TargetTaskId = EduRTOS_GetCurrentRunningTask();
	/*Switch task from running to blocked*/
	EduRTOS_TCBsArray[TargetTaskId].TaskStatus = EduRTOS_TaskStatus_Blocked;
	/*Assign blocking reason*/
	EduRTOS_TCBsArray[TargetTaskId].BlockingReason = EduTask_BlockingReason_SemaphoreLocked;
	/*Pass semaphore that blocks task.*/
	EduRTOS_TCBsArray[TargetTaskId].BlockingSemaphore = Arg_Semaphore;
	__enable_irq();
	EduRTOS_ContextSwitchingTrigger();

}

void EduRTOS_TaskUnBlock_Mutex(Mutex *Arg_Mutex){
	if (EduRTOS_NULL == Arg_Mutex){
		EduRTOS_ErrorHandler();
	}

	Edu_Bool IsContextSwitchingRequired = Edu_False;

	/*Move tasks that hold this mutex from blocked state to ready state.*/
	for (int i =0; i<EduRTOS_Config_MAX_NUMBER_OF_TASKS; i++){
		/*If it's Idle task, continue.*/
		if (EduRTOS_IDLE_TASK == i){
			continue;
		}
		/*If it's not-valid task, continue.*/
		if (Edu_False == EduRTOS_TCBsArray[i].ValidTask){
			continue;
		}
		/*If it's not blocked, continue*/
		if (EduRTOS_TaskStatus_Blocked != EduRTOS_TCBsArray[i].TaskStatus){
			continue;
		}
		/*If it's reason of blocking is not due to mutex, continue*/
		if (EduTask_BlockingReason_MutexLocked != EduRTOS_TCBsArray[i].BlockingReason){
			continue;
		}
		/*If passed mutex not the same mutex that block task, continue*/
		if (Arg_Mutex != EduRTOS_TCBsArray[i].BlockingMutex){
			continue;
		}

		if (EduRTOS_Mutex_State_Unlocked == GetMutexStatus(Arg_Mutex)){
			EduRTOS_TCBsArray[i].TaskStatus = EduRTOS_TaskStatus_Ready;
			if (EduRTOS_TCBsArray[i].Priority > EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].Priority){
				IsContextSwitchingRequired = Edu_True;
			}
			break;
		}

	}

	if (Edu_True == IsContextSwitchingRequired){
		EduRTOS_ContextSwitchingTrigger();
	}
}

void EduRTOS_TaskUnBlock_Semaphore(Semaphore *Arg_Semaphore){
	if (EduRTOS_NULL == Arg_Semaphore){
		EduRTOS_ErrorHandler();
	}
	Edu_Bool IsContextSwitchingRequired = Edu_False;
	/*Find which function*/
	for (int i =0; i<EduRTOS_Config_MAX_NUMBER_OF_TASKS; i++){
		/*Idle task*/
		if (EduRTOS_Config_MAX_NUMBER_OF_TASKS == i){
			continue;
		}

		if (!EduRTOS_TCBsArray[i].ValidTask){
			continue;
		}
		if (EduRTOS_TaskStatus_Blocked != EduRTOS_TCBsArray[i].TaskStatus){
			continue;
		}
		if (EduTask_BlockingReason_SemaphoreLocked != EduRTOS_TCBsArray[i].BlockingReason){
			continue;
		}
		if (Arg_Semaphore != EduRTOS_TCBsArray[i].BlockingSemaphore){
			continue;
		}

		EduRTOS_TCBsArray[i].TaskStatus = EduRTOS_TaskStatus_Ready;
		if (EduRTOS_TCBsArray[i].Priority > EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].Priority){
			IsContextSwitchingRequired = 1;
		}
		break;


	}

	if (Edu_True == IsContextSwitchingRequired){
		EduRTOS_ContextSwitchingTrigger();
	}
}

Edu_u32 EduRTOS_GetCurrentTickNumber(void){
	return EduRTOS_CurrentTickNumber;
}

Edu_u32 EduRTOS_GetCurrentRunningTask(void){
	return EduRTOS_CurrentRunningTask;
}

Edu_u32 EduRTOS_GetNumberOfCreatedTasks(void){
	return EduRTOS_NumberOfCreatedTasks;
}

void EduRTOS_TaskDelayUntil(const Edu_u32 ticks){
	__disable_irq();
	TaskId TargetTaskId = EduRTOS_GetCurrentRunningTask();

	EduRTOS_TCBsArray[TargetTaskId].TaskStatus = EduRTOS_TaskStatus_Blocked;
	EduRTOS_TCBsArray[TargetTaskId].BlockingReason = EduTask_BlockingReason_WaitingTickTime;
	EduRTOS_TCBsArray[TargetTaskId].WaitingTillTick = EduRTOS_GetCurrentTickNumber() + ticks;
	__enable_irq();
	EduRTOS_ContextSwitchingTrigger();


}

TaskId EduRTOS_CreateTask(void (*TaskFunction) (),
		Edu_u32 TaskPriority,
		Edu_u32 TaskStackSize,
		TaskParameters const * const TaskParameters
){

	TaskId CreatedTaskId = EduRTOS_Config_MAX_NUMBER_OF_TASKS-TaskPriority;

	/*Initialize and reserve stack frame for task.*/

	StackFrameM4WithoutFPU *TaskStackFrame;
	TaskStackFrame = (StackFrameM4WithoutFPU *)(EduRTOS_NextTopTaskStackAddress - sizeof(StackFrameM4WithoutFPU));
	TaskStackFrame->r4 	= 0;
	TaskStackFrame->r5 	= 0;
	TaskStackFrame->r6 	= 0;
	TaskStackFrame->r7 	= 0;
	TaskStackFrame->r8 	= 0;
	TaskStackFrame->r9 	= 0;
	TaskStackFrame->r10 = 0;
	TaskStackFrame->r11 = 0;
	if (TaskParameters->parameters){
		TaskStackFrame->r0  = TaskParameters->parameters[0];
		TaskStackFrame->r1 	= TaskParameters->parameters[1];
		TaskStackFrame->r2 	= TaskParameters->parameters[2];
		TaskStackFrame->r3 	= TaskParameters->parameters[3];
	}
	else{
		TaskStackFrame->r0  = 0;
		TaskStackFrame->r1 	= 0;
		TaskStackFrame->r2 	= 0;
		TaskStackFrame->r3 	= 0;
	}
	TaskStackFrame->r12 = 0;
	TaskStackFrame->pc 	= (Edu_u32)TaskFunction;
	TaskStackFrame->lr 	= 0;
	TaskStackFrame->psr	= 0x1000000;

	/*Set task as ready*/
	EduRTOS_TCBsArray[CreatedTaskId].TaskStatus = EduRTOS_TaskStatus_Ready;

	EduRTOS_TCBsArray[CreatedTaskId].TaskIdentifier = CreatedTaskId;
	/*Set task as valid*/
	EduRTOS_TCBsArray[CreatedTaskId].ValidTask = 1;
	/*Assign stask pointer for task*/
	EduRTOS_TCBsArray[CreatedTaskId].pStackPointer = EduRTOS_NextTopTaskStackAddress;
	/*Assign task priority*/
	EduRTOS_TCBsArray[CreatedTaskId].Priority = TaskPriority;
	/*Total timer slots of task = task priority*/
	EduRTOS_TCBsArray[CreatedTaskId].TotalTimeSlots = TaskPriority;
	/*At initialization, Finished time slots for a task equal zero.*/
	EduRTOS_TCBsArray[CreatedTaskId].FinishedTimeSlots = 0;
	/*Assign task function of a task*/
	EduRTOS_TCBsArray[CreatedTaskId].Function = TaskFunction;
	/*Reserve stack size of task & Calculate top stack for the next task.*/
	EduRTOS_NextTopTaskStackAddress -= TaskStackSize;
	EduRTOS_NumberOfCreatedTasks++;

	return CreatedTaskId;
}

__attribute__ ((naked)) void RTOS_Start_Scheduler(void){

	/*Get first valid task id in other words (Highest priority task)*/
	for (Edu_u32 i = 0;i<EduRTOS_Config_MAX_NUMBER_OF_TASKS;i++){
		if (EduRTOS_TCBsArray[i].ValidTask){
			EduRTOS_CurrentRunningTask = i;
			break;
		}
	}
	/*Set stack pointer to highest priority task.*/
	EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TaskStatus = EduRTOS_TaskStatus_Running;
	__set_PSP(EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].pStackPointer-sizeof(StackFrameM4WithoutFPU));
	/*Switch control to process stack pointer*/
	__set_CONTROL(0x02);
	__asm volatile ("pop {r4-r11}\n");
	__asm volatile ("pop {r0-r3}\n");
	__asm volatile ("pop {r12}\n");
	__asm volatile ("pop {lr}\n");
	__asm volatile ("pop {pc}\n");

	__enable_irq();
}


void SysTick_Handler(void){
	EduRTOS_CurrentTickNumber++;
	Edu_u32 IsContextSwitchingRequired = 0;

	/*Find tasks that was blocked due to waiting tick time.*/
	for (int i =0; i<EduRTOS_Config_MAX_NUMBER_OF_TASKS; i++){
		/*Idle task*/
		if (EduRTOS_Config_MAX_NUMBER_OF_TASKS == i){
			continue;
		}

		if (!EduRTOS_TCBsArray[i].ValidTask){
			continue;
		}
		if (EduRTOS_TaskStatus_Blocked != EduRTOS_TCBsArray[i].TaskStatus){
			continue;
		}
		if (EduTask_BlockingReason_WaitingTickTime != EduRTOS_TCBsArray[i].BlockingReason){
			continue;
		}
		if (EduRTOS_TCBsArray[i].WaitingTillTick <= EduRTOS_GetCurrentTickNumber()){

			EduRTOS_TCBsArray[i].TaskStatus = EduRTOS_TaskStatus_Ready;
			/*
			 * Check if this task has higher priority than current running task.
			 * If so, do context switching
			 * */
			if (EduRTOS_TCBsArray[i].Priority > EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].Priority){
				IsContextSwitchingRequired = 1;
			}
		}
	}

	/*If task finished all it's time slots, trigger context switching
	 * Otherwise increase FinishedTimeSlots by 1*/
	if (EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TotalTimeSlots == EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].FinishedTimeSlots){
		if (EduRTOS_TaskStatus_Running == EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TaskStatus ){
			EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TaskStatus = EduRTOS_TaskStatus_Ready;
		}
		else{

		}
		EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].FinishedTimeSlots = 0;
		IsContextSwitchingRequired = 1;

	}
	else{
		EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].FinishedTimeSlots++;
	}

	if (IsContextSwitchingRequired){
		EduRTOS_ContextSwitchingTrigger();
	}
}

__attribute__ ((naked)) void PendSV_Handler(void){
	__disable_irq();
	Edu_u32 FoundTaskInReadyState = 0;

	asm volatile ("MRS r0, PSP");
	asm volatile ("STMDB r0!, {r4-r11}");
	asm volatile ("MSR psp, r0");
	EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].pStackPointer = __get_PSP()+sizeof(StackFrameM4WithoutFPU);

	/*Circular loop to Search for next task to run.
	 * And avoid selecting current task.
	 * when this function is called it's must to do context switching, so avoid choosing current task.*/
	for (Edu_u32 i = (((EduRTOS_CurrentRunningTask+1) >= EduRTOS_Config_MAX_NUMBER_OF_TASKS) ? 0 : (EduRTOS_CurrentRunningTask+1))
			; i != EduRTOS_CurrentRunningTask;
			i++){

		/*Idle task*/
		if (EduRTOS_Config_MAX_NUMBER_OF_TASKS == i){
			i = 0;
			continue;
		}

		if (EduRTOS_TCBsArray[i].ValidTask && (EduRTOS_TaskStatus_Ready == EduRTOS_TCBsArray[i].TaskStatus)){

			if (EduRTOS_TaskStatus_Running == EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TaskStatus){
				EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TaskStatus = EduRTOS_TaskStatus_Ready;
			}

			EduRTOS_CurrentRunningTask = i;
			FoundTaskInReadyState = 1;
			break;
		}

	}
	/*In case of no available task to run, run idle task.*/
	if (!FoundTaskInReadyState){
		if (EduRTOS_TaskStatus_Running == EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TaskStatus){
			EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TaskStatus = EduRTOS_TaskStatus_Ready;
		}
		EduRTOS_CurrentRunningTask = EduRTOS_Config_MAX_NUMBER_OF_TASKS;
	}
	else{
		EduRTOS_TCBsArray[EduRTOS_Config_MAX_NUMBER_OF_TASKS].TaskStatus = EduRTOS_TaskStatus_Ready;
	}


	EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].TaskStatus = EduRTOS_TaskStatus_Running;

	__set_PSP(EduRTOS_TCBsArray[EduRTOS_CurrentRunningTask].pStackPointer-sizeof(StackFrameM4WithoutFPU));

	asm volatile ("MRS r0, psp \n");
	asm volatile ("LDMFD r0!, {r4-r11}\n");
	asm volatile ("MSR psp, r0\n");
	__enable_irq();
	asm volatile ("BX lr");
}


#undef MIN
#undef MAX
