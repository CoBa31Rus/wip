
#include "pid.h"

unsigned char pK = 0, iK = 0, dK = 0;
int pres, ires, dres;
int lastTerm = 0;
int sumError = 0;

unsigned char calc_pwm(int needT, int realT){
	int err = 0, tmp = 0;

	err = needT - realT;
	//Пропорцианальный; Prop
	pres = pK * err;
	if(pres > MAX_PRES)
		pres = MAX_PRES;
	if(pres < -MAX_PRES)
		pres = -MAX_PRES;

	//Интегральный; Integ
	tmp = sumError + err;
	if(tmp > MAX_ERR)
		tmp = MAX_ERR;
	if(tmp < -MAX_ERR)
		tmp = -MAX_ERR;
	ires = (tmp/100) * iK;
	sumError = tmp;

	//Дифференциальный; Div
	dres = dK * (realT - lastTerm);
	lastTerm = realT;

	//Результирующий; Result
	tmp = (pres + ires - dres) / SCALING;

	if(tmp > MAX_PID)
		tmp = MAX_PID;
	if(tmp < MIN_PID)
		tmp = MIN_PID;
	return tmp;
}
