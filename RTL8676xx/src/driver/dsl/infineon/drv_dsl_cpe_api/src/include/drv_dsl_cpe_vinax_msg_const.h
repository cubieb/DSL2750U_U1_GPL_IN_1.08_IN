/******************************************************************************

                               Copyright (c) 2006 
                            Infineon Technologies AG 
                     Am Campeon 1-12; 81726 Munich, Germany 

  THE DELIVERY OF THIS SOFTWARE AS WELL AS THE HEREBY GRANTED NON-EXCLUSIVE, 
  WORLDWIDE LICENSE TO USE, COPY, MODIFY, DISTRIBUTE AND SUBLICENSE THIS 
  SOFTWARE IS FREE OF CHARGE. 

  THE LICENSED SOFTWARE IS PROVIDED "AS IS" AND INFINEON EXPRESSLY DISCLAIMS 
  ALL REPRESENTATIONS AND WARRANTIES, WHETHER EXPRESS OR IMPLIED, INCLUDING 
  WITHOUT LIMITATION, WARRANTIES OR REPRESENTATIONS OF WORKMANSHIP, 
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, DURABILITY, THAT THE 
  OPERATING OF THE LICENSED SOFTWARE WILL BE ERROR FREE OR FREE OF ANY THIRD 
  PARTY CLAIMS, INCLUDING WITHOUT LIMITATION CLAIMS OF THIRD PARTY INTELLECTUAL 
  PROPERTY INFRINGEMENT. 

  EXCEPT FOR ANY LIABILITY DUE TO WILFUL ACTS OR GROSS NEGLIGENCE AND EXCEPT 
  FOR ANY PERSONAL INJURY INFINEON SHALL IN NO EVENT BE LIABLE FOR ANY CLAIM 
  OR DAMAGES OF ANY KIND, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
  DEALINGS IN THE SOFTWARE. 

******************************************************************************/


#ifndef _VDSL2_FWDES_CONST_H_
#define _VDSL2_FWDES_CONST_H_

/** \file
   
*/

#ifndef __PACKED__
#if defined (__GNUC__) || defined (__GNUG__)
   /* GNU C or C++ compiler */
   #define __PACKED__ __attribute__ ((packed))
#else
   /* Byte alignment adjustment */
   #pragma pack(1)
      #if !defined (__PACKED__)
   #define __PACKED__      /* nothing */
#endif
#endif
#endif


/** @defgroup _VDSL2_FWDES_CONST_ 
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Include section ----- */
/* ----- Include section (End) ----- */

/* ----- Define section ----- */
/* ----- Define section (End) ----- */

/**
   Common constants used in the messages.
*/
/** Disable a functionality or state */
#define VNX_DISABLE 0
/** Enable a functionality or state */
#define VNX_ENABLE 1
/** Disable a functionality or state */
#define VNX_DISABLE_N 1
/** Enable a functionality or state */
#define VNX_ENABLE_N 0
/** Logical value TRUE */
#define VNX_TRUE 1
/** Logical value FALSE */
#define VNX_FALSE 0
/** Functionality is supported */
#define SUPPORTED 1
/** Functionality is not supported */
#define NOT_SUPPORTED 0
/** Counter value is below threshold */
#define CNTR_OK 0
/** Counter threshold met or exceeded */
#define CNTR_THRESH_HIT 1
/** Fixed Value of 0; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV0 0
/** Fixed Value of 1; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV1 1
/** Fixed Value of 2; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV2 2
/** Fixed Value of 3; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV3 3
/** Fixed Value of 4; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV4 4
/** Fixed Value of 5; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV5 5
/** Fixed Value of 6; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV6 6
/** Fixed Value of 7; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV7 7
/** Fixed Value of 8; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV8 8
/** Fixed Value of 9; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV9 9
/** Fixed Value of 10; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV10 10
/** Fixed Value of 15; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV15 15
/** Fixed Value of 16; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV16 16
/** Fixed Value of 32; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV32 32
/** Fixed Value of 127; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV127 127
/** Fixed Value of 128; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV128 128
/** Fixed Value of 256; Used for numbering parameters (like Array Index number or Length
   number) */
#define FV256 256
/** Link 0 is selected */
#define LINK0 0x0
/** Link 1 is selected */
#define LINK1 0x1
/** No Failure */
#define VNX_OK 0x00
/** Failure */
#define VNX_FAIL 0x01
/** EOC Failure */
#define VNX_EOCFAIL 0x2


/* ----- Constants Definition section ----- */
/* ----- Constants Definition section (End) ----- */

/**
   
*/
typedef struct VNX_BAT_TableEntry VNX_BAT_TableEntry_t;

/**
   
*/
typedef struct VNX_ToneIndex VNX_ToneIndex_t;

/**
   
*/
typedef struct VNX_PSDbreak VNX_PSDbreak_t;

/**
   
*/
typedef struct VNX_RxRef2PSD VNX_RxRef2PSD_t;

/**
   
*/
typedef struct VNX_LineStatus_pb VNX_LineStatus_pb_t;

/**
   
*/
typedef struct VNX_TSSIbreak VNX_TSSIbreak_t;

/**
*/
typedef struct VNX_TSSI VNX_TSSI_t;

/**
   
*/
typedef struct VNX_QLN_NE VNX_QLN_NE_t;

/**
*/
typedef struct VNX_TestParam VNX_TestParam_t;

/**
   
*/
typedef struct VNX_SNR VNX_SNR_t;

/**
*/
typedef struct VNX_HLIN VNX_HLIN_t;

/**
*/
typedef struct VNX_ClearEOC VNX_ClearEOC_t;

/**
*/
typedef struct VNX_UER VNX_UER_t;

/**
*/
typedef struct VNX_TIME VNX_TIME_t;

/**
   
*/
struct VNX_BAT_TableEntry
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Bit Allocation */
   DSL_uint8_t data_01;
   /** Bit Allocation */
   DSL_uint8_t data_00;
#else
   /** Bit Allocation */
   DSL_uint8_t data_00;
   /** Bit Allocation */
   DSL_uint8_t data_01;
#endif
} __PACKED__ ;


/**
   
*/
struct VNX_ToneIndex
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** First Tone Index */
   DSL_uint16_t first;
   /** Last Tone Index */
   DSL_uint16_t last;
#else
   /** First Tone Index */
   DSL_uint16_t first;
   /** Last Tone Index */
   DSL_uint16_t last;
#endif
} __PACKED__ ;


/**
   
*/
struct VNX_PSDbreak
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Tone Index of Breakpoint */
   DSL_uint16_t toneIndex;
   /** PSD Level of Breakpoint */
   DSL_uint16_t level;
   /** Reserved */
   DSL_uint16_t Res00;
#else
   /** Tone Index of Breakpoint */
   DSL_uint16_t toneIndex;
   /** PSD Level of Breakpoint */
   DSL_uint16_t level;
   /** Reserved */
   DSL_uint16_t Res00;
#endif
} __PACKED__ ;


/**
   
*/
struct VNX_RxRef2PSD
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** UPBO RxRefPSD Parameter "a" */
   DSL_int16_t a;
   /** UPBO RxRefPSD Parameter "b" */
   DSL_int16_t b;
#else
   /** UPBO RxRefPSD Parameter "a" */
   DSL_int16_t a;
   /** UPBO RxRefPSD Parameter "b" */
   DSL_int16_t b;
#endif
} __PACKED__ ;


/**
   
*/
struct VNX_LineStatus_pb
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** LATN per Band */
   DSL_uint16_t LATNpb;
   /** SATN per Band */
   DSL_uint16_t SATNpb;
   /** SNRM per Band */
   DSL_uint16_t SNRMpb;
#else
   /** LATN per Band */
   DSL_uint16_t LATNpb;
   /** SATN per Band */
   DSL_uint16_t SATNpb;
   /** SNRM per Band */
   DSL_uint16_t SNRMpb;
#endif
} __PACKED__ ;


/**
   
*/
struct VNX_TSSIbreak
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Supported Set Status */
   DSL_bf16_t supset : 1;
   /** Tone Index of Breakpoint */
   DSL_bf16_t toneIndex : 15;
   /** PSD Level of Breakpoint */
   DSL_uint16_t level;
#else
   /** Tone Index of Breakpoint */
   DSL_bf16_t toneIndex : 15;
   /** Supported Set Status */
   DSL_bf16_t supset : 1;
   /** PSD Level of Breakpoint */
   DSL_uint16_t level;
#endif
} __PACKED__ ;


/**
*/
struct VNX_TSSI
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Number of breakpoints */
   DSL_uint16_t Num;
   /** TSSI breakpoints */
   VNX_TSSIbreak_t TSSIbrkp[64];
#else
   /** Number of breakpoints */
   DSL_uint16_t Num;
   /** TSSI breakpoints */
   VNX_TSSIbreak_t TSSIbrkp[64];
#endif
} __PACKED__ ;


/**
   
*/
struct VNX_QLN_NE
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln_01;
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln_00;
#else
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln_00;
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln_01;
#endif
} __PACKED__ ;


/**
*/
struct VNX_TestParam
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** HLOG per Subcarrier Group */
   DSL_uint16_t hlog;
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr;
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln;
#else
   /** HLOG per Subcarrier Group */
   DSL_uint16_t hlog;
   /** QLN per Subcarrier Group */
   DSL_uint8_t qln;
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr;
#endif
} __PACKED__ ;


/**
   
*/
struct VNX_SNR
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr_01;
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr_00;
#else
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr_00;
   /** SNR per Subcarrier Group */
   DSL_uint8_t snr_01;
#endif
} __PACKED__ ;


/**
*/
struct VNX_HLIN
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** HLIN per Subcarrier Group, Real Part */
   DSL_int16_t hlin_real;
   /** HLIN per Subcarrier Group, Imaginary Part */
   DSL_int16_t hlin_imag;
#else
   /** HLIN per Subcarrier Group, Real Part */
   DSL_int16_t hlin_real;
   /** HLIN per Subcarrier Group, Imaginary Part */
   DSL_int16_t hlin_imag;
#endif
} __PACKED__ ;


/**
*/
struct VNX_ClearEOC
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Reserved */
   DSL_bf16_t Res00 : 6;
   /** Length of the Clear EOC Message */
   DSL_bf16_t Length : 10;
   /** Protocol Identifier */
   DSL_uint16_t ProtIdent;
   /** Clear EOC data field */
   DSL_uint8_t Data[508];
#else
   /** Length of the Clear EOC Message */
   DSL_bf16_t Length : 10;
   /** Reserved */
   DSL_bf16_t Res00 : 6;
   /** Protocol Identifier */
   DSL_uint16_t ProtIdent;
   /** Clear EOC data field */
   DSL_uint8_t Data[508];
#endif
} __PACKED__ ;


/**
*/
struct VNX_UER
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** UERps Real Part, LSW */
   DSL_uint16_t UER_Real_LSW;
   /** UERps Real Part, MSW */
   DSL_uint16_t UER_Real_MSW;
   /** UERps Imaginary Part, LSW */
   DSL_uint16_t UER_Imag_LSW;
   /** UERps Imaginary Part, MSW */
   DSL_uint16_t UER_Imag_MSW;
#else
   /** UERps Real Part, LSW */
   DSL_uint16_t UER_Real_LSW;
   /** UERps Real Part, MSW */
   DSL_uint16_t UER_Real_MSW;
   /** UERps Imaginary Part, LSW */
   DSL_uint16_t UER_Imag_LSW;
   /** UERps Imaginary Part, MSW */
   DSL_uint16_t UER_Imag_MSW;
#endif
} __PACKED__ ;


/**
*/
struct VNX_TIME
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Hours */
   DSL_uint8_t hours;
   /** Minutes */
   DSL_uint8_t minutes;
   /** Seconds */
   DSL_uint8_t seconds;
   /** Reserved */
   DSL_uint8_t Res00;
#else
   /** Minutes */
   DSL_uint8_t minutes;
   /** Hours */
   DSL_uint8_t hours;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Seconds */
   DSL_uint8_t seconds;
#endif
} __PACKED__ ;


#ifdef __cplusplus
}
#endif

#if !defined (__GNUC__) && !defined (__GNUG__)
   #pragma pack()
#endif

/** @} */
#endif
