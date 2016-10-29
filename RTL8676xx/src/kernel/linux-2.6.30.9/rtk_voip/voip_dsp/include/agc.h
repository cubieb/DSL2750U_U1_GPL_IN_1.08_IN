#ifndef AGC_H
#define AGC_H

#define AGC_RX_DIR	0 /* phone mic -> dsp -> network */
#define AGC_TX_DIR	1 /* network -> dsp -> phone speaker */

/*
 * chid: channel id 0 ~ 15
 * dir: direction, 0:AGC_RX_DIR ; 1:AGC_TX_DIR
 */

void rtk_agc_reset(unsigned int chid, unsigned int dir);

/*
 *  lvl: target level 0~8, 0 -> min level, 8 -> max level
 *  max gain up: inpuat range 0~9,  0 -> 0db, 1 -> 1db, ... , 9 -> 9db
 *  max gain down: input range 0~9, 0 -> 0db, 1 -> -1db, ... , 9 -> -9db
 */
void rtk_agc_set_lvl(unsigned int chid, unsigned int dir, unsigned int lvl, unsigned int max_gain_up, unsigned int max_gain_down);

/*
 * addr: address of voice, 80 sample (160byte)
 */
void rtk_agc(unsigned int chid, unsigned int dir, short* addr);



#endif