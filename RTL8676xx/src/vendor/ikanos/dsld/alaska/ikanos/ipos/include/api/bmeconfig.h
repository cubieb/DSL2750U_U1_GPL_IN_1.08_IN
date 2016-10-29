#ifndef 	BMECONFIGH
#define 	BMECONFIGH

/**************************************************************
$Workfile:   bmeconfig.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communications, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
$Revision: 1.1 $
$Date: 2008/07/18 11:09:32 $
***************************************************************/

/***************************************************************
** 
**	BME configuration definitions
**
***************************************************************/
/*#define USE_VXWORKS */          /* Comment this out to use other RTOS - IXDP425*/
#define USE_BIG_ENDIAN_FORMAT /* Comment this out to use little endian format */
#ifndef USE_VXWORKS /* IXDP425 */
#define gets(str)	fgets(str, sizeof(str), stdin)
#endif

/****************************************************************/
#ifdef CO_30
/****************************************************************/
    #ifdef CO_30_REFBOARD_2BME
	#define BME_BASE_ADDR             (0x08000000)      /* use this for porting */
	#define BME1_BASE_ADDR            (BME_BASE_ADDR)   /* use this for porting */
	#define BME2_BASE_ADDR            (0x08000010)      /* use this for porting */
        #define BME_BASE_ADDRS            {BME1_BASE_ADDR, BME2_BASE_ADDR}
	#define PTMF_MAX_CHIPS            (1)           //PTMF ASIC now inside BME
	#define PTMF_MDIO_ADDR            (0xe1)        /* access thru HIC-PIO */
	#define PTMF_MODE                 (1)
        #define PTMF_ENET_PHY_BASE_ADDR   (0x18)          /* this value not yet known */
        #define PTMF_TASK_DELAY           (1000)

        #define IPOS_MAX_BME              (2)   /* 1 BME with 2 quads */
        #define IPOS_MAX_PORT_PER_BME     (8)   /* maximum ports per BME  */
        #define IPOS_MAX_PORTS            (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME)

        #define PTMF_MAX_PORTS_PER_CHIP   (8)   /* PTMF inside CO3 BME and fixed 8 ports */
        #define PTMF_MII_PORT_MAP         (0x00ff) /* fixed 8 ports in 2 quads per CO3 BME */
    #else
        #define BME_BASE_ADDR             (0x08000000)      /* use this for porting */
        #define BME1_BASE_ADDR            (BME_BASE_ADDR)   /* use this for porting */
        #define BME2_BASE_ADDR            (0x08000010)      /* use this for porting */
/*      #define BME3_BASE_ADDR              (0x08000020)   */   /* use this for porting */
        #define BME_BASE_ADDRS            {BME1_BASE_ADDR, BME2_BASE_ADDR /*, BME3_BASE_ADDR*/}
        #define PTMF_MAX_CHIPS            (1)           //PTMF ASIC now inside BME
        #define PTMF_MDIO_ADDR            (0xe1)        /* access thru HIC-PIO */
        #define PTMF_MODE                 (1)
        #define PTMF_ENET_PHY_BASE_ADDR   (0x18)          /* this value not yet known */
	#define PTMF_TASK_DELAY           (1000)
	
	#define IPOS_MAX_BME              (1)   /* 1 BME with 2 quads */
	#define IPOS_MAX_PORT_PER_BME     (8)   /* maximum ports per BME  */
	#define IPOS_MAX_PORTS            (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME)
	
	#define PTMF_MAX_PORTS_PER_CHIP   (8)   /* PTMF inside CO3 BME and fixed 8 ports */
	#define PTMF_MII_PORT_MAP         (0x00ff) /* fixed 8 ports in 2 quads per CO3 BME */
    #endif

/****************************************************************/
#elif defined(CO_20)
 /****************************************************************/
 #if defined(CO_20_REFBOARD_1BME)

#if CPE_ATM
 #define BME_BASE_ADDR                 (0x51000000)   /* use this for porting */
 #define BME1_BASE_ADDR                (BME_BASE_ADDR)   /* use this for porting */
 #define BME2_BASE_ADDR                (BME_BASE_ADDR)   /* use this for porting */
#else
	#define BME_BASE_ADDR                 (0x08000000)   /* use this for porting */
	#define BME1_BASE_ADDR                (BME_BASE_ADDR)   /* use this for porting */
	#define BME2_BASE_ADDR              (0x08000010)    /* use this for porting */
#endif    

	#define PTMF_MAX_CHIPS            (1)         //PTMF ASIC in system
	#define PTMF_MDIO_ADDR            (0)
	#define PTMF_MODE                 (1)
	#define PTMF_ENET_PHY_BASE_ADDR   (16)
	#define PTMF_TASK_DELAY           (1000)

	#define IPOS_MAX_BME                      (1)       /* maximum number of  BME */
	#define IPOS_MAX_PORT_PER_BME       (8)   /* maximum ports per BME  */
	#define IPOS_MAX_PORTS            (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME)


	// Ikanos 8 Port Reference system: 1 PTMF connected to 1 BMEs. Each BME supporting
	// 8 PORTS. BUT the hardware is designed to use just the first 8 xMII ports.like a
	// 16 port design.
	#define PTMF_MAX_PORTS_PER_CHIP   (16)
	#define PTMF_MII_PORT_MAP         (0xffff)


 /****************************************************************/
 #elif defined(CO_20_REFBOARD_2BME)

	#define BME_BASE_ADDR                 (0x08000000)   /* use this for porting */
	#define BME1_BASE_ADDR                (BME_BASE_ADDR)   /* use this for porting */
	#define BME2_BASE_ADDR              (0x08000010)    /* use this for porting */
	
	#define PTMF_MAX_CHIPS            (1)         //PTMF ASIC in system
	#define PTMF_MDIO_ADDR            (0)
	#define PTMF_MODE                 (1)
	#define PTMF_ENET_PHY_BASE_ADDR   (16)
	#define PTMF_TASK_DELAY           (1000)
	
	#define IPOS_MAX_BME                      (2)       /* maximum number of  BME */
	#define IPOS_MAX_PORT_PER_BME       (8)   /* maximum ports per BME  */
	#define IPOS_MAX_PORTS            (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME)
	
	
	// Ikanos 8 Port Reference system: 1 PTMF connected to 1 BMEs. Each BME supporting
	// 8 PORTS. BUT the hardware is designed to use just the first 8 xMII ports.like a
	// 16 port design.
	#define PTMF_MAX_PORTS_PER_CHIP   (16)
	#define PTMF_MII_PORT_MAP         (0xffff)


 /****************************************************************/
 #else // BRING UP BOARD

	#define BME_BASE_ADDR             (0x08000000)      /* use this for porting */
	#define BME1_BASE_ADDR            (BME_BASE_ADDR)   /* use this for porting */
	#define BME2_BASE_ADDR            (0x08000010)      /* use this for porting */
	
	#define PTMF_MAX_CHIPS            (1)         //PTMF ASIC in system
	#define PTMF_MDIO_ADDR            (0)
	#define PTMF_MODE                 (1)
	#define PTMF_ENET_PHY_BASE_ADDR   (24)
	#define PTMF_TASK_DELAY           (1000)
	
	#define IPOS_MAX_BME              (1)       /* maximum number of  BME */
	#define IPOS_MAX_PORT_PER_BME     (8)   /* maximum ports per BME  */
	#define IPOS_MAX_PORTS            (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME)
	#define PTMF_MAX_PORTS_PER_CHIP   (8)
	#define PTMF_MII_PORT_MAP         (0x3333)
 #endif

#else	// CO10, CPE2.0 and CPE3.0

 
#if CPE_ATM
 #define BME_BASE_ADDR                 (0x51000000)   /* use this for porting */
 #define BME1_BASE_ADDR                (BME_BASE_ADDR)   /* use this for porting */
#else
 #define BME_BASE_ADDR                 (0xbd000000)  //Fusiv (0x08000000)   /* use this for porting */
 #define BME1_BASE_ADDR                (BME_BASE_ADDR)   /* use this for porting */
#endif
  #ifdef CO_4P_BOARD

    #define	IPOS_MAX_BME				        (2)		  	/* maximum number of  BME  	*/ 	
    #define	IPOS_MAX_PORT_PER_BME		    (3)			  /* maximum ports per BME	  */
    #define IPOS_MAX_PORTS              (4)
    #define PTMF_TASK_DELAY             (100)
    #define BME2_BASE_ADDR              (0x08000010)   /* use this for porting */

  #elif defined(P2CO)
//{
    #define PTMF_MAX_CHIPS              (1)         //PTMF ASIC in system
    #define BME2_BASE_ADDR              (0x09000000)    /* use this for porting */
    #ifndef SIM16P
//  {
      #define	IPOS_MAX_BME				      (1)		/* maximum number of  BME */ 	
      #define PTMF_MDIO_ADDR            (0)
      #define PTMF_MODE                 (1)
      #define PTMF_ENET_PHY_BASE_ADDR   (24)
      
      // Ikanos 8 Port Reference system: 1 PTMF connected to 1 BMEs. Each BME supporting
      // 8 PORTS. BUT the hardware is designed to use just the first 8 xMII ports.like a 
      // 16 port design.
      #define PTMF_MAX_PORTS_PER_CHIP   (16)    
      #define PTMF_MII_PORT_MAP         (0xffff)    
      
      #ifdef SIM4P
//    {
        #define	IPOS_MAX_PORT_PER_BME	  (6)	  /* maximum ports per BME	*/
        #define IPOS_MAX_PORTS          (4)
//    }
      #elif defined(SIM2P)
//    {
        #define	IPOS_MAX_PORT_PER_BME	  (5)	  /* maximum ports per BME	*/
        #define IPOS_MAX_PORTS          (2)
//    }
      #else 
//    {
        #define	IPOS_MAX_PORT_PER_BME	  (8)   /* maximum ports per BME	*/
        #define IPOS_MAX_PORTS          (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME)
//    }
      #endif  //(SIM2P)

      #define PTMF_TASK_DELAY           (1000)
      #else //SIM16P
//    {
        #define	IPOS_MAX_BME				    (2)		    /* maximum number of  BME  	*/ 	
        #define	IPOS_MAX_PORT_PER_BME		(8)			  /* maximum ports per BME	  */
        #define IPOS_MAX_PORTS          (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME) 
        #define PTMF_TASK_DELAY         (1000)
        
        #define PTMF_MDIO_ADDR          (0)
        #define PTMF_MODE               (1)
        #define PTMF_ENET_PHY_BASE_ADDR (16)
        
        // Ikanos 16 Port Reference system: 1 PTMF connected to 2 BMEs. Each BME supporting
        // 8 PORTS.
        #define PTMF_MAX_PORTS_PER_CHIP (16)    
        #define PTMF_MII_PORT_MAP       (0xffff)    

//    }
//  } // SIM16P
    #endif
//}
  #elif defined(BME_CPE_20)
//{
    // AZTEC CHIP BASED HARDWARE
    #define	IPOS_MAX_BME				        (1)	  	/* maximum number of  BME  	*/ 	
    #define	IPOS_MAX_PORT_PER_BME		    (1)			/* maximum ports per BME	*/

    #define IPOS_MAX_PORTS              (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME)
    #define PTMF_TASK_DELAY             (1000)
    #define BME2_BASE_ADDR              (BME_BASE_ADDR)   /* use this for porting */

    #define PTMF_MAX_PORTS_PER_CHIP     (1)    
    #define PTMF_MAX_CHIPS              (1)         // PTMF ASIC in system
    #define PTMF_FPGA_BASE_ADDR         (0x09000000)
    #define PTMF_MDIO_ADDR              (0xe1)      // PTMF interface = HIC PIO
    #define PTMF_MODE                   (0)

#if defined(CPE_30)
  #ifdef RMII_MODE
      #define PTMF_ENET_PHY_BASE_ADDR     (0xf0)
  #else
      #define PTMF_ENET_PHY_BASE_ADDR     (0xf5)
  #endif
#else
  #ifdef RMII_MODE
      #define PTMF_ENET_PHY_BASE_ADDR     (0xf0)
  #else
      #define PTMF_ENET_PHY_BASE_ADDR     (0xf8)
  #endif
#endif //#if defined(CPE_30)

    #define PTMF_MII_PORT_MAP           (0xffff)    

//}
  #else 
//{
    // CPE HARDWARE
    #define	IPOS_MAX_BME				        (1)	  	/* maximum number of  BME  	*/ 	
    #if defined(VTU_R) || defined(P2CPE)
      #define	IPOS_MAX_PORT_PER_BME		  (1)			/* maximum ports per BME	*/
    #else 
      #define	IPOS_MAX_PORT_PER_BME		  (4)			/* maximum ports per BME	*/
    #endif //(VTU_R) || defined(P2CPE)

    #define IPOS_MAX_PORTS              (IPOS_MAX_BME * IPOS_MAX_PORT_PER_BME)
    #define PTMF_TASK_DELAY             (100)
    #define BME2_BASE_ADDR              (BME_BASE_ADDR)   /* use this for porting */

    #define PTMF_MAX_PORTS_PER_CHIP     (1)    
    #define PTMF_MAX_CHIPS              (1)         //PTMF ASIC in system
    #define PTMF_ENET_PHY_BASE_ADDR     (0x18)
    #define PTMF_FPGA_BASE_ADDR         (0x09000000)

 #endif //CO_4P_BOARD
#endif
			 
#define	IPOS_MAX_AUTONOMOUS_MSG		      (6)			/* Autonomous MSg for BME:  */



/***************************************************************
** 
**	IPOS configuration definitions
**
***************************************************************/

#define		IPOS_MSG_LEN	256			/* maximum msgbuf length	*/
#define		IPOS_MAX_MSG	128			/* maximum number of msg buf	*/


#define		IPOS_CALL_EXPIRED_TICK	100	/* number of tickers for API call expired	*/

#ifdef CO_50_EMULATION_BOARD
#define     IPOS_TX_TIMEOUT     300     /* timeout for pollTxMsgRdy polling */
#else
#define		IPOS_TX_TIMEOUT		100		/* timeout for pollTxMsgRdy polling	*/
#endif

/***************************************************************
** 
**	IPOS configuration definitions
**
***************************************************************/

		  
							
/***************************************************************
**
**
**	List of default values for the data structure
**
**
****************************************************************/

#define		sysProv2Band	  		  	\
			{					 		\
				33,			 				   	/* Vendor ID for T1E1	*/	\
				55,0,0,0,0,0,0,0,			  	/* VendorID for ITU		*/  \
				17183,							/* revision Number		*/ 	\
				1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   	/* serial 	Number	*/	\
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  							\
				0,								/* UTOPIA option: PMT mode = 0x08 */ \
				53,								/* 53 byte cell size	*/	\
				0,								/* chip option			*/	\
				3,}					  			/* number of ports		*/	 
				



#define		sysProv3Band	  		  	\
			{					 		\
				33,			 				   	/* Vendor ID for T1E1	*/	\
				55,0,0,0,0,0,0,0,			  	/* VendorID for ITU		*/  \
				17183,							/* revision Number		*/ 	\
				1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   	/* serial 	Number	*/	\
				0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  							\
				0,								/* UTOPIA option: PMT mode = 0x08 */ \
				53,								/* 53 byte cell size	*/	\
				0,								/* chip option			*/	\
				2,}					  			/* number of ports		*/	\
			
#endif	//BMECONFIGH

 												 
																	 
																	 
