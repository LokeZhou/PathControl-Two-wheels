/**
  ******************************************************************************
  * @file      
  * @author  2016Robcon-NEU Action团队
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  *
  * 
  ******************************************************************************
  */ 
/* Includes -------------------------------------------------------------------*/
#include <math.h>
#include "walk.h"
#include "stdint.h"
#include "math.h"
#include "elmo.h"
#include "timer.h"
#include "usart.h"
#include "gpio.h"
#include "update.h"
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/


/*************************************************************************/
/*----------------------------Walking Part-------------------------------*/
/*************************************************************************/
//实际速度转脉冲速度 
int VelTransform(float vel)
{
	return ((vel*2000*REDUCE_RATE*43/30)/(PI*2*WHEEL_RADIUS));//500线（500脉转一圈），减速比，直径
}


void LockWheel(void)
{
	VelCrl(1,0);
	VelCrl(2,0);
}


//vel=mm/s,rotate=度数
//+ClockWise -AntiClock
//最好不要在task.c中调用此函数
void BasicWalk(float vel,float rotate)
{ 
	float temp_vel[2];
  
	rotate=rotate/180*PI;
	
	temp_vel[0]=(vel+rotate*WHEEL_DISTANCE/2);
	temp_vel[1]=(vel-rotate*WHEEL_DISTANCE/2);
	
	temp_vel[0]=VelTransform(temp_vel[0]);
	temp_vel[1]=VelTransform(temp_vel[1]);
	VelCrl(1,-temp_vel[0]);
	VelCrl(2, temp_vel[1]);
}

uint8_t CloseLine(float ref_angle,float desx,float desy,float vel)
{
	float p=0;
	float errangle=0;
	float tempdisx,tempdisy;
	p=2.0f;
	tempdisx=desx-GetTempPosx();
	tempdisy=desy-GetTempPosy(); 
	
		     if((tempdisx>=0)&&(tempdisy>=0))
			 {
				 if((fabs(tempdisx)>fabs(tempdisy)))
				 {
					 while(!(GetPosx()>=desx))
						 {
							errangle=GetAngle()-ref_angle;
	            BasicWalk(vel,errangle*p);
						 }
				 }
				 else  
				 {
					 while(!(GetPosy()>=desy))
						 {
						  errangle=GetAngle()-ref_angle;
	            BasicWalk(vel,errangle*p);
					   }
				 }
			 }
	else if((tempdisx<=0)&&(tempdisy>=0))
	{
		     if((fabs(tempdisx)>fabs(tempdisy)))
				 {
					 while(!(GetPosx()<=desx))
						 {
							errangle=GetAngle()-ref_angle;
	            BasicWalk(vel,errangle*p);
						 }
				 }
				 else  
				 {
					 while(!(GetPosy()>=desy))
						 {
						  errangle=GetAngle()-ref_angle;
	            BasicWalk(vel,errangle*p);
					   }
				 }
	}
	else if((tempdisx>=0)&&(tempdisy<=0))
	{
		     if((fabs(tempdisx)>fabs(tempdisy)))
				 {
					 while(!(GetPosx()>=desx))
						 {
							errangle=GetAngle()-ref_angle;
	            BasicWalk(vel,errangle*p);
						 }
				 }
				 else  
				 {
					 while(!(GetPosy()<=desy))
						 {
						  errangle=GetAngle()-ref_angle;
	            BasicWalk(vel,errangle*p);
					   }
				 }
	}
	else if((tempdisx<=0)&&(tempdisy<=0))
	{
		     if((fabs(tempdisx)>fabs(tempdisy)))
				 {
					 while(!(GetPosx()<=desx))
						 {
							errangle=GetAngle()-ref_angle;
	            BasicWalk(vel,errangle*p);
						 }
				 }
				 else  
				 {
					 while(!(GetPosy()<=desy))
						 {
						  errangle=GetAngle()-ref_angle;
	            BasicWalk(vel,errangle*p);
					   }
				 }
	}
	BasicWalk(0,0);
	
	SetTempPosx(GetPosx());
  SetTempPosy(GetPosy());	
	return 0;
}

//rotate=度数
//rotate +ClockWise -AntiClock
uint8_t Circle(float radius,float des_angle,float rotate)
{
	float temp_vel[2];
	rotate=rotate/180*PI;
	
	temp_vel[0]=fabsf(rotate)*(radius+rotate/fabsf(rotate)*WHEEL_DISTANCE/2);
	temp_vel[1]=fabsf(rotate)*(radius-rotate/fabsf(rotate)*WHEEL_DISTANCE/2);
	
	temp_vel[0]=VelTransform(temp_vel[0]);
	temp_vel[1]=VelTransform(temp_vel[1]);
	
	VelCrl(1,-temp_vel[0]);
	VelCrl(2, temp_vel[1]);
	
	if(GetTempAngle()>0&&des_angle<0&&rotate<0){
		
	}//special
	if(GetTempAngle()<0&&des_angle>0&&rotate>0){
		
	}
	if(rotate>0)     while(!(GetAngle()<des_angle));
	else if(rotate<0)while(!(GetAngle()>des_angle));	

	BasicWalk(0,0);
	
	SetTempPosx(GetPosx());
  SetTempPosy(GetPosy());
  SetTempAngle(GetAngle());	
	return 0;
}


//rotate=PI
uint8_t ToDestination(float desx,float desy,float des_angle,float vel,float rotate)
{
	float temp_angle1,temp_angle2,temp_angle;
	float sou_xo,sou_yo,des_xo,des_yo;
	float distance;
	//算圆心
	/****目标点在相对第二象限******/
	sou_xo=GetTempPosx()-RADIUS*sinf(PI/180*GetTempAngle());
	sou_yo=GetTempPosy()+RADIUS*cosf(PI/180*GetTempAngle());
	
	des_xo=desx+RADIUS*sinf(PI/180*GetTempAngle());
	des_yo=desy-RADIUS*cosf(PI/180*GetTempAngle());
	
	distance=sqrtf(powf(des_yo-sou_yo,2)+powf(des_xo-sou_xo,2));	
	
	temp_angle1=180/PI*atan2f((des_yo-sou_yo),(des_xo-sou_xo));//(-180,+180];
	temp_angle2=180/PI*asinf (RADIUS/(distance/2));//(-180,+180];
	
	temp_angle =temp_angle1+temp_angle2;
	
	if((GetAngle()>=0&&temp_angle>=0)||(GetAngle()<=0&&temp_angle<=0)){
		if(temp_angle<=GetAngle())//GetAngle())
		{
		 Circle(RADIUS,temp_angle,rotate);//ClockWise
		 while(!(temp_angle>=GetAngle()));//GetAngle()));
		}
		else 
		{
			Circle(RADIUS,temp_angle,-rotate);
			if(fabs(temp_angle-180)<1.5f)while(!((temp_angle-1.5f)<=GetAngle()));
			else while(!(temp_angle<=GetAngle()));
		}
	}
	
	else if(GetAngle()>0&&temp_angle<0){
		if(temp_angle>(GetAngle()-180))
		{
			Circle(RADIUS,temp_angle,rotate);//ClockWise
			while(!(temp_angle>=GetAngle()));
		}
	  else 
		{
			Circle(RADIUS,temp_angle,-rotate);//Anti-Clock
			while(!((temp_angle<=GetAngle())&&(GetAngle()<0)));
		}
	}
	
	else if(GetAngle()<0&&temp_angle>0){
		if(temp_angle>GetAngle()+180)
		{
			Circle(RADIUS,temp_angle,rotate);//ClockWise
			while(!((temp_angle>=GetAngle())&&(GetAngle()>0)));
		}
		else 
		{
			Circle(RADIUS,temp_angle,-rotate);
			while(!(temp_angle<=GetAngle()));
		}
	}
	
	desx=desx-RADIUS*sinf(PI/180*(180-temp_angle));
	desy=desy-RADIUS*cosf(PI/180*(180-temp_angle));
	CloseLine(temp_angle,desx,desy,vel);
	
	if((GetAngle()>=0&&des_angle>=0)||(GetAngle()<=0&&des_angle<=0)){
		if(des_angle<=GetAngle())//GetAngle())
		{
		 Circle(RADIUS,des_angle,rotate);//ClockWise
		 while(!(des_angle>=GetAngle()));//GetAngle()));
		}
		else 
		{
			Circle(RADIUS,des_angle,-rotate);
			if(fabs(des_angle-180)<1.5f)while(!((des_angle-1.5f)<=GetAngle()));
			else while(!(des_angle<=GetAngle()));
		}
	}
	
	else if(GetAngle()>0&&des_angle<0){
		if(des_angle>(GetAngle()-180))
		{
			Circle(RADIUS,des_angle,rotate);//ClockWise
			while(!(des_angle>=GetAngle()));
		}
	  else 
		{
			Circle(RADIUS,des_angle,-rotate);//Anti-Clock
			while(!((des_angle<=GetAngle())&&(GetAngle()<0)));
		}
	}
	
	else if(GetAngle()<0&&des_angle>0){
		if(des_angle>GetAngle()+180)
		{
			Circle(RADIUS,temp_angle,rotate);//ClockWise
			while(!((des_angle>=GetAngle())&&(GetAngle()>0)));
		}
		else 
		{
			Circle(RADIUS,des_angle,-rotate);
			while(!(des_angle<=GetAngle()));
		}
	}
	
	BasicWalk(0,0);
	SetTempPosx(GetPosx());
  SetTempPosy(GetPosy());
	return 0;
	
}






/************************ (C) COPYRIGHT 2015 ACTION *****END OF FILE****/

