/*
 * File Name --> mailbox.c
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Mailbox implementation.
 * */

#include "mailbox.h"
EduRTOS_StdType_Status MailBoxInit(MailBox *Arg_Mailbox, Edu_u32 *Arg_Array , Edu_u32 BufferSize){
	if (EduRTOS_NULL == Arg_Array){
		return EduRTOS_StdType_Status_Fail;
	}

	if (0 == BufferSize){
		/*Buffer size must be bigger than 0*/
		return EduRTOS_StdType_Status_Fail;
	}

	Arg_Mailbox->MailboxBuffer = Arg_Array;
	Arg_Mailbox->BufferSize = BufferSize;
	Arg_Mailbox->MailboxLength = 0;
	Arg_Mailbox->Front = -1;
	Arg_Mailbox->Rear = -1;

	SemaphoreInit(&Arg_Mailbox->ReadingSemaphore, 0);
	SemaphoreInit(&Arg_Mailbox->WritingSemaphore, BufferSize);
	return EduRTOS_StdType_Status_Succeed;
}

EduMailbox_State MailBoxWrite(MailBox *Arg_Mailbox, Edu_u32 Arg_Data, const Edu_u32 Arg_TicksToWait){
	if (EduRTOS_NULL == Arg_Mailbox){
		return EduRTOS_Mailbox_Error;
	}

	if (EduRTOS_StdType_Status_Succeed != SemaphoreTake(&Arg_Mailbox->WritingSemaphore, Arg_TicksToWait)){
		return EduRTOS_Mailbox_Error;
	}

	/*Check if it's full.
	 * If it's full, It can't write any more data.*/
	if ((Arg_Mailbox->Rear+1)%Arg_Mailbox->BufferSize == Arg_Mailbox->Front){
		return EduRTOS_Mailbox_Full;
	}
	/*Check if it's empty*/
	else if (Arg_Mailbox->Front == -1){
		Arg_Mailbox->Front = 0;
		Arg_Mailbox->Rear = 0;
	}
	else{
		Arg_Mailbox->Rear = (Arg_Mailbox->Rear + 1) %Arg_Mailbox->BufferSize;
	}
	Arg_Mailbox->MailboxLength++;
	Arg_Mailbox->MailboxBuffer[Arg_Mailbox->Rear] = Arg_Data;

	if (EduRTOS_StdType_Status_Succeed != SemaphoreGive(&Arg_Mailbox->ReadingSemaphore, Arg_TicksToWait)){
		return EduRTOS_Mailbox_Error;
	}
	return EduRTOS_Mailbox_WriteSucceed;
}

EduMailbox_State MailBoxRead(MailBox *Arg_Mailbox, Edu_u32 *Arg_Data, const Edu_u32 Arg_TicksToWait){
	if (EduRTOS_NULL == Arg_Mailbox){
		return EduRTOS_Mailbox_Error;
	}

	if (EduRTOS_NULL == Arg_Data){
		return EduRTOS_Mailbox_Error;
	}

	if (EduRTOS_StdType_Status_Succeed != SemaphoreTake(&Arg_Mailbox->ReadingSemaphore, Arg_TicksToWait)){
		return EduRTOS_Mailbox_Error;
	}
	/*Check if it's empty.
	 * If it's empty, you can't read data.*/
	if (Arg_Mailbox->Front == -1){
		return EduRTOS_Mailbox_Empty;
	}
	else if (Arg_Mailbox->Front == Arg_Mailbox->Rear){

		*Arg_Data = Arg_Mailbox->MailboxBuffer[Arg_Mailbox->Front];
		Arg_Mailbox->Front = -1;
		Arg_Mailbox->Rear  = -1;

	}
	else{
		*Arg_Data = Arg_Mailbox->MailboxBuffer[Arg_Mailbox->Front];
		Arg_Mailbox->Front = (Arg_Mailbox->Front + 1) % Arg_Mailbox->BufferSize;
	}
	Arg_Mailbox->MailboxLength--;

	if (EduRTOS_StdType_Status_Succeed != SemaphoreGive(&Arg_Mailbox->WritingSemaphore, Arg_TicksToWait)){
		return EduRTOS_Mailbox_Error;
	}
	return EduRTOS_Mailbox_ReadSucceed;
}
