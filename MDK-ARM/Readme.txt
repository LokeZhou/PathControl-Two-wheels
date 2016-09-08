Ç°ÂÖµ½ºóÂÖ  178mm
ÂÖ¿í         40mm
float temp_angle;
	float distance;
	distance=sqrtf(powf(desy-GetTempPosy(),2)+powf(desx-GetTempPosx(),2));
	temp_angle=180/PI*asinf(RADIUS/(distance/2));//(-180,+180];