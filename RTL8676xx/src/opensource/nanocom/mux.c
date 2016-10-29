/***************************************************************************
** File: mux.c
** Description: the main program loop
**
** Based upon code from microcom by Anca and Lucian Jurubita (C) Copyright 1999
** All rights reserved.
****************************************************************************
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details at www.gnu.org
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*****************************************************************************/

#include "nanocom.h"
#define BUFSIZE 1024

extern char scr_name[];


#if 0
/* main program loop */
void mux_loop(int pf) {
  fd_set  ready;        /* used for select */
  int i = 0;        /* used in the multiplex loop */
  int done = 0;
  char buf[BUFSIZE];
  struct timeval tv;
  tv.tv_usec = 0;

    do { /* loop forever */
        FD_ZERO(&ready);
        FD_SET(STDIN_FILENO, &ready);
        FD_SET(pf, &ready);
    
        select(pf+1, &ready, NULL, NULL, NULL);
    
        if (FD_ISSET(pf, &ready)) {
            /* pf has characters for us */
            i = read(pf, buf, BUFSIZE);
            
            if (i > 0) {
                write(STDOUT_FILENO, buf, i);
            }
            else {
                done = 1;
            }
        } /* if */
    
        if (FD_ISSET(STDIN_FILENO, &ready)) {
                /* standard input has characters for us */
                i = read(STDIN_FILENO, buf, BUFSIZE);
            if (i > 0) {
                cook_buf(pf, buf, i);
            }
            else {
                done = 1;
            }
        } /* if */
    }  while (!done); /* do */
}

#else


#define NANOCOM_AT_CMD "/var/pppd3g/atcmd"
#define NANOCOM_AT_RESULT	"/var/pppd3g/atresult"


/* main program loop */
void mux_loop(int pf, int fd, int num) {
  fd_set  ready;        /* used for select */
  int i = 0;        /* used in the multiplex loop */
  int done = 0;
  char buf[BUFSIZE];
  char temp[BUFSIZE];
  char filename[32];
  struct timeval tv;
  
  tv.tv_usec = 0;
  tv.tv_sec = 3;
  
  int script=0;
  int script_read=0;


	memset(buf,0,BUFSIZE);
	memset(temp,0,BUFSIZE);
	memset(filename,0,32);
	if(fd == 0)
    {
        script = open(NANOCOM_AT_CMD,O_RDONLY);
        if (script < 0) 
        {
            fprintf(stderr,"open file %s fail\n",NANOCOM_AT_CMD);
            usage("Cannot open file  /var/pppd3g/atcmd");
            return ;
        }
    }
	else
	    script = fd;
	i = read(script, buf, BUFSIZE);	
	close(script);
	
	sprintf(filename,"%s_%d",NANOCOM_AT_RESULT,num);
	if ( 0==access(filename,F_OK) )
	{		
		      unlink(filename);
	}
	script=0;
	script=open(filename,O_CREAT|O_APPEND|O_RDWR);
	script_read=open(filename,O_RDONLY);
	if(script_read<0 ||script<0)
	{		
		goto NANOCOMFAIL;
	}
	
       if(i>0)
       {       	
		//cook_buf(pf,"at+csq\n",strlen("at+csq\n"));
		//excute at cmd ,add by tj
		 write (pf, buf, i); 

		do 
		{ /* loop forever */
		        FD_ZERO(&ready);
		        //FD_SET(STDIN_FILENO, &ready);
		        FD_SET(pf, &ready);				
		    
		        select(pf+1, &ready, NULL, NULL, &tv);				
		    
		        if (FD_ISSET(pf, &ready)) 
			{
		            /* pf has characters for us */
		            i = read(pf, buf, BUFSIZE);		            
			       if (i > 0) 
				{			
					 //write(STDOUT_FILENO, buf, i);
					 //write at cmd's result to file,another moudule need to parse it,add by tj
					  write(script,buf,i);					 
					  while((i=read(script_read,temp,BUFSIZE))>0)				
					 {					
					 	//if got at cmd's result, resturn,add by tj
						  if(strstr(temp,"OK"))
						  {						  	
						  	close(script);
							close(script_read);
							return;	
						  }
					}					
			        }
			       else
				{					
			                done = 1;
			        }
		        } /* if (FD_ISSET(pf, &ready))  */
			else
			{		
			// if select timeout,and cannot read from pf,add by tj
				break;
			}
		}  while (!done); /* do */
	 }/*end if(i>0)*/

NANOCOMFAIL:
        if(script )
            close(script);
        if(script_read )
            close(script_read);
}


#endif


