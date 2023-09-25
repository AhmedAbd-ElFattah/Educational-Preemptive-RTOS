/*
 * File Name --> mailbox.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> RTOS
 * Brief	 --> Provides API for Mailbox.
 * */

#ifndef MAILBOX_H_
#define MAILBOX_H_

/*
==================================================
  Start Section --> File Includes
==================================================
 */
#include "EduRTOS_Config.h"
#include "EduRTOS_defines.h"
#include "Semaphore/semaphore.h"

/*
==================================================
  End Section --> File Includes
==================================================
 */

/*
==================================================
  Start Section --> Mailbox defines
==================================================
 */
typedef struct{
	Edu_u32 *MailboxBuffer;
	Edu_u32 BufferSize;

	Edu_u32 MailboxLength;
	Edu_u32 Front;
	Edu_u32 Rear;

	Semaphore ReadingSemaphore;
	Semaphore WritingSemaphore;
}MailBox;

typedef enum{
	EduRTOS_Mailbox_Error,
	EduRTOS_Mailbox_Full,
	EduRTOS_Mailbox_Empty,
	EduRTOS_Mailbox_WriteSucceed,
	EduRTOS_Mailbox_ReadSucceed
}EduMailbox_State;
/*
==================================================
  End Section --> Mailbox defines
==================================================
 */

/*
==================================================
  Start Section --> APIs
==================================================
 */

/* @brief  				--> Initialize Mailbox.
 * @param  MailBox 		--> Pointer of mailbox.
 * @param  Arg_Array	--> Array that holds mailbox elements.
 * @param  BufferSize 	--> Size of array.
 * @retval --> Status if it's initialized successfully or not.
 * */
EduRTOS_StdType_Status MailBoxInit(MailBox *Arg_Mailbox,Edu_u32 *Arg_Array , Edu_u32 Arg_BufferSize);

/* @brief  				--> Write data to Mailbox.
 * @param  MailBox 		--> Pointer of mailbox.
 * @param  Arg_Data 	--> Data that you need to write in Mailbox.
 * @param  Arg_TicksToWait  --> Timeout value for writing to mailbox.
 * @retval --> Status if data written successfully ,or failed due to specific reason like mailbox is full.
 * */
EduMailbox_State MailBoxWrite(MailBox *Arg_Mailbox, Edu_u32 Arg_Data, const Edu_u32 Arg_TicksToWait);

/* @brief  --> Read data from Mailbox.
 * @param  MailBox 		--> Pointer of mailbox.
 * @param  Arg_Data 	--> Pointer of data that will be stored in.
 * @param  Arg_TicksToWait  --> Timeout value for reading from mailbox.
 * @retval --> Status if data reading is successfully ,or failed due to specific reason like mailbox is empty.
 * */
EduMailbox_State MailBoxRead(MailBox *Arg_Mailbox, Edu_u32 *Arg_Data, const Edu_u32 Arg_TicksToWait);
/*
==================================================
  End Section --> APIs
==================================================
 */
#endif /* MAILBOX_H_ */
