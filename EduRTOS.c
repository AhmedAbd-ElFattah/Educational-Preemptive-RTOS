/*
 * File Name --> EduRTOS.c
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Implements basic function for EduRTOS
 * */

#include "EduRTOS.h"
#include "EduRTOS_defines.h"


#if EduRTOS_USER_IDLE_TASK
void EduRTOS_IdleTaskCallback(void);
#endif

static void IdleTask(){
	while(1){
#if EduRTOS_USER_IDLE_TASK
		EduRTOS_IdleTaskCallback();
#endif

	}
}

void EduRTOS_Init(void){
	/*Disable interrupts.*/
	__disable_irq();

	SysTick_Config(EduRTOS_SystickReloadTicks);

	/* Set priority levels */
	NVIC_SetPriority(SVCall_IRQn, 0);
	NVIC_SetPriority(SysTick_IRQn, 1);

	/* Set PendSV to lowest possible priority */
	NVIC_SetPriority(PendSV_IRQn, 0xFF);

	/* Enable SVC and PendSV interrupts */
	NVIC_EnableIRQ(PendSV_IRQn);
	NVIC_EnableIRQ(SVCall_IRQn);

	/*Enable interrupts.*/
	__enable_irq();

	/*Create Idle task which has 0 priority (Lowest priority)*/
	EduRTOS_CreateTask(IdleTask, 0, EduRTOS_Config_TASK_MIN_STACK_SIZE, EduRTOS_NULL);
}

void EduRTOS_ErrorHandler(void){
	__disable_irq();
	while(1){

	}
}
