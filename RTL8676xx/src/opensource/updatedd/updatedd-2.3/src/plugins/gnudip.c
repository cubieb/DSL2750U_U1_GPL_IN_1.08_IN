/* -- updatedd: dyndns.c --
 *
 * Copyright (C) 2002, 2003, 2004 Philipp Benner
 *
 * This file is part of UpdateDD - http://updatedd.philipp-benner.de.
 *
 * UpdateDD is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * UpdateDD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UpdateDD; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#include <get_connection.h>
#include <libexception_handle.h>
#include <ret_codes.h>
#include <version.h>

#include "../updatedd.h"
//#include "../../md5/md5.h"
#include "gnudip.h"

////////////////////////////////////
#ifndef PROTOTYPES
#define PROTOTYPES 1
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
   If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
  returns an empty list.
 */

#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

/* MD5 context. */
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

void MD5Init PROTO_LIST ((MD5_CTX *));
void MD5Update PROTO_LIST
  ((MD5_CTX *, unsigned char *, unsigned int));
void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));

/* Constants for MD5Transform routine.
 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static void MD5Transform PROTO_LIST ((UINT4 [4], unsigned char [64]));
static void Encode PROTO_LIST
  ((unsigned char *, UINT4 *, unsigned int));
static void Decode PROTO_LIST
  ((UINT4 *, unsigned char *, unsigned int));
static void MD5_memcpy PROTO_LIST ((POINTER, POINTER, unsigned int));
static void MD5_memset PROTO_LIST ((POINTER, int, unsigned int));

static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic MD5 functions.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
 */
#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }

/* MD5 initialization. Begins an MD5 operation, writing a new context.
 */
void MD5Init (context)
MD5_CTX *context;                                        /* context */
{
  context->count[0] = context->count[1] = 0;
  /* Load magic initialization constants.
*/
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
  operation, processing another message block, and updating the
  context.
 */
void MD5Update (context, input, inputLen)
MD5_CTX *context;                                        /* context */
unsigned char *input;                                /* input block */
unsigned int inputLen;                     /* length of input block */
{
  unsigned int i, index, partLen;

  /* Compute number of bytes mod 64 */
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);

  /* Update number of bits */
  if ((context->count[0] += ((UINT4)inputLen << 3))
   < ((UINT4)inputLen << 3))
 context->count[1]++;
  context->count[1] += ((UINT4)inputLen >> 29);

  partLen = 64 - index;

  /* Transform as many times as possible.
*/
  if (inputLen >= partLen) {
 MD5_memcpy
   ((POINTER)&context->buffer[index], (POINTER)input, partLen);
 MD5Transform (context->state, context->buffer);

 for (i = partLen; i + 63 < inputLen; i += 64)
   MD5Transform (context->state, &input[i]);

 index = 0;
  }
  else
 i = 0;

  /* Buffer remaining input */
  MD5_memcpy
 ((POINTER)&context->buffer[index], (POINTER)&input[i],
  inputLen-i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
  the message digest and zeroizing the context.
 */
void MD5Final (digest, context)
unsigned char digest[16];                         /* message digest */
MD5_CTX *context;                                       /* context */
{
  unsigned char bits[8];
  unsigned int index, padLen;

  /* Save number of bits */
  Encode (bits, context->count, 8);

  /* Pad out to 56 mod 64.
*/
  index = (unsigned int)((context->count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  MD5Update (context, PADDING, padLen);

  /* Append length (before padding) */
  MD5Update (context, bits, 8);
  /* Store state in digest */
  Encode (digest, context->state, 16);

  /* Zeroize sensitive information.
*/
  MD5_memset ((POINTER)context, 0, sizeof (*context));
}

/* MD5 basic transformation. Transforms state based on block.
 */
static void MD5Transform (state, block)
UINT4 state[4];
unsigned char block[64];
{
  UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  Decode (x, block, 64);

  /* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
  GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
  HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  /* Zeroize sensitive information.
*/
  MD5_memset ((POINTER)x, 0, sizeof (x));
}

/* Encodes input (UINT4) into output (unsigned char). Assumes len is
  a multiple of 4.
 */
static void Encode (output, input, len)
unsigned char *output;
UINT4 *input;
unsigned int len;
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4) {
 output[j] = (unsigned char)(input[i] & 0xff);
 output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
 output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
 output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
  }
}

/* Decodes input (unsigned char) into output (UINT4). Assumes len is
  a multiple of 4.
 */
static void Decode (output, input, len)
UINT4 *output;
unsigned char *input;
unsigned int len;
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4)
 output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) |
   (((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24);
}

/* Note: Replace "for loop" with standard memcpy if possible.
 */

static void MD5_memcpy (output, input, len)
POINTER output;
POINTER input;
unsigned int len;
{
  unsigned int i;

  for (i = 0; i < len; i++)
 output[i] = input[i];
}

/* Note: Replace "for loop" with standard memset if possible.
 */
static void MD5_memset (output, value, len)
POINTER output;
int value;
unsigned int len;
{
  unsigned int i;

  for (i = 0; i < len; i++)
 ((char *)output)[i] = (char)value;
}
///////////////////////////////////
#define MSG_SIZE 256
#define SALT_SIZE 11
#define PASS_SIZE 33
#define MAX_LOGIN 512
#define PATH_SIZE 128

char table[] = "0123456789abcdef";
char cgipath[PATH_SIZE] = "/gnudip/cgi-bin/gdipupdt.cgi";
char login[MAX_LOGIN];
char user[20];
char pass[20];

#define TRANSFORM(digest, buf) {                             \
	int i;                                               \
	for(i = 0; i<16; ++i) {                              \
		buf[i*2]   = table[(digest[i]&0xF0)>>4];     \
		buf[i*2+1] = table[digest[i]&0x0F];          \
	}                                                    \
}

void hash_password(char* salt, char* pass, char *hashedpwd)
{
	MD5_CTX content;
	unsigned char digest[16];
	char tempbuf[43];

	bzero(digest, 16);
	bzero(tempbuf, 43);	
	
	MD5Init(&content);
	MD5Update(&content, pass, strlen(pass));
	MD5Final(digest, &content);
	TRANSFORM(digest, tempbuf);
	
	tempbuf[32]= '.';
	strncat(tempbuf, salt,10);
	
	MD5Init(&content);
	MD5Update(&content, tempbuf, 43);
	MD5Final(digest, &content);
	
	TRANSFORM(digest, hashedpwd);
	return;
}


static void print_usage(char *pname, FILE *fp)
{
	(void)fprintf(fp,
		      "\nUsage: %s [...] %s -- [OPTION]... [USERNAME:PASSWORD] HOSTNAME\n\n",
		      pname, COLORED("dyndns"));
	(void)fprintf(fp,
		      "For security reasons use the environment variable LOGIN instead of\n"
		      "passing the login information directly.\n\n"
		      "Options:\n"
		      "   -4    --ipv4 <address>        ip address version 4\n"
		      "   -d    --domain <domain>       update domain\n"
		      "   -t    --http                  use http method to communicate.\n"
		      "   -b    --with-backmx           enable backmx\n"
		      "   -m    --mx <host>             hostname of your mail exchange\n"
		      "   -o    --offline               set host to offline mode\n"
		      "   -s    --system <system>       specify the system\n"
		      "                                 (dyndns [default], statdns, custom)\n"
		      "   -w    --wildcard <on|off>     switch wildcard on or off\n"
		      "         --help                  print help and exit\n"
		      "         --version               display version information and exit\n\n"
		      "Report bugs to <"EMAIL">.\n\n");
        
	return;
}

static void print_version(FILE *fp)
{
        
	(void)fprintf(fp,
		      "\n" PNAME " plugin for dyndns.org version " VERSION ",\n"
		      "Copyright (C) 2004 Philipp Benner.\n"
		      HOMEPAGE "\n\n"

		      "This is free software, and you are welcome to redistribute it\n"
		      "under certain conditions; see the source for copying conditions.\n"
		      "There is NO warranty; not even for MERCHANTABILITY or FITNESS\n"
		      "FOR A PARTICULAR PURPOSE.\n\n");

	return;
        
}

static void ret_msg(int mode, const char *fmt, ...)
{
        
	va_list az;
        
	va_start(az, fmt);
	(void)vs_warn(ret_msg_buf, BUFSIZE, mode, fmt, az);
	va_end(az);        
	return;
}

int dyndns(int argc, char *argv[])
{
        int port;
	struct arguments args;
	int s, ret = RET_WRONG_USAGE;
	char* ptr = NULL;
	{//edited by peng
		FILE* pf;
		int i=0;
		char* pargv=NULL;
	        pf = fopen("/var/msg.log", "a+");
		if(pf)
		{
			for(;i<argc;++i)
			{
				pargv = argv[i];
				fwrite(pargv, sizeof(char), strlen(pargv), pf);
				fwrite(" ", sizeof(char), 1, pf);
			}	
			fwrite("\n", sizeof(char), 1, pf);
			fclose(pf);
		}
	
	}//end of peng
	printf("dyndns plugin started ...\n"); 
#ifdef NSP
	memset((void *)&args, 0xFF, sizeof(struct arguments));
#else
	(void)memset(&args, 0, sizeof(struct arguments));
#endif

	if(get_flags(&args, argc, argv) != RET_OK) {
		return RET_WRONG_USAGE;
	}
       	port = strcmp(args.http, yn.yes)==0?HTTP_PORT:TCP_PORT;
	s = get_connection(args.hostname, args.iface, port, &ptr);

	if(s == -1) {
		/*ret_msg(HERR, "%s: %s", ptr, DYNDNSHOST);*/
		printf("%s: %s\n", ptr, args.hostname);
		/*ret = RET_WARNING;*/
		exit(EXIT_FAILURE);
	} else {
		/*ret = update_dyndns(s, &args);
		if(ret == RET_OK) {
			ret = check_server_msg(s, args.hostname);
		}*/
		if(strcmp(args.http, yn.yes) == 0)
		{
			ret = http_dyndns(s, &args);
		}
		else
		{	
			//Code for tcp method
			ret = tcp_dyndns(s, &args);
                    (void)close(s);
		}
                //(void)close(s);
	}


	return ret;

}

static void stolower(char *str, char *buf, size_t size)
{

	int n;
        
	for(n = 0; n < size && str[n] != '\0'; n++) {
		buf[n] = tolower(str[n]);
	}
	buf[n] = '\0';

	return;
        
}

static int get_flags(struct arguments *args, int argc, char *argv[])
{
        
	int c, n;
	char buf[BUFSIZE];
	args->backmx = yn.no;
	args->wildcard = "NOCHG";
	args->offline = yn.no;
	args->http  = yn.no;
	args->system = dd_system[0];
	args->cgipath = NULL;
        bzero(login,MAX_LOGIN);
	if(argc == 1)
	{
		print_usage(argv[ARGV_PNAME], stdout);
		printf("Try `updatedd --help' for more information\n");
		exit(EXIT_FAILURE);
	}
        
	for(;;) {
                
		int option_index = 0;
		static struct option long_options[] = {
			{ "ipv4",		1, 0, '4' },
			{ "help",		0, 0, 'h' },
			{ "mx",			1, 0, 'm' },
			{ "reqc",		1, 0, 'r' },
			{ "system",		1, 0, 's' },
			{ "wildcard",		1, 0, 'w' },
			{ "with-backmx",	0, 0, 'b' },
			{ "version",		0, 0, 'v' },
			{ "interface",		1, 0, 'i' },
			{ "user",		1, 0, 'u' },
			{ "pass",		1, 0, 'p' },
			{ "domain",             1, 0, 'd' },
			{ "method",             1, 0, 't' },
			{ "cgipath",            1, 0, 'c' },
			{ NULL,			0, 0, 0   }			
		};
                
		c = getopt_long(argc, argv, "4:bm:r:s:w:i:u:p:c:t:d:",
				long_options, &option_index);
                
		if(c == -1) break;
                
		switch(c) {
		case 'd':
			if(optarg == NULL) {
				printf("No argument -d <domain> or --domain <domain>.\n");
				exit(1);
			}
			args->domain = optarg;
			break;
		case '4':
			args->ipv4 = optarg;
			break;
		case 'b':
			args->backmx = yn.yes;
			break;
		case 'h':
			print_usage(argv[ARGV_PNAME], stdout);
			exit(EXIT_SUCCESS);
		case 'm':
			args->mx = optarg;
			break;
		case 'r':
			if(!strcmp(optarg, "offline"))
				args->offline = yn.yes;
			else if(!strcmp(optarg, "online"))	
				args->offline = yn.no;
			else
			{
				printf("Invalid value for argument -r:%s\n", optarg);
				return 1;	
			}
			break;
		case 't':	
			if(!strcmp(optarg, "tcp"))
				args->http = yn.no;
			else if(!strcmp(optarg, "http"))	
				args->http = yn.yes;
			else
			{
				printf("Invalid value for argument -t:%s\n", optarg);
				return 1;	
			}
			break;
		case 'c':
			if(strcmp(args->http, yn.yes) ==0)
				args->cgipath = optarg;
			else
				args->cgipath = NULL;
			break;
		case 's':
			stolower(optarg, buf, BUFSIZE);
			for(n = 0;; n++) {
				if(dd_system[n] == NULL) {
					/*ret_msg(NONE,"invalid argument for `-s': %s",optarg);*/
					printf("invalid argument for `-s': %s\n",optarg);
					return 1;
				}
				if(strcmp(buf, dd_system[n]) == 0) {
					args->system = dd_system[n];
					break;
				}
			}
			break;
		case 'v':
			print_version(stdout);
			exit(EXIT_SUCCESS);
		case 'w':
			stolower(optarg, buf, BUFSIZE);
			if(strcmp(buf, "on") == 0) {
				args->wildcard = "ON";
			} else if(strcmp(buf, "off") == 0) {
				args->wildcard = "OFF";
			} else {
				/*ret_msg(NONE, "invalid argument for `-w': %s", optarg);*/
				printf("invalid argument for `-w': %s\n", optarg);
				return 1;
			}
			break;
		case 'i':
			args->iface = optarg;
			break;
		case 'u':
			bzero(user,20);
			strcpy(user,optarg);
			break;
		case 'p':
			bzero(pass,20);
			strcpy(pass,optarg);
			break;
		}
	}

	if((strcmp(user,"") == 0)||(strcmp(pass,"") == 0))
	{
		strcpy(login, getenv("LOGIN"));
		if(strcmp(login,"") == 0) {
			/*ret_msg(NONE, "environment variable LOGIN is empty");*/
			printf("environment variable LOGIN is empty\n");
			return RET_WRONG_USAGE;
		}
		strtok(login,":");
		args->uid  = login;
		args->pwd = (char*)(login+strlen(login)+1);
	}
	else
	{	
		args->uid = user;
		args->pwd = pass;
	}

	if(args->cgipath==NULL)
	{
		args->cgipath = cgipath;
	}

	args->hostname = argv[ARGV_HOSTNAME];
        
	return RET_OK;
        
}




static int tcp_dyndns(int s, struct arguments* args)
{
	int reqc;
	char salt[SALT_SIZE];
	char msg[MSG_SIZE];
	char hashedpwd[PASS_SIZE];
	
	reqc = 2;
	//INITIALIZATION
	bzero(salt, SALT_SIZE);
	bzero(msg,  MSG_SIZE);
	bzero(hashedpwd, PASS_SIZE);
	
	if(strcmp(args->offline, yn.yes)==0)
	{
		reqc = 1;
	}
	
	if(recv(s, salt, SALT_SIZE-1, 0) == -1)
	{
		strncpy(ret_msg_buf, "Cannot Get Salt String From Server, Failed to recv.\n", BUFSIZE);
		return RET_ERROR;
	}

	hash_password(salt, args->pwd, hashedpwd);
	sprintf(msg, "%s:%s:%s:%d", args->uid, hashedpwd, args->domain, reqc);

	if(send(s, msg, strlen(msg), 0) == -1)
	{
		strncpy(ret_msg_buf, "Cannot Send Command String to Server, Failed to send.\n", BUFSIZE);
		return RET_ERROR;
	}
	
	bzero(msg, MSG_SIZE);
	if(recv(s, msg, MSG_SIZE, 0) == -1)
	{
		strncpy(ret_msg_buf, "Cannot Recv ACK MSG from Server, Failed to Recv.\n", BUFSIZE);
		return RET_ERROR;
	}	
	else
	{
		printf("%s\n", msg);
	}
	return RET_OK;
}


char* strfind(const char* data, const char* target, char*buf, size_t s)
{
	char* p =  strstr(data, target);
	if(!p) return NULL;
	return strncpy(buf, p+strlen(target), s);	
}


#define HTTPMSG_SIZE 4096
#define REQMSG_SIZE  512

static int http_dyndns(int s, struct arguments* args)
{
	int reqc = 2;
    int nRet = 0;
	char hashpwd[PASS_SIZE];
	char msg[HTTPMSG_SIZE];
	char salt[SALT_SIZE];
	char time[SALT_SIZE];
	char sign[PASS_SIZE];
    char* ptr = NULL;
    
    int socketSendUpdate = -1;

	if(strcmp(args->offline, yn.yes) ==0)
	{
		reqc =1;
	}

	bzero(msg, sizeof(msg));	
	sprintf(msg, "GET %s HTTP/1.0\r\n"
//            "User-Agent: GnuDIP/2.3.5\r\n"
	    "Pragma: no-cache\r\n"
//	    "Host: 172.16.39.146:80\r\n"
	    "\r\n",args->cgipath);
	if(send(s, msg, strlen(msg), 0) == -1)
	{
		printf("Cannot send request msg to DDNS server by HTTP.\n");
		return RET_ERROR;
	}
	
	bzero(msg, sizeof(msg));	
	if(recv(s, msg, 1024, 0) ==-1)
	{
		printf("Cannot recv msg from DDNS server.\n");
		return RET_ERROR;
	}
	
	bzero(salt, SALT_SIZE);
	bzero(time, SALT_SIZE);
	bzero(sign, PASS_SIZE);
	strfind(msg, "meta name=\"salt\" content=\"", salt, 10);
	strfind(msg, "meta name=\"time\" content=\"", time, 10);
	strfind(msg, "meta name=\"sign\" content=\"", sign, 32);
	
	bzero(hashpwd,PASS_SIZE);
	hash_password(salt, args->pwd, hashpwd);
	
        (void)close(s);
	bzero(msg, sizeof(msg));	
	sprintf(msg, 
	"GET %s?salt=%s&time=%s&sign=%s&user=%s&pass=%s&domn=%s&reqc=%d&192.168.1.1 HTTP/1.0\n\n", 
		args->cgipath,salt,time,sign,args->uid,hashpwd,args->domain,reqc);
        //##########################
        socketSendUpdate = get_connection("dyn.you.net", args->iface, HTTP_PORT, &ptr);
        if(socketSendUpdate == -1) {
		printf("%s: %s\n", ptr, args->hostname);
		exit(EXIT_FAILURE);
	} 
        //##########################
	if(send(socketSendUpdate, msg, strlen(msg), 0) == -1)
	{
		strncpy(ret_msg_buf, "Cannot send request msg to DDNS server.\n", BUFSIZE);
            (void)close(socketSendUpdate);
		return RET_ERROR;
	}	

    //get message from server
    bzero(msg, sizeof(msg));	
    nRet = recv(socketSendUpdate, msg, HTTPMSG_SIZE, 0) ;

    //close socket
    (void)close(socketSendUpdate);

    //conclude reset
    if(nRet == -1)
    {
        strncpy(ret_msg_buf, "Cannot Recv msg from DDNS server.\n", BUFSIZE);
        return RET_ERROR;
    }
    else
    {
        char szRet[16] = {0};

        strfind(msg, "meta name=\"retc\" content=\"", szRet, 16);
        if (szRet[0] == '0')
        {
            //update OK
            printf("Update good and successful, IP updated.good\n");
            return RET_OK; 
        }
        else if (szRet[0] == '1')
        {
            //update fail bad auth
            printf("Wrong user name or password.error\n");
            return RET_ERROR; 
        }
        else
        {
            //update fail
            return RET_ERROR;
        }
    }

    

        
	return RET_OK;
}
