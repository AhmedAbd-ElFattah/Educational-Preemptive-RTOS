# Mailbox
Mailbox is a synchronization and communication primitive that allows tasks to send and receive messages or data between them. It is typically used to pass data or notifications between tasks in multi-tasking environment


## Table of Contents
- [API Reference](#api-reference)
- [Examples](#examples)

## API Reference

- **EduRTOS_StdType_Status MailBoxInit(MailBox *Arg_Mailbox,Edu_u32 *Arg_Array, Edu_u32 Arg_BufferSize)**

	**Explanation** 

	Initialize Mailbox
	
	**Parameters**
	
	MailBox    : Pointer of mailbox
    Arg_Array  : Array that holds mailbox elements
    BufferSize : Size of array

	**Return value** 

	Status if it's initialized successfully or not

- **EduMailbox_State MailBoxWrite(MailBox *Arg_Mailbox, Edu_u32 Arg_Data, const Edu_u32 Arg_TicksToWait)**

	**Explanation** 

	Write data to Mailbox
	
	**Parameters**
	
	MailBox    : Pointer of mailbox
    Arg_Data   : Data that you need to write in Mailbox
	Arg_TicksToWait : Timeout value for writing to mailbox

	**Return value** 

	Status if data written successfully ,or failed due to specific reason like mailbox is full

- **EduMailbox_State MailBoxRead(MailBox *Arg_Mailbox, Edu_u32 *Arg_Data)**

	**Explanation** 

	Read data from Mailbox
	
	**Parameters**
	
	MailBox    : Pointer of mailbox
    Arg_Data   : Pointer of data that will be stored in
	Arg_TicksToWait :  Timeout value for reading from mailbox.

	**Return value** 

	Status if data reading is successfully ,or failed due to specific reason like mailbox is empty

## Examples
In this example we can monitor "LastValueReceived" value using debugger. you will notice that 
- When a task attempts to write to a mailbox that is already full, it will be blocked until the mailbox is no longer at its maximum capacity.
- When a task attempts to read to a mailbox that is empty, it will be blocked until the mailbox is no longer empty.
- Old data is not overwritten when the buffer is full.
- When monitoring the "LastValueReceived" variable, data is received in the same order as it was written, ensuring that no data loss occurs.

```c
#include "EduRTOS.h"

MailBox exampleMailbox;
Edu_u32 MailboxArray[10];

Edu_u32 LastValueReceived=-1;


void Task1_Mailbox(MailBox *Arg_MailBox){
	for (Edu_u32 i = 0; i<30; i++){
		while(EduRTOS_Mailbox_WriteSucceed != MailBoxWrite(&exampleMailbox, i, EduRTOS_INFINITE)){
			/*Wait 1000ms (1 second)*/
			EduRTOS_TaskDelayUntil(EduRTOS_TicksToMs(1000));
		}
	}

	while(1){

	}
}

void Task2_Mailbox(MailBox *Arg_MailBox){
	uint32_t tempValue;
	while(1){
		if (EduRTOS_Mailbox_ReadSucceed == MailBoxRead(&exampleMailbox, &tempValue, EduRTOS_INFINITE)){
			LastValueReceived = tempValue;
			/*Wait 1000ms (1 second)*/
			EduRTOS_TaskDelayUntil(EduRTOS_TicksToMs(1000));
		}
	}
}
int main(void)
{
	MailBoxInit(&exampleMailbox, MailboxArray, EduRTOS_ArrayLength(MailboxArray));
	EduRTOS_Init();

	/*Pass pointer of mailbox to task arguments*/
	TaskParameters task1MailboxArguments;
	task1MailboxArguments.arg1_pointer = &exampleMailbox;
	EduRTOS_CreateTask(Task1_Mailbox, 2, 2*1024, &task1MailboxArguments);

	/*Pass pointer of mailbox to task arguments*/
	TaskParameters task2MailboxArguments;
	task2MailboxArguments.arg1_pointer = &exampleMailbox;
	EduRTOS_CreateTask(Task2_Mailbox, 1, 2*1024, &task2MailboxArguments);
	RTOS_Start_Scheduler();

	while(1){

	}
}

```