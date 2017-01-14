/*
 * pid.c
 *
 *  Created on: 14 янв. 2017 г.
 *      Author: coba31rus
 */

#include "pid.h"

int err;
unsigned char tmp_pid, p_const = 1;

unsigned char update_pid(int n_temp, int r_temp){
	err = n_temp - r_temp;
	if(err<0)
		return 0;
	tmp_pid = 0;
	tmp_pid += p_const*err;

	return tmp_pid;
}
