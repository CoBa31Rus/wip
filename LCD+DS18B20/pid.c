
#include "pid.h"

unsigned char pK = 0, iK = 0, dK = 0;
int pres, ires, dres;
int lastTerm = 0;
char sumError = 0;

void pid_init(unsigned char _pK, unsigned char _iK, unsigned char _dK){
	pK = _pK;
	iK = _iK;
	dK = _dK;
}

unsigned char calc_pwm(int needT, int realT){
	int err = 0, tmp = 0;

	err = needT - realT;
	//Пропорцианальный
	pres = pK * err;
	if(pres > MAX_PRES)
		pres = MAX_PRES;
	if(pres < -MAX_PRES)
		pres = -MAX_PRES;

	//Интегральный
	tmp = sumError + err;
	if(tmp > MAX_ERR*pK)
		tmp = MAX_ERR*pK;
	if(tmp < -MAX_ERR*pK)
		tmp = -MAX_ERR*pK;
	ires = tmp * iK;
	sumError = tmp;

	//Дифференциальный
	dres = dK * (lastTerm - realT);
	lastTerm = realT;

	//Результирующий
	tmp = (pres + ires + dres) / SCALING;

	if(tmp > MAX_PID)
		tmp = MAX_PID;
	if(tmp < MIN_PID)
		tmp = MIN_PID;

	return tmp;
}
