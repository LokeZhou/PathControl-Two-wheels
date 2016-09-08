/**
  ******************************************************************************
  * @file    
  * @author  Tzy
  * @version 
  * @date   
  * @brief   This file contains the headers of walk.c
  ******************************************************************************
  * @attention
  *
  *
  * 
  * 
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __WALK_H__
#define  __WALK_H__

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define PI                3.1415926f
#define REDUCE_RATE       13/3   
#define WHEEL_RADIUS      63       //40  //178
#define WHEEL_DISTANCE    264


#define RADIUS            500


 
/* Exported functions ------------------------------------------------------- */
int VelTransform(float vel);
void LockWheel(void);
void BasicWalk(float vel,float rotate);
uint8_t Circle(float radius,float des_angle,float rotate);
uint8_t CloseLine(float ref_angle,float desx,float desy,float vel);
uint8_t ToDestination(float desx,float desy,float des_angle,float vel,float rotate);
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/

