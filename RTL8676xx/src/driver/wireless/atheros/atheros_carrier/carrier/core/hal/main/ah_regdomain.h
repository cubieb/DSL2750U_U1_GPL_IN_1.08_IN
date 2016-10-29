/*
 * Copyright (c) 2002-2005 Atheros Communications, Inc.
 * All rights reserved.
 *
 * $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/hal/main/ah_regdomain.h#2 $
 */

#ifndef _ATH_AH_REGDOMAIN_H_
#define _ATH_AH_REGDOMAIN_H_

#define DEF_REGDMN		FCC3_FCCA

#define	YES	AH_TRUE
#define	NO	AH_FALSE

/*
 * Country/Region Codes from MS WINNLS.H
 * Numbering from ISO 3166
 */
enum CountryCode {
    CTRY_ALBANIA              = 8,       /* Albania */
    CTRY_ALGERIA              = 12,      /* Algeria */
    CTRY_ARGENTINA            = 32,      /* Argentina */
    CTRY_ARMENIA              = 51,      /* Armenia */
    CTRY_AUSTRALIA            = 36,      /* Australia */
    CTRY_AUSTRIA              = 40,      /* Austria */
    CTRY_AZERBAIJAN           = 31,      /* Azerbaijan */
    CTRY_BAHRAIN              = 48,      /* Bahrain */
    CTRY_BELARUS              = 112,     /* Belarus */
    CTRY_BELGIUM              = 56,      /* Belgium */
    CTRY_BELIZE               = 84,      /* Belize */
    CTRY_BOLIVIA              = 68,      /* Bolivia */
    CTRY_BOSNIA_HERZ          = 70,      /* Bosnia and Herzegowina */
    CTRY_BRAZIL               = 76,      /* Brazil */
    CTRY_BRUNEI_DARUSSALAM    = 96,      /* Brunei Darussalam */
    CTRY_BULGARIA             = 100,     /* Bulgaria */
    CTRY_CANADA               = 124,     /* Canada */
    CTRY_CHILE                = 152,     /* Chile */
    CTRY_CHINA                = 156,     /* People's Republic of China */
    CTRY_COLOMBIA             = 170,     /* Colombia */
    CTRY_COSTA_RICA           = 188,     /* Costa Rica */
    CTRY_CROATIA              = 191,     /* Croatia */
    CTRY_CYPRUS               = 196,     /* Cyprus */
    CTRY_CZECH                = 203,     /* Czech Republic */
    CTRY_DENMARK              = 208,     /* Denmark */
    CTRY_DOMINICAN_REPUBLIC   = 214,     /* Dominican Republic */
    CTRY_ECUADOR              = 218,     /* Ecuador */
    CTRY_EGYPT                = 818,     /* Egypt */
    CTRY_EL_SALVADOR          = 222,     /* El Salvador */
    CTRY_ESTONIA              = 233,     /* Estonia */
    CTRY_FAEROE_ISLANDS       = 234,     /* Faeroe Islands */
    CTRY_FINLAND              = 246,     /* Finland */
    CTRY_FRANCE               = 250,     /* France */
    CTRY_GEORGIA              = 268,     /* Georgia */
    CTRY_GERMANY              = 276,     /* Germany */
    CTRY_GREECE               = 300,     /* Greece */
    CTRY_GUATEMALA            = 320,     /* Guatemala */
    CTRY_HONDURAS             = 340,     /* Honduras */
    CTRY_HONG_KONG            = 344,     /* Hong Kong S.A.R., P.R.C. */
    CTRY_HUNGARY              = 348,     /* Hungary */
    CTRY_ICELAND              = 352,     /* Iceland */
    CTRY_INDIA                = 356,     /* India */
    CTRY_INDONESIA            = 360,     /* Indonesia */
    CTRY_IRAN                 = 364,     /* Iran */
    CTRY_IRAQ                 = 368,     /* Iraq */
    CTRY_IRELAND              = 372,     /* Ireland */
    CTRY_ISRAEL               = 376,     /* Israel */
    CTRY_ISRAEL2              = 377,     /* Israel2 */
    CTRY_ITALY                = 380,     /* Italy */
    CTRY_JAMAICA              = 388,     /* Jamaica */
    CTRY_JAPAN                = 392,     /* Japan */
    CTRY_JORDAN               = 400,     /* Jordan */
    CTRY_KAZAKHSTAN           = 398,     /* Kazakhstan */
    CTRY_KENYA                = 404,     /* Kenya */
    CTRY_KOREA_NORTH          = 408,     /* North Korea */
    CTRY_KOREA_ROC            = 410,     /* South Korea */
    CTRY_KOREA_ROC2           = 411,     /* South Korea */
    CTRY_KOREA_ROC3           = 412,     /* South Korea */
    CTRY_KUWAIT               = 414,     /* Kuwait */
    CTRY_LATVIA               = 428,     /* Latvia */
    CTRY_LEBANON              = 422,     /* Lebanon */
    CTRY_LIBYA                = 434,     /* Libya */
    CTRY_LIECHTENSTEIN        = 438,     /* Liechtenstein */
    CTRY_LITHUANIA            = 440,     /* Lithuania */
    CTRY_LUXEMBOURG           = 442,     /* Luxembourg */
    CTRY_MACAU                = 446,     /* Macau */
    CTRY_MACEDONIA            = 807,     /* the Former Yugoslav Republic of Macedonia */
    CTRY_MALAYSIA             = 458,     /* Malaysia */
    CTRY_MALTA	              = 470,     /* Malta */
    CTRY_MEXICO               = 484,     /* Mexico */
    CTRY_MONACO               = 492,     /* Principality of Monaco */
    CTRY_MOROCCO              = 504,     /* Morocco */
    CTRY_NEPAL                = 524,     /* Nepal */
    CTRY_NETHERLANDS          = 528,     /* Netherlands */
    CTRY_NETHERLANDS_ANTILLES = 530,     /* Netherlands-Antilles */
    CTRY_NEW_ZEALAND          = 554,     /* New Zealand */
    CTRY_NICARAGUA            = 558,     /* Nicaragua */
    CTRY_NORWAY               = 578,     /* Norway */
    CTRY_OMAN                 = 512,     /* Oman */
    CTRY_PAKISTAN             = 586,     /* Islamic Republic of Pakistan */
    CTRY_PANAMA               = 591,     /* Panama */
    CTRY_PAPUA_NEW_GUINEA     = 598,     /* Papua New Guinea */
    CTRY_PARAGUAY             = 600,     /* Paraguay */
    CTRY_PERU                 = 604,     /* Peru */
    CTRY_PHILIPPINES          = 608,     /* Republic of the Philippines */
    CTRY_POLAND               = 616,     /* Poland */
    CTRY_PORTUGAL             = 620,     /* Portugal */
    CTRY_PUERTO_RICO          = 630,     /* Puerto Rico */
    CTRY_QATAR                = 634,     /* Qatar */
    CTRY_ROMANIA              = 642,     /* Romania */
    CTRY_RUSSIA               = 643,     /* Russia */
    CTRY_SAUDI_ARABIA         = 682,     /* Saudi Arabia */
    CTRY_SERBIA_MONT          = 891,     /* Serbia and Montenegro */
    CTRY_SINGAPORE            = 702,     /* Singapore */
    CTRY_SLOVAKIA             = 703,     /* Slovak Republic */
    CTRY_SLOVENIA             = 705,     /* Slovenia */
    CTRY_SOUTH_AFRICA         = 710,     /* South Africa */
    CTRY_SPAIN                = 724,     /* Spain */
    CTRY_SRI_LANKA            = 144,     /* Sri Lanka */
    CTRY_SWEDEN               = 752,     /* Sweden */
    CTRY_SWITZERLAND          = 756,     /* Switzerland */
    CTRY_SYRIA                = 760,     /* Syria */
    CTRY_TAIWAN               = 158,     /* Taiwan */
    CTRY_THAILAND             = 764,     /* Thailand */
    CTRY_TRINIDAD_Y_TOBAGO    = 780,     /* Trinidad y Tobago */
    CTRY_TUNISIA              = 788,     /* Tunisia */
    CTRY_TURKEY               = 792,     /* Turkey */
    CTRY_UAE                  = 784,     /* U.A.E. */
    CTRY_UKRAINE              = 804,     /* Ukraine */
    CTRY_UNITED_KINGDOM       = 826,     /* United Kingdom */
    CTRY_UNITED_STATES        = 840,     /* United States */
    CTRY_UNITED_STATES_FCC49  = 842,     /* United States (Public Safety)*/
    CTRY_URUGUAY              = 858,     /* Uruguay */
    CTRY_UZBEKISTAN           = 860,     /* Uzbekistan */
    CTRY_VENEZUELA            = 862,     /* Venezuela */
    CTRY_VIET_NAM             = 704,     /* Viet Nam */
    CTRY_YEMEN                = 887,     /* Yemen */
    CTRY_ZIMBABWE             = 716,     /* Zimbabwe */

	/*
	** Japan special codes.  Boy, do they have a lot
	*/

    CTRY_JAPAN1               = 393,     /* Japan (JP1) */
    CTRY_JAPAN2               = 394,     /* Japan (JP0) */
    CTRY_JAPAN3               = 395,     /* Japan (JP1-1) */
    CTRY_JAPAN4               = 396,     /* Japan (JE1) */
    CTRY_JAPAN5               = 397,     /* Japan (JE2) */
    CTRY_JAPAN6               = 4006,    /* Japan (JP6) */
    CTRY_JAPAN7            	  = 4007,    /* Japan (J7) */
    CTRY_JAPAN8           	  = 4008,    /* Japan (J8) */
    CTRY_JAPAN9           	  = 4009,    /* Japan (J9) */
    CTRY_JAPAN10          	  = 4010,    /* Japan (J10) */
    CTRY_JAPAN11          	  = 4011,    /* Japan (J11) */
    CTRY_JAPAN12          	  = 4012,    /* Japan (J12) */
    CTRY_JAPAN13          	  = 4013,    /* Japan (J13) */
    CTRY_JAPAN14          	  = 4014,    /* Japan (J14) */
    CTRY_JAPAN15          	  = 4015,    /* Japan (J15) */
    CTRY_JAPAN16              = 4016,    /* Japan (J16) */
    CTRY_JAPAN17              = 4017,    /* Japan (J17) */
    CTRY_JAPAN18              = 4018,    /* Japan (J18) */
    CTRY_JAPAN19              = 4019,    /* Japan (J19) */
    CTRY_JAPAN20              = 4020,    /* Japan (J20) */
    CTRY_JAPAN21              = 4021,    /* Japan (J21) */
    CTRY_JAPAN22              = 4022,    /* Japan (J22) */
    CTRY_JAPAN23              = 4023,    /* Japan (J23) */
    CTRY_JAPAN24              = 4024,    /* Japan (J24) */
    CTRY_JAPAN25              = 4025,    /* Japan (J25) */
    CTRY_JAPAN26              = 4026,    /* Japan (J26) */
    CTRY_JAPAN27              = 4027,    /* Japan (J27) */
    CTRY_JAPAN28              = 4028,    /* Japan (J28) */
    CTRY_JAPAN29              = 4029,    /* Japan (J29) */
    CTRY_JAPAN30              = 4030,    /* Japan (J30) */
    CTRY_JAPAN31              = 4031,    /* Japan (J31) */
    CTRY_JAPAN32              = 4032,    /* Japan (J32) */
    CTRY_JAPAN33              = 4033,    /* Japan (J33) */
    CTRY_JAPAN34              = 4034,    /* Japan (J34) */
    CTRY_JAPAN35              = 4035,    /* Japan (J35) */
    CTRY_JAPAN36              = 4036,    /* Japan (J36) */
    CTRY_JAPAN37              = 4037,    /* Japan (J37) */
    CTRY_JAPAN38              = 4038,    /* Japan (J38) */
    CTRY_JAPAN39              = 4039,    /* Japan (J39) */
    CTRY_JAPAN40              = 4040,    /* Japan (J40) */
    CTRY_JAPAN41              = 4041,    /* Japan (J41) */
    CTRY_JAPAN42              = 4042,    /* Japan (J42) */
    CTRY_JAPAN43              = 4043,    /* Japan (J43) */
    CTRY_JAPAN44              = 4044,    /* Japan (J44) */
    CTRY_JAPAN45              = 4045,    /* Japan (J45) */
    CTRY_JAPAN46              = 4046,    /* Japan (J46) */
    CTRY_JAPAN47              = 4047,    /* Japan (J47) */
    CTRY_JAPAN48              = 4048,    /* Japan (J48) */
    CTRY_JAPAN49              = 4049,    /* Japan (J49) */
    CTRY_JAPAN50              = 4050,    /* Japan (J50) */
    CTRY_JAPAN51              = 4051,    /* Japan (J51) */
    CTRY_JAPAN52              = 4052,    /* Japan (J52) */
    CTRY_JAPAN53              = 4053,    /* Japan (J53) */
    CTRY_JAPAN54              = 4054,    /* Japan (J54) */
    CTRY_JAPAN55              = 4055,    /* Japan (J55) */
    CTRY_JAPAN56              = 4056,    /* Japan (J56) */
    CTRY_JAPAN57              = 4057,    /* Japan (J57) */
    CTRY_JAPAN58              = 4058,    /* Japan (J58) */
    CTRY_JAPAN59              = 4059,    /* Japan (J59) */

	/*
	** "Special" codes for multiply defined countries, with the exception
	** of Japan and US.
	*/

    CTRY_AUSTRALIA2           = 5000,    /* Australia */
    CTRY_CANADA2              = 5001,    /* Canada */
    CTRY_BELGIUM2             = 5002     /* Belgium/Cisco implementation */	
};

/* Mask to check whether a domain is a multidomain or a single
   domain */

#define MULTI_DOMAIN_MASK 0xFF00

/* Enumerated Regulatory Domain Information 8 bit values indicate that
 * the regdomain is really a pair of unitary regdomains.  12 bit values
 * are the real unitary regdomains and are the only ones which have the
 * frequency bitmasks and flags set.
 */

enum EnumRd {
	/*
	 * The following regulatory domain definitions are
	 * found in the EEPROM. Each regulatory domain
	 * can operate in either a 5GHz or 2.4GHz wireless mode or
	 * both 5GHz and 2.4GHz wireless modes.
	 * In general, the value holds no special
	 * meaning and is used to decode into either specific
	 * 2.4GHz or 5GHz wireless mode for that particular
	 * regulatory domain.
	 */
	NO_ENUMRD	= 0x00,
	NULL1_WORLD	= 0x03,		/* For 11b-only countries (no 11a allowed) */
	NULL1_ETSIB	= 0x07,		/* Israel */
	NULL1_ETSIC	= 0x08,
	FCC1_FCCA	= 0x10,		/* USA */
	FCC1_WORLD	= 0x11,		/* Hong Kong */
	FCC4_FCCA	= 0x12,		/* USA - Public Safety */
	FCC5_FCCA	= 0x13,		/* USA - with no DFS (UNII-1 + UNII-3 only) */
	FCC6_FCCA	= 0x14,         /* Canada */

	FCC2_FCCA	= 0x20,		/* Canada */
	FCC2_WORLD	= 0x21,		/* Australia & HK */
	FCC2_ETSIC	= 0x22,
	FCC6_WORLD	= 0x23,         /* Australia */
	FRANCE_RES	= 0x31,		/* Legacy France for OEM */
	FCC3_FCCA	= 0x3A,		/* USA & Canada w/5470 band, 11h, DFS enabled */
	FCC3_WORLD	= 0x3B,		/* USA & Canada w/5470 band, 11h, DFS enabled */

	ETSI1_WORLD	= 0x37,
	ETSI3_ETSIA	= 0x32,		/* France (optional) */
	ETSI2_WORLD	= 0x35,		/* Hungary & others */
	ETSI3_WORLD	= 0x36,		/* France & others */
	ETSI4_WORLD	= 0x30,
	ETSI4_ETSIC	= 0x38,
	ETSI5_WORLD	= 0x39,
	ETSI6_WORLD	= 0x34,		/* Bulgaria */
	ETSI_RESERVED	= 0x33,		/* Reserved (Do not used) */

	MKK1_MKKA	= 0x40,		/* Japan (JP1) */
	MKK1_MKKB	= 0x41,		/* Japan (JP0) */
	APL4_WORLD	= 0x42,		/* Singapore */
	MKK2_MKKA	= 0x43,		/* Japan with 4.9G channels */
	APL_RESERVED	= 0x44,		/* Reserved (Do not used)  */
	APL2_WORLD	= 0x45,		/* Korea */
	APL2_APLC	= 0x46,
	APL3_WORLD	= 0x47,
	MKK1_FCCA	= 0x48,		/* Japan (JP1-1) */
	APL2_APLD	= 0x49,		/* Korea with 2.3G channels */
	MKK1_MKKA1	= 0x4A,		/* Japan (JE1) */
	MKK1_MKKA2	= 0x4B,		/* Japan (JE2) */
	MKK1_MKKC	= 0x4C,		/* Japan (MKK1_MKKA,except Ch14) */

	APL3_FCCA   = 0x50,
	APL1_WORLD	= 0x52,		/* Latin America */
	APL1_FCCA	= 0x53,
	APL1_APLA	= 0x54,
	APL1_ETSIC	= 0x55,
	APL2_ETSIC	= 0x56,		/* Venezuela */
	APL5_WORLD	= 0x58,		/* Chile */
	APL6_WORLD	= 0x5B,		/* Singapore */
	APL7_FCCA   = 0x5C,     /* Taiwan 5.47 Band */
	APL8_WORLD  = 0x5D,     /* Malaysia 5GHz */
	APL9_WORLD  = 0x5E,     /* Korea 5GHz */

	/*
	 * World mode SKUs
	 */
	WOR0_WORLD	= 0x60,		/* World0 (WO0 SKU) */
	WOR1_WORLD	= 0x61,		/* World1 (WO1 SKU) */
	WOR2_WORLD	= 0x62,		/* World2 (WO2 SKU) */
	WOR3_WORLD	= 0x63,		/* World3 (WO3 SKU) */
	WOR4_WORLD	= 0x64,		/* World4 (WO4 SKU) */
	WOR5_ETSIC	= 0x65,		/* World5 (WO5 SKU) */

	WOR01_WORLD	= 0x66,		/* World0-1 (WW0-1 SKU) */
	WOR02_WORLD	= 0x67,		/* World0-2 (WW0-2 SKU) */
	EU1_WORLD	= 0x68,		/* Same as World0-2 (WW0-2 SKU), except active scan ch1-13. No ch14 */

	WOR9_WORLD	= 0x69,		/* World9 (WO9 SKU) */
	WORA_WORLD	= 0x6A,		/* WorldA (WOA SKU) */

	MKK3_MKKB	= 0x80,		/* Japan UNI-1 even + MKKB */
	MKK3_MKKA2	= 0x81,		/* Japan UNI-1 even + MKKA2 */
	MKK3_MKKC	= 0x82,		/* Japan UNI-1 even + MKKC */

	MKK4_MKKB	= 0x83,		/* Japan UNI-1 even + UNI-2 + MKKB */
	MKK4_MKKA2	= 0x84,		/* Japan UNI-1 even + UNI-2 + MKKA2 */
	MKK4_MKKC	= 0x85,		/* Japan UNI-1 even + UNI-2 + MKKC */

	MKK5_MKKB	= 0x86,		/* Japan UNI-1 even + UNI-2 + mid-band + MKKB */
	MKK5_MKKA2	= 0x87,		/* Japan UNI-1 even + UNI-2 + mid-band + MKKA2 */
	MKK5_MKKC	= 0x88,		/* Japan UNI-1 even + UNI-2 + mid-band + MKKC */

	MKK6_MKKB	= 0x89,		/* Japan UNI-1 even + UNI-1 odd MKKB */
	MKK6_MKKA2	= 0x8A,		/* Japan UNI-1 even + UNI-1 odd + MKKA2 */
	MKK6_MKKC	= 0x8B,		/* Japan UNI-1 even + UNI-1 odd + MKKC */

	MKK7_MKKB	= 0x8C,		/* Japan UNI-1 even + UNI-1 odd + UNI-2 + MKKB */
	MKK7_MKKA2	= 0x8D,		/* Japan UNI-1 even + UNI-1 odd + UNI-2 + MKKA2 */
	MKK7_MKKC	= 0x8E,		/* Japan UNI-1 even + UNI-1 odd + UNI-2 + MKKC */

	MKK8_MKKB	= 0x8F,		/* Japan UNI-1 even + UNI-1 odd + UNI-2 + mid-band + MKKB */
	MKK8_MKKA2	= 0x90,		/* Japan UNI-1 even + UNI-1 odd + UNI-2 + mid-band + MKKA2 */
	MKK8_MKKC	= 0x91,		/* Japan UNI-1 even + UNI-1 odd + UNI-2 + mid-band + MKKC */

	MKK14_MKKA1     = 0x92,         /* Japan UNI-1 even + UNI-1 odd + 4.9GHz + MKKA1 */
	MKK15_MKKA1     = 0x93,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + 4.9GHz + MKKA1 */

	MKK6_MKKA1      = 0xF8,         /* Japan UNI-1 even + UNI-1 odd + MKKA1 */
	MKK6_FCCA       = 0xF9,         /* Japan UNI-1 even + UNI-1 odd + FCCA */
	MKK7_MKKA1      = 0xFA,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + MKKA1 */
	MKK7_FCCA       = 0xFB,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + FCCA */
	MKK9_FCCA       = 0xFC,         /* Japan UNI-1 even + 4.9GHz + FCCA */
	MKK9_MKKA1      = 0xFD,         /* Japan UNI-1 even + 4.9GHz + MKKA1 */
	MKK9_MKKC       = 0xFE,         /* Japan UNI-1 even + 4.9GHz + MKKC */
	MKK9_MKKA2      = 0xFF,         /* Japan UNI-1 even + 4.9GHz + MKKA2 */

	MKK10_FCCA      = 0xD0,         /* Japan UNI-1 even + UNI-2 + 4.9GHz + FCCA */
	MKK10_MKKA1     = 0xD1,         /* Japan UNI-1 even + UNI-2 + 4.9GHz + MKKA1 */
	MKK10_MKKC      = 0xD2,         /* Japan UNI-1 even + UNI-2 + 4.9GHz + MKKC */
	MKK10_MKKA2     = 0xD3,         /* Japan UNI-1 even + UNI-2 + 4.9GHz + MKKA2 */

	MKK11_MKKA      = 0xD4,         /* Japan UNI-1 even + UNI-2 + Midband + 4.9GHz + MKKA */
	MKK11_FCCA      = 0xD5,         /* Japan UNI-1 even + UNI-2 + Midband + 4.9GHz + FCCA */
	MKK11_MKKA1     = 0xD6,         /* Japan UNI-1 even + UNI-2 + Midband + 4.9GHz + MKKA1 */
	MKK11_MKKC      = 0xD7,         /* Japan UNI-1 even + UNI-2 + Midband + 4.9GHz + MKKC */
	MKK11_MKKA2     = 0xD8,         /* Japan UNI-1 even + UNI-2 + Midband + 4.9GHz + MKKA2 */

	MKK12_MKKA      = 0xD9,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + Midband + 4.9GHz + MKKA */
	MKK12_FCCA      = 0xDA,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + Midband + 4.9GHz + FCCA */
	MKK12_MKKA1     = 0xDB,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + Midband + 4.9GHz + MKKA1 */
	MKK12_MKKC      = 0xDC,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + Midband + 4.9GHz + MKKC */
	MKK12_MKKA2     = 0xDD,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + Midband + 4.9GHz + MKKA2 */

	MKK13_MKKB      = 0xDE,         /* Japan UNI-1 even + UNI-1 odd + UNI-2 + mid-band + MKKB + All passive + no adhoc */

	/* Following definitions are used only by s/w to map old
 	 * Japan SKUs.
	 */
	MKK3_MKKA       = 0xF0,         /* Japan UNI-1 even + MKKA */
	MKK3_MKKA1      = 0xF1,         /* Japan UNI-1 even + MKKA1 */
	MKK3_FCCA       = 0xF2,         /* Japan UNI-1 even + FCCA */
	MKK4_MKKA       = 0xF3,         /* Japan UNI-1 even + UNI-2 + MKKA */
	MKK4_MKKA1      = 0xF4,         /* Japan UNI-1 even + UNI-2 + MKKA1 */
	MKK4_FCCA       = 0xF5,         /* Japan UNI-1 even + UNI-2 + FCCA */
	MKK9_MKKA       = 0xF6,         /* Japan UNI-1 even + 4.9GHz + MKKA*/
	MKK10_MKKA      = 0xF7,         /* Japan UNI-1 even + UNI-2 + 4.9GHz + MKKA */

	/*
	 * Regulator domains ending in a number (e.g. APL1,
	 * MK1, ETSI4, etc) apply to 5GHz channel and power
	 * information.  Regulator domains ending in a letter
	 * (e.g. APLA, FCCA, etc) apply to 2.4GHz channel and
	 * power information.
	 */
	APL1		= 0x0150,	/* LAT & Asia */
	APL2		= 0x0250,	/* LAT & Asia */
	APL3		= 0x0350,	/* Taiwan */
	APL4		= 0x0450,	/* Jordan */
	APL5		= 0x0550,	/* Chile */
	APL6		= 0x0650,	/* Singapore */
	APL8		= 0x0850,	/* Malaysia */
	APL9		= 0x0950,	/* Korea (South) ROC 3 */

	ETSI1		= 0x0130,	/* Europe & others */
	ETSI2		= 0x0230,	/* Europe & others */
	ETSI3		= 0x0330,	/* Europe & others */
	ETSI4		= 0x0430,	/* Europe & others */
	ETSI5		= 0x0530,	/* Europe & others */
	ETSI6		= 0x0630,	/* Europe & others */
	ETSIA		= 0x0A30,	/* France */
	ETSIB		= 0x0B30,	/* Israel */
	ETSIC		= 0x0C30,	/* Latin America */

	FCC1		= 0x0110,	/* US & others */
	FCC2		= 0x0120,	/* Canada, Australia & New Zealand */
	FCC3		= 0x0160,	/* US w/new middle band & DFS */
	FCC4		= 0x0165,	/* US Public Safety */
	FCC5		= 0x0510,	/* US no DFS */
	FCC6		= 0x0610,       /* Canada & Australia */
	FCCA		= 0x0A10,

	APLD		= 0x0D50,	/* South Korea */

	MKK1		= 0x0140,	/* Japan (UNI-1 odd)*/
	MKK2		= 0x0240,	/* Japan (4.9 GHz + UNI-1 odd) */
	MKK3		= 0x0340,	/* Japan (UNI-1 even) */
	MKK4		= 0x0440,	/* Japan (UNI-1 even + UNI-2) */
	MKK5		= 0x0540,	/* Japan (UNI-1 even + UNI-2 + mid-band) */
	MKK6		= 0x0640,	/* Japan (UNI-1 odd + UNI-1 even) */
	MKK7		= 0x0740,	/* Japan (UNI-1 odd + UNI-1 even + UNI-2 */
	MKK8		= 0x0840,	/* Japan (UNI-1 odd + UNI-1 even + UNI-2 + mid-band) */
	MKK9		= 0x0940,   /* Japan (UNI-1 even + 4.9 GHZ) */
	MKK10		= 0x0B40,   /* Japan (UNI-1 even + UNI-2 + 4.9 GHZ) */
	MKK11		= 0x1140,   /* Japan (UNI-1 even + UNI-2 + mid-band + 4.9 GHZ) */
	MKK12		= 0x1240,   /* Japan (UNI-1 even + UNI-1 odd + UNI-2 + mid-band + 4.9 GHZ) */
	MKK13		= 0x0C40,	/* Same as MKK8 but all passive and no adhoc 11a */
	MKK14   = 0x1440,   /* Japan UNI-1 even + UNI-1 odd + 4.9GHz */
	MKK15   = 0x1540,   /* Japan UNI-1 even + UNI-1 odd + UNI-2 + 4.9GHz */
	MKKA		= 0x0A40,	/* Japan */
	MKKC		= 0x0A50,

	NULL1		= 0x0198,
	WORLD		= 0x0199,
	DEBUG_REG_DMN	= 0x01ff,
};

typedef struct {
	HAL_CTRY_CODE	countryCode;
	HAL_REG_DOMAIN	regDmnEnum;
	const char*		isoName;
	const char*		name;
	HAL_BOOL		allow11g;
	HAL_BOOL		allow11aTurbo;
	HAL_BOOL		allow11gTurbo;
    HAL_BOOL        allow11na;      /* HT-40 allowed in 5GHz? */
    HAL_BOOL        allow11ng;      /* HT-40 allowed in 2GHz? */
	u_int16_t		outdoorChanStart;
} COUNTRY_CODE_TO_ENUM_RD;

static COUNTRY_CODE_TO_ENUM_RD allCountries[] = {
    {CTRY_DEBUG,       NO_ENUMRD,     "DB", "DEBUG",          YES, YES, YES, YES, YES, 7000 },
    {CTRY_DEFAULT,     DEF_REGDMN,    "NA", "NO_COUNTRY_SET", YES, YES, YES, YES, YES, 7000 },
    {CTRY_ALBANIA,     NULL1_WORLD,   "AL", "ALBANIA",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_ALGERIA,     NULL1_WORLD,   "DZ", "ALGERIA",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_ARGENTINA,   APL3_WORLD,    "AR", "ARGENTINA",      YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_ARMENIA,     ETSI4_WORLD,   "AM", "ARMENIA",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_AUSTRALIA,   FCC2_WORLD,    "AU", "AUSTRALIA",      YES, YES, YES, YES, YES, 7000 },
    {CTRY_AUSTRALIA2,  FCC6_WORLD,    "A2", "AUSTRALIA2",     YES, YES, YES, YES, YES, 7000 },
    {CTRY_AUSTRIA,     ETSI2_WORLD,   "AT", "AUSTRIA",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_AZERBAIJAN,  ETSI4_WORLD,   "AZ", "AZERBAIJAN",     YES, YES, YES, YES, YES, 7000 },
    {CTRY_BAHRAIN,     APL6_WORLD,    "BH", "BAHRAIN",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_BELARUS,     ETSI1_WORLD,   "BY", "BELARUS",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_BELGIUM,     ETSI1_WORLD,   "BE", "BELGIUM",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_BELGIUM2,    ETSI4_WORLD,   "BL", "BELGIUM",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_BELIZE,      APL1_ETSIC,    "BZ", "BELIZE",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_BOLIVIA,     APL1_ETSIC,    "BO", "BOLVIA",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_BOSNIA_HERZ, ETSI1_WORLD,   "BA", "BOSNIA_HERZGOWINA",YES,NO, YES,  NO, YES, 7000 },
    {CTRY_BRAZIL,      FCC3_WORLD,    "BR", "BRAZIL",         NO,  NO,  NO,  NO,  NO,  7000 },
    {CTRY_BRUNEI_DARUSSALAM,APL1_WORLD,"BN", "BRUNEI DARUSSALAM", YES, YES, YES,  YES, YES, 7000 },
    {CTRY_BULGARIA,    ETSI6_WORLD,   "BG", "BULGARIA",       YES, NO,  YES, YES, YES, 7000 },
    {CTRY_CANADA,      FCC2_FCCA,     "CA", "CANADA",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_CANADA2,     FCC6_FCCA,     "C2", "CANADA2",        YES, YES, YES, YES, YES, 7000 },
    {CTRY_CHILE,       APL6_WORLD,    "CL", "CHILE",          YES, YES, YES, YES, YES, 7000 },
    {CTRY_CHINA,       APL1_WORLD,    "CN", "CHINA",          YES, YES, YES, YES, YES, 7000 },
    {CTRY_COLOMBIA,    FCC1_FCCA,     "CO", "COLOMBIA",       YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_COSTA_RICA,  FCC1_WORLD,    "CR", "COSTA RICA",     YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_CROATIA,     ETSI3_WORLD,   "HR", "CROATIA",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_CYPRUS,      ETSI3_WORLD,   "CY", "CYPRUS",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_CZECH,       ETSI3_WORLD,   "CZ", "CZECH REPUBLIC", YES, NO, YES,  YES, YES, 7000 },
    {CTRY_DENMARK,     ETSI1_WORLD,   "DK", "DENMARK",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_DOMINICAN_REPUBLIC,FCC1_FCCA,"DO", "DOMINICAN REPUBLIC", YES, YES, YES, YES, YES, 7000 },
    {CTRY_ECUADOR,     FCC1_WORLD,    "EC", "ECUADOR",        YES, NO,  NO,  NO,  YES, 7000 },
    {CTRY_EGYPT,       ETSI3_WORLD,   "EG", "EGYPT",          YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_EL_SALVADOR, FCC1_WORLD,    "SV", "EL SALVADOR",    YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_ESTONIA,     ETSI1_WORLD,   "EE", "ESTONIA",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_FINLAND,     ETSI1_WORLD,   "FI", "FINLAND",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_FRANCE,      ETSI1_WORLD,   "FR", "FRANCE",         YES, NO,  YES, YES, YES, 7000 },
    {CTRY_GEORGIA,     ETSI4_WORLD,   "GE", "GEORGIA",        YES, YES, YES, YES, YES, 7000 },
    {CTRY_GERMANY,     ETSI1_WORLD,   "DE", "GERMANY",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_GREECE,      ETSI1_WORLD,   "GR", "GREECE",         YES, NO,  YES, YES, YES, 7000 },
    {CTRY_GUATEMALA,   FCC1_FCCA,     "GT", "GUATEMALA",      YES, YES, YES, YES, YES, 7000 },
    {CTRY_HONDURAS,    NULL1_WORLD,   "HN", "HONDURAS",       YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_HONG_KONG,   FCC2_WORLD,    "HK", "HONG KONG",      YES, YES, YES, YES, YES, 7000 },
    {CTRY_HUNGARY,     ETSI4_WORLD,   "HU", "HUNGARY",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_ICELAND,     ETSI1_WORLD,   "IS", "ICELAND",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_INDIA,       APL6_WORLD,    "IN", "INDIA",          YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_INDONESIA,   APL1_WORLD,    "ID", "INDONESIA",      YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_IRAN,        APL1_WORLD,    "IR", "IRAN",           YES, YES, YES, YES, YES, 7000 },
    {CTRY_IRELAND,     ETSI1_WORLD,   "IE", "IRELAND",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_ISRAEL,      ETSI3_WORLD,   "IL", "ISRAEL",         YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_ISRAEL2,     NULL1_ETSIB,   "ISR","ISRAEL_RES",     YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_ITALY,       ETSI1_WORLD,   "IT", "ITALY",          YES, NO,  YES, YES, YES, 7000 },
    {CTRY_JAMAICA,     ETSI1_WORLD,   "JM", "JAMAICA",        YES, NO,  YES, YES, YES, 7000 },
    {CTRY_JAPAN,       MKK1_MKKA,     "JP", "JAPAN",          YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN1,      MKK1_MKKB,     "J1", "JAPAN1",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN2,      MKK1_FCCA,     "J2", "JAPAN2",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN3,      MKK2_MKKA,     "J3", "JAPAN3",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN4,      MKK1_MKKA1,    "J4", "JAPAN4",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN5,      MKK1_MKKA2,    "J5", "JAPAN5",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN6,      MKK1_MKKC,     "J6", "JAPAN6",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN7,      MKK3_MKKB,     "J7", "JAPAN7",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN8,      MKK3_MKKA2,    "J8", "JAPAN8",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN9,      MKK3_MKKC,     "J9", "JAPAN9",         YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN10,     MKK4_MKKB,     "J10", "JAPAN10",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN11,     MKK4_MKKA2,    "J11", "JAPAN11",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN12,     MKK4_MKKC,     "J12", "JAPAN12",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN13,     MKK5_MKKB,     "J13", "JAPAN13",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN14,     MKK5_MKKA2,    "J14", "JAPAN14",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN15,     MKK5_MKKC,     "J15", "JAPAN15",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN16,     MKK6_MKKB,     "J16", "JAPAN16",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN17,     MKK6_MKKA2,    "J17", "JAPAN17",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN18,     MKK6_MKKC,     "J18", "JAPAN18",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN19,     MKK7_MKKB,     "J19", "JAPAN19",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN20,     MKK7_MKKA2,    "J20", "JAPAN20",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN21,     MKK7_MKKC,     "J21", "JAPAN21",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN22,     MKK8_MKKB,     "J22", "JAPAN22",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN23,     MKK8_MKKA2,    "J23", "JAPAN23",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN24,     MKK8_MKKC,     "J24", "JAPAN24",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN25,     MKK3_MKKA,     "J25", "JAPAN25",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN26,     MKK3_MKKA1,    "J26", "JAPAN26",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN27,     MKK3_FCCA,     "J27", "JAPAN27",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN28,     MKK4_MKKA1,    "J28", "JAPAN28",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN29,     MKK4_FCCA,     "J29", "JAPAN29",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN30,     MKK6_MKKA1,    "J30", "JAPAN30",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN31,     MKK6_FCCA,     "J31", "JAPAN31",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN32,     MKK7_MKKA1,    "J32", "JAPAN32",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN33,     MKK7_FCCA,     "J33", "JAPAN33",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN34,     MKK9_MKKA,     "J34", "JAPAN34",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN35,     MKK10_MKKA,    "J35", "JAPAN35",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN36,     MKK4_MKKA,     "J36", "JAPAN36",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN37,     MKK9_FCCA,     "J37", "JAPAN37",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN38,     MKK9_MKKA1,    "J38", "JAPAN38",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN39,     MKK9_MKKC,     "J39", "JAPAN39",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN40,     MKK10_MKKA2,   "J40", "JAPAN40",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN41,     MKK10_FCCA,    "J41", "JAPAN41",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN42,     MKK10_MKKA1,   "J42", "JAPAN42",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN43,     MKK10_MKKC,    "J43", "JAPAN43",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN44,     MKK10_MKKA2,   "J44", "JAPAN44",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN45,     MKK11_MKKA,    "J45", "JAPAN45",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN46,     MKK11_FCCA,    "J46", "JAPAN46",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN47,     MKK11_MKKA1,   "J47", "JAPAN47",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN48,     MKK11_MKKC,    "J48", "JAPAN48",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN49,     MKK11_MKKA2,   "J49", "JAPAN49",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN50,     MKK12_MKKA,    "J50", "JAPAN50",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN51,     MKK12_FCCA,    "J51", "JAPAN51",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN52,     MKK12_MKKA1,   "J52", "JAPAN52",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN53,     MKK12_MKKC,    "J53", "JAPAN53",       YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_JAPAN54,     MKK12_MKKA2,   "J54", "JAPAN54",       YES, NO,  NO,  NO,  NO,  7000 },
/*
    {CTRY_JAPAN55,     MKK5_MKKA,     "JP", "JAPAN56",        YES, NO,  NO, YES, YES,  7000 },
    {CTRY_JAPAN56,     MKK5_FCCA,     "JP", "JAPAN57",        YES, NO,  NO, YES, YES,  7000 },
*/
    {CTRY_JAPAN57,     MKK13_MKKB,    "JP", "JAPAN57",        YES, NO,  NO, YES, YES,  7000 },
    {CTRY_JAPAN58,     MKK14_MKKA1,   "JP", "JAPAN58",        YES, NO,  NO, YES, YES,  7000 },
    {CTRY_JAPAN59,     MKK15_MKKA1,   "JP", "JAPAN59",        YES, NO,  NO, YES, YES,  7000 },

    {CTRY_JORDAN,      ETSI2_WORLD,   "JO", "JORDAN",         YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_KAZAKHSTAN,  NULL1_WORLD,   "KZ", "KAZAKHSTAN",     YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_KENYA,       NULL1_WORLD,   "KE", "KENYA",          YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_KOREA_NORTH, APL9_WORLD,    "KP", "NORTH KOREA",    YES, NO,  NO,  YES, YES, 7000 },
    {CTRY_KOREA_ROC,   APL9_WORLD,    "KR", "KOREA REPUBLIC", YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_KOREA_ROC2,  APL2_APLD,     "K2", "KOREA REPUBLIC2",YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_KOREA_ROC3,  APL9_WORLD,    "K3", "KOREA REPUBLIC3",YES, NO,  NO,  NO,  NO,  7000 },
    {CTRY_KUWAIT,      NULL1_WORLD,   "KW", "KUWAIT",         YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_LATVIA,      ETSI1_WORLD,   "LV", "LATVIA",         YES, NO,  YES, YES, YES, 7000 },
    {CTRY_LEBANON,     NULL1_WORLD,   "LB", "LEBANON",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_LIECHTENSTEIN,ETSI1_WORLD,  "LI", "LIECHTENSTEIN",  YES, NO,  YES, YES, YES, 7000 },
    {CTRY_LITHUANIA,   ETSI1_WORLD,   "LT", "LITHUANIA",      YES, NO,  YES, YES, YES, 7000 },
    {CTRY_LUXEMBOURG,  ETSI1_WORLD,   "LU", "LUXEMBOURG",     YES, NO,  YES, YES, YES, 7000 },
    {CTRY_MACAU,       FCC2_WORLD,    "MO", "MACAU",          YES, YES, YES, YES, YES, 7000 },
    {CTRY_MACEDONIA,   NULL1_WORLD,   "MK", "MACEDONIA",      YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_MALAYSIA,    APL8_WORLD,    "MY", "MALAYSIA",       NO,  NO,  NO,  NO,  NO,  7000 },
    {CTRY_MALTA,       ETSI1_WORLD,   "MT", "MALTA",          YES, NO,  YES, YES, YES, 7000 },
    {CTRY_MEXICO,      FCC1_FCCA,     "MX", "MEXICO",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_MONACO,      ETSI4_WORLD,   "MC", "MONACO",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_MOROCCO,     NULL1_WORLD,   "MA", "MOROCCO",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_NEPAL,       APL1_WORLD,    "NP", "NEPAL",          YES, NO,  YES, YES, YES, 7000 },
    {CTRY_NETHERLANDS, ETSI1_WORLD,   "NL", "NETHERLANDS",    YES, NO,  YES, YES, YES, 7000 },
    {CTRY_NETHERLANDS_ANTILLES, ETSI1_WORLD,   "AN", "NETHERLANDS-ANTILLES", YES, NO,  YES, YES, YES, 7000 },
    {CTRY_NEW_ZEALAND, FCC2_ETSIC,    "NZ", "NEW ZEALAND",    YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_NORWAY,      ETSI1_WORLD,   "NO", "NORWAY",         YES, NO,  YES, YES, YES, 7000 },
    {CTRY_OMAN,        APL6_WORLD,    "OM", "OMAN",           YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_PAKISTAN,    NULL1_WORLD,   "PK", "PAKISTAN",       YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_PANAMA,      FCC1_FCCA,     "PA", "PANAMA",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_PAPUA_NEW_GUINEA,     FCC1_WORLD,"PG", "PAPUA NEW GUINEA",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_PERU,        APL1_WORLD,    "PE", "PERU",           YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_PHILIPPINES, APL1_WORLD,    "PH", "PHILIPPINES",    YES, YES, YES, YES, YES, 7000 },
    {CTRY_POLAND,      ETSI1_WORLD,   "PL", "POLAND",         YES, NO,  YES, YES, YES, 7000 },
    {CTRY_PORTUGAL,    ETSI1_WORLD,   "PT", "PORTUGAL",       YES, NO,  YES, YES, YES, 7000 },
    {CTRY_PUERTO_RICO, FCC1_FCCA,     "PR", "PUERTO RICO",    YES, YES, YES, YES, YES, 7000 },
    {CTRY_QATAR,       NULL1_WORLD,   "QA", "QATAR",          YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_ROMANIA,     NULL1_WORLD,   "RO", "ROMANIA",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_RUSSIA,      NULL1_WORLD,   "RU", "RUSSIA",         YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_SAUDI_ARABIA,NULL1_WORLD,   "SA", "SAUDI ARABIA",   YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_SERBIA_MONT, ETSI1_WORLD,   "CS", "SERBIA & MONTENEGRO", YES, NO,  YES, YES,  YES, 7000 },
    {CTRY_SINGAPORE,   APL6_WORLD,    "SG", "SINGAPORE",      YES, YES, YES, YES, YES, 7000 },
    {CTRY_SLOVAKIA,    ETSI1_WORLD,   "SK", "SLOVAK REPUBLIC",YES, NO,  YES, YES, YES, 7000 },
    {CTRY_SLOVENIA,    ETSI1_WORLD,   "SI", "SLOVENIA",       YES, NO,  YES, YES, YES, 7000 },
    {CTRY_SOUTH_AFRICA,FCC3_WORLD,    "ZA", "SOUTH AFRICA",   YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_SPAIN,       ETSI1_WORLD,   "ES", "SPAIN",          YES, NO,  YES, YES, YES, 7000 },
    {CTRY_SRI_LANKA,   FCC3_WORLD,    "LK", "SRI LANKA",      YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_SWEDEN,      ETSI1_WORLD,   "SE", "SWEDEN",         YES, NO,  YES, YES, YES, 7000 },
    {CTRY_SWITZERLAND, ETSI1_WORLD,   "CH", "SWITZERLAND",    YES, NO,  YES, YES, YES, 7000 },
    {CTRY_SYRIA,       NULL1_WORLD,   "SY", "SYRIA",          YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_TAIWAN,      APL3_FCCA,     "TW", "TAIWAN",         YES, YES, YES, YES, YES, 7000 },
    {CTRY_THAILAND,    NULL1_WORLD,   "TH", "THAILAND",       YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_TRINIDAD_Y_TOBAGO,ETSI4_WORLD,"TT", "TRINIDAD & TOBAGO", YES, NO, YES, NO, YES, 7000 },
    {CTRY_TUNISIA,     ETSI3_WORLD,   "TN", "TUNISIA",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_TURKEY,      ETSI3_WORLD,   "TR", "TURKEY",         YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_UKRAINE,     NULL1_WORLD,   "UA", "UKRAINE",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_UAE,         NULL1_WORLD,   "AE", "UNITED ARAB EMIRATES", YES, NO, YES, NO, YES, 7000 },
    {CTRY_UNITED_KINGDOM, ETSI1_WORLD,"GB", "UNITED KINGDOM", YES, NO,  YES, YES, YES, 7000 },
    {CTRY_UNITED_STATES, FCC3_FCCA,   "US", "UNITED STATES",  YES, YES, YES, YES, YES, 5825 },
    {CTRY_UNITED_STATES_FCC49, FCC4_FCCA,   "PS", "UNITED STATES (PUBLIC SAFETY)",  YES, YES, YES, YES, YES, 7000 },
    {CTRY_URUGUAY,     FCC1_WORLD,    "UY", "URUGUAY",        YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_UZBEKISTAN,  FCC3_FCCA,     "UZ", "UZBEKISTAN",     YES, YES, YES, YES, YES, 7000 },
    {CTRY_VENEZUELA,   APL2_ETSIC,    "VE", "VENEZUELA",      YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_VIET_NAM,    NULL1_WORLD,   "VN", "VIET NAM",       YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_YEMEN,       NULL1_WORLD,   "YE", "YEMEN",          YES, NO,  YES, NO,  YES, 7000 },
    {CTRY_ZIMBABWE,    NULL1_WORLD,   "ZW", "ZIMBABWE",       YES, NO,  YES, NO,  YES, 7000 }
};

#undef	YES
#undef	NO
#endif /* _ATH_AH_REGDOMAIN_H_ */

