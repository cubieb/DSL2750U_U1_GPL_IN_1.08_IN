/* ----------------------------------------------------------------------------
 * Copyright (C), 1988-2009, Shenzheng T&W Tech. Co., Ltd. 
 * File Name: s3g_dev_def.h
 * Author: Zhao Yanming (zhaoyanming@twsz.com)     
 * Version: V1.0     
 * Date: 08/13/2009
 * --------------------------------------------------------------------------*/

#ifndef _S3G_DEV_DEF_H_
#define _S3G_DEV_DEF_H_

/* 
 * CDMA2K card definition 
 */
{ "DLINK",      "DWM-162-U5",      "/dev/ttyUSB0",      "/dev/ttyUSB2",  0x1e0e,  0xce16, 
    S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
/*{"ThinkWill",   "1",               "/dev/ttyUSB2",      "/dev/ttyUSB4",  0x05c6,  0x6000,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FEATURE_DUAL_CH },*/
{"ThinkWill",   "2",               "/dev/ttyUSB2",      "/dev/ttyUSB4",  0x19f5,  0x9901,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"TW",          "CR100",           "/dev/ttyUSB0",      "/dev/ttyUSB2",  0x05c6,  0x6000,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ZTE",         "1",               "/dev/ttyUSB0",      "/dev/ttyUSB0",   0x19d2, 0xffff,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ZTE",         "AC8710-AC2726-AC2736", "/dev/ttyUSB0", "/dev/ttyUSB1",   0x19d2, 0xfff1,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ZTE",         "AC560-AC580",     "/dev/ttyUSB0",      "/dev/ttyUSB1",   0x19d2, 0x0073,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ZTE",         "MC2716",          "/dev/ttyUSB0",      "/dev/ttyUSB1",   0x19d2, 0xffed,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ZTE", 		"AC581",		   "/dev/ttyUSB0",		"/dev/ttyUSB2",   0x19d2, 0x0094,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"WeWin",       "1",               "/dev/ttyUSB0",      "/dev/ttyUSB1",   0x1ed1, 0x0090,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"DLINK",      "DWM-162",          "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x07b8, 0x0040,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },	
{"TLINK",      "1",                "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x1e0e, 0xce18,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
/*{"TLINK",      "2",                "/dev/ttyUSB0",      "/dev/ttyUSB1",   0x05c6, 0x6000,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },*/
{"Himinway",   "Z6",               "/dev/ttyUSB0",      "/dev/ttyUSB0",   0x19d2, 0xfffe,
	S3G_PROTO_CDMA2K, S3G_FEATURE_SING_CH, S3G_FLAG_UNDEF },
{"HuaWei",     "EC1260-EC169",     "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x12d1, 0x1001,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ETCOM",      "1",                "/dev/ttyUSB1",      "/dev/ttyUSB3",   0x1bbb, 0x000f,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ETON",       "EU830",            "/dev/ttyUSB0",      "/dev/ttyUSB0",   0x0eab, 0x511b,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"surfing",    "2",                "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x028a, 0x1009,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"BellMAN",    "1",                "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x05c6, 0x9001,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"BellMAN",    "2",                "/dev/ttyUSB2",      "/dev/ttyUSB1",   0x2004, 0x6151,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"pctx",       "1",                "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x201e, 0x3000,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"xinke",      "1",                "/dev/ttyUSB2",      "/dev/ttyUSB4",   0x19f5, 0x9904,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"SIMCOM",     "2",                "/dev/ttyUSB2",      "/dev/ttyUSB4",   0x19f5, 0x9909,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"gaokeSurfing", "2",              "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x16d5, 0x6502,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"twc1m100",  "2",                 "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x2077, 0x7000,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"twc1m1001", "2",                 "/dev/ttyUSB2",      "/dev/ttyUSB1",   0x1e0e, 0xce1e,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"Dlink",     "3",                 "/dev/ttyUSB2",      "/dev/ttyUSB1",   0x1e0e, 0xce17,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"VTION",    "E1916",			   "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x1e89, 0x1e16,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"SHBER",	 "E750",			   "/dev/ttyUSB0",		"/dev/ttyUSB0",   0x0eab, 0xc893,
	S3G_PROTO_CDMA2K, S3G_FEATURE_SING_CH, S3G_FLAG_UNDEF },
{"SHBER",	 "E751+",			   "/dev/ttyUSB0",		"/dev/ttyUSB2",   0x1d09, 0xaef4,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"SHBER",	 "E751",			   "/dev/ttyUSB0",		"/dev/ttyUSB2",   0x1d09, 0x4303,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"HAIXIN",	 "E6801",			   "/dev/ttyUSB0",		"/dev/ttyUSB0",   0x109b , 0x9078,
	S3G_PROTO_CDMA2K, S3G_FEATURE_SING_CH, S3G_FLAG_UNDEF },
/* 
 * WCDMA card definition 
 */
{"SIMCOM",     "SIM5250",          "/dev/ttyUSB0",      "/dev/ttyUSB1",   0x05c6, 0x6000,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"SIMCOM",     "SIM5250_1",        "/dev/ttyUSB1",      "/dev/ttyUSB1",   0x1e0e, 0x7001,
	S3G_PROTO_WCDMA, S3G_FEATURE_SING_CH, S3G_FLAG_UNDEF },
{"GLOBAL",     "E169",             "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x05c6, 0x0015,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"TECHFAITH",  "1",                "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x1d09, 0x1011,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"T&W",        "W1U100",           "/dev/ttyUSB2",      "/dev/ttyUSB1",   0x2077, 0x9002,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"longsung",   "1",                "/dev/ttyUSB2",      "/dev/ttyUSB1",   0x1c9e, 0x9603,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"HuaWei",     "E1750",     	   "/dev/ttyUSB0",      "/dev/ttyUSB2",   0x12d1, 0x1001,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"HUAWEI", 	  "EC2010", 		       "/dev/ttyUSB0",	"/dev/ttyUSB3",   0x12d1, 0x140c,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ZTE",	      "MF626", 		       "/dev/ttyUSB2",		"/dev/ttyUSB1",   0x19d2, 0x0031,
	S3G_PROTO_CDMA2K, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },	
{"ZTE",	      "MF637U", 		   "/dev/ttyUSB2",		"/dev/ttyUSB1",   0x19d2, 0x0031,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"HUAWEI", 	  "E220", 		       "/dev/ttyUSB0",		"/dev/ttyUSB1",   0x12d1, 0x1003,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"HUAWEI", 	  "E2010", 		       "/dev/ttyUSB0",		"/dev/ttyUSB3",   0x12d1, 0x140C,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"NOVATEL",	  "MC950D",			   "/dev/ttyUSB0",		"/dev/ttyUSB0",   0x1410, 0x4400,
	S3G_PROTO_WCDMA, S3G_FEATURE_SING_CH, S3G_FLAG_UNDEF },
{"SHBER",     "H951", 		       "/dev/ttyUSB1",		"/dev/ttyUSB2",   0x1c9e, 0x9603,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"SHBER",	  "H953",			   "/dev/ttyUSB3",		"/dev/ttyUSB1",   0x1d09, 0x1026,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
/*  add by lbw at 20100508 */
{"D-LINK",	  "DWM-152",			"/dev/ttyUSB0",		"/dev/ttyUSB2",   0x07d1, 0x3E01,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"D-LINK",	  "DWM-152-v3",			"/dev/ttyUSB2",		"/dev/ttyUSB1",   0x19d2, 0x0039,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },	
{"D-LINK",	  "DWM-156-v3",			"/dev/ttyUSB1",		"/dev/ttyUSB1",   0x19d2, 0x0039,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ZTE",	  "MF668",			"/dev/ttyUSB2",		"/dev/ttyUSB1",   0x19d2, 0x0016,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"Nokia",	  "CS-18",			"/dev/ttyUSB2",		"/dev/ttyUSB0",   0x421, 0x627,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
	//°¢¸ùÍ¢3G¿¨
{"HUAWEI",	  "E1756",			"/dev/ttyUSB0",		"/dev/ttyUSB1",   0x12d1, 0x1406,
	S3G_PROTO_WCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },

/*  end by lbw at 20100508 */
/* 
 * TDSCDMA card definition 
 */
 {"T&W",       "T1U100",             "/dev/ttyUSB0",       "/dev/ttyUSB1",  0x2077, 0x6000,
	S3G_PROTO_TDSCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"lenovo",     "368-1",            "/dev/ttyACM0",      "/dev/ttyACM1",   0x04cc, 0x226f,
	S3G_PROTO_TDSCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"lenovo&T3G", "368-2",            "/dev/ttyACM0",      "/dev/ttyACM1",   0x04cc, 0x226e,
	S3G_PROTO_TDSCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"huawei",     "ET128",            "/dev/ttyACM0",      "/dev/ttyACM1",   0x12d1, 0x1d09,
	S3G_PROTO_TDSCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"POSTCOM",    "H301",             "/dev/ttyACM0",      "/dev/ttyUSB1",   0x1234, 0x6119,
	S3G_PROTO_TDSCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },
{"ZTE",       "MU350",             "/dev/ttyUSB2",       "/dev/ttyUSB0",  0x19d2, 0x0003,
	S3G_PROTO_TDSCDMA, S3G_FEATURE_DUAL_CH, S3G_FLAG_UNDEF },

#endif /* _S3G_DEV_DEF_H_ */

