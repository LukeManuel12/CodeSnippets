#include "header.h"
#define RADIAN_MULTIPLYER 0.0174532925
#define PI 3.14159265

float DegreeToRadian(float degree)
{	
	fixAngle(&degree);
	/*
	float angle = degree;
	double pi = (atan(1.00)*4);	
	return (float)((angle / 180.0f) * pi);
	*/
	return (float)(RADIAN_MULTIPLYER * degree);
}
float RadianToDegree(float radian)
{	
	/*
	float angle = radian;
	double pi = (atan(1.00)*4);	
	return (float)((angle * 180.0f) / pi);
	*/
	float degree = (float)(radian / RADIAN_MULTIPLYER);
	fixAngle(&degree);
	return degree;
}
void fixAngle(float * degree)
{
	while(*degree >= 360)
		*degree -= 360;	
	while(*degree <= 0)
		*degree += 360;
}
void fixRadian(float * radian)
{
	float degree = RadianToDegree(*radian);
	fixAngle(&degree);
	*radian = DegreeToRadian(degree);
}