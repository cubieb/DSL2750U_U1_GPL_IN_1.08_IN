/* vi: set sw=4 ts=4: */
/*
 * seq implementation for busybox
 *
 * Copyright (C) 2004, Glenn McGrath
 *
 * Licensed under the GPL v2, see the file LICENSE in this tarball.
 */

#include "libbb.h"

/* This is a NOFORK applet. Be very careful! */


int seq_main(int argc, char **argv);
int seq_main(int argc, char **argv)
{
	int last, increment, i;

	i = increment = 1;
	switch (argc) {
		case 4:
			increment = atoi(argv[2]);
		case 3:
			i = atoi(argv[1]);
		case 2:
			last = atoi(argv[argc-1]);
			break;
		default:
			bb_show_usage();
	}

	/* You should note that this is pos-5.0.91 semantics, -- FK. */
	while ((increment > 0 && i <= last) || (increment < 0 && i >= last)) {
		printf("%d\n", i);
		i += increment;
	}

	return fflush(stdout);
}
