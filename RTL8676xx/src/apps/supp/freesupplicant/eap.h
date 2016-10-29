#ifndef _EAP_H
#define _EAP_H

#define EAP_TYPE_IDENTITY 1
#define EAP_TYPE_NOTIFICATION 2
#define EAP_TYPE_NAK 3
#define EAP_TYPE_MD5_CHALLENGE 4
#define EAP_TYPE_OTP 5
#define EAP_TYPE_GTC 6
#define EAP_TYPE_EXPANDED 254
struct md5_values {
  char length;
  char randval[16];
};
#endif

