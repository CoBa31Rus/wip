
#include "pid.h"

unsigned char pK = 0, iK = 0, dK = 0; //Коэффициенты
int pres, ires, dres; //Состовляющие регулятора
int lastTerm;
char sumError;

void pid_init(unsigned char _pK, unsigned char _iK, unsigned char _dK){
	pK = _pK;
	iK = _iK;
	dK = _dK;
	lastTerm =0;
	sumError = 0;
}

unsigned char calc_pwm(int needT, int realT){
	int err, tmp;

	err = needT - realT;
	//Пропорцианальный
	pres = pK * err;
	if(pres > MAX_PRES)
		pres = MAX_PRES;
	if(pres < -MAX_PRES)
		pres = -MAX_PRES;

	//Интегральный
	tmp = sumError + err;
	if(tmp > MAX_ERR)
		tmp = MAX_ERR;
	if(tmp < -MAX_ERR)
		tmp = -MAX_ERR;
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
