#ifndef __3G_MNGR_PIN_H_
#define __3G_MNGR_PIN_H_

typedef struct {
	void *mngr;
	DEF_PRI;
} pin_mngr_t;

pin_mngr_t *pin_mngr_new(void *mn);
int pin_mngr_init(pin_mngr_t *pin);
int pin_mngr_free(pin_mngr_t *pin);

int pin_mngr_lock(pin_mngr_t *pin, int argc, char *argv[]);
int pin_mngr_set_pin(pin_mngr_t *pin, int argc, char *argv[]);
int pin_mngr_set_puk(pin_mngr_t *pin, int argc, char *argv[]);
int pin_mngr_chg_pin(pin_mngr_t *pin, int argc, char *argv[]);

#endif
