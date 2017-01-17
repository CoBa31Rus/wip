
#ifndef PID_H
	#define PID_H
	//Ограничители составляющих
	#define MAX_ERR 14
	#define MAX_PRES 100
	#define MIN_PID 0
	#define MAX_PID 255
	//Масштабирование
	#define SCALING 1


	typedef struct PID_DATA{
		unsigned char pK, iK, dK;
		int pres, ires, dres, lastTerm;
		char sumError;
	}pidData_t;

	void pid_init(unsigned char _pK, unsigned char _iK, unsigned char _dK);
	unsigned char calc_pwm(int needT, int realT);
#endif
