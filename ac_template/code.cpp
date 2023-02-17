#include "pch.h"

void setwangle(float* r_yaw, float* w_yaw, float angle) {
	float temp_angle = fmodf(*r_yaw, 360);
	temp_angle -= temp_angle * 2 - angle;
	*w_yaw += temp_angle;
}

void __main() {
	
	if (KeyPressed(VK_INSERT)) {
		
		
		clients->cgameViewangles[YAW]; //r_yaw
		clients->viewangles[YAW]; //w_yaw

		setwangle(&clients->cgameViewangles[ROLL], &clients->viewangles[ROLL], 45);

	}
	
	Sleep(3);
}