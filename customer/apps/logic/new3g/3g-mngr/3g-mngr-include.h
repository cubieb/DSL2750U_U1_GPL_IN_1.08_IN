#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if !defined(BRCM)
#include <time.h>
#endif
#include <signal.h>

typedef unsigned char UBOOL8;


#include "lib/3g-lib-cmd-msg.h"
#include "lib/3g-lib.h"
#include "3g-mngr-comm.h"
#include "3g-mngr-modem.h"
#include "3g-mngr-at.h"
#include "3g-mngr-parameter.h"
#include "3g-mngr-dial.h"
#include "3g-mngr-switch.h"
#include "3g-mngr-pin.h"
#include "3g-mngr-connect-check.h"
#include "3g-mngr-wan.h"
#include "3g-mngr-web-interface.h"
#include "3g-mngr.h"
#include "3g-mngr-version.h"
