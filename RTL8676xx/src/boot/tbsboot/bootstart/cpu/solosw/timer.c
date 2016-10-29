
#include <common.h>

#define TIMER_LOAD_VAL	0x800000

#define TIMER_OUTPUT	0Xff000100
#define TIMER_VAL	0xff000108
#define TIMER_CTRL	0xff000114
#define TIMER_RELOAD	0xff000118

/* macro to read the 32 bit timer */
#define READ_TIMER (*(volatile ulong *)(TIMER_VAL))

static ulong timestamp;
static ulong lastdec;

int timer_init (void)
{
	int32_t val;


	/* Start the decrementer ticking down from 0xffffffff */

	//*((int32_t *) (TIMER_CTRL)) = 0x9; 

	//*((int32_t *) (TIMER_VAL)) = TIMER_LOAD_VAL; 

	//*((int32_t *) (TIMER_RELOAD)) = TIMER_LOAD_VAL;

	*((int32_t *) (TIMER_OUTPUT)) = 0x9 ;   /*Enable timer */

	/* init the timestamp and lastdec value */
	reset_timer_masked();

	return 0;
}

/*
 * timer without interrupts
 */

void reset_timer (void)
{
	reset_timer_masked ();
}

ulong get_timer (ulong base)
{
	return get_timer_masked () - base;
}

void set_timer (ulong t)
{
	timestamp = t;
}

/* delay x useconds AND perserve advance timstamp value */
void udelay (unsigned long usec)
{
	ulong tmo, tmp;

        if(usec >= 1000){               /* if "big" number, spread normalization to seconds */
                tmo = usec / 1000;      /* start to normalize for usec to ticks per sec */
                tmo *= CFG_HZ;          /* find number of "ticks" to wait to achieve target */
                tmo /= 1000;            /* finish normalize. */
        }else{                          /* else small number, don't kill it prior to HZ multiply */
                tmo = usec * CFG_HZ;
                tmo /= (1000*1000);
        }

	tmp = get_timer (0);		/* get current timestamp */
	if( (tmo + tmp + 1) < tmp )	/* if setting this fordward will roll time stamp */
		reset_timer_masked ();	/* reset "advancing" timestamp to 0, set lastdec value */
	else
		tmo += tmp;		/* else, set advancing stamp wake up time */

	while (get_timer_masked () < tmo)/* loop till event */
		/*NOP*/;
}

void reset_timer_masked (void)
{
	/* reset time */
	lastdec = READ_TIMER;  /* capure current decrementer value time */
	timestamp = 0;	       /* start "advancing" time stamp from 0 */
}

ulong get_timer_masked (void)
{
	ulong now = READ_TIMER;		/* current tick value */

	if (lastdec < now) {		/* normal mode (non roll) */
		/* normal mode */
		timestamp += now - lastdec ; /* move stamp fordward with absoulte diff ticks */
	} else {			/* we have overflow of the count down timer */

		timestamp +=  TIMER_LOAD_VAL - lastdec + now ;

	}
	lastdec = now;

	return timestamp;
}

/* waits specified delay value and resets timestamp */
void udelay_masked (unsigned long usec)
{
	ulong tmo;
	ulong endtime;
	signed long diff;


	if (usec >= 1000) {             /* if "big" number, spread normalization to seconds */
                tmo = usec / 1000;      /* start to normalize for usec to ticks per sec */
                tmo *= CFG_HZ;          /* find number of "ticks" to wait to achieve target */
                tmo /= 1000;            /* finish normalize. */
        } else {                        /* else small number, don't kill it prior to HZ multiply */
                tmo = usec * CFG_HZ;
                tmo /= (1000*1000);
        }


	endtime = get_timer_masked () + tmo;

	do {
		ulong now = get_timer_masked ();
		diff = endtime - now;
	} while (diff >= 0);
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk (void)
{
	ulong tbclk;

	tbclk = CFG_HZ;
	return tbclk;
}
