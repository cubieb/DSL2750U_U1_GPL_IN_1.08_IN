
/*=========================================================================
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : tcp.c
 文件描述 : TCP协议栈

 说明:本协议栈从TI移植而来,并对其做了精简,为TBS web升级提供TCP协议支持
 函数列表 :


 修订记录 :
          1 创建 : 轩光磊
            日期 : 2008-7-22
            描述 :
            
          2 修改 : 夏超仁
            日期 : 2011-08-16
            描述 :            
=========================================================================*/

#include <common.h>
#include <command.h>
#include <flash_layout_private.h>
#include <types.h>
#include <net.h>
#include <tcp.h>
#include <crc.h>

#ifdef CMD_TCP

tcp_Socket *tcp_allsocs;
tcp_Socket http_data;

static char boundary_data[255];
static unsigned long boundary_len;
static unsigned long http_content_length;
static unsigned long data_length;
static unsigned long content_data_len;
static unsigned long total_packet_len;
static unsigned long received_content_len;
static unsigned long received_start_len;
unsigned char image_ok;
unsigned long update_timer_start;
unsigned long socket_timer_start;
static unsigned char force_flag;
static char procedure;
static struct update_parameters uppav;

/* IP identification numbers */
static word tcp_id;

/* Timer definitions */
#define tcp_RETRANSMITTIME 1000     /* interval at which retransmitter is called */
#define tcp_LONGTIMEOUT 31000       /* timeout for opens */
#define tcp_TIMEOUT 10000           /* timeout during a connection */
#define DEFAULT_HTTP_CONTENT_LENGTH 0x1000 /* Default http content length */


#if defined(FTP)
unsigned ftp_pkt_counter;
#endif

void variable_init(void)
{
	char blank[255] = {'-', '-', '\0'};

	if(image_ok == 0)
		{
		data_length = 0;
		}
	memcpy(boundary_data, blank, 255);
	http_content_length = DEFAULT_HTTP_CONTENT_LENGTH;
	received_content_len = 0;
	force_flag = 0;
	boundary_len = 0;
	procedure = 0;
	content_data_len = 0;
	total_packet_len = 0;
	received_start_len = 0;
}

int atoi(const char *s)
{
    int idx = 0;
    int sign = 1, val = 0;

    if (s == 0)
        return 0;
    while (s[idx] != 0)
    {
        if ((s[idx] == ' ') || (s[idx] == '\t'))
            idx++;
        else
            break;
    }
    if (s[idx] == 0)
        return 0;
    else if (s[idx] == '-')
    {
        sign = -1;
        idx++;
    }
    while ((s[idx] >= '0') && (s[idx] <= '9'))
    {
        val = val * 10 + (s[idx] - '0');
        idx++;
    }
    return (sign*val);
}

void TcpTimeoutCheck(void)
{/* Check for a tcp socket timeout, and run the timeout handler if we have one. */
	unsigned long t;

	t = get_timer(0);
	if((image_ok == 1) && (t > (update_timer_start + CFG_HZ)))
		{
		image_ok = 0;
		if(flash_update((void *)load_addr, data_length, force_flag, &uppav) == ERROR_OK)
			{
			do_reset(NULL, 0, 0, NULL);
			}
		data_length = 0;
		variable_init();
		NetStartAgain();
		}
	if((NULL != tcp_allsocs) && (tcp_allsocs->state == TCP_STATE_ESTABLISHED))
		{
		if(t > (socket_timer_start + ((tcp_allsocs->timeout/ 1000) * CFG_HZ)))
			{
			printf("\nSocket timed out, Abort connection\n");
			tcp_Abort(tcp_allsocs);
			}
		}
}


static void check_4_web_reply(tcp_Socket *s, void *dp, unsigned char get_post, unsigned long len)
{

	static char html_head[]="HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n<html><head><title>Firmware Upgrade</title>";
	static char html_post_index[]="</head><body><p align= 'center'><b>Firmware Upgrade Failed!</b></p>";
	static char html_get_index[]="</head><body><p align= 'center'><b>Firmware Upgrade</b></p>";
	static char htmlerror_size[]="Illegal file size, please check the file and try again!";
	static char htmlerror_crc[]="Bad CRC checksum! please check the file and try again!";
	static char htmlerror_img_type[]="File type unrecognized, please check the file and try again!";
	static char htmlerror_product[]="Product type %s not match with target system %s! Please check the file and try again!";
	static char htmlerror_internal[]="Internal errors, Please reset target device manually!";
	static char html_tail[]="<br><form enctype=multipart/form-data method=post><br>\
	<b>Step 1:</b> Enter the full path or click &quot;Browse&quot; to locate the firmware image file.<br><br>\
	<b>Step 2:</b> For BIN file, Select(NOT recommended) &quot;Force&quot if bypassing product check is in need.<br><br>\
	<b>Step 3:</b> Click &quot;Update&quot; to upload the image file and start the upgrading process.<br><br>\
	<b>Step 4:</b> Wait for the upgrading process to complete. target device will reboot automatically.<br><br><br>\
	<b>WARNING:</b> Make sure the file is compatible with the hardware, or target device will be damaged!<br><br><tr>\
	<p align= 'center'><td>File:&nbsp;</td><td><input type='file' name='filename' size='15'></td></tr><tr>\
	<td>Force:&nbsp;</td><td><input type='checkbox' name='force_upgrade'></td></tr></p>\
	<p align='center'><input type=submit value='Update'></p></form></body></html>"; 
	static char htmlredirect_url[]="<meta http-equiv=Refresh content=240;url=http://%s>";
	static char htmlupdate_ok[]="</head><body><table border=1 width=800 cellspacing=0 cellpadding=4 bgcolor=#FFFFEC height=87>\
	<tr><td bgcolor=#33AAFF height=24 align=center><b>!!!DO NOT INTERRUPT THIS!!!</b></td></tr><tr><td>Firmware upgrade confirmed, \
	please wait for the upgrading process...<marquee style='border:1px solid #000000' direction=right width=800 scrollamount=1 \
	scrolldelay=300 bgcolor=#ECF2FF><table cellspacing=1 cellpadding=0><tr height=16><td bgcolor=#FF00FF width=65535></td></tr></table>\
	</marquee></td></tr></table></body></html>";
	char web_string[1024] = {'\0'};
	char temp[512] = {'\0'};
	char ips[20] = {'\0'};
	unsigned long ip;
	int ret_val;

	if(get_post == 0)
		{
		sprintf(web_string, "%s%s%s", html_head, html_get_index, html_tail);
		}

	else if(get_post == 1)
		{
		if(len >= 1)
			{
			if(NULL == dp)
				{
				printf("data pointer is null!\n");
				ret_val = ERROR_NO_MEM;
				}
			ret_val = file_check(dp, len, force_flag, &uppav);
			}
		else
			{
			ret_val = ERROR_IMG_SIZE;
			}
		image_ok = 0;
		switch(ret_val)
			{
			case ERROR_IMG_SIZE:
				sprintf(web_string, "%s%s%s%s", html_head, html_post_index, htmlerror_size, html_tail);
				break;
			case ERROR_OK:
				get_sys_ip(&ip);
				ip_to_string(ip, ips);
				sprintf(temp,htmlredirect_url,ips);
				sprintf(web_string, "%s%s%s", html_head, temp, htmlupdate_ok);
				image_ok = 1;
				update_timer_start = get_timer(0);
				break;
			case ERROR_CRC:
				sprintf(web_string, "%s%s%s%s", html_head, html_post_index, htmlerror_crc, html_tail);
				break;
			case ERROR_PRODUCT:
				sprintf(temp, htmlerror_product, uppav.tail.product, DIGIT_SIG);
				sprintf(web_string, "%s%s%s%s", html_head, html_post_index, temp, html_tail);
				break;
			case ERROR_IMG_TYPE:
				sprintf(web_string, "%s%s%s%s", html_head, html_post_index, htmlerror_img_type, html_tail);
				break;						
			default:
				sprintf(web_string, "%s%s%s%s", html_head, html_post_index, htmlerror_internal, html_tail);
				break;
			}	
		}
	tcp_Write(s, web_string, strlen(web_string));
}

/*
 * Data_Handler: assemble packets into data block.
 */

void data_handler(tcp_Socket *s, byte *dp, int len, int operation)
{
	int	i = 0;
	static char *packet_data = NULL;

	if(NULL == s)
		{
		printf("NUll Socket!\n");
        return;
		}
	else
		{
		packet_data = (char *)load_addr;
		}
	
	switch(operation)
		{
		case SDATA:
			if(NULL == dp)
				{
				printf("Invalid data pointer!\n");
				tcp_Abort(s);
				return;
				}
			if(strncmp(dp, "GET", 3) == 0)
				{
				check_4_web_reply(s, NULL, 0, 0);
	            }
			else if(received_content_len < 4)
				{
				if(strncmp(dp, "POST", 4) == 0)
		        	{
		        	received_content_len = 4;
					}
				else if(s->unhappy == FALSE)
					{
					tcp_Close(s);
					}
				}
			if(received_content_len >= 4)
				{
				if(received_content_len <= http_content_length)
					{
					for(i=0; i<len; i++)
						{
						if((http_content_length <= info->size) || (received_content_len <= 4))
							{
							packet_data[total_packet_len] = dp[i];
							}
						total_packet_len++;
						}
					}
				if(boundary_len <= 0)
					{
					for(i=0;i<total_packet_len;i++)
		            	{  /* 查找边界线字符串 */
		                if(strncmp(packet_data + i, "boundary=", 9) == 0)
		                	{
		                    i += 9;
		                    for(boundary_len=2;i<total_packet_len; i++)
		                    	{
		                        if((packet_data[i] == '\r') && (packet_data[i+1] == '\n'))
		                        	{
		                        	break;
		                        	}
								else
									{
			                        boundary_data[boundary_len] = packet_data[i];
		        	                boundary_len++;
									}
		                    	}
							etdebug("Boundary line: %s\n", boundary_data);
							break;
		                	}
		            	}
					}
				if(boundary_len > 0)
					{
					if(procedure == 0)
						{
						for(i=0; i<total_packet_len; i++)
							{
							if(strncmp(packet_data + i, "Content-Length:", 15) == 0)
								{  /* 查找发送数据总长度，包括换行回车符和两个边界线长度 */
								http_content_length = atoi(packet_data + i + 15);
								procedure++;
								printf("http_content_length=%d\n", http_content_length);
								break;
								}
							}
						}
					if(procedure == 1)
						{
						for(i=0; i<total_packet_len; i++)
							{
					 		if(strncmp(packet_data + i, boundary_data, boundary_len) == 0)
					 			{
						 		procedure++;
								received_content_len = 0;
								received_start_len = i;
								break;
						 		}
						 	}
						}
					if(procedure == 2)
						{
						received_content_len = total_packet_len - received_start_len;
						if(received_content_len >= http_content_length)
							{
							if(received_content_len > info->size)
								{
								data_length = 0;
								procedure = 5;
								}
							else
								{
								for(i=received_start_len; i < total_packet_len; i++)
									{
									if((packet_data[i] == '\r') && (packet_data[i+1] == '\n') && (packet_data[i+2] == '\r') && (packet_data[i+3] == '\n'))
										{  /* 找到数据开始点 */
										printf("Data start point found!\n");
										procedure++;
										received_start_len = i + 4;
										break;
										}
									else
										{
										received_start_len++;
										}
									}
								}
							}
						}
					if(procedure == 3)
						{
						for(i=received_start_len; i<total_packet_len; i++)
							{
							if(strncmp((char *)(packet_data + i), boundary_data, boundary_len) == 0)
								{ /* 查找到有效数据边界线字符串 */
								printf("Data end point found!\n");
								procedure++;
								data_length -= 2; //换行符"\r\n"									
								break;
								}
							else
								{
								data_length++;
								}
							}
						}
					if(procedure == 4)
						{
						for(i=data_length+received_start_len; i<total_packet_len; i++)
							{
							if(strncmp((char *)(packet_data + i), "force_upgrade", 13) == 0)
								{
								force_flag = 1;
								}
							if((strncmp((char *)(packet_data + i), boundary_data, boundary_len) == 0) && 
								(strncmp((char *)(packet_data + i + boundary_len), "--", 2) == 0))
								{
								received_content_len = http_content_length;
								if((data_length + received_start_len) != i)
									{
									procedure++;
									}
								break;
								}
							}
						}
					if(procedure >= 4)
						{
						printf("received_content_len =%d\n", received_content_len);
						debug("load_addr=0x%08x\n", (unsigned long)load_addr);
						printf("data_length=%d, force_flag=%d\n", data_length, force_flag);
						for(i=0; i<data_length; i++)
							{
							packet_data[i] = packet_data[i + received_start_len];
							}
						check_4_web_reply(s, (void *)load_addr, 1, data_length);
						variable_init();
						}
					}
				}
			break;

		case SCLOSE:
			variable_init();
			printf("Connection was closed!\n");
			break;
			
		default:
			variable_init();
			etdebug("Operation is %d\n",operation);
			break;
		}

}



/*
 * Passive open: listen for a connection on a particular port
 */
void tcp_Listen(tcp_Socket *s, word port, void (*datahandler)(struct _tcp_socket *, char *, int, int), int timeout)
{

	if ((s->state != TCP_STATE_LISTEN) && (s->state != TCP_STATE_CLOSED))
		{
		s->flags = tcp_FlagACK;
		tcp_Send(s);
		s->state = TCP_STATE_CLOSED;
		s->dataHandler(s, 0, 0, SABORT);
		tcp_Unthread(s);
		}

	s->ip_type = 6;
	s->state = TCP_STATE_LISTEN;
	if (timeout == 0)
		s->timeout = 0x7ffffff; /* forever... */
	else
		s->timeout = timeout;
	s->myport = port;
	s->hisport = 0;
	s->seqnum = 0;
	s->dataSize = 0;
	s->flags = 0;
	s->unhappy = 0;
	s->dataHandler = datahandler;
	s->next = tcp_allsocs;
	tcp_allsocs = s;
	printf("Listening on local port %d\n", port);
}

/*
 * Send a FIN on a particular port -- only works if it is open
 */
void tcp_Close(tcp_Socket *s)
{
	etdebug("Inside tcp_Close\n");
	switch(s->state)
		{
		case TCP_STATE_ESTABLISHED:
		case TCP_STATE_SYN_RCVD:
			s->flags = tcp_FlagACK | tcp_FlagFIN;
			s->state = TCP_STATE_FIN_WAIT_1;
			s->unhappy = TRUE;
			break;
		default:
			break;
		}
}

/*
 * Abort a tcp connection
 */
void tcp_Abort(tcp_Socket *s)
{
	if( s->state != TCP_STATE_LISTEN && s->state != TCP_STATE_CLOSED)
		{
		s->flags = tcp_FlagRST | tcp_FlagACK;
		tcp_Send(s);
		}
	s->unhappy = 0;
	s->dataSize = 0;
	s->state = TCP_STATE_CLOSED;
	s->dataHandler(s, 0, -1, SABORT);
	tcp_Unthread(s);
	tcp_Listen(&http_data, 80, data_handler, 0);
}

/*
 * Unthread a socket from the socket list, if it's there
 */
void tcp_Unthread(tcp_Socket *ds)
{
	tcp_Socket *s, **sp;

	sp = &tcp_allsocs;
	for (;;)
		{
		s = *sp;
		if(s == ds)
			{
			*sp = s->next;
			break;
			}
		if(s == NULL)
			break;
		sp = &s->next;
		}
}


/*
 * Write data to a connection.
 * Returns number of bytes written, == 0 when connection is not in
 * established state.
 */
int tcp_Write(tcp_Socket *s, byte *dp, int len)
{
	int x;

	etdebug("Inside tcp_Write, len: %d, s->dataSize: %d\n\n", len, s->dataSize);
	if(s->state != TCP_STATE_ESTABLISHED)
		{
		len = 0;
		}
	if(len > (x = (tcp_MaxData - s->dataSize)))
		{
		len = x;
		}
	etdebug("len: %d, x: %d, tcp_MaxData: %d, s->dataSize: %d, tcp_MaxData: %d\n", len, x, tcp_MaxData, s->dataSize, tcp_MaxData);
	if(len > 0)
		{
		memcpy(&s->data[s->dataSize], dp, len);
		s->dataSize += len;
		etdebug("len: %d, s->dataSize: %d\n", len, s->dataSize);
		/*tcp_Flush(s); */
		}

	return (len);
}

/*
 * Send pending data
 */
void tcp_Flush(tcp_Socket *s)
{
	etdebug("Inside tcp_Flush ...\n");
	if(s->dataSize > 0)
		{
		s->flags |= tcp_FlagPUSH;
		tcp_Send(s);
		}
}

/* Process the data in an incoming packet on states established, fin-wait-1, fin-wait-2 where data received. */
void tcp_ProcessData(tcp_Socket *s, tcp_Header *tp, int len)
{
	int diff;
	int x;
	word flags;
	byte *dp;

	/* Check if this packet was not received before */
	if(lfix4(tp->seqnum) >= s->acknum)
		{
		flags = wfix(tp->flags);
		diff = s->acknum - lfix4(tp->seqnum);
		if(flags & tcp_FlagSYN)
			{
			diff--;
			}
		x = tcp_GetDataOffset(tp) << 2;
		dp = (byte *)tp + x;
		len -= x;
		if(diff >= 0)
			{
			dp += diff;
			len -= diff;
			s->acknum += len;
			s->dataHandler(s, dp, len, SDATA);
			if(flags & tcp_FlagFIN) 
				{
                s->acknum++;
				}
			
			if(s->state !=	TCP_STATE_FIN_WAIT_2)
	            tcp_Send(s);
			else
				s->state = TCP_STATE_LAST_ACK;				
			}
		}
    else
    	{/* lost ACK for the recieved packet. re-send */
        tcp_Send(s);
    	}
	s->timeout = tcp_TIMEOUT;
}

/* Handler for incoming packets. */
void tcp_Handler(in_Header *ip)
{
	tcp_Header *tp;
	tcp_PseudoHeader ph;
	int len;
	int diff;
	tcp_Socket *s;
	word flags;

	if(NULL == tcp_allsocs)
		{
		return;
		}
	len = in_GetHdrlenBytes(ip);
	tp = (tcp_Header *)((byte *)ip + len);
	len = wfix(ip->length) - len;

	/* demux to active sockets */
	for(s=tcp_allsocs; s; s = s->next)
		{
		if(s->hisport != 0 && wfix(tp->dstPort) == s->myport && wfix(tp->srcPort) == s->hisport && lfix(ip->source) == s->hisaddr)
			break;
		}
	if(NULL == s)
		{/* demux to passive sockets */
		for(s=tcp_allsocs; s; s=s->next)
			{
			if(s->hisport == 0 && wfix(tp->dstPort) == s->myport)
				break;
			}
		}
	if(NULL == s)
		{
		etdebug("NULL Socket!\n");
		return;
		}

	/* save his ethernet address */
	memcpy(&s->hisethaddr[0], &((((eth_Header *)ip) - 1)->source[0]), sizeof(eth_HwAddress));
	ph.src = ip->source;
	ph.dst = ip->destination;
	ph.mbz = 0;
	ph.protocol = 6;
	ph.length = wfix(len);
	ph.checksum = wfix(tcp_checksum(tp, len));
	if(tcp_checksum(&ph, sizeof ph) != 0xffff)
		{
		printf("Checksum wrong!\n");
		return;
		}
	flags = wfix(tp->flags);
	if(flags & tcp_FlagRST)
		{
		s->state = TCP_STATE_CLOSED;
		s->dataHandler(s, 0, -1,SABORT);
		tcp_Unthread(s);
		printf("Connection was reset!\n");
		}
	if(flags & tcp_FlagSYN)
		{
		s->dataHandler(s, 0, -1,SABORT);
		s->state = TCP_STATE_LISTEN;
		etdebug("Got new connection request\n");
		}
	etdebug("Received SEQNUM=0x%08x, ACKNUM=0x%08x, STATE=%d\n", s->seqnum, s->acknum, s->state);
	switch(s->state)
		{
		case TCP_STATE_LISTEN:
			if(flags & tcp_FlagSYN)
				{
				s->acknum = lfix4(tp->seqnum) + 1;
				s->hisport = wfix(tp->srcPort);
				s->hisaddr = lfix(ip->source);
				s->flags = tcp_FlagSYN | tcp_FlagACK;
				tcp_Send(s);
				s->state = TCP_STATE_SYN_RCVD;
				s->unhappy = TRUE;
				s->timeout = tcp_TIMEOUT;
				}
			break;

		case TCP_STATE_SYN_SENT:
			if(flags & tcp_FlagACK)
				{
				if (lfix4(tp->acknum) != (s->seqnum + 1))
					{
					s->flags = tcp_FlagRST;
					tcp_Send(s);
					s->flags = tcp_FlagSYN;
					}
				}
			if(flags & tcp_FlagSYN)
				{
				s->acknum++;
				s->flags = tcp_FlagACK;
				s->timeout = tcp_TIMEOUT;
				if((flags & tcp_FlagACK) && lfix4(tp->acknum) == (s->seqnum + 1))
					{
					s->state = TCP_STATE_ESTABLISHED;
					s->seqnum++;
					s->acknum = lfix4(tp->seqnum) + 1;
					s->unhappy = FALSE;
					tcp_Send(s);    /*DRB Added*/
					s->dataHandler(s, 0, 0,SOPEN);
					}
				else
					{
					s->state = TCP_STATE_SYN_RCVD;
					}
				}
			break;

		case TCP_STATE_SYN_RCVD:
			if(flags & tcp_FlagSYN)
				{
				s->flags = tcp_FlagSYN | tcp_FlagACK;
				tcp_Send(s);
				s->timeout = tcp_TIMEOUT;
				}
			if((flags & tcp_FlagACK) && lfix4(tp->acknum) == (s->seqnum + 1))
				{
				s->flags = tcp_FlagACK;
				s->seqnum++;
				s->unhappy = FALSE;
				s->state = TCP_STATE_ESTABLISHED;
				s->timeout = tcp_TIMEOUT;
				s->dataHandler(s, 0, 0,SOPEN);
				}
			break;

		case TCP_STATE_ESTABLISHED:
			if((flags & tcp_FlagACK) == 0)
				{
				return;
				}
			/* process ack value in packet */
			diff = lfix4(tp->acknum) - s->seqnum;
			if(diff > 0)
				{
				memcpy(&s->data[0], &s->data[diff], diff);
				s->dataSize -= diff;
				s->seqnum += diff;
				}
			
			if(flags & tcp_FlagFIN)
				{
				s->unhappy = TRUE;
				s->state = TCP_STATE_CLOSE_WAIT;
				}
			s->flags = tcp_FlagACK;
			tcp_ProcessData(s, tp, len);
			if(flags & tcp_FlagFIN)
				{
				s->flags = tcp_FlagFIN | tcp_FlagACK;
				etdebug("Send last FIN to stage TIME_WAIT\n");
				tcp_Send(s);
				s->state = TCP_STATE_TIME_WAIT;
				return;
				}
			break;

		case TCP_STATE_FIN_WAIT_1:
			if((flags & tcp_FlagACK) == 0)
				{
				etdebug("No ACK in state TCP_STATE_FIN_WAIT_1\n");
				return;
				}
			diff = lfix4(tp->acknum) - s->seqnum - 1;
			s->flags = tcp_FlagACK | tcp_FlagFIN;
			if(diff == 0)
				{
				etdebug("ack and seq equals in TCP_STATE_FIN_WAIT_1\n");
				s->state = TCP_STATE_FIN_WAIT_2;
				s->flags = tcp_FlagACK;
				s->seqnum++;
				}
			tcp_ProcessData(s, tp, len);
    	    break;

		case TCP_STATE_CLOSE_WAIT:
			if(tp && (lfix(tp->acknum) == (s->seqnum + 1)))
				{
				etdebug("ack and seq equals in TCP_STATE_CLOSE_WAIT\n");
				s->state = TCP_STATE_TIME_WAIT;
				s->timeout = tcp_TIMEOUT;
				}
			break;
			
		case TCP_STATE_FIN_WAIT_2:		
			s->flags = tcp_FlagACK;
			tcp_ProcessData(s, tp, len);
			s->state = TCP_STATE_CLOSED;
			s->unhappy = FALSE;
			s->dataSize = 0;
			s->dataHandler(s, 0, 0,SCLOSE);
			tcp_Unthread(s);
			break;

		case TCP_STATE_LAST_ACK:
			if(lfix4(tp->acknum) == (s->seqnum + 1))
				{
				s->unhappy = FALSE;
				s->dataSize = 0;
				s->dataHandler(s, 0, 0,SCLOSE);
				tcp_Unthread(s);
				}
			else			
				{
				s->acknum++;
				s->flags = tcp_FlagACK;
				tcp_Send(s);
				s->state = TCP_STATE_CLOSED;
				s->unhappy = FALSE;
				s->dataSize = 0;
				s->dataHandler(s, 0, 0,SCLOSE);
				tcp_Unthread(s);
				}
			break;

		case TCP_STATE_TIME_WAIT:
			if(flags & tcp_FlagFIN)
				{
				etdebug("Socket has FIN in TCP_STATE_TIME_WAIT\n");
				s->flags = tcp_FlagACK;
				tcp_Send(s);
				}
			else
				{
				s->state = TCP_STATE_CLOSED;
				s->unhappy = FALSE;
				s->dataSize = 0;
				s->dataHandler(s, 0, 0,SCLOSE);
				tcp_Unthread(s);
				}
			break;

		default:
			break;
		}
	
	socket_timer_start = get_timer(0);
	if(s->state == TCP_STATE_CLOSED)
		tcp_Listen(&http_data, 80, data_handler, 0);
}

/* Format and send an outgoing segment */
void tcp_Send(tcp_Socket *s)
{
	tcp_PseudoHeader ph;
	struct _pkt {
		in_Header in;
		tcp_Header tcp;
		longword maxsegopt;
	} *pkt;
	byte *dp;

	etdebug("Sending Size=%d, SEQNUM=0x%08x, ACKNUM=0x%08x, STATE=%d\n", s->dataSize, s->seqnum, lfix(s->acknum), s->state);
	pkt = (struct _pkt *)sed_FormatPacket((unsigned char *)(&s->hisethaddr[0]), 0x800);
	dp = (byte *)&pkt->maxsegopt;
	pkt->in.length = wfix(sizeof(in_Header) + sizeof(tcp_Header) + s->dataSize);
	/* tcp header */
	pkt->tcp.srcPort = wfix(s->myport);
	pkt->tcp.dstPort = wfix(s->hisport);
	pkt->tcp.seqnum = lfix(s->seqnum);
	pkt->tcp.acknum = lfix(s->acknum);
	pkt->tcp.window = wfix(1024);
	pkt->tcp.flags = wfix(s->flags | 0x5000);
	pkt->tcp.checksum = 0;
	pkt->tcp.urgentPointer = 0;
	if(s->flags & tcp_FlagSYN)
		{
		pkt->tcp.flags = wfix(0x1000+wfix(pkt->tcp.flags));
		pkt->in.length = wfix(4+wfix(pkt->in.length));
		pkt->maxsegopt = lfix(0x02040578); /* 1400 bytes */
		dp += 4;
		}
	memcpy(dp, s->data, s->dataSize);
	/* internet header */
	pkt->in.vht = wfix(0x4500);   /* version 4, hdrlen 5, tos 0 */
	pkt->in.identification = wfix(tcp_id++);
	pkt->in.frag = 0;
	pkt->in.ttlProtocol = wfix((250<<8) + 6);
	pkt->in.checksum = 0;
	pkt->in.source = lfix(NetOurIP);
	pkt->in.destination = lfix(s->hisaddr);
	pkt->in.checksum = wfix(~(tcp_checksum(&pkt->in, sizeof(in_Header))));
	/* compute tcp checksum */
	ph.src = pkt->in.source;
	ph.dst = pkt->in.destination;
	ph.mbz = 0;
	ph.protocol = 6;
	ph.length = wfix(wfix(pkt->in.length) - sizeof(in_Header));
	ph.checksum = wfix(tcp_checksum(&pkt->tcp, wfix(ph.length)));
	pkt->tcp.checksum = wfix(~(tcp_checksum(&ph, sizeof ph)));
	sed_Send(wfix(pkt->in.length));
}

/* Do a one's complement checksum */
int tcp_checksum(void *vdp, int length)
{
	int len;
	longword sum;
	word *dp=vdp;

	len = length >> 1;
	sum = 0;
	while(len-- > 0)
		{
		sum += wfix(*dp);
		dp++;
		}
	if(length & 1)
		{
		sum += (wfix(*dp) & 0xFF00);
		}
	sum = (sum & 0xFFFF) + ((sum >> 16) & 0xFFFF);
	sum = (sum & 0xFFFF) + ((sum >> 16) & 0xFFFF);

	return ( sum );
}

/*
 * Initialize the tcp implementation
 */


void tbs_tcp(void)
{

	tcp_allsocs = NULL;
	tcp_id = 0;
	image_ok = 0;
	data_length = 0;
	variable_init();
	NetCopyIP(&NetOurIP, (unsigned long *)&(gd->bi_ip_addr));
	memcpy(NetOurEther, (unsigned char *)&(gd->bi_enetaddr[0]), 6);
	memcpy(sed_lclEthAddr, NetOurEther, 6);
	tcp_Listen(&http_data, 80, data_handler, 0);
}

#endif


