#include  <includes.h>
#include  <app_cfg.h>
#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "timer.h"
#include "walk.h"
#include "gpio.h"
#include "usart.h"
#include "can.h"
#include "elmo.h"
#include "ucos_ii.h"
#include "update.h"

//////////////////Area of defining semaphore////////////////////////
OS_EVENT 		  			*PeriodSem;
int8_t status						   = 0 ;
   
void App_Task()
{
	CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */
	
	/******************Create Semaphore***********************/
    PeriodSem				=	OSSemCreate(0);
  /******************Create Task**************************/	
	 os_err = OSTaskCreate(	(void (*)(void *)) ConfigTask,					//Initial Task
	                      	(void          * ) 0,							
													(OS_STK        * )&App_ConfigStk[Config_TASK_START_STK_SIZE-1],		
													(INT8U           ) Config_TASK_START_PRIO  );	
						
													
	 os_err = OSTaskCreate(	(void (*)(void *)) WalkTask,					
	                      	(void          * ) 0,							
													(OS_STK        * )&WalkTaskStk[Walk_TASK_STK_SIZE-1],		
													(INT8U           ) Walk_TASK_PRIO  );				
}

void ConfigTask(void)
{	
	  CPU_INT08U  os_err;
	  os_err = os_err;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	  CAN_Config(CAN1,500,GPIOB,GPIO_Pin_8, GPIO_Pin_9);  //1 1
	  TIM_Delayms(TIM5,1000);
	  GYRO_USART2_Init(115200);
    TIM_Delayms(TIM5,14000);
		elmo_Init();
		elmo_Enable(1);//Driver 1
		elmo_Enable(2);//Driver 2
		
		/*********************MOTOR DRIVE INTI**************************/
		Vel_cfg(1,100000,100000);  //
		Vel_cfg(2,100000,100000);  //configuring Acceleration and Deceleration
    TIM_Delayms(TIM5,6000);
	
	  /*********************TIMER INIT*******************************/
	  TIM_Init(TIM2,999,839,3,3);//3 3
  	OSSemSet(PeriodSem,0,&os_err);	
		OSTaskSuspend(OS_PRIO_SELF);
}

void WalkTask(void)
{
	CPU_INT08U  os_err;
	
  while(DEF_TRUE)
	{
	  OSSemPend(PeriodSem,0,&os_err);     //release once semaphore per 5 millisecond
		
		switch(status)
		{
			case 0:/*出发区直线*/
			  SetTempPosx(GetPosx());
			  SetTempPosy(GetPosy());
			  status++;
			  break;
			case 1:
				//CloseLine(500,90);
			  Circle(0,30,20);
			  ToDestination(-4000,4000, 60, 500,20);
			  status++;
			  
			  break;
			case 2:
			  
			  break;
			
			default:
				break;
		 }
	}
}

