/* Copyright (C) 2006 Ikanos Communications 
 * See IKANOS_PROP_LICENSE.txt for license information. */



/*****************************************************************************
 * File Name     : utilcommon.h
 * Author        : Arijit
 * Description   : Macro and structure for utilmain.c file
 *****************************************************************************/

#define SUCCESS		0
#define FAILURE		-1
#define VERSION		"\nSYSUTIL version 0.0 28-07-2005\n"

#define FALSE		0
#define TRUE		1
#define BOOL		unsigned char
#define SYSUTIL_PATH    "/bin/sysutil"
#define INSTALL_DIR     "/bin/"
#define  PATHLEN        40

	/* COMMAND MACROS FOR utilfunction */
#define SHOWUSAGE	1
#define CREATELINKS	2
#define SEARCHCMDS	3

#define SYSUTIL_COMMAND_SYNTAX "\nUSAGE: sysutil [function] [arguments] \n \
    \tor \n \
    [function] [arguments] \n\n \
sysutil is a multi-call binary that combines many diagnostic as well as utility commands into a single executable. Functional links are created to sysutil and when these are invoked, the appropriate function gets executed. \n\n \
Currently defined functions are: \n"

typedef struct util_cmds_s{
    const char *cmdname;
    int  (*main)(int argc, char **argv);
}util_cmds;

