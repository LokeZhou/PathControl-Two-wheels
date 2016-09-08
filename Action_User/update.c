#include "update.h"

 float angle=0;
 float pos_x=0;
 float pos_y=0;

float tempangle=0;
float temppos_x=0;
float temppos_y=0;

//Angle
void SetAngle(float temp)
{
	angle=temp;
}
float GetAngle(void)
{
	return angle;
}
//Tempangle
void SetTempAngle(float temp)
{
	tempangle=temp;
}
float GetTempAngle(void)
{
	return tempangle;
}
//Posx
void SetPosx(float temp)
{
	pos_x=temp;
}
float GetPosx(void)
{
	return pos_x;
}
//TempPosx
void SetTempPosx(float temp)
{
	temppos_x=temp;
}
float GetTempPosx(void)
{
	return temppos_x;
}
//Posy
void SetPosy(float temp)
{
	pos_y=temp;
}
float GetPosy(void)
{
	return pos_y;
}
//TempPosy
void SetTempPosy(float temp)
{
	temppos_y=temp;
}
float GetTempPosy(void)
{
	return temppos_y;
}


