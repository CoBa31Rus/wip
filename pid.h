
#ifndef PID_H
	#define PID_H
	//������������ ������������
	#define MAX_ERR 5000
	#define MAX_PRES 20000
	#define MIN_PID 0
	#define MAX_PID 255
	//���������������; Scaling for PWM
	#define SCALING 16

	unsigned char calc_pwm(int needT, int realT);
#endif
