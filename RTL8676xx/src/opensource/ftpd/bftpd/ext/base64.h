#ifndef	__FTPD_BASE64_H__
#define	__FTPD_BASE64_H__

char * base64_encode(const unsigned char *value, int vlen);
unsigned char * base64_decode(const char *value, int *rlen);
#endif
