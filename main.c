#include "main.h"

//memory location to write to in the device
__IO uint16_t memLocation = 0x000A; //pick any location within range

uint8_t Tx1_Buffer = 'p';
uint8_t Rx1_Buffer;
uint16_t NumDataRead = 1;
RTC_TimeTypeDef current_time_struct;

RTC_AlarmTypeDef RTC_alarm;

char current_time_string[10];
uint16_t line;
int i,j=0;
uint8_t RTC_second_alarm=0;
 
int main(void){
	
	//configure push-button interrupts
	PB_Config();
	
	 /* LCD initiatization */
  LCD_Init();
  
  /* LCD Layer initiatization */
  LCD_LayerInit();
    
  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

	
	
	//======You need to develop the following functions======
	//Note: these are just placeholders; function definitions are at bottom of this file
	//configure real-time clock
	RTC_Config();
	
	//configure external push-buttons and interrupts
	ExtPB_Config();
	
	
	//main program
	
	LCD_Clear(LCD_COLOR_WHITE);
		
	line=0;
	//Display a string in one line, on the first line (line=0)
	LCD_DisplayStringLine(LINE(line),  (uint8_t *) "Init EEPROM...");
	line++;
	
	//i2c_init(); //initialize the i2c chip
	//sEE_Init();  

	
	LCD_DisplayStringLine(LINE(line),  (uint8_t *) "done..."); 
	line++;
	
	LCD_DisplayStringLine(LINE(line),  (uint8_t *) "Writing...");
	line++;
	
	
	/* First write in the memory followed by a read of the written data --------*/
  /* Write on I2C EEPROM from memLocation */
  //sEE_WriteBuffer(&Tx1_Buffer, memLocation,1); 

  /* Wait for EEPROM standby state */
  //sEE_WaitEepromStandbyState();  
 
  
	LCD_DisplayStringLine(LINE(line),  (uint8_t *) "Reading...");
  /* Read from I2C EEPROM from memLocation */
 // sEE_ReadBuffer(&Rx1_Buffer, memLocation, (uint16_t *)(&NumDataRead)); 
	line++;
	
	LCD_DisplayStringLine(LINE(line),  (uint8_t *) "Comparing...");  
	line++;
	
	
	/*if(Tx1_Buffer== Rx1_Buffer){
		LCD_DisplayStringLine(LINE(line),  (uint8_t *) "Success!");  
	}else{
		LCD_DisplayStringLine(LINE(line),  (uint8_t *) "Mismatch!"); 
	}*/
	
	//main loop
	while(1){
		if(RTC_second_alarm){
					//LCD_Clear(LCD_COLOR_WHITE);
						//LCD_DisplayStringLine(4,  (uint8_t *)"interrupt triggere"); 
					//while(1);
		RTC_GetTime(RTC_Format_BIN,&current_time_struct);
		sprintf(current_time_string,"%02d:%02d:%02d",current_time_struct.RTC_Hours,current_time_struct.RTC_Minutes,current_time_struct.RTC_Seconds);
		LCD_Clear(LCD_COLOR_WHITE);
		LCD_DisplayStringLine(4,  (uint8_t *)current_time_string);
			RTC_second_alarm=0;
		}
	/*	for(i=0; i< 10; i++){
			for (j=0;j<10000;j++){}
		}
		RTC_GetTime(RTC_Format_BIN,&current_time_struct);
		sprintf(current_time_string,"%02d:%02d:%02d",current_time_struct.RTC_Hours,current_time_struct.RTC_Minutes,current_time_struct.RTC_Seconds);
		LCD_Clear(LCD_COLOR_WHITE);
		LCD_DisplayStringLine(4,  (uint8_t *)current_time_string);  */

		
	}
}



void PB_Config(void)
{
/* Initialize User_Button on STM32F4-Discovery
   * Normally one would need to initialize the EXTI interrupt
   * to handle the 'User' button, however the function already
   * does this.
   */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
}


/**
 * Use this function to configure the GPIO to handle input from
 * external pushbuttons and configure them so that you will handle
 * them through external interrupts.
 */
void ExtPB_Config(void){
	
}

/**
 * Configure the RTC to operate based on the LSI (Internal Low Speed oscillator)
 * and configure one of the alarms (A or B) to trigger an external interrupt every second
 * (e.g. EXTI line 17 for alarm A).
 */
void RTC_Config(void){
	
		  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
	
	RTC_InitTypeDef RTC_InitStructure;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;
/* Enable write access to the RTC ****************************/
/* Enable the PWR clock */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
/* Allow access to RTC */
PWR_BackupAccessCmd(ENABLE);
/* Configure the RTC clock source ****************************/
/* Enable the LSE OSC */
//RCC_LSEConfig(RCC_LSE_ON);
RCC_LSICmd(ENABLE);
/* Wait till LSE is ready */
while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	/* Select the RTC Clock Source */
RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
/* Enable the RTC Clock */
RCC_RTCCLKCmd(ENABLE);
/* Wait for RTC APB registers synchronisation */
RTC_WaitForSynchro();
/* Configure the RTC calendar, Time and Date *****************/
/* RTC time base = LSE / ((AsynchPrediv+1) * (SynchPrediv+1))
= 1 Hz
*/
RTC_InitStructure.RTC_AsynchPrediv = 127;
RTC_InitStructure.RTC_SynchPrediv = 255;
RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
RTC_Init(&RTC_InitStructure);
/* Set the Time */
RTC_TimeStructure.RTC_Hours = 0x00;
RTC_TimeStructure.RTC_Minutes = 0x00;
RTC_TimeStructure.RTC_Seconds = 0x00;
RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
/* Set the Date */
RTC_DateStructure.RTC_Month = RTC_Month_May;
RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
RTC_DateStructure.RTC_Date = 0x30;
RTC_DateStructure.RTC_Year = 0x11;
RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

RTC_AlarmStructInit(&RTC_alarm);
  /*RTC_alarm.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
  RTC_alarm.RTC_AlarmTime.RTC_Hours   = 0x05;
  RTC_alarm.RTC_AlarmTime.RTC_Minutes = 0x20;
  RTC_alarm.RTC_AlarmTime.RTC_Seconds = 0x10;
  RTC_alarm.RTC_AlarmDateWeekDay = 0x31;
	  RTC_alarm.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;*/
		 // RTC_alarm.RTC_AlarmTime.RTC_Seconds = 0x1;

  RTC_alarm.RTC_AlarmMask = RTC_AlarmMask_All;



  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_alarm);
RTC_ITConfig(RTC_IT_ALRA, ENABLE);


RTC_AlarmCmd(RTC_Alarm_A,ENABLE);

RTC_ClearFlag(RTC_FLAG_ALRAF);

  /* Set the date: Friday January 11th 2013 */
 /* RTC_DateStructure.RTC_Year = 0x13;
  RTC_DateStructure.RTC_Month = RTC_Month_January;
  RTC_DateStructure.RTC_Date = 0x11;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);*/
  
  /* Set the time to 05h 20mn 00s AM */
  /*RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x05;
  RTC_TimeStructure.RTC_Minutes = 0x20;
  RTC_TimeStructure.RTC_Seconds = 0x00; 
  
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure); */

	
}
