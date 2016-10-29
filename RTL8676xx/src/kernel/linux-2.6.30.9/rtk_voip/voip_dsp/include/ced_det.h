#ifndef CED_DET_H
#define CED_DET_H
/* ced.c function prototype */

char CED_routine(int chid); /* return value: 1: fax, 2: local-modem, 3: remote-modem */
void ced_time_power_ths_set(int power);	/* set the power threshold for ced tone detection */
void modem_time_power_local_ths_set(int power); /* set the power threshold for local modem tone detection */
void modem_time_power_remote_ths_set(int power); /* set the power threshold for remote modeom tone detection */


#endif

