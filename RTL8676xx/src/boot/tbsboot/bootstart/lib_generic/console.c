

#include <common.h>
#include <stdarg.h>
#include <malloc.h>
#include <console.h>

void serial_printf (const char *fmt, ...)
{
	va_list args;
	uint i;
	char printbuffer[CFG_PBSIZE];

	va_start (args, fmt);

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	i = vsprintf (printbuffer, fmt, args);
	va_end (args);

	serial_puts (printbuffer);
}

int getc (void)
{


	/* Send directly to the handler */
	return serial_getc ();
}

int tstc (void)
{

	/* Send directly to the handler */
	return serial_tstc ();
}

void putc (const char c)
{

		/* Send directly to the handler */
		serial_putc (c);

}

void puts (const char *s)
{

		/* Send directly to the handler */
		serial_puts (s);

}

void printf (const char *fmt, ...)
{
	va_list args;
	uint i;
	char printbuffer[CFG_PBSIZE];

	va_start (args, fmt);

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	i = vsprintf (printbuffer, fmt, args);
	va_end (args);

	/* Print the string */
	puts (printbuffer);
}

void vprintf (const char *fmt, va_list args)
{
	uint i;
	char printbuffer[CFG_PBSIZE];

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	i = vsprintf (printbuffer, fmt, args);

	/* Print the string */
	puts (printbuffer);
}

/* test if ctrl-c was pressed */
static int ctrlc_disabled = 0;	/* see disable_ctrl() */
static int ctrlc_was_pressed = 0;
int ctrlc (void)
{
	if (!ctrlc_disabled && gd->have_console) {
		if (tstc ()) {
			switch (getc ()) {
			case 0x03:		/* ^C - Control C */
				ctrlc_was_pressed = 1;
				return 1;
			default:
				break;
			}
		}
	}
	return 0;
}

int esc (void)
{
	if (tstc ()) 
	{
		switch (getc ()) 
		{
			case 0x1b:		/* esc */
				return 1;
			default:
				break;
		}
	}	
	return 0;
}

/* pass 1 to disable ctrlc() checking, 0 to enable.
 * returns previous state
 */
int disable_ctrlc (int disable)
{
	int prev = ctrlc_disabled;	/* save previous state */

	ctrlc_disabled = disable;
	return prev;
}

int had_ctrlc (void)
{
	return ctrlc_was_pressed;
}

void clear_ctrlc (void)
{
	ctrlc_was_pressed = 0;
}


inline void dbg(const char *fmt, ...)
{
}

int console_init_f (void)
{
	gd->have_console = 1;

	return (0);
}



/* Called after the relocation - use desired console functions */
int console_init_r (void)
{
//	device_t *inputdev = NULL, *outputdev = NULL;
//	int i, items = ListNumItems (devlist);

	return (0);
}

