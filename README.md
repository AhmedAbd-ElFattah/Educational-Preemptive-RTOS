# EduRTOS
Educational RTOS is a lightweight, open-source **preemptive real-time operating system** created with the primary goal of providing a hands-on learning experience for enthusiasts, students, and developers eager to explore the inner workings of RTOS components. **EduRTOS uses round-robin algorithm for scheduling.**
While Educational RTOS is a simplified implementation, it encompasses essential features that are commonly found in professional RTOS systems.

It is an ideal starting point for individuals looking to grasp the fundamentals of RTOS design and operation. It serves as a practical platform for gaining insights into various RTOS components and concepts. By using Educational RTOS, you can embark on a journey to:

1. Understand Multitasking: Educational RTOS allows you to run multiple tasks concurrently. You can explore how tasks are scheduled, managed, and synchronized, gaining a deep understanding of task switching and context management.

2. Explore Synchronization: With support for Mutexes, Semaphores, and Mailboxes, Educational RTOS lets you experiment with synchronization mechanisms that are crucial for preventing data corruption and race conditions in real-time systems.

3. Master Portability: Educational RTOS is designed with portability in mind, utilizing the CMSIS (Common Microcontroller Software Interface Standard) APIs. This means you can easily adapt it to various microcontroller platforms, providing a hands-on experience in making your RTOS code versatile and adaptable.

4. Grasp Low-Level Details: Educational RTOS allows you to peek under the hood of an RTOS, dissecting its core components such as the scheduler, task management, and resource allocation. This insight will help you develop a deeper appreciation for the inner workings of real-time systems.

5. Prototype Embedded Systems: By using Educational RTOS, you can create small-scale embedded applications, experiment with sensor inputs, control outputs, and gain practical experience in building real-time, responsive systems.

Please take note that EduRTOS has been validated for use on the STM32F401CC microcontroller. Furthermore, it is highly portable, meaning it can be easily adapted for deployment on various other ARM Cortex-M processors.

## Table of Contents
- [Dependences](#dependences)
- [Configurations](#configurations)
- [Priority in EduRTOS](#priority-in-edurtos)
- [Passing arguments to task](#passing-arguments-to-task)
- [Creating Task Example](#creating-task-example)

## Dependences
You should include CMSIS-Librarity in "EduRTOS_Config.h" file.

For me, I included CMSIS In [this repository.](https://github.com/STMicroelectronics/STM32CubeF4/tree/master/Drivers/CMSIS)

## Configurations

EduRTOS configured through "EduRTOS_Config.h" file, The following table descripes each configuration. 


| Configuration                      | Description                                                                                                                                                                                                                                                       |
|------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| EduRTOS_Config_MAX_NUMBER_OF_TASKS | Configure maximum number of tasks in system                                                                                                                                                                                                                       |
| EduRTOS_Config_USER_IDLE_TASK      | Configure wheter you need to execute function in Idle task or not. To configure that you need to execute function define this value with 1 otherwise define it with 0.  Note function name must have the following prorotype void EduRTOS_IdleTaskCallback(void); |
| EduRTOS_Config_OS_STACK_TOP        | Configure Operating system stack top address                                                                                                                                                                                                                      |
| EduRTOS_Config_OS_STACK_SIZE       | Configure Operating system stack size                                                                                                                                                                                                                             |
| EduRTOS_Config_TASK_MIN_STACK_SIZE | Configure minimum stack size                                                                                                                                                                                                                                      |
| EduRTOS_Config_CLOCK_HZ            | Define clock of systick timer                                                                                                                                                                                                                                     |
| EduRTOS_Config_Systick_RATE_ms     | Configure systick rate in milliseconds                                                                                                                                                                                                                            |
| EduRTOS_Config_USE_MUTEX           | Configure wheter you need to use semaphore or not. To configure using semaphore define it with 1 otherwise 0.                                                                                                                                                     |
| EduRTOS_Config_USE_SEMAPHORE       | Configure wheter you need to use mutex or not. To configure using mutex define it with 1 otherwise 0.                                                                                                                                                             |
| EduRTOS_Config_USE_MAILBOX         | Configure wheter you need to use mailbox or not. To configure using mailbox define it with 1 otherwise 0.                                                                                                                                                         |                                                                                                                                                        |                                                                                                                                                    |

## Priority in EduRTOS
- Priorities in EduRTOS are static,thus You can't change priority during runtime.
- The Higher priority number, the highest priority of task.
- It is not allowed for two tasks to have the same priority, Each task must have a unique priority.
- Idle task has zero priority (Lowest priority).
- Priority range is from 1 to "EduRTOS_MAX_NUMBER_OF_TASKS", where EduRTOS_MAX_NUMBER_OF_TASKS is defined and configured in "EduRTOS_Config.h" which indicated the maximum number of tasks in the system

## Passing arguments to task
Due to rules that ARM cortex-M4 processor behaves with function argument, In EduRTOS values passed to tasks are classified into Register parameters & Stack parameters.

#### Task parameters
- Includes maximum 4 parameters each parameter has datatype less than or equal 32-bit
- If one parameter is datatype has 64-bit , two parametrs only allowed each one mustn't exceed 32-bit
- If two parameters have datatype size equal 64-bit, no other parameters are considered register parameters

#### Examples of Task parameters
##### Passing arguments of the following sizes in order 16-bit, 32-bit, 8-bit, and 16-bit.
```c

TaskParameters taskArguments;
    taskArguments.arg1_16bit = 0x5f82;
    taskArguments.arg2_32bit = 0x403c03a4;
    taskArguments.arg3_8bit	 = 0x1a;
    taskArguments.arg4_16bit = 0x5315;


void Task(
		Edu_u16 parameter_1, // Has value 0x5f82
		Edu_u32 parameter_2, // Has value 0x403c03a4
		Edu_u8 parameter_3,  // Has value 0x1a
		Edu_u16 parameter_4  // Has value 0x5315
		){
	while(1);
}

```
##### Passing arguments of the following sizes in order 64-bit, 32-bit, and 32-bit.
```c

	TaskParameters taskArguments;
	taskArguments.arg1_64bit = 0xc205c96b4efade;
	/*As arg1 is 64bit, 64bit value occupies in this union arg1 & arg2.
	 * so parameter 2 & 3 need to be stored in arg3_32bit & arg4_32bi (skipping_arg2_x)*/
	taskArguments.arg3_32bit = 0x146f41fb;
	taskArguments.arg4_32bit = 0xf7585a6a;

void Task(
		Edu_u64 parameter_1, // Has value 0xc205c96b4efade
		Edu_u32 parameter_2, // Has value 0x146f41fb
		Edu_u32 parameter_3  // Has value 0xf7585a6a
		){
	while(1){
	}
}
```

##### Passing arguments of the following sizes in order 32-bit, 32-bit, and 64-bit.
```c

TaskParameters taskArguments;
	taskArguments.arg1_32bit = 0x146f41fb;
	taskArguments.arg2_32bit = 0xf7585a6a;
	taskArguments.arg2_64bit = 0xc205c96b4efade;

void Task(
		Edu_u32 parameter_1, // Has value 0x146f41fb
		Edu_u32 parameter_2, // Has value 0xf7585a6a
		Edu_u64 parameter_3  // Has value 0xc205c96b4efade
		){
	while(1){
	}
}
```


##### Passing arguments of the following sizes in order 64-bit, and 64-bit.
```c
    TaskParameters taskArguments;
	taskArguments.arg1_64bit = 0x146f41fbaf7585a6;
	taskArguments.arg2_64bit = 0x14fa37b1ec1a5aa0;

void Task(
		Edu_u64 parameter_1, // Has value 0x146f41fbaf7585a6
		Edu_u64 parameter_2  // Has value 0x14fa37b1ec1a5aa0
		){
	while(1){
	}
}
```



## Creating Task Example

```c
#include "EduRTOS.h"

void Task1_Counter(Edu_u32 Counter_Init){

	while(1){
		Counter_Init ++;
	}
}

int main(void)
{
	/*Basic initialization of RTOS*/
	EduRTOS_Init();


	TaskParameters taskArguments;
	taskArguments.arg1_32bit = 1000;

	EduRTOS_CreateTask(
			Task1_Counter, // Task function
			1,			   // Task priority
			EduRTOS_Config_TASK_MIN_STACK_SIZE,		   // Task stack size
			&taskArguments // Union of TaskParameters that passes values of task-parameters.
			);


	RTOS_Start_Scheduler();
	while(1){

	}
}
```
