
#include "pid.h"

//unsigned char pK = 0, iK = 0, dK = 0; //Коэффициенты
//int pres, ires, dres; //Состовляющие регулятора
//int lastTerm;
//char sumError;
/*
void pid_init(unsigned char _pK, unsigned char _iK, unsigned char _dK){
	pK = _pK;
	iK = _iK;
	dK = _dK;
	lastTerm =0;
	sumError = 0;
}
*/
unsigned char calc_pwm(int needT, int realT, pidData_t* p){
	int err = 0, tmp = 0;

	err = needT - realT;
	//Пропорцианальный
	p->pres = p->pK * err;
	if(p->pres > MAX_PRES)
		p->pres = MAX_PRES;
	if(p->pres < -MAX_PRES)
		p->pres = -MAX_PRES;

	//Интегральный
	tmp = p->sumError + err;
	if(tmp > MAX_ERR)
		tmp = MAX_ERR;
	if(tmp < -MAX_ERR)
		tmp = -MAX_ERR;
	p->ires = tmp * p->iK;
	p->sumError = tmp;

	//Дифференциальный
	p->dres = p->dK * (p->lastTerm - realT);
	p->lastTerm = realT;

	//Результирующий
	tmp = (p->pres + p->ires + p->dres) / SCALING;

	if(tmp > MAX_PID)
		tmp = MAX_PID;
	if(tmp < MIN_PID)
		tmp = MIN_PID;

	return tmp;
}
