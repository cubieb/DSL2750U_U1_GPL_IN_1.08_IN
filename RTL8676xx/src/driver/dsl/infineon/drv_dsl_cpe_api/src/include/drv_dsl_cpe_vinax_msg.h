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


#ifndef _DRV_DSL_CPE_VINAX_MSG_H_
#define _DRV_DSL_CPE_VINAX_MSG_H_

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


/** @defgroup _DRV_DSL_CPE_VINAX_MSG_H_ Definitions
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Include section ----- */
/* ----- Include section (End) ----- */

/* ----- Define section ----- */
/* ----- Define section (End) ----- */

/* ----- Constants Definition section ----- */
#define  ACK_ModemFSM_StateGet_RESET_STATE  0
#define  ACK_ModemFSM_StateGet_READY_STATE  1
#define  ACK_ModemFSM_StateGet_FAIL_STATE  2
#define  ACK_ModemFSM_StateGet_DIAG_COMPLETE_STATE  3
#define  ACK_ModemFSM_StateGet_GHS_STATE  5
#define  ACK_ModemFSM_StateGet_FULLINIT_STATE  6
#define  ACK_ModemFSM_StateGet_STEADY_STATE_TC_NOSYNC  7
#define  ACK_ModemFSM_StateGet_STEADY_STATE_TC_SYNC  8
#define  ACK_ModemFSM_StateGet_DIAGMODE_STATE  9
#define  ACK_ModemFSM_StateGet_SELT_STATE  12
#define  ACK_ModemFSM_StateGet_SELT_COMPLETE_STATE  13
#define  ACK_ModemFSM_StateGet_TEST_STATE  240
#define  EVT_ModemFSM_StateGet_RESET_STATE  0
#define  EVT_ModemFSM_StateGet_READY_STATE  1
#define  EVT_ModemFSM_StateGet_FAIL_STATE  2
#define  EVT_ModemFSM_StateGet_DIAG_COMPLETE_STATE  3
#define  EVT_ModemFSM_StateGet_GHS_STATE  5
#define  EVT_ModemFSM_StateGet_FULLINIT_STATE  6
#define  EVT_ModemFSM_StateGet_STEADY_STATE_TC_NOSYNC  7
#define  EVT_ModemFSM_StateGet_STEADY_STATE_TC_SYNC  8
#define  EVT_ModemFSM_StateGet_DIAGMODE_STATE  9
#define  EVT_ModemFSM_StateGet_SELT_STATE  12
#define  EVT_ModemFSM_StateGet_SELT_COMPLETE_STATE  13
#define  EVT_ModemFSM_StateGet_TEST_STATE  240
#define  EVT_ModemReady_MRRCOK  0x0000
#define  EVT_ModemReady_MRRCERR  0x0101
#define  CMD_ModemFSM_StateSet_LINKRES  0
#define  CMD_ModemFSM_StateSet_LINKINI  2
#define  CMD_ModemFSM_StateSet_TESTSTA  8
#define  CMD_ModemFSM_StateSet_SELTINI  9
#define  CMD_DSL_ModeModify_VDSL  0x0
#define  CMD_DSL_ModeModify_ADSL  0x1
#define  ALM_ModemFSM_FailReasonGet_S_OK  0
#define  ALM_ModemFSM_FailReasonGet_S_CODESWAP  1
#define  ALM_ModemFSM_FailReasonGet_S_PSDCONFIG  2
#define  ALM_ModemFSM_FailReasonGet_S_MINRATE_US  3
#define  ALM_ModemFSM_FailReasonGet_S_MINRATE_DS  4
#define  ALM_ModemFSM_FailReasonGet_S_INIT  5
#define  ALM_ModemFSM_FailReasonGet_S_MODE  6
#define  ALM_ModemFSM_FailReasonGet_E_OK  0
#define  ALM_ModemFSM_FailReasonGet_E_CONFIG  1
#define  ALM_ModemFSM_FailReasonGet_E_NOTFEASIBLE  2
#define  ALM_ModemFSM_FailReasonGet_E_COMM  3
#define  ALM_ModemFSM_FailReasonGet_E_COMMCRC  4
#define  ALM_ModemFSM_FailReasonGet_E_NOPEER  5
#define  ALM_ModemFSM_FailReasonGet_E_CHIPSET  6
#define  ALM_ModemFSM_FailReasonGet_E_UNKNOWN  7
#define  ALM_ModemFSM_FailReasonGet_E_RESERVED  8
#define  ACK_ModemFSM_FailReasonGet_S_OK  0
#define  ACK_ModemFSM_FailReasonGet_S_CODESWAP  1
#define  ACK_ModemFSM_FailReasonGet_S_PSDCONFIG  2
#define  ACK_ModemFSM_FailReasonGet_S_MINRATE_US  3
#define  ACK_ModemFSM_FailReasonGet_S_MINRATE_DS  4
#define  ACK_ModemFSM_FailReasonGet_S_INIT  5
#define  ACK_ModemFSM_FailReasonGet_S_MODE  6
#define  ACK_ModemFSM_FailReasonGet_E_OK  0
#define  ACK_ModemFSM_FailReasonGet_E_CONFIG  1
#define  ACK_ModemFSM_FailReasonGet_E_NOTFEASIBLE  2
#define  ACK_ModemFSM_FailReasonGet_E_COMM  3
#define  ACK_ModemFSM_FailReasonGet_E_COMMCRC  4
#define  ACK_ModemFSM_FailReasonGet_E_NOPEER  5
#define  ACK_ModemFSM_FailReasonGet_E_CHIPSET  6
#define  ACK_ModemFSM_FailReasonGet_E_UNKNOWN  7
#define  ACK_ModemFSM_FailReasonGet_E_RESERVED  8
#define  ACK_FW_ImageInfoGet_STANDALONE  0x0
#define  ACK_FW_ImageInfoGet_COMBINED  0x1
#define  ACK_FW_ImageInfoGet_VDSL_FW  0x0
#define  ACK_FW_ImageInfoGet_ADSL_FW  0x1
#define  ACK_FW_ImageInfoGet_CO_FW  0x0
#define  ACK_FW_ImageInfoGet_CPE_FW  0x1
#define  CMD_AlgorithmControlSet_MAXPSD  0x0
#define  CMD_AlgorithmControlSet_PSDREF  0x1
#define  CMD_DigitalFilterSet_POTS  0
#define  CMD_DigitalFilterSet_ISDN  1
#define  CMD_DigitalFilterSet_POTS_2  2
#define  CMD_DigitalFilterSet_POTS_3  3
#define  CMD_DigitalFilterSet_OFF  0
#define  CMD_DigitalFilterSet_TX_ON  1
#define  CMD_DigitalFilterSet_RX_ON  2
#define  CMD_DigitalFilterSet_TXRX_ON  3
#define  CMD_HybridSelect_HYB_AD1_138_17  0x0001
#define  CMD_HybridSelect_HYB_AD1_25_17  0x0002
#define  CMD_HybridSelect_HYB_AD1_138_30  0x0003
#define  CMD_HybridSelect_HYB_AD1_138_17_CPE  0x0010
#define  CMD_HybridSelect_HYB_F1_998  0x0011
#define  CMD_HybridSelect_HYB_F1_998_U0  0x0012
#define  CMD_HybridSelect_HYB_F1_998_U017_O_ISDN  0x0013
#define  CMD_HybridSelect_HYB_F1_JAP30  0x0014
#define  CMD_HybridSelect_HYB_F1_998E17_U0  0x0015
#define  CMD_ACE_FilterSet_OLD  0
#define  CMD_ACE_FilterSet_EQ  1
#define  CMD_ACE_FilterSet_OFF  2
#define  CMD_ACE_FilterSet_TEST  3
#define  CMD_ACE_FilterSet_ADAPTIVE   4
#define  ACK_ACE_FilterGet_EQ  1
#define  ACK_ACE_FilterGet_OFF  2
#define  ACK_ACE_FilterGet_TEST  3
#define  ACK_ACE_FilterGet_ADAPTIVE   4
#define  CMD_Misc_ConfigSet_AVGPOWER  1
#define  ACK_XTSE_StatusGet_L0  0
#define  ACK_XTSE_StatusGet_L3  3
#define  CMD_TR1ControlSet_NOT_USED  0
#define  CMD_TR1ControlSet_RESET_NOW  1
#define  CMD_TR1ControlSet_RESET_EVER  2
#define  CMD_TR1ControlSet_RESET_TR1  3
#define  CMD_TR1ControlSet_SHORTEN  4
#define  CMD_TR1ControlSet_COPY  5
#define  CMD_TimeControl_READ_TIME  2
#define  ACK_TimeGet_PROG  1
#define  CMD_ClearEOC_TxTrigger_IDLE  0
#define  CMD_ClearEOC_TxTrigger_START  1
#define  ACK_ClearEOCStatusGet_IDLE  0
#define  ACK_ClearEOCStatusGet_TXPROG  1
#define  ACK_ClearEOCStatusGet_Reserved  2
#define  ACK_ClearEOCStatusGet_TXERR  3
#define  ACK_ClearEOCStatusGet_RXPROG  1
#define  ACK_ClearEOCStatusGet_RXDONE  2
#define  EVT_ClearEOCStatusGet_TXPROG  2
#define  EVT_ClearEOCStatusGet_TXDONE  2
#define  EVT_ClearEOCStatusGet_TXERR  3
#define  EVT_ClearEOCStatusGet_RXPROG  1
#define  EVT_ClearEOCStatusGet_RXDONE  2
#define  EVT_ClearEOCStatusGet_RXERR  3
#define  ACK_ClearEOCStatusGet_RXERR  3
#define  CMD_TestParamsFE_Request_TRIGGER  0x1
#define  CMD_TestParamsFE_Request_ABORT  0x0
#define  ACK_TestParamsFE_Poll_ONGOING  0x1
#define  CMD_DebugTrailControl_CYCLIC_ON  1
#define  CMD_SELT_Configure_FLOATINGP  1
/* ----- Constants Definition section (End) ----- */

/** Message ID for CMD_ModemFSM_StateGet */
#define CMD_MODEMFSM_STATEGET 0x0002

/**
   Requests information about the current state of the modem state-machine.
   The command can be sent in all states of the modem state machine (see Figure2).
*/
typedef struct CMD_ModemFSM_StateGet CMD_ModemFSM_StateGet_t;

/** Message ID for ACK_ModemFSM_StateGet */
#define ACK_MODEMFSM_STATEGET 0x0002

/**
   Returns information about the current state of the modem state-machine.
*/
typedef struct ACK_ModemFSM_StateGet ACK_ModemFSM_StateGet_t;

/** Message ID for EVT_ModemFSM_StateGet */
#define EVT_MODEMFSM_STATEGET 0x0002

/**
   Returns information about the current state of the modem state-machine. This message
   is sent autonomously, if the modem enters a new state and reporting is enabled for
   that state (see CMD_ModemFSM_EventConfigure).
*/
typedef struct EVT_ModemFSM_StateGet EVT_ModemFSM_StateGet_t;

/** Message ID for EVT_ModemReady */
#define EVT_MODEMREADY 0xFF02

/**
   Autonomous message indicating that the modem online code was reached and that
   initialization was completed. The management entity may not perform any
   query-response messaging until the EVT_ModemReady message is received. Its
   generation cannot be disabled. If it is not received this indicates a problem,
   possibly with the boot sequence. If the message is received, it contains a result
   code that may also indicate that an error was detected by the firmware during
   initialization.
*/
typedef struct EVT_ModemReady EVT_ModemReady_t;

/** Message ID for CMD_ModemFSM_StateSet */
#define CMD_MODEMFSM_STATESET 0x0041

/**
   Controls state transitions of the modem state-machine.
*/
typedef struct CMD_ModemFSM_StateSet CMD_ModemFSM_StateSet_t;

/** Message ID for ACK_ModemFSM_StateSet */
#define ACK_MODEMFSM_STATESET 0x0041

/**
   Acknowledgement for message CMD_ModemFSM_StateSet
*/
typedef struct ACK_ModemFSM_StateSet ACK_ModemFSM_StateSet_t;

/** Message ID for CMD_DSL_ModeModify */
#define CMD_DSL_MODEMODIFY 0x0141

/**
   Changes the DSL mode of a line between VDSL and ADSL. After applying this message, a
   reboot with the selected FW takes place. This command is only valid in boot modes
   0x8 and 0x9 and ignored otherwise.
   Note: This message needs to be applied for changing the DSL mode only, not for
   every line initialization.
   Note: Before applying this message, the VINAX device driver writes the wanted DSL
   mode to a DFE HW register which is evaluated during the boot process.
   (PDSCR register, Bit PYPRG1, "0"=VDSL)
   Note: The parameter "DLS Mode Boot Selection" is needed for a common ADSL/VDSL
   FW. Otherwise, this parameter is not evaluated.
*/
typedef struct CMD_DSL_ModeModify CMD_DSL_ModeModify_t;

/** Message ID for ACK_DSL_ModeModify */
#define ACK_DSL_MODEMODIFY 0x0141

/**
   Acknowledgement for message CMD_DSL_ModeModify
*/
typedef struct ACK_DSL_ModeModify ACK_DSL_ModeModify_t;

/** Message ID for ALM_ModemFSM_FailReasonGet */
#define ALM_MODEMFSM_FAILREASONGET 0x0502

/**
   Reports failure information after entering FAIL state. This message is sent
   autonomously without host request.
*/
typedef struct ALM_ModemFSM_FailReasonGet ALM_ModemFSM_FailReasonGet_t;

/** Message ID for CMD_ModemFSM_FailReasonGet */
#define CMD_MODEMFSM_FAILREASONGET 0x0502

/**
   Requests failure information after entering FAIL state.
*/
typedef struct CMD_ModemFSM_FailReasonGet CMD_ModemFSM_FailReasonGet_t;

/** Message ID for ACK_ModemFSM_FailReasonGet */
#define ACK_MODEMFSM_FAILREASONGET 0x0502

/**
   Returns failure information after entering FAIL state.
*/
typedef struct ACK_ModemFSM_FailReasonGet ACK_ModemFSM_FailReasonGet_t;

/** Message ID for CMD_ModemFSM_OptionsSet */
#define CMD_MODEMFSM_OPTIONSSET 0x0449

/**
   Configuration of options for the modem state machine.
*/
typedef struct CMD_ModemFSM_OptionsSet CMD_ModemFSM_OptionsSet_t;

/** Message ID for ACK_ModemFSM_OptionsSet */
#define ACK_MODEMFSM_OPTIONSSET 0x0449

/**
   Acknowledgement to CMD_ModemFSM_OptionsSet (configuration of automatic re-start).
*/
typedef struct ACK_ModemFSM_OptionsSet ACK_ModemFSM_OptionsSet_t;

/** Message ID for CMD_ModemFSM_EventConfigure */
#define CMD_MODEMFSM_EVENTCONFIGURE 0x0049

/**
   Sets options for the modem state-machine. Enables/Disables EVENT messages (EVT) for
   specific state transitions. The EVT message is generated after entering the
   particular state listed in the parameter description of the enable bits.
*/
typedef struct CMD_ModemFSM_EventConfigure CMD_ModemFSM_EventConfigure_t;

/** Message ID for ACK_ModemFSM_EventConfigure */
#define ACK_MODEMFSM_EVENTCONFIGURE 0x0049

/**
   Acknowledgement for CMD_ModemFSM_EventConfigure. (Enabling/Disabling EVENT messages
   (EVTs) for specific state transitions.)
*/
typedef struct ACK_ModemFSM_EventConfigure ACK_ModemFSM_EventConfigure_t;

/** Message ID for CMD_FailuresNE_AlarmConfigure */
#define CMD_FAILURESNE_ALARMCONFIGURE 0x0149

/**
   Enables/Disables the generation of ALARM messages (ALM) for specific near-end line
   failures. If the corresponding Enable bit for a line failure is set, then the modem
   firmware will send an autonomous message ALM_LineFailureNE_Get if this failure
   occurs.
*/
typedef struct CMD_FailuresNE_AlarmConfigure CMD_FailuresNE_AlarmConfigure_t;

/** Message ID for ACK_FailuresNE_AlarmConfigure */
#define ACK_FAILURESNE_ALARMCONFIGURE 0x0149

/**
   Acknowledgement to CMD_FailuresNE_AlarmConfigure.
*/
typedef struct ACK_FailuresNE_AlarmConfigure ACK_FailuresNE_AlarmConfigure_t;

/** Message ID for CMD_FailuresFE_AlarmConfigure */
#define CMD_FAILURESFE_ALARMCONFIGURE 0x0249

/**
   Enables/Disables the generation of ALARM messages (ALM) for specific far-end line
   failures. If the corresponding Enable bit for a line failure is set, then the modem
   firmware will send an autonomous message ALM_LineFailureFE_Get if this failure
   occurs.
*/
typedef struct CMD_FailuresFE_AlarmConfigure CMD_FailuresFE_AlarmConfigure_t;

/** Message ID for ACK_FailuresFE_AlarmConfigure */
#define ACK_FAILURESFE_ALARMCONFIGURE 0x0249

/**
   Acknowledgement to CMD_FailuresFE_AlarmConfigure.
*/
typedef struct ACK_FailuresFE_AlarmConfigure ACK_FailuresFE_AlarmConfigure_t;

/** Message ID for CMD_ReInitNE_Configure */
#define CMD_REINITNE_CONFIGURE 0x0549

/**
   Configures re-initialization triggers for near-end failure conditions in modem state
   STEADY STATE transmission (see Figure1). This command is accepted in RESET state
   only.
*/
typedef struct CMD_ReInitNE_Configure CMD_ReInitNE_Configure_t;

/** Message ID for ACK_ReInitNE_Configure */
#define ACK_REINITNE_CONFIGURE 0x0549

/**
   Acknowledgment to CMD_ReInitNE_Configure (Configuration of re-initialization
   triggers for near-end failures).
*/
typedef struct ACK_ReInitNE_Configure ACK_ReInitNE_Configure_t;

/** Message ID for CMD_ReInitFE_Configure */
#define CMD_REINITFE_CONFIGURE 0x0649

/**
   Configures Re-initialization triggers for Far-End failure conditions in modem state
   STEADY STATE.
*/
typedef struct CMD_ReInitFE_Configure CMD_ReInitFE_Configure_t;

/** Message ID for ACK_ReInitFE_Configure */
#define ACK_REINITFE_CONFIGURE 0x0649

/**
   Acknowledgment to CMD_ReInitFE_Configure (Configuration of Re-initialization
   triggers for far-end failures).
*/
typedef struct ACK_ReInitFE_Configure ACK_ReInitFE_Configure_t;

/** Message ID for CMD_FW_ImageInfoGet */
#define CMD_FW_IMAGEINFOGET 0xD103

/**
   Requests information about the loaded FW image. This info is used e.g. for swapping
   between ADSL and VDSL.
*/
typedef struct CMD_FW_ImageInfoGet CMD_FW_ImageInfoGet_t;

/** Message ID for ACK_FW_ImageInfoGet */
#define ACK_FW_IMAGEINFOGET 0xD103

/**
   Provides the information about the FW image requested by ACK_FW_ImageInfoGet
*/
typedef struct ACK_FW_ImageInfoGet ACK_FW_ImageInfoGet_t;

/** Message ID for CMD_XTSE_Configure */
#define CMD_XTSE_CONFIGURE 0x0045

/**
   Configuration of the VTU Transmission System Enabling (XTSE).Configures the
   transmission system coding types to be supported on the line (VDSL flavour and annex
   support).
   (References: Section 7.3.1.1.1 of G.997.1 and G.994.1 Amd4 [6] Tables "Standard
   information field – SPar(1) coding")
*/
typedef struct CMD_XTSE_Configure CMD_XTSE_Configure_t;

/** Message ID for ACK_XTSE_Configure */
#define ACK_XTSE_CONFIGURE 0x0045

/**
   Acknowledgement for message CMD_XTSE_Configure.
*/
typedef struct ACK_XTSE_Configure ACK_XTSE_Configure_t;

/** Message ID for CMD_BandControl_DS_Set */
#define CMD_BANDCONTROL_DS_SET 0x2448

/**
   Controls the downstream band usage.
*/
typedef struct CMD_BandControl_DS_Set CMD_BandControl_DS_Set_t;

/** Message ID for ACK_BandControl_DS_Set */
#define ACK_BANDCONTROL_DS_SET 0x2448

/**
   Acknowledgement for message CMD_BandControl_DS_Set.
*/
typedef struct ACK_BandControl_DS_Set ACK_BandControl_DS_Set_t;

/** Message ID for CMD_BandControl_US_Set */
#define CMD_BANDCONTROL_US_SET 0x2548

/**
   Controls the upstream band usage.
   For CO side only, if used to configure normal modem operation in state RESET (and
   not for DMTscope tests). Then the data is transmitted from CO to the CPE during
   initialization (in GHS and FULL_INITIALIZATION state).
   If used for DMTscope tests without handshake and training phases it must be applied
   at the CPE directly.
*/
typedef struct CMD_BandControl_US_Set CMD_BandControl_US_Set_t;

/** Message ID for ACK_BandControl_US_Set */
#define ACK_BANDCONTROL_US_SET 0x2548

/**
   Acknowledgement for message CMD_BandControl_US_Set.
*/
typedef struct ACK_BandControl_US_Set ACK_BandControl_US_Set_t;

/** Message ID for CMD_RFI_BandControlSet */
#define CMD_RFI_BANDCONTROLSET 0x2B48

/**
   Sets the RFI bands to be notched out in the PSD.
   To delete all already configured RFI bands, apply the message with NumRfiBands=1 and
   RfiBand=(0,0).
*/
typedef struct CMD_RFI_BandControlSet CMD_RFI_BandControlSet_t;

/** Message ID for ACK_RFI_BandControlSet */
#define ACK_RFI_BANDCONTROLSET 0x2B48

/**
   Acknowledgement for message CMD_RFI_BandControlSet.
*/
typedef struct ACK_RFI_BandControlSet ACK_RFI_BandControlSet_t;

/** Message ID for CMD_OH_RateSet */
#define CMD_OH_RATESET 0x0448

/**
   Configures the minimum rate of the message based overhead. (Section 7.3.1.5 of
   G.997.1)
   For VDSL1, the minimum overhead rate refers to the EOC overhead rate.
*/
typedef struct CMD_OH_RateSet CMD_OH_RateSet_t;

/** Message ID for ACK_OH_RateSet */
#define ACK_OH_RATESET 0x0448

/**
   Acknowledgement for message CMD_OH_RateSet.
*/
typedef struct ACK_OH_RateSet ACK_OH_RateSet_t;

/** Message ID for CMD_PSD_Set */
#define CMD_PSD_SET 0x2348

/**
   Configuration of PSD and Power parameters.
   For CO side only unless used for DMTscope tests! If used to configure normal modem
   operation in state RESET (and not for DMTscope tests), the data is transmitted from
   CO to the CPE during initialization. If used for DMTscope tests without handshake
   and training phases it is applicable for both sides.
*/
typedef struct CMD_PSD_Set CMD_PSD_Set_t;

/** Message ID for ACK_PSD_Set */
#define ACK_PSD_SET 0x2348

/**
   For CO side only unless used for DMTscope tests!
   Acknowledgement for the message CMD_PSD_Set.
*/
typedef struct ACK_PSD_Set ACK_PSD_Set_t;

/** Message ID for CMD_PSD_BreakpointsTxDS_Set */
#define CMD_PSD_BREAKPOINTSTXDS_SET 0x2648

/**
   Controls the transmit signal at the VTU-O via transmit PSD breakpoints (along with
   the MAXNOMPSD).These PSD breakpoints shape the PSD level relative to the MAXNOMPSD.
*/
typedef struct CMD_PSD_BreakpointsTxDS_Set CMD_PSD_BreakpointsTxDS_Set_t;

/** Message ID for ACK_PSD_BreakpointsTxDS_Set */
#define ACK_PSD_BREAKPOINTSTXDS_SET 0x2648

/**
   Acknowledgement for the message CMD_PSD_BreakpointsTxDS_Set.
*/
typedef struct ACK_PSD_BreakpointsTxDS_Set ACK_PSD_BreakpointsTxDS_Set_t;

/** Message ID for CMD_PSD_BreakpointsTxUS_Set */
#define CMD_PSD_BREAKPOINTSTXUS_SET 0x2848

/**
   Specifies the maximum upstream transmit PSD by means of breakpoints. Values are set
   at the VTU-O and transferred to the VTU-R in the O-SIGNATURE message (except if used
   for DMTscope). See chapter 12.4.4 in G.993.1 [2] and chapter 12.2.4.2.1 in G.993.2
   [3].
   If used for DMTscope tests without handshake and training phases it must be applied
   at the VTU-R directly.
*/
typedef struct CMD_PSD_BreakpointsTxUS_Set CMD_PSD_BreakpointsTxUS_Set_t;

/** Message ID for ACK_PSD_BreakpointsTxUS_Set */
#define ACK_PSD_BREAKPOINTSTXUS_SET 0x2848

/**
   Acknowledgement for the message CMD_PSD_BreakpointsTxUS_Set.
*/
typedef struct ACK_PSD_BreakpointsTxUS_Set ACK_PSD_BreakpointsTxUS_Set_t;

/** Message ID for CMD_PSD_BreakpointsRxRefUS_Set */
#define CMD_PSD_BREAKPOINTSRXREFUS_SET 0x2748

/**
   For VDSL1 only. In VDSL2 use CMD_PSD_BreakpointsRxRef2US_Set.
   Specifies the Upstream Receive Reference PSD by means of breakpoints. It is used for
   upstream power back-off (UPBO). Values are set at the VTU-O and transferred to the
   VTU-R in the O-SIGNATURE message (See chapter 12.4.4 in G.993.1 [2]).
*/
typedef struct CMD_PSD_BreakpointsRxRefUS_Set CMD_PSD_BreakpointsRxRefUS_Set_t;

/** Message ID for ACK_PSD_BreakpointsRxRefUS_Set */
#define ACK_PSD_BREAKPOINTSRXREFUS_SET 0x2748

/**
   Acknowledgement for the message CMD_PSD_BreakpointsRxRefUS_Set.
*/
typedef struct ACK_PSD_BreakpointsRxRefUS_Set ACK_PSD_BreakpointsRxRefUS_Set_t;

/** Message ID for CMD_PSD_BreakpointsRxRef2US_Set */
#define CMD_PSD_BREAKPOINTSRXREF2US_SET 0x2E48

/**
   Specifies the VDSL2 Upstream Receive Reference PSD in the (a, b) format (Section
   7.3.1.2.13 of G997.1). Contains the parameters to compute the reference PSD that
   shall be used for the calculation of upstream power back-off (UPBO). One couple of
   parameters (a, b) is defined per upstream band (except US0). Values are set at the
   VTU-O and transferred to the VTU-R in the O-SIGNATURE message (See chapter 12.2.4 in
   G.993.2 [3]).
*/
typedef struct CMD_PSD_BreakpointsRxRef2US_Set CMD_PSD_BreakpointsRxRef2US_Set_t;

/** Message ID for ACK_PSD_BreakpointsRxRef2US_Set */
#define ACK_PSD_BREAKPOINTSRXREF2US_SET 0x2E48

/**
   Acknowledgement for the message CMD_PSD_BreakpointsRxRef2US_Set.
*/
typedef struct ACK_PSD_BreakpointsRxRef2US_Set ACK_PSD_BreakpointsRxRef2US_Set_t;

/** Message ID for CMD_PBO_Control */
#define CMD_PBO_CONTROL 0x3448

/**
   Controls the Upstream Power Back-Off (UPBO) mode of operation. There are 2 modes: an
   "UPBO standard mode" ( 7.3.1.2.13 of G997.1) and an "UPBO boost mode", which
   calculates the LOSS(kl0, f) parameter in a different way (see Parameter 2 below).
*/
typedef struct CMD_PBO_Control CMD_PBO_Control_t;

/** Message ID for ACK_PBO_Control */
#define ACK_PBO_CONTROL 0x3448

/**
   Acknowledgement for the message CMD_PBO_Control.
*/
typedef struct ACK_PBO_Control ACK_PBO_Control_t;

/** Message ID for CMD_UPBO_KL0Get */
#define CMD_UPBO_KL0GET 0xD603

/**
   Requests the electrical loop length estimate kl0.
*/
typedef struct CMD_UPBO_KL0Get CMD_UPBO_KL0Get_t;

/** Message ID for ACK_UPBO_KL0Get */
#define ACK_UPBO_KL0GET 0xD603

/**
   Delivers the data requested by CMD_UPBO_KL0Get.
   (Section 7.5.1.23 of G.997.1 [7])
*/
typedef struct ACK_UPBO_KL0Get ACK_UPBO_KL0Get_t;

/** Message ID for CMD_PSD_CeilingDS_Set */
#define CMD_PSD_CEILINGDS_SET 0x3848

/**
   Enables for each VDSL2 Profile an adaptive PSD optimization (ceiling) algorithm.
   Currently only required for Profile 8b due to the high transmit power.
*/
typedef struct CMD_PSD_CeilingDS_Set CMD_PSD_CeilingDS_Set_t;

/** Message ID for ACK_PSD_CeilingDS_Set */
#define ACK_PSD_CEILINGDS_SET 0x3848

/**
   Acknowledgement for message CMD_PSD_CeilingDS_Set.
*/
typedef struct ACK_PSD_CeilingDS_Set ACK_PSD_CeilingDS_Set_t;

/** Message ID for CMD_NoiseMarginSet */
#define CMD_NOISEMARGINSET 0x2248

/**
   Configuration of noise margin parameters.
*/
typedef struct CMD_NoiseMarginSet CMD_NoiseMarginSet_t;

/** Message ID for ACK_NoiseMarginSet */
#define ACK_NOISEMARGINSET 0x2248

/**
   Acknowledgement for the message CMD_NoiseMarginSet.
*/
typedef struct ACK_NoiseMarginSet ACK_NoiseMarginSet_t;

/** Message ID for CMD_TssiDS_ADSL_Set */
#define CMD_TSSIDS_ADSL_SET 0x044D

/**
   Set the TSSI breakpoints for PSD shaping for ADSL in downstream direction.
*/
typedef struct CMD_TssiDS_ADSL_Set CMD_TssiDS_ADSL_Set_t;

/** Message ID for ACK_TssiDS_ADSL_Set */
#define ACK_TSSIDS_ADSL_SET 0x044D

/**
   This is the acknowledgement for CMD_TssiDS_ADSL_Set.
*/
typedef struct ACK_TssiDS_ADSL_Set ACK_TssiDS_ADSL_Set_t;

/** Message ID for CMD_TssiUS_ADSL_Set */
#define CMD_TSSIUS_ADSL_SET 0x054D

/**
   Set the TSSI breakpoints for PSD shaping for ADSL in upstream direction.
*/
typedef struct CMD_TssiUS_ADSL_Set CMD_TssiUS_ADSL_Set_t;

/** Message ID for ACK_TssiUS_ADSL_Set */
#define ACK_TSSIUS_ADSL_SET 0x054D

/**
   This is the acknowledgement for CMD_TssiUS_ADSL_Set.
*/
typedef struct ACK_TssiUS_ADSL_Set ACK_TssiUS_ADSL_Set_t;

/** Message ID for CMD_ADSL_CarMaskDS_Set */
#define CMD_ADSL_CARMASKDS_SET 0x024D

/**
   Attention: To be applied after TSSI configuration!
   This configures the ADSL downstream carrier mask per tone.
   The carrier masking must be applied after TSSI configuration with
   CMD_TssiDS_ADSL_Set, because it is meant for masking, i.e. switching off carriers
   but not for switching on carriers with are suppressed via the TSSI breakpoints.
   The message has a fix length to support all ADSL modes, in ADSL and ADSL2 the values
   for the subcarrier indices >255 are unused ("Don’t care").
*/
typedef struct CMD_ADSL_CarMaskDS_Set CMD_ADSL_CarMaskDS_Set_t;

/** Message ID for ACK_ADSL_CarMaskDS_Set */
#define ACK_ADSL_CARMASKDS_SET 0x024D

/**
   This is the acknowledgement for CMD_ADSL_CarMaskDS_Set.
*/
typedef struct ACK_ADSL_CarMaskDS_Set ACK_ADSL_CarMaskDS_Set_t;

/** Message ID for CMD_ADSL_CarMaskUS_Set */
#define CMD_ADSL_CARMASKUS_SET 0x034D

/**
   Attention: To be applied after TSSI configuration!
   This configures the ADSL upstream carrier mask per tone.
   The carrier masking must be applied after TSSI configuration with
   CMD_TssiUS_ADSL_Set, because it is meant for masking, i.e. switching off carriers
   but not for switching them on if suppressed via the TSSI breakpoints.
*/
typedef struct CMD_ADSL_CarMaskUS_Set CMD_ADSL_CarMaskUS_Set_t;

/** Message ID for ACK_ADSL_CarMaskUS_Set */
#define ACK_ADSL_CARMASKUS_SET 0x034D

/**
   This is the acknowledgement for CMD_ADSL_CarMaskUS_Set.
*/
typedef struct ACK_ADSL_CarMaskUS_Set ACK_ADSL_CarMaskUS_Set_t;

/** Message ID for CMD_ADSL_PCB_DS_Set */
#define CMD_ADSL_PCB_DS_SET 0x004D

/**
   This configures the ADSL downstream powercutback for the different ADSL flavours.
*/
typedef struct CMD_ADSL_PCB_DS_Set CMD_ADSL_PCB_DS_Set_t;

/** Message ID for ACK_ADSL_PCB_DS_Set */
#define ACK_ADSL_PCB_DS_SET 0x004D

/**
   This is the acknowledgement for CMD_ADSL_PCB_DS_Set.
*/
typedef struct ACK_ADSL_PCB_DS_Set ACK_ADSL_PCB_DS_Set_t;

/** Message ID for CMD_ADSL_PCB_US_Set */
#define CMD_ADSL_PCB_US_SET 0x014D

/**
   This configures the ADSL upstream power cutback for the different ADSL flavours.
*/
typedef struct CMD_ADSL_PCB_US_Set CMD_ADSL_PCB_US_Set_t;

/** Message ID for ACK_ADSL_PCB_US_Set */
#define ACK_ADSL_PCB_US_SET 0x014D

/**
   This is the acknowledgement for CMD_ADSL_PCB_US_Set.
*/
typedef struct ACK_ADSL_PCB_US_Set ACK_ADSL_PCB_US_Set_t;

/** Message ID for CMD_BearerCh0_DS_Set */
#define CMD_BEARERCH0_DS_SET 0x0048

/**
   Sets parameters for downstream bearer channel 0.
*/
typedef struct CMD_BearerCh0_DS_Set CMD_BearerCh0_DS_Set_t;

/** Message ID for ACK_BearerCh0_DS_Set */
#define ACK_BEARERCH0_DS_SET 0x0048

/**
   Acknowledgement for CMD_BearerCh0_DS_Set. (Configuration of bearer channel 0).
*/
typedef struct ACK_BearerCh0_DS_Set ACK_BearerCh0_DS_Set_t;

/** Message ID for CMD_BearerCh0_US_Set */
#define CMD_BEARERCH0_US_SET 0x0248

/**
   Sets parameters for upstream bearer channel 0.
*/
typedef struct CMD_BearerCh0_US_Set CMD_BearerCh0_US_Set_t;

/** Message ID for ACK_BearerCh0_US_Set */
#define ACK_BEARERCH0_US_SET 0x0248

/**
   Acknowledgement for CMD_BearerCh0_US_Set. (Configuration of bearer channel 0).
*/
typedef struct ACK_BearerCh0_US_Set ACK_BearerCh0_US_Set_t;

/** Message ID for CMD_ADSL_TransportModeSet */
#define CMD_ADSL_TRANSPORTMODESET 0x004F

/**
   Configures the bearer channel to TPS-TC mapping for both directions as ATM.
   Note: The value written with this message will be made the default value starting
   with the end-of-July 06 release so that it is not necessary to apply it
   explicitely then.
*/
typedef struct CMD_ADSL_TransportModeSet CMD_ADSL_TransportModeSet_t;

/** Message ID for ACK_ADSL_TransportModeSet */
#define ACK_ADSL_TRANSPORTMODESET 0x004F

/**
   Acknowledgment for message CMD_ADSL_TransportModeSet.
*/
typedef struct ACK_ADSL_TransportModeSet ACK_ADSL_TransportModeSet_t;

/** Message ID for CMD_AlgorithmControlSet */
#define CMD_ALGORITHMCONTROLSET 0x0245

/**
   Controls certain funtions of the chip set.
   Attention: The default values are different for VDSL and ADSL. VDSL uses the
   Parameters 3 to 4 only, here the defaults are individually assigned
   as listed with the parameter below.In ADSL (including ADSL2/2+) only
   the control bits in Parameter 2 use individual defaults, all other
   (Parameter 3 & 4) have currently the same default value "0".
*/
typedef struct CMD_AlgorithmControlSet CMD_AlgorithmControlSet_t;

/** Message ID for ACK_AlgorithmControlSet */
#define ACK_ALGORITHMCONTROLSET 0x0245

/**
   Acknowledgement for message CMD_AlgorithmControlSet.
*/
typedef struct ACK_AlgorithmControlSet ACK_AlgorithmControlSet_t;

/** Message ID for CMD_DigitalFilterSet */
#define CMD_DIGITALFILTERSET 0x0445

/**
   The message controls the digital high-pass filters in the VINAX.
*/
typedef struct CMD_DigitalFilterSet CMD_DigitalFilterSet_t;

/** Message ID for ACK_DigitalFilterSet */
#define ACK_DIGITALFILTERSET 0x0445

/**
   Acknowledgement for message CMD_DigitalFilterSet.
*/
typedef struct ACK_DigitalFilterSet ACK_DigitalFilterSet_t;

/** Message ID for CMD_HybridSelect */
#define CMD_HYBRIDSELECT 0x0545

/**
   Selection of the used hybrid.
*/
typedef struct CMD_HybridSelect CMD_HybridSelect_t;

/** Message ID for ACK_HybridSelect */
#define ACK_HYBRIDSELECT 0x0545

/**
   Acknowledgement for message CMD_HybridSelect.
*/
typedef struct ACK_HybridSelect ACK_HybridSelect_t;

/** Message ID for CMD_GainCalibrationSet */
#define CMD_GAINCALIBRATIONSET 0x3348

/**
   Sets transmit and receive gain value for the gain calibration.
*/
typedef struct CMD_GainCalibrationSet CMD_GainCalibrationSet_t;

/** Message ID for ACK_GainCalibrationSet */
#define ACK_GAINCALIBRATIONSET 0x3348

/**
   Acknowledgement to CMD_GainCalibrationSet.
*/
typedef struct ACK_GainCalibrationSet ACK_GainCalibrationSet_t;

/** Message ID for CMD_PARR_ThreshSet */
#define CMD_PARR_THRESHSET 0x3648

/**
   This command is used to configure the power level threshold at which the Peak to
   Average Ratio Reduction (PARR) algorithm is enabled. The PARR is operating when the
   transmit power level is above the threshold.
   Only valid if PARR was switched on with CMD_AlgorithmControlSet.
*/
typedef struct CMD_PARR_ThreshSet CMD_PARR_ThreshSet_t;

/** Message ID for ACK_PARR_ThreshSet */
#define ACK_PARR_THRESHSET 0x3648

/**
   This is the acknowledgement for CMD_PARR_ThreshSet.
*/
typedef struct ACK_PARR_ThreshSet ACK_PARR_ThreshSet_t;

/** Message ID for CMD_PARR_ToneSelect */
#define CMD_PARR_TONESELECT 0x3548

/**
   The command selects PARR tones to be used by the Peak to Average Ratio Reduction
   (PARR) algorithm. Only valid if PARR was enabled with CMD_AlgorithmControlSet.
   Note: Care must be taken in selecting the parameters.  A poor selection will
   result in decreased performance!
*/
typedef struct CMD_PARR_ToneSelect CMD_PARR_ToneSelect_t;

/** Message ID for ACK_PARR_ToneSelect */
#define ACK_PARR_TONESELECT 0x3548

/**
   Acknowledgement for CMD_PARR_ToneSelect.
*/
typedef struct ACK_PARR_ToneSelect ACK_PARR_ToneSelect_t;

/** Message ID for CMD_US_DeinterleaverMemPartition */
#define CMD_US_DEINTERLEAVERMEMPARTITION 0x3748

/**
   Controls the amount of de-interleaver memory claimed by the CO US receiver. This is
   necessary for interoperability reasons.
   For known far-end modems, it is advantageous to use as much as possible of the
   de-interleaving memory available in the HW.  For interoperability purposes with
   unknown CPEs, the US memory usage is limited to a configurable part, which is
   dependant on profile and data rate.
   Note: EXAMPLE:G.993.2 requires 64 kB of total interleaving and de-interleaving
   memory (altogether for CO+CPE) for band plans 8 a-d. The Vinax supports 128
   kB in total, corresponding to 64 kB for each direction (CO+CPE), thus at
   the CO there are 32 kB available for de-interleaving.Known CPE: CO uses all
   32 kB of the available HW memory for de-interleaving.Unknown CPE: The
   memory required at the CO for de-interleaving is profile and data rate
   dependant.
   Note: The VDSL2 standard defines the interleaver/de-interleaver memory as a
   single sized block. i.e. its definition assumes that the single memory
   block can be shared by the VTU's interleaver and de-interleaver at any
   ratio as long as the total memory (interleaver + de-interleaver) does not
   exceed the memory requirements from the standard. The interleaver setting
   is controlled by the CO. The CO estimates how much memory is needed for the
   US de-interleaving, based on it specifies to the CPE how much
   de-interleaving memory it can use for the DS. The CPE then chooses the
   interleaver depth and other framing parameters based on the memory
   restrictions provided by the CO.
*/
typedef struct CMD_US_DeinterleaverMemPartition CMD_US_DeinterleaverMemPartition_t;

/** Message ID for ACK_US_DeinterleaverMemPartition */
#define ACK_US_DEINTERLEAVERMEMPARTITION 0x3748

/**
   This is the acknowledgement for CMD_US_DeinterleaverMemPartition.
*/
typedef struct ACK_US_DeinterleaverMemPartition ACK_US_DeinterleaverMemPartition_t;

/** Message ID for CMD_ACE_FilterSet */
#define CMD_ACE_FILTERSET 0x3B48

/**
   The message controls the ACE filter in the AFE’s receive part.
*/
typedef struct CMD_ACE_FilterSet CMD_ACE_FilterSet_t;

/** Message ID for ACK_ACE_FilterSet */
#define ACK_ACE_FILTERSET 0x3B48

/**
   Acknowledgement for message CMD_ACE_FilterSet.
*/
typedef struct ACK_ACE_FilterSet ACK_ACE_FilterSet_t;

/** Message ID for CMD_ACE_FilterGet */
#define CMD_ACE_FILTERGET 0xD703

/**
   Reads the configured ACE filter mode.
*/
typedef struct CMD_ACE_FilterGet CMD_ACE_FilterGet_t;

/** Message ID for ACK_ACE_FilterGet */
#define ACK_ACE_FILTERGET 0xD703

/**
   Provides the configured ACE filter mode as requested by CMD_ACE_FilterGet.
*/
typedef struct ACK_ACE_FilterGet ACK_ACE_FilterGet_t;

/** Message ID for CMD_Misc_ConfigSet */
#define CMD_MISC_CONFIGSET 0x3A48

/**
   Performs some miscellaneous chip set configurations.
*/
typedef struct CMD_Misc_ConfigSet CMD_Misc_ConfigSet_t;

/** Message ID for ACK_Misc_ConfigSet */
#define ACK_MISC_CONFIGSET 0x3A48

/**
   This is the acknowledgement for ACK_Misc_ConfigSet.
*/
typedef struct ACK_Misc_ConfigSet ACK_Misc_ConfigSet_t;

/** Message ID for CMD_AlgorithmControlGet */
#define CMD_ALGORITHMCONTROLGET 0x0205

/**
   Reads back the parameter values configured with CMD_AlgorithmControlSet.
*/
typedef struct CMD_AlgorithmControlGet CMD_AlgorithmControlGet_t;

/** Message ID for ACK_AlgorithmControlGet */
#define ACK_ALGORITHMCONTROLGET 0x0205

/**
   Provides the information requested by CMD_AlgorithmControlGet.
*/
typedef struct ACK_AlgorithmControlGet ACK_AlgorithmControlGet_t;

/** Message ID for CMD_DigitalFilterGet */
#define CMD_DIGITALFILTERGET 0x0405

/**
   Reads back the settings for the digital high-pass filters as configured with
   CMD_DigitalFilterSet.
*/
typedef struct CMD_DigitalFilterGet CMD_DigitalFilterGet_t;

/** Message ID for ACK_DigitalFilterGet */
#define ACK_DIGITALFILTERGET 0x0405

/**
   Provides the configured settings of the digital high-pass filters as requested by
   CMD_DigitalFilterGet.
*/
typedef struct ACK_DigitalFilterGet ACK_DigitalFilterGet_t;

/** Message ID for CMD_HybridSelectionGet */
#define CMD_HYBRIDSELECTIONGET 0x0505

/**
   Reads back the selected hybrid as configured with CMD_HybridSelect.
*/
typedef struct CMD_HybridSelectionGet CMD_HybridSelectionGet_t;

/** Message ID for ACK_HybridSelectionGet */
#define ACK_HYBRIDSELECTIONGET 0x0505

/**
   Provides the used hybrid as requested by CMD_HybridSelectionGet.
*/
typedef struct ACK_HybridSelectionGet ACK_HybridSelectionGet_t;

/** Message ID for CMD_HS_ToneGroupSet */
#define CMD_HS_TONEGROUPSET 0x2D48

/**
   Selects one or more groups of tones to be used during the handshake phase. (See
   G.994.1 Amendment 2 [5], Table 1/G.994.1)
*/
typedef struct CMD_HS_ToneGroupSet CMD_HS_ToneGroupSet_t;

/** Message ID for ACK_HS_ToneGroupSet */
#define ACK_HS_TONEGROUPSET 0x2D48

/**
   Acknowledgement to CMD_HS_ToneGroupSet.
*/
typedef struct ACK_HS_ToneGroupSet ACK_HS_ToneGroupSet_t;

/** Message ID for CMD_HS_VDSL2ProfileControl */
#define CMD_HS_VDSL2PROFILECONTROL 0x2C48

/**
   Configures the profile settings to be used for the handshake phase. (See G.994.1
   Amendment 4 [6], NPAR(3) coding)
*/
typedef struct CMD_HS_VDSL2ProfileControl CMD_HS_VDSL2ProfileControl_t;

/** Message ID for ACK_HS_VDSL2ProfileControl */
#define ACK_HS_VDSL2PROFILECONTROL 0x2C48

/**
   Acknowledgement to CMD_HS_VDSL2ProfileControl.
*/
typedef struct ACK_HS_VDSL2ProfileControl ACK_HS_VDSL2ProfileControl_t;

/** Message ID for CMD_HS_StandardInfoFE_VDSL2Get */
#define CMD_HS_STANDARDINFOFE_VDSL2GET 0xCC03

/**
   Requests the VDSL2 far-end Standard Info (See G.994.1 Amendment 4 [6], NPAR(3)
   coding).
*/
typedef struct CMD_HS_StandardInfoFE_VDSL2Get CMD_HS_StandardInfoFE_VDSL2Get_t;

/** Message ID for ACK_HS_StandardInfoFE_VDSL2Get */
#define ACK_HS_STANDARDINFOFE_VDSL2GET 0xCC03

/**
   Delivers the VDSL2 far-end info as requested by CMD_HS_StandardInfoFE_VDSL2Get (see
   G.994.1 Amendment 4 [6], NPAR(3) coding).
*/
typedef struct ACK_HS_StandardInfoFE_VDSL2Get ACK_HS_StandardInfoFE_VDSL2Get_t;

/** Message ID for CMD_HS_SelectedProfileVDSL2Get */
#define CMD_HS_SELECTEDPROFILEVDSL2GET 0xCD03

/**
   Requests the actual selected VDSL2 Profile (See G.994.1 Amendment 4 [6], NPAR(3)
   coding).
*/
typedef struct CMD_HS_SelectedProfileVDSL2Get CMD_HS_SelectedProfileVDSL2Get_t;

/** Message ID for ACK_HS_SelectedProfileVDSL2Get */
#define ACK_HS_SELECTEDPROFILEVDSL2GET 0xCD03

/**
   Requests the actual selected VDSL2 Profile as requested by
   CMD_HS_SelectedProfileVDSL2Get (see G.994.1 Amendment 4 [6], NPAR(3) coding).
*/
typedef struct ACK_HS_SelectedProfileVDSL2Get ACK_HS_SelectedProfileVDSL2Get_t;


/** Message ID for CMD_HS_StandardInfoFE_SPAR1Get */
#define CMD_HS_STANDARDINFOFE_SPAR1GET 0xCB03

/**
   Requests the handshake information about the far-end operating mode (See G.994.1 [4]
   and Amendments 1-4 [5]-[8], standard information fields- SPAR(1) coding).
*/
typedef struct CMD_HS_StandardInfoFE_SPAR1Get CMD_HS_StandardInfoFE_SPAR1Get_t;

/** Message ID for ACK_HS_StandardInfoFE_SPAR1Get */
#define ACK_HS_STANDARDINFOFE_SPAR1GET 0xCB03

/**
   Provides the handshake information about the far-end operating mode (See G.994.1 [4]
   and Amendments 1-4 [5]-[8], Standard information field - SPAR(1) coding). The bit
   numbering per octet below follows the standard (starting with "1").
*/
typedef struct ACK_HS_StandardInfoFE_SPAR1Get ACK_HS_StandardInfoFE_SPAR1Get_t;


/** Message ID for CMD_SysVendorID_O_Set */
#define CMD_SYSVENDORID_O_SET 0x3843

/**
   Sets the in System Vendor ID at the xTU-C. (Section 7.4.3 of G997.1 [7])
*/
typedef struct CMD_SysVendorID_O_Set CMD_SysVendorID_O_Set_t;

/** Message ID for ACK_SysVendorID_O_Set */
#define ACK_SYSVENDORID_O_SET 0x3843

/**
   Acknowledgement to CMD_SysVendorID_O_Set.
*/
typedef struct ACK_SysVendorID_O_Set ACK_SysVendorID_O_Set_t;

/** Message ID for CMD_SysVendorID_R_Set */
#define CMD_SYSVENDORID_R_SET 0x3C43

/**
   Sets the in System Vendor ID at the xTU-R. (Section 7.4.4 of G997.1 [7])
*/
typedef struct CMD_SysVendorID_R_Set CMD_SysVendorID_R_Set_t;

/** Message ID for ACK_SysVendorID_R_Set */
#define ACK_SYSVENDORID_R_SET 0x3C43

/**
   Acknowledgement to CMD_SysVendorID_R_Set.
*/
typedef struct ACK_SysVendorID_R_Set ACK_SysVendorID_R_Set_t;

/** Message ID for CMD_SysVendorVersionNum_O_Set */
#define CMD_SYSVENDORVERSIONNUM_O_SET 0x3A43

/**
   Sets the in system vendor version number at the xTU-C. (Section 7.4.5 of G997.1
   [7])
*/
typedef struct CMD_SysVendorVersionNum_O_Set CMD_SysVendorVersionNum_O_Set_t;

/** Message ID for ACK_SysVendorVersionNum_O_Set */
#define ACK_SYSVENDORVERSIONNUM_O_SET 0x3A43

/**
   Acknowledgement to CMD_SysVendorVersionNum_O_Set.
*/
typedef struct ACK_SysVendorVersionNum_O_Set ACK_SysVendorVersionNum_O_Set_t;

/** Message ID for CMD_SysVendorVersionNum_R_Set */
#define CMD_SYSVENDORVERSIONNUM_R_SET 0x3D43

/**
   Sets the in system vendor version number at the xTU-R. (Section 7.4.6 of G997.1
   [7])
*/
typedef struct CMD_SysVendorVersionNum_R_Set CMD_SysVendorVersionNum_R_Set_t;

/** Message ID for ACK_SysVendorVersionNum_R_Set */
#define ACK_SYSVENDORVERSIONNUM_R_SET 0x3D43

/**
   Acknowledgement to CMD_SysVendorVersionNum_R_Set.
*/
typedef struct ACK_SysVendorVersionNum_R_Set ACK_SysVendorVersionNum_R_Set_t;

/** Message ID for CMD_SysVendorSerialNum_O_Set */
#define CMD_SYSVENDORSERIALNUM_O_SET 0x3943

/**
   For CO side only!
   Sets the in system vendor version number at the xTU-C. (Section 7.4.7 of G997.1
   [7])
*/
typedef struct CMD_SysVendorSerialNum_O_Set CMD_SysVendorSerialNum_O_Set_t;

/** Message ID for ACK_SysVendorSerialNum_O_Set */
#define ACK_SYSVENDORSERIALNUM_O_SET 0x3943

/**
   Acknowledgement to CMD_SysVendorSerialNum_O_Set.
*/
typedef struct ACK_SysVendorSerialNum_O_Set ACK_SysVendorSerialNum_O_Set_t;

/** Message ID for CMD_SysVendorSerialNum_R_Set */
#define CMD_SYSVENDORSERIALNUM_R_SET 0x3E43

/**
   Sets the in system vendor serial number at the xTU-R. (Section 7.4.8 of G997.1 [7])
*/
typedef struct CMD_SysVendorSerialNum_R_Set CMD_SysVendorSerialNum_R_Set_t;

/** Message ID for ACK_SysVendorSerialNum_R_Set */
#define ACK_SYSVENDORSERIALNUM_R_SET 0x3E43

/**
   Acknowledgement to CMD_SysVendorSerialNum_R_Set.
*/
typedef struct ACK_SysVendorSerialNum_R_Set ACK_SysVendorSerialNum_R_Set_t;

/** Message ID for CMD_AuxInventoryInfo_O_Set */
#define CMD_AUXINVENTORYINFO_O_SET 0x4243

/**
   Sets auxiliary inventory information at the VTU-O (11.2.3.2.4 of G993.2 [3]).
*/
typedef struct CMD_AuxInventoryInfo_O_Set CMD_AuxInventoryInfo_O_Set_t;

/** Message ID for ACK_AuxInventoryInfo_O_Set */
#define ACK_AUXINVENTORYINFO_O_SET 0x4243

/**
   Acknowledgement to CMD_AuxInventoryInfo_O_Set.
*/
typedef struct ACK_AuxInventoryInfo_O_Set ACK_AuxInventoryInfo_O_Set_t;

/** Message ID for CMD_AuxInventoryInfo_R_Set */
#define CMD_AUXINVENTORYINFO_R_SET 0x4343

/**
   Sets auxiliary inventory information at the VTU-R (11.2.3.2.4 of G993.2 [3]).
*/
typedef struct CMD_AuxInventoryInfo_R_Set CMD_AuxInventoryInfo_R_Set_t;

/** Message ID for ACK_AuxInventoryInfo_R_Set */
#define ACK_AUXINVENTORYINFO_R_SET 0x4343

/**
   Acknowledgement to CMD_AuxInventoryInfo_R_Set.
*/
typedef struct ACK_AuxInventoryInfo_R_Set ACK_AuxInventoryInfo_R_Set_t;

/** Message ID for CMD_VendorID_O_Get */
#define CMD_VENDORID_O_GET 0x4003

/**
   Requests the xTU-C Vendor ID. (Section 7.4.1 of G997.1 [7])
*/
typedef struct CMD_VendorID_O_Get CMD_VendorID_O_Get_t;

/** Message ID for ACK_VendorID_O_Get */
#define ACK_VENDORID_O_GET 0x4003

/**
   Provides the xTU-C Vendor ID as requested by CMD_VendorID_O_Get.
*/
typedef struct ACK_VendorID_O_Get ACK_VendorID_O_Get_t;

/** Message ID for CMD_VendorID_R_Get */
#define CMD_VENDORID_R_GET 0x4103

/**
   Requests the xTU-R Vendor ID. (Section 7.4.2 of G997.1 [7])
*/
typedef struct CMD_VendorID_R_Get CMD_VendorID_R_Get_t;

/** Message ID for ACK_VendorID_R_Get */
#define ACK_VENDORID_R_GET 0x4103

/**
   Provides the xTU-R Vendor ID as requested by CMD_VendorID_R_Get.
*/
typedef struct ACK_VendorID_R_Get ACK_VendorID_R_Get_t;

/** Message ID for CMD_SysVendorID_O_Get */
#define CMD_SYSVENDORID_O_GET 0x3803

/**
   Requests the xTU-C System Vendor ID. (Section 7.4.3 of G997.1 [7])
*/
typedef struct CMD_SysVendorID_O_Get CMD_SysVendorID_O_Get_t;

/** Message ID for ACK_SysVendorID_O_Get */
#define ACK_SYSVENDORID_O_GET 0x3803

/**
   Delivers the xTU-C System Vendor ID as requested by CMD_SysVendorID_O_Get.
*/
typedef struct ACK_SysVendorID_O_Get ACK_SysVendorID_O_Get_t;

/** Message ID for CMD_SysVendorID_R_Get */
#define CMD_SYSVENDORID_R_GET 0x3C03

/**
   Requests the xTU-R System Vendor ID. (Section 7.4.4 of G997.1 [7])
*/
typedef struct CMD_SysVendorID_R_Get CMD_SysVendorID_R_Get_t;

/** Message ID for ACK_SysVendorID_R_Get */
#define ACK_SYSVENDORID_R_GET 0x3C03

/**
   Delivers the xTU-R System Vendor ID as requested by CMD_SysVendorID_R_Get.
*/
typedef struct ACK_SysVendorID_R_Get ACK_SysVendorID_R_Get_t;

/** Message ID for CMD_SysVendorVersionNumO_Get */
#define CMD_SYSVENDORVERSIONNUMO_GET 0x3A03

/**
   Requests the xTU-C system vendor version number. (Section 7.4.5 of G997.1 [7])
*/
typedef struct CMD_SysVendorVersionNumO_Get CMD_SysVendorVersionNumO_Get_t;

/** Message ID for ACK_SysVendorVersionNum_O_Get */
#define ACK_SYSVENDORVERSIONNUM_O_GET 0x3A03

/**
   Delivers the xTU-C system vendor version number as requested by
   CMD_SysVendorVersionNumO_Get.
*/
typedef struct ACK_SysVendorVersionNum_O_Get ACK_SysVendorVersionNum_O_Get_t;

/** Message ID for CMD_SysVendorVersionNum_R_Get */
#define CMD_SYSVENDORVERSIONNUM_R_GET 0x3D03

/**
   Requests the xTU-R system vendor version number. (Section 7.4.6 of G997.1 [7])
*/
typedef struct CMD_SysVendorVersionNum_R_Get CMD_SysVendorVersionNum_R_Get_t;

/** Message ID for ACK_SysVendorVersionNum_R_Get */
#define ACK_SYSVENDORVERSIONNUM_R_GET 0x3D03

/**
   Delivers the xTU-R system vendor version number as requested by
   CMD_SysVendorVersionNum_R_Get.
*/
typedef struct ACK_SysVendorVersionNum_R_Get ACK_SysVendorVersionNum_R_Get_t;

/** Message ID for CMD_SysVendorSerialNum_O_Get */
#define CMD_SYSVENDORSERIALNUM_O_GET 0x3903

/**
   Requests the xTU-C system vendor serial number. (Section 7.4.7 of G997.1 [7])
*/
typedef struct CMD_SysVendorSerialNum_O_Get CMD_SysVendorSerialNum_O_Get_t;

/** Message ID for ACK_SysVendorSerialNum_O_Get */
#define ACK_SYSVENDORSERIALNUM_O_GET 0x3903

/**
   Delivers the xTU-C system vendor serial number as requested by
   CMD_SysVendorSerialNum_O_Get.
*/
typedef struct ACK_SysVendorSerialNum_O_Get ACK_SysVendorSerialNum_O_Get_t;

/** Message ID for CMD_SysVendorSerialNum_R_Get */
#define CMD_SYSVENDORSERIALNUM_R_GET 0x3E03

/**
   Requests the xTU-R system vendor serial number. (Section 7.4.8 of G997.1 [7])
*/
typedef struct CMD_SysVendorSerialNum_R_Get CMD_SysVendorSerialNum_R_Get_t;

/** Message ID for ACK_SysVendorSerialNum_R_Get */
#define ACK_SYSVENDORSERIALNUM_R_GET 0x3E03

/**
   Delivers the xTU-R system vendor version number as requested by
   CMD_SysVendorSerialNum_R_Get.
*/
typedef struct ACK_SysVendorSerialNum_R_Get ACK_SysVendorSerialNum_R_Get_t;

/** Message ID for CMD_AuxInventoryInfo_O_Get */
#define CMD_AUXINVENTORYINFO_O_GET 0x4203

/**
   Requests VTU-O auxiliary inventory information (11.2.3.2.4 of G993.2 [3]).
*/
typedef struct CMD_AuxInventoryInfo_O_Get CMD_AuxInventoryInfo_O_Get_t;

/** Message ID for ACK_AuxInventoryInfo_O_Get */
#define ACK_AUXINVENTORYINFO_O_GET 0x4203

/**
   Delivers the VTU-O auxiliary inventory information as requested by
   CMD_AuxInventoryInfo_O_Get.
*/
typedef struct ACK_AuxInventoryInfo_O_Get ACK_AuxInventoryInfo_O_Get_t;

/** Message ID for CMD_AuxInventoryInfo_R_Get */
#define CMD_AUXINVENTORYINFO_R_GET 0x4303

/**
   Requests VTU-R auxiliary inventory information (11.2.3.2.4 of G993.2 [3]).
*/
typedef struct CMD_AuxInventoryInfo_R_Get CMD_AuxInventoryInfo_R_Get_t;

/** Message ID for ACK_AuxInventoryInfo_R_Get */
#define ACK_AUXINVENTORYINFO_R_GET 0x4303

/**
   Delivers the VTU-R auxiliary inventory information as requested by
   CMD_AuxInventoryInfo_R_Get.
*/
typedef struct ACK_AuxInventoryInfo_R_Get ACK_AuxInventoryInfo_R_Get_t;

/** Message ID for CMD_XTSE_StatusGet */
#define CMD_XTSE_STATUSGET 0x0102

/**
   Requests the transmission system in use on the line (DSL flavour).
   (Section 7.5.1.1. of G.997.1 [7])
*/
typedef struct CMD_XTSE_StatusGet CMD_XTSE_StatusGet_t;

/** Message ID for ACK_XTSE_StatusGet */
#define ACK_XTSE_STATUSGET 0x0102

/**
   Reports the transmission system mode in use as requested by CMD_XTSE_StatusGet.
*/
typedef struct ACK_XTSE_StatusGet ACK_XTSE_StatusGet_t;

/** Message ID for CMD_BandControl_DS_Get */
#define CMD_BANDCONTROL_DS_GET 0xC803

/**
   Reads the used downstream band configuration.
   Note: The reported used DS band configuration reflects the MEDLEYds set as
   defined in the O-PRM message of the channel discovery phase according to
   G.993.2.
*/
typedef struct CMD_BandControl_DS_Get CMD_BandControl_DS_Get_t;

/** Message ID for ACK_BandControl_DS_Get */
#define ACK_BANDCONTROL_DS_GET 0xC803

/**
   Acknowledgement for message CMD_BandControl_DS_Get.
*/
typedef struct ACK_BandControl_DS_Get ACK_BandControl_DS_Get_t;

/** Message ID for CMD_BandControl_US_Get */
#define CMD_BANDCONTROL_US_GET 0xC903

/**
   Reads the used upstream band configuration.
   Note: The reported used US band configuration reflects the MEDLEYus set as
   defined in the R-PRM message of the channel discovery phase in G.993.2.
*/
typedef struct CMD_BandControl_US_Get CMD_BandControl_US_Get_t;

/** Message ID for ACK_BandControl_US_Get */
#define ACK_BANDCONTROL_US_GET 0xC903

/**
   Acknowledgement for message CMD_BandControl_US_Get.
*/
typedef struct ACK_BandControl_US_Get ACK_BandControl_US_Get_t;

/** Message ID for CMD_RFI_BandControlGet */
#define CMD_RFI_BANDCONTROLGET 0xCA03

/**
   Reads the RFI band configuration.
*/
typedef struct CMD_RFI_BandControlGet CMD_RFI_BandControlGet_t;

/** Message ID for ACK_RFI_BandControlGet */
#define ACK_RFI_BANDCONTROLGET 0xCA03

/**
   Acknowledgement for message CMD_RFI_BandControlGet.
*/
typedef struct ACK_RFI_BandControlGet ACK_RFI_BandControlGet_t;

/** Message ID for CMD_LineStatusDS_Get */
#define CMD_LINESTATUSDS_GET 0x4403

/**
   This message requests data about the line status (like noise margin) for downstream
   direction.
*/
typedef struct CMD_LineStatusDS_Get CMD_LineStatusDS_Get_t;

/** Message ID for ACK_LineStatusDS_Get */
#define ACK_LINESTATUSDS_GET 0x4403

/**
   The message delivers the line status data for downstream direction requested by
   CMD_LineStatusDS_Get.
*/
typedef struct ACK_LineStatusDS_Get ACK_LineStatusDS_Get_t;

/** Message ID for CMD_LineStatusUS_Get */
#define CMD_LINESTATUSUS_GET 0x4503

/**
   This message requests data about the upstream line status (like noise margin).
*/
typedef struct CMD_LineStatusUS_Get CMD_LineStatusUS_Get_t;

/** Message ID for ACK_LineStatusUS_Get */
#define ACK_LINESTATUSUS_GET 0x4503

/**
   The message delivers the line status data for upstream direction requested by
   CMD_LineStatusUS_Get.
*/
typedef struct ACK_LineStatusUS_Get ACK_LineStatusUS_Get_t;

/** Message ID for CMD_LineStatusPerBandDS_Get */
#define CMD_LINESTATUSPERBANDDS_GET 0xD203

/**
   This message requests data about the line status (noise margin, line attenuation and
   signal attenuation) for downstream direction in a "per band" granularity.
*/
typedef struct CMD_LineStatusPerBandDS_Get CMD_LineStatusPerBandDS_Get_t;

/** Message ID for ACK_LineStatusPerBandDS_Get */
#define ACK_LINESTATUSPERBANDDS_GET 0xD203

/**
   The message delivers the line status data (line attenuation, signal attenuation and
   noise margin) for downstream direction in a "per band" granularity as requested by
   CMD_LineStatusPerBandDS_Get.
*/
typedef struct ACK_LineStatusPerBandDS_Get ACK_LineStatusPerBandDS_Get_t;

/** Message ID for CMD_LineStatusPerBandUS_Get */
#define CMD_LINESTATUSPERBANDUS_GET 0xD303

/**
   This message requests data about the line status (noise margin, line - and signal
   attenuation) for upstream direction in a "per band" granularity.
*/
typedef struct CMD_LineStatusPerBandUS_Get CMD_LineStatusPerBandUS_Get_t;

/** Message ID for ACK_LineStatusPerBandUS_Get */
#define ACK_LINESTATUSPERBANDUS_GET 0xD303

/**
   The message delivers the line status data (line attenuation, signal attenuation and
   noise margin) for upstream direction in a "per band" granularity as requested by
   CMD_LineStatusPerBandUS_Get.
*/
typedef struct ACK_LineStatusPerBandUS_Get ACK_LineStatusPerBandUS_Get_t;

/** Message ID for CMD_PSD_BreakpointsDS_Get */
#define CMD_PSD_BREAKPOINTSDS_GET 0xD403

/**
   Requests the breakpoints of the actual PSD for the downstream direction.
*/
typedef struct CMD_PSD_BreakpointsDS_Get CMD_PSD_BreakpointsDS_Get_t;

/** Message ID for ACK_PSD_BreakpointsDS_Get */
#define ACK_PSD_BREAKPOINTSDS_GET 0xD403

/**
   Delivers the breakpoints of the actual PSD for the downstream direction as requested
   by CMD_PSD_BreakpointsDS_Get.
*/
typedef struct ACK_PSD_BreakpointsDS_Get ACK_PSD_BreakpointsDS_Get_t;

/** Message ID for CMD_PSD_BreakpointsUS_Get */
#define CMD_PSD_BREAKPOINTSUS_GET 0xD503

/**
   Requests the breakpoints of the actual PSD for the upstream direction.
*/
typedef struct CMD_PSD_BreakpointsUS_Get CMD_PSD_BreakpointsUS_Get_t;

/** Message ID for ACK_PSD_BreakpointsUS_Get */
#define ACK_PSD_BREAKPOINTSUS_GET 0xD503

/**
   Delivers the breakpoints of the actual PSD for the upstream direction as requested
   by CMD_PSD_BreakpointsUS_Get.
*/
typedef struct ACK_PSD_BreakpointsUS_Get ACK_PSD_BreakpointsUS_Get_t;

/** Message ID for CMD_TssiDS_VDSL2_Get */
#define CMD_TSSIDS_VDSL2_GET 0x2803

/**
   The Downstream TSSI Breakpoints are specified by the transmitter (VTU-O) and
   communicated to the receiver (VTU-R) via the O-PRM message.   The TSSI Breakpoints
   are used to shape the O-P-TEQ signal. This message provides read only access to the
   DS TSSI Breakpoints. The DS TSSI Breakpoints are not configurable via messages.
*/
typedef struct CMD_TssiDS_VDSL2_Get CMD_TssiDS_VDSL2_Get_t;

/** Message ID for ACK_TssiDS_VDSL2_Get */
#define ACK_TSSIDS_VDSL2_GET 0x2803

/**
   This is the acknowledgement of CMD_TssiDS_VDSL2_Get.
*/
typedef struct ACK_TssiDS_VDSL2_Get ACK_TssiDS_VDSL2_Get_t;

/** Message ID for CMD_TssiUS_VDSL2_Get */
#define CMD_TSSIUS_VDSL2_GET 0x2903

/**
   The Upstream TSSI Breakpoints are specified by the transmitter (VTU-R) and
   communicated to the receiver (VTU-O) via the R-PRM message.   The TSSI Breakpoints
   are used to shape the R-P-TEQ signal.
   Note: The TSSI Breakpoints can be read only but are not configurable via
   messages.
*/
typedef struct CMD_TssiUS_VDSL2_Get CMD_TssiUS_VDSL2_Get_t;

/** Message ID for ACK_TssiUS_VDSL2_Get */
#define ACK_TSSIUS_VDSL2_GET 0x2903

/**
   This is the acknowledgement of CMD_TssiUS_VDSL2_Get.
*/
typedef struct ACK_TssiUS_VDSL2_Get ACK_TssiUS_VDSL2_Get_t;

/** Message ID for CMD_ADSL_PCB_Get */
#define CMD_ADSL_PCB_GET 0x000E

/**
   Reads the actual power cutback value.
*/
typedef struct CMD_ADSL_PCB_Get CMD_ADSL_PCB_Get_t;

/** Message ID for ACK_ADSL_PCB_Get */
#define ACK_ADSL_PCB_GET 0x000E

/**
   Provides the actual power cutback vaues requested with CMD_ADSL_PCB_Get.
*/
typedef struct ACK_ADSL_PCB_Get ACK_ADSL_PCB_Get_t;

/** Message ID for CMD_BearerChsDS_Get */
#define CMD_BEARERCHSDS_GET 0x0106

/**
   Requests status information for the downstream bearer channels.
*/
typedef struct CMD_BearerChsDS_Get CMD_BearerChsDS_Get_t;

/** Message ID for ACK_BearerChsDS_Get */
#define ACK_BEARERCHSDS_GET 0x0106

/**
   Delivers status information for the downstream bearer channels.
*/
typedef struct ACK_BearerChsDS_Get ACK_BearerChsDS_Get_t;

/** Message ID for CMD_BearerChsUS_Get */
#define CMD_BEARERCHSUS_GET 0x0006

/**
   Requests status information for the upstream bearer channels.
*/
typedef struct CMD_BearerChsUS_Get CMD_BearerChsUS_Get_t;

/** Message ID for ACK_BearerChsUS_Get */
#define ACK_BEARERCHSUS_GET 0x0006

/**
   Delivers status information for the upstream bearer channels.
*/
typedef struct ACK_BearerChsUS_Get ACK_BearerChsUS_Get_t;

/** Message ID for CMD_FrameData1DS_Get */
#define CMD_FRAMEDATA1DS_GET 0x1E03

/**
   Requests basic VDSL1 frame setting data for the downstream direction.
*/
typedef struct CMD_FrameData1DS_Get CMD_FrameData1DS_Get_t;

/** Message ID for ACK_FrameData1DS_Get */
#define ACK_FRAMEDATA1DS_GET 0x1E03

/**
   Delivers basic VDSL1 framing parameters for the downstream direction requested by
   CMD_FrameData1DS_Get.These parameters are covered in sections 8.5 & 8.4 of G.993.1
   [2].
*/
typedef struct ACK_FrameData1DS_Get ACK_FrameData1DS_Get_t;

/** Message ID for CMD_FrameData1US_Get */
#define CMD_FRAMEDATA1US_GET 0x1F03

/**
   Requests basic VDSL1 frame setting data for the upstream direction.
*/
typedef struct CMD_FrameData1US_Get CMD_FrameData1US_Get_t;

/** Message ID for ACK_FrameData1US_Get */
#define ACK_FRAMEDATA1US_GET 0x1F03

/**
   Delivers basic VDSL1 frame setting data for the upstream direction requested by
   CMD_FrameData1US_Get. These parameters are covered in sections 8.5 & 8.4 of G.993.1
   [2].
*/
typedef struct ACK_FrameData1US_Get ACK_FrameData1US_Get_t;

/** Message ID for CMD_FrameDataExt1DS_Get */
#define CMD_FRAMEDATAEXT1DS_GET 0x2003

/**
   Requests an extended set of VDSL1 framing parameters for the downstream direction.
*/
typedef struct CMD_FrameDataExt1DS_Get CMD_FrameDataExt1DS_Get_t;

/** Message ID for ACK_FrameDataExt1DS_Get */
#define ACK_FRAMEDATAEXT1DS_GET 0x2003

/**
   Delivers an extended set of VDSL1 framing parameters for the downstream direction
   requested by CMD_FrameDataExt1DS_Get. (Sections 8.5 & 8.4 of G.993.1 [2])
*/
typedef struct ACK_FrameDataExt1DS_Get ACK_FrameDataExt1DS_Get_t;

/** Message ID for CMD_FrameDataExt1US_Get */
#define CMD_FRAMEDATAEXT1US_GET 0x2103

/**
   Requests an extended set of VDSL1 framing parameters for the upstream direction.
*/
typedef struct CMD_FrameDataExt1US_Get CMD_FrameDataExt1US_Get_t;

/** Message ID for ACK_FrameDataExt1US_Get */
#define ACK_FRAMEDATAEXT1US_GET 0x2103

/**
   Delivers an extended set of VDSL1 framing parameters for the upstream direction
   requested by CMD_FrameDataExt1US_Get. (Sections 8.5 & 8.4 of G.993.1 [2])
*/
typedef struct ACK_FrameDataExt1US_Get ACK_FrameDataExt1US_Get_t;

/** Message ID for CMD_FrameDataExt2DS_Get */
#define CMD_FRAMEDATAEXT2DS_GET 0x2203

/**
   Requests an extended set of VDSL2 framing parameters for the downstream direction.
*/
typedef struct CMD_FrameDataExt2DS_Get CMD_FrameDataExt2DS_Get_t;

/** Message ID for ACK_FrameDataExt2DS_Get */
#define ACK_FRAMEDATAEXT2DS_GET 0x2203

/**
   Delivers an extended set of VDSL2 framing parameters for the downstream direction
   requested by CMD_FrameDataExt2DS_Get. (Sections 7.5.2.6 of G.997.1 and 9.5 & 9.6 of
   G.993.2 [3])
*/
typedef struct ACK_FrameDataExt2DS_Get ACK_FrameDataExt2DS_Get_t;

/** Message ID for CMD_FrameDataExt2US_Get */
#define CMD_FRAMEDATAEXT2US_GET 0x2303

/**
   Requests an extended set of VDSL2 framing parameters for the upstream direction.
*/
typedef struct CMD_FrameDataExt2US_Get CMD_FrameDataExt2US_Get_t;

/** Message ID for ACK_FrameDataExt2US_Get */
#define ACK_FRAMEDATAEXT2US_GET 0x2303

/**
   Delivers an extended set of VDSL2 framing parameters for the upstream direction
   requested by CMD_FrameDataExt2US_Get. (Sections 7.5.2.6 of G.997.1 and 9.5 & 9.6 of
   G.993.2 [3])
*/
typedef struct ACK_FrameDataExt2US_Get ACK_FrameDataExt2US_Get_t;

/** Message ID for CMD_ADSL_FrameDataDS_Get */
#define CMD_ADSL_FRAMEDATADS_GET 0x010E

/**
   Requests ADSL framing parameters for the downstream direction.
*/
typedef struct CMD_ADSL_FrameDataDS_Get CMD_ADSL_FrameDataDS_Get_t;

/** Message ID for ACK_ADSL_FrameDataDS_Get */
#define ACK_ADSL_FRAMEDATADS_GET 0x010E

/**
   Delivers the ADSL framing parameters for the downstream direction requested by
   CMD_ADSL_FrameDataDS_Get. (Sections 7.5 & 7.6 of G.992.3 [1])
*/
typedef struct ACK_ADSL_FrameDataDS_Get ACK_ADSL_FrameDataDS_Get_t;

/** Message ID for CMD_ADSL_FrameDataUS_Get */
#define CMD_ADSL_FRAMEDATAUS_GET 0x020E

/**
   Requests ADSL framing parameters for the upstream direction.
*/
typedef struct CMD_ADSL_FrameDataUS_Get CMD_ADSL_FrameDataUS_Get_t;

/** Message ID for ACK_ADSL_FrameDataUS_Get */
#define ACK_ADSL_FRAMEDATAUS_GET 0x020E

/**
   Delivers the ADSL framing parameters for the upstream direction requested by
   CMD_ADSL_FrameDataDS_Get. (Sections 7.5 & 7.6 of G.992.3 [1])
*/
typedef struct ACK_ADSL_FrameDataUS_Get ACK_ADSL_FrameDataUS_Get_t;

/** Message ID for CMD_TR1ControlSet */
#define CMD_TR1CONTROLSET 0x004A

/**
   Controls the behaviour of the performance counters.
*/
typedef struct CMD_TR1ControlSet CMD_TR1ControlSet_t;

/** Message ID for ACK_TR1ControlSet */
#define ACK_TR1CONTROLSET 0x004A

/**
   Acknowledgement for message CMD_TR1ControlSet.
*/
typedef struct ACK_TR1ControlSet ACK_TR1ControlSet_t;

/** Message ID for EVT_TR1Expiry */
#define EVT_TR1EXPIRY 0x8002

/**
   Autonomous message indicating the expiry of a TR1 period.
*/
typedef struct EVT_TR1Expiry EVT_TR1Expiry_t;

/** Message ID for CMD_G997_PMD_StatsThreshSet */
#define CMD_G997_PMD_STATSTHRESHSET 0x014A

/**
   The message configures the thresholds for the "current period" PMD counters. If any
   of the configured thresholds is met within a 15-min TR1 period, an autonomous
   message ALM_TR1CounterThreshHit is sent to the host, provided the ALM generation was
   enabled for it with CMD_TR1ThreshAlarmConfigure.
   (See section 7.3.1.8 of G997.1 [7])
*/
typedef struct CMD_G997_PMD_StatsThreshSet CMD_G997_PMD_StatsThreshSet_t;

/** Message ID for ACK_G997_PMD_StatsThreshSet */
#define ACK_G997_PMD_STATSTHRESHSET 0x014A

/**
   Acknowledgement for message CMD_G997_PMD_StatsThreshSet.
*/
typedef struct ACK_G997_PMD_StatsThreshSet ACK_G997_PMD_StatsThreshSet_t;

/** Message ID for CMD_G997_PMD_StatsThreshGet */
#define CMD_G997_PMD_STATSTHRESHGET 0x010A

/**
   Requests the configured threshold values for the PMD counters.
*/
typedef struct CMD_G997_PMD_StatsThreshGet CMD_G997_PMD_StatsThreshGet_t;

/** Message ID for ACK_G997_PMD_StatsThreshGet */
#define ACK_G997_PMD_STATSTHRESHGET 0x010A

/**
   The message reports the thresholds configured for the "current period" PMD counters.
   (see also )
*/
typedef struct ACK_G997_PMD_StatsThreshGet ACK_G997_PMD_StatsThreshGet_t;

/** Message ID for CMD_TR1ThreshAlarmConfigure */
#define CMD_TR1THRESHALARMCONFIGURE 0x0749

/**
   Enables/Disables the generation of ALARM messages (ALM) for specific performance
   counters with TR1 threshold supervision. If the corresponding Enable bit for a
   counter is set, then the modem firmware will send an autonomous message
   ALM_TR1CounterThreshHitif the "current period" counter meets its configured
   threshold.
*/
typedef struct CMD_TR1ThreshAlarmConfigure CMD_TR1ThreshAlarmConfigure_t;

/** Message ID for ACK_TR1ThreshAlarmConfigure */
#define ACK_TR1THRESHALARMCONFIGURE 0x0749

/**
   Acknowledgement to CMD_TR1ThreshAlarmConfigure.
*/
typedef struct ACK_TR1ThreshAlarmConfigure ACK_TR1ThreshAlarmConfigure_t;

/** Message ID for ALM_TR1CounterThreshHit */
#define ALM_TR1COUNTERTHRESHHIT 0x0207

/**
   Autonomous message that reports when TR1 thresholds are reached. It is sent if at
   least one of the "current TR1 period" performance counters with enabled threshold
   supervision met its configured threshold.
*/
typedef struct ALM_TR1CounterThreshHit ALM_TR1CounterThreshHit_t;

/** Message ID for CMD_TR1CounterThreshHit */
#define CMD_TR1COUNTERTHRESHHIT 0x0207

/**
   Requests the threshold exceedance status of the TR1 performance counters.
*/
typedef struct CMD_TR1CounterThreshHit CMD_TR1CounterThreshHit_t;

/** Message ID for ACK_TR1CounterThreshHit */
#define ACK_TR1COUNTERTHRESHHIT 0x0207

/**
   Reports which TR1 performance counters reached or exceeded their individually
   configurable thresholds.
*/
typedef struct ACK_TR1CounterThreshHit ACK_TR1CounterThreshHit_t;

/** Message ID for ALM_LineFailureNE_Get */
#define ALM_LINEFAILURENE_GET 0x0007

/**
   This autonomous message indicates near-end line failures.
*/
typedef struct ALM_LineFailureNE_Get ALM_LineFailureNE_Get_t;

/** Message ID for CMD_LineFailureNE_Get */
#define CMD_LINEFAILURENE_GET 0x0007

/**
   Requests the near-end line failure status.
*/
typedef struct CMD_LineFailureNE_Get CMD_LineFailureNE_Get_t;

/** Message ID for ACK_LineFailureNE_Get */
#define ACK_LINEFAILURENE_GET 0x0007

/**
   Provides the near-end line failure status requested by the command
   CMD_LineFailureNE_Get.
*/
typedef struct ACK_LineFailureNE_Get ACK_LineFailureNE_Get_t;

/** Message ID for ALM_LineFailureFE_Get */
#define ALM_LINEFAILUREFE_GET 0x0107

/**
   This autonomous message indicates far-end line failures.
*/
typedef struct ALM_LineFailureFE_Get ALM_LineFailureFE_Get_t;

/** Message ID for CMD_LineFailureFE_Get */
#define CMD_LINEFAILUREFE_GET 0x0107

/**
   Requests the far-end line failure status.
*/
typedef struct CMD_LineFailureFE_Get CMD_LineFailureFE_Get_t;

/** Message ID for ACK_LineFailureFE_Get */
#define ACK_LINEFAILUREFE_GET 0x0107

/**
   Provides the far-end line failure status requested by the command
   CMD_LineFailureFE_Get.
*/
typedef struct ACK_LineFailureFE_Get ACK_LineFailureFE_Get_t;

/** Message ID for CMD_LinePerfCountNE_Get */
#define CMD_LINEPERFCOUNTNE_GET 0x030A

/**
   Requests near-end line performance counter values (Section 7.2.1.1 of G.997.1) for
   the current and for the previous 15-min-TR1 period.
*/
typedef struct CMD_LinePerfCountNE_Get CMD_LinePerfCountNE_Get_t;

/** Message ID for ACK_LinePerfCountNE_Get */
#define ACK_LINEPERFCOUNTNE_GET 0x030A

/**
   Provides near-end line performance counter values requested by
   CMD_LinePerfCountNE_Get (Section 7.2.1.1 of G.997.1). Contains counter values for
   the current and for the previous 15-min-TR1 interval.
*/
typedef struct ACK_LinePerfCountNE_Get ACK_LinePerfCountNE_Get_t;

/** Message ID for CMD_LinePerfCountNE_Set */
#define CMD_LINEPERFCOUNTNE_SET 0x034A

/**
   Writes near-end line performance counter values (Section 7.2.1.1 of G.997.1) 
   for the current and for the previous 15-min-TR1 period.
*/
typedef struct CMD_LinePerfCountNE_Set CMD_LinePerfCountNE_Set_t;

/** Message ID for ACK_LinePerfCountNE_Set */
#define ACK_LINEPERFCOUNTNE_SET 0x034A

/**
   Provides near-end line performance counter values requested by 
   CMD_LinePerfCountNE_Set (Section 7.2.1.1 of G.997.1). 
   Contains counter values for the current and for the previous 15-min-TR1 interval.
*/
typedef struct ACK_LinePerfCountNE_Set ACK_LinePerfCountNE_Set_t;

/** Message ID for CMD_LinePerfCountFE_Get */
#define CMD_LINEPERFCOUNTFE_GET 0x050A

/**
   Requests far-end line performance counter values (Section 7.2.1.2 of G.997.1) for
   the current and for the previous 15-min-TR1 period.
*/
typedef struct CMD_LinePerfCountFE_Get CMD_LinePerfCountFE_Get_t;

/** Message ID for ACK_LinePerfCountFE_Get */
#define ACK_LINEPERFCOUNTFE_GET 0x050A

/**
   Provides far-end line performance counter values requested by
   CMD_LinePerfCountFE_Get (Section 7.2.1.2 of G.997.1). Contains counter values for
   the current and for the previous 15-min-TR1 interval.
*/
typedef struct ACK_LinePerfCountFE_Get ACK_LinePerfCountFE_Get_t;

/** Message ID for CMD_LinePerfCountFE_Set */
#define CMD_LINEPERFCOUNTFE_SET 0x054A

/**
   Writes far-end line performance counter values (Section 7.2.1.2 of G.997.1)
   for the current and for the previous 15-min-TR1 period.
*/
typedef struct CMD_LinePerfCountFE_Set CMD_LinePerfCountFE_Set_t;

/** Message ID for ACK_LinePerfCountFE_Set */
#define ACK_LINEPERFCOUNTFE_SET 0x054A

/**
   Acknowledgement for the message CMD_LinePerfCountFE_Set.
*/
typedef struct ACK_LinePerfCountFE_Set ACK_LinePerfCountFE_Set_t;

/** Message ID for CMD_LinePerfCountNE_NoTR1Get*/
#define CMD_LINEPERFCOUNTNE_NOTR1_GET 0x020A

/**
   Requests near-end line performance counter values (Section 7.2.1.1 of G.997.1). 
   These are wrap-around counters which are not affected by the TR1-period.
   Only supported in the ADSL mode
*/
typedef struct CMD_LinePerfCountNE_NoTR1Get CMD_LinePerfCountNE_NoTR1Get_t;

/** Message ID for ACK_LinePerfCountNE_NoTR1Get*/
#define ACK_LINEPERFCOUNTNE_NOTR1_GET 0x020A

/**
   Provides near-end line performance counter values requested by 
   CMD_LinePerfCountNE_NoTR1Get (Section 7.2.1.1 of G.997.1). 
   These are wrap-around counters which are not affected by the TR1-period.
*/
typedef struct ACK_LinePerfCountNE_NoTR1Get ACK_LinePerfCountNE_NoTR1Get_t;

/** Message ID for CMD_LinePerfCountNE_NoTR1Set */
#define CMD_LINEPERFCOUNTNE_NOTR1SET 0x024A

/**
   Writes near-end line performance counter values (Section 7.2.1.1 of G.997.1). These
   are wrap-around counters which are not affected by the TR1-period.
*/
typedef struct CMD_LinePerfCountNE_NoTR1Set CMD_LinePerfCountNE_NoTR1Set_t;

/** Message ID for ACK_LinePerfCountNE_NoTR1Set */
#define ACK_LINEPERFCOUNTNE_NOTR1SET 0x024A

/**
   Acknowledgement for messages CMD_LinePerfCountNE_NoTR1Set.
*/
typedef struct ACK_LinePerfCountNE_NoTR1Set ACK_LinePerfCountNE_NoTR1Set_t;

/** Message ID for CMD_CRC_StatsNE_NoTR1Set */
#define CMD_CRC_STATSNE_NOTR1SET 0x084A

/**
   Writes near-end CRC counter values. (Section 7.2.2.1.1 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.
*/
typedef struct CMD_CRC_StatsNE_NoTR1Set CMD_CRC_StatsNE_NoTR1Set_t;

/** Message ID for ACK_CRC_StatsNE_NoTR1Set */
#define ACK_CRC_STATSNE_NOTR1SET 0x084A

/**
   Acknowledgement for messages CMD_CRC_StatsNE_NoTR1Set.
*/
typedef struct ACK_CRC_StatsNE_NoTR1Set ACK_CRC_StatsNE_NoTR1Set_t;

/** Message ID for CMD_FEC_StatsNE_NoTR1Set */
#define CMD_FEC_STATSNE_NOTR1SET 0x0A4A

/**
   Writes near-end FEC counter values (Section 7.2.2.1.2 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.
*/
typedef struct CMD_FEC_StatsNE_NoTR1Set CMD_FEC_StatsNE_NoTR1Set_t;

/** Message ID for ACK_FEC_StatsNE_NoTR1Set */
#define ACK_FEC_STATSNE_NOTR1SET 0x0A4A

/**
   Acknowledgement for messages CMD_FEC_StatsNE_NoTR1Set.
*/
typedef struct ACK_FEC_StatsNE_NoTR1Set ACK_FEC_StatsNE_NoTR1Set_t;


/** Message ID for CMD_LineInitCountGet */
#define CMD_LINEINITCOUNTGET 0x070A

/**
   Requests line initialization performance counter values (Section 7.2.1.3 of G.997.1)
   for the current and previous 15-min-TR1 period.
*/
typedef struct CMD_LineInitCountGet CMD_LineInitCountGet_t;

/** Message ID for ACK_LineInitCountGet */
#define ACK_LINEINITCOUNTGET 0x070A

/**
   Provides line initialization performance counter values requested by
   CMD_LineInitCountGet (Section 7.2.1.3 of G.997.1). Contains counter values for the
   current and for the previous 15-min-TR1 interval. These counters are not reset at
   link re-initialization.
*/
typedef struct ACK_LineInitCountGet ACK_LineInitCountGet_t;

/** Message ID for CMD_LineInitCountSet */
#define CMD_LINEINITCOUNTSET 0x074A

/**
   Writes line initialization performance counter values (Section 7.2.1.3 of G.997.1)
   for the current and previous 15-min-TR1 period.
*/
typedef struct CMD_LineInitCountSet CMD_LineInitCountSet_t;

/** Message ID for ACK_LineInitCountSet */
#define ACK_LINEINITCOUNTSET 0x074A

/**
   Acknowledgement for the message CMD_LineInitCountSet.
*/
typedef struct ACK_LineInitCountSet ACK_LineInitCountSet_t;

/** Message ID for CMD_CRC_StatsNE_Get */
#define CMD_CRC_STATSNE_GET 0x090A

/**
   Requests near-end CRC counter values.
*/
typedef struct CMD_CRC_StatsNE_Get CMD_CRC_StatsNE_Get_t;

/** Message ID for ACK_CRC_StatsNE_Get */
#define ACK_CRC_STATSNE_GET 0x090A

/**
   Returns near-end CRC counter values. (Section 7.2.2.1.1 of G997.1)
*/
typedef struct ACK_CRC_StatsNE_Get ACK_CRC_StatsNE_Get_t;

/** Message ID for CMD_CRC_StatsNE_Set */
#define CMD_CRC_STATSNE_SET 0x094A

/**
   Writes the near-end CRC counter values (Section 7.2.2.1.1 of G997.1)
*/
typedef struct CMD_CRC_StatsNE_Set CMD_CRC_StatsNE_Set_t;

/** Message ID for ACK_CRC_StatsNE_Set */
#define ACK_CRC_STATSNE_SET 0x094A

/**
   Acknowledgement for the message CMD_CRC_StatsNE_Set.
*/
typedef struct ACK_CRC_StatsNE_Set ACK_CRC_StatsNE_Set_t;


/** Message ID for CMD_CRC_StatsNE_NoTR1Get*/
#define CMD_CRC_STATSNE_NOTR1_GET 0x080A

/**
   Requests near-end CRC counter values. These are wrap-around counters 
   which are not affected by the TR1-period.
*/
typedef struct CMD_CRC_StatsNE_NoTR1Get CMD_CRC_StatsNE_NoTR1Get_t;

/** Message ID for ACK_CRC_StatsNE_NoTR1Get*/
#define ACK_CRC_STATSNE_NOTR1_GET 0x080A

/**
   Returns near-end CRC counter values. (Section 7.2.2.1.1 of G997.1). 
   These are wrap-around counters which are not affected by the TR1-period.
*/
typedef struct ACK_CRC_StatsNE_NoTR1Get ACK_CRC_StatsNE_NoTR1Get_t;

/** Message ID for CMD_CRC_StatsFE_Get */
#define CMD_CRC_STATSFE_GET 0x0D0A

/**
   Requests far-end CRC counter values.
*/
typedef struct CMD_CRC_StatsFE_Get CMD_CRC_StatsFE_Get_t;

/** Message ID for ACK_CRC_StatsFE_Get */
#define ACK_CRC_STATSFE_GET 0x0D0A

/**
   Returns far-end CRC counter values. (Section 7.2.2.2.1 of G997.1)
*/
typedef struct ACK_CRC_StatsFE_Get ACK_CRC_StatsFE_Get_t;

/** Message ID for CMD_CRC_StatsFE_Set */
#define CMD_CRC_STATSFE_SET 0x0D4A

/**
   Writes the far-end CRC counter values (Section 7.2.2.2.1 of G997.1).
*/
typedef struct CMD_CRC_StatsFE_Set CMD_CRC_StatsFE_Set_t;

/** Message ID for ACK_CRC_StatsFE_Get */
#define ACK_CRC_STATSFE_SET 0x0D4A

/**
   Acknowledgement for the message CMD_CRC_StatsFE_Set.
*/
typedef struct ACK_CRC_StatsFE_Set ACK_CRC_StatsFE_Set_t;

/** Message ID for CMD_FEC_StatsNE_Get */
#define CMD_FEC_STATSNE_GET 0x0B0A

/**
   Requests near-end FEC counter values.
*/
typedef struct CMD_FEC_StatsNE_Get CMD_FEC_StatsNE_Get_t;

/** Message ID for ACK_FEC_StatsNE_Get */
#define ACK_FEC_STATSNE_GET 0x0B0A

/**
   Returns near-end FEC counter values. (Section 7.2.2.1.2 of G997.1)
*/
typedef struct ACK_FEC_StatsNE_Get ACK_FEC_StatsNE_Get_t;

/** Message ID for CMD_FEC_StatsNE_Set */
#define CMD_FEC_STATSNE_SET 0x0B4A

/**
   Writes near-end FEC counter values. (Section 7.2.2.1.2 of G997.1).
*/
typedef struct CMD_FEC_StatsNE_Set CMD_FEC_StatsNE_Set_t;

/** Message ID for ACK_FEC_StatsNE_Set */
#define ACK_FEC_STATSNE_SET 0x0B4A

/**
   Acknowledgement for the message CMD_FEC_StatsNE_Set.
*/
typedef struct ACK_FEC_StatsNE_Set ACK_FEC_StatsNE_Set_t;

/** Message ID for CMD_FEC_StatsNE_NoTR1Get*/
#define CMD_FEC_STATSNE_NOTR1_GET 0x0A0A

/**
   Requests near-end FEC counter values. These are wrap-around counters which 
   are not affected by the TR1-period.
*/
typedef struct CMD_FEC_StatsNE_NoTR1Get CMD_FEC_StatsNE_NoTR1Get_t;

/** Message ID for ACK_FEC_StatsNE_NoTR1Get*/
#define ACK_FEC_STATSNE_NOTR1_GET 0x0A0A

/**
   Returns near-end FEC counter values (Section 7.2.2.1.2 of G997.1). 
   These are wrap-around counters which are not affected by the TR1-period.
*/
typedef struct ACK_FEC_StatsNE_NoTR1Get ACK_FEC_StatsNE_NoTR1Get_t;

/** Message ID for CMD_FEC_StatsFE_Get */
#define CMD_FEC_STATSFE_GET 0x0F0A

/**
   Requests far-end FEC counter values.
*/
typedef struct CMD_FEC_StatsFE_Get CMD_FEC_StatsFE_Get_t;

/** Message ID for ACK_FEC_StatsFE_Get */
#define ACK_FEC_STATSFE_GET 0x0F0A

/**
   Returns the far-end FEC counter values requested by CMD_FEC_StatsFE_Get.(Section
   7.2.2.2.2 of G997.1)
*/
typedef struct ACK_FEC_StatsFE_Get ACK_FEC_StatsFE_Get_t;

/** Message ID for CMD_FEC_StatsFE_Set */
#define CMD_FEC_STATSFE_SET 0x0F4A

/**
   Writes far-end FEC counter values. (Section 7.2.2.2.2 of G997.1)
*/
typedef struct CMD_FEC_StatsFE_Set CMD_FEC_StatsFE_Set_t;

/** Message ID for ACK_FEC_StatsFE_Set */
#define ACK_FEC_STATSFE_SET 0x0F4A

/**
   Acknowledgement for the message CMD_FEC_StatsFE_Set.
*/
typedef struct ACK_FEC_StatsFE_Set ACK_FEC_StatsFE_Set_t;

/** Message ID for CMD_TimeControl */
#define CMD_TIMECONTROL 0x0E49

/**
   Controls the "Set-Time" and "Read-Time" EOC messages to either synchronize or read
   the time of the xTU-R.
   Note: Modifying the Time does not affect the TR1 supervision period and the
   TR1-counters.
*/
typedef struct CMD_TimeControl CMD_TimeControl_t;

/** Message ID for ACK_TimeControl */
#define ACK_TIMECONTROL 0x0E49

/**
   Acknowledgement for message CMD_TimeControl.
*/
typedef struct ACK_TimeControl ACK_TimeControl_t;

/** Message ID for CMD_TimeSet */
#define CMD_TIMESET 0x6143

/**
   Sets the time.
   To synchronize the time of the xTU-C and xTU-R, the xTU-C sets the far-end time
   parameter and then triggers the sending of the EOC time command to transfer the
   configured value to the opposite side using CMD_TimeControl.
   Note: Modifying the Time does not affect the TR1 supervision period and the
   TR1-counters.
*/
typedef struct CMD_TimeSet CMD_TimeSet_t;

/** Message ID for ACK_TimeSet */
#define ACK_TIMESET 0x6143

/**
   Acknowledgement for message CMD_TimeSet.
*/
typedef struct ACK_TimeSet ACK_TimeSet_t;

/** Message ID for CMD_TimeGet */
#define CMD_TIMEGET 0x6103

/**
   Requests the status of the "Set Time and Read Time" EOC messages.
*/
typedef struct CMD_TimeGet CMD_TimeGet_t;

/** Message ID for ACK_TimeGet */
#define ACK_TIMEGET 0x6103

/**
   Provides the status of the "Set Time and Read Time" EOC messages.
*/
typedef struct ACK_TimeGet ACK_TimeGet_t;

/** Message ID for CMD_ClearEOC_Configure */
#define CMD_CLEAREOC_CONFIGURE 0x0A49

/**
   This message is used to configure the autonomous messaging related to Clear EOC
   transmission.
*/
typedef struct CMD_ClearEOC_Configure CMD_ClearEOC_Configure_t;

/** Message ID for ACK_ClearEOC_Configure */
#define ACK_CLEAREOC_CONFIGURE 0x0A49

/**
   This is the acknowledgement for CMD_ClearEOC_Configure.
*/
typedef struct ACK_ClearEOC_Configure ACK_ClearEOC_Configure_t;

/** Message ID for CMD_ClearEOC_TxTrigger */
#define CMD_CLEAREOC_TXTRIGGER 0x0949

/**
   The message is used to trigger the transmission of  Clear EOC messages that were
   placed into the Clear EOC transmit buffer before with CMD_ClearEOC_Write.
*/
typedef struct CMD_ClearEOC_TxTrigger CMD_ClearEOC_TxTrigger_t;

/** Message ID for ACK_ClearEOC_TxTrigger */
#define ACK_CLEAREOC_TXTRIGGER 0x0949

/**
   This is the acknowledgement for CMD_ClearEOC_TxTrigger.
*/
typedef struct ACK_ClearEOC_TxTrigger ACK_ClearEOC_TxTrigger_t;

/** Message ID for CMD_ClearEOC_Write */
#define CMD_CLEAREOC_WRITE 0x5143

/**
   This message is used to write data to the ClearEOC write buffer of type
   VNX_ClearEOC_t. When the buffer is filled, the transmission is started applying
   CMD_ClearEOC_TxTrigger. If the message to transmit is longer than the mailbox size,
   a sequence of writes to the ClearEOC buffer has to be done before the transmission
   is started with CMD_ClearEOC_TxTrigger. When autonomous Tx status messaging is
   activated via CMD_ClearEOC_Configure, then the finished transmission is indicated by
   EVT_ClearEOCStatusGet.
*/
typedef struct CMD_ClearEOC_Write CMD_ClearEOC_Write_t;

/** Message ID for ACK_ClearEOC_Write */
#define ACK_CLEAREOC_WRITE 0x5143

/**
   This message is the acknowledgement for CMD_ClearEOC_Write.
*/
typedef struct ACK_ClearEOC_Write ACK_ClearEOC_Write_t;

/** Message ID for CMD_ClearEOC_Read */
#define CMD_CLEAREOC_READ 0x5203

/**
   This message is used to read data from the ClearEOC buffer of type VNX_ClearEOC_t.
   The length of the actual Clear EOC message can be found in the buffer. Please refer
   to VNX_ClearEOC_t. The availability of data can either be checked via
   CMD_ClearEOCStatusGet in polling mode or it can be reported by an autonomous
   EVT_ClearEOCStatusGet message when data is received (to be enabled using
   CMD_ClearEOC_Configure).
*/
typedef struct CMD_ClearEOC_Read CMD_ClearEOC_Read_t;

/** Message ID for ACK_ClearEOC_Read */
#define ACK_CLEAREOC_READ 0x5203

/**
   This message is the acknowledgement to CMD_ClearEOC_Read.
*/
typedef struct ACK_ClearEOC_Read ACK_ClearEOC_Read_t;

/** Message ID for EVT_ClearEOC_Read */
#define EVT_CLEAREOC_READ 0x5203

/**
   This message is an autonomous message that is generated when ClearEOC data was
   received and autonomous Clear EOC data messaging has been activated via
   CMD_ClearEOC_Configure. If the ClearEOC data does not fit in one message, then a
   sequence of messages is generated. The ClearEOC buffer is of type VNX_ClearEOC_t.
*/
typedef struct EVT_ClearEOC_Read EVT_ClearEOC_Read_t;

/** Message ID for CMD_ClearEOCStatusGet */
#define CMD_CLEAREOCSTATUSGET 0x0B09

/**
   This message is used to retrieve the status of the clear eoc data transmission.
*/
typedef struct CMD_ClearEOCStatusGet CMD_ClearEOCStatusGet_t;

/** Message ID for ACK_ClearEOCStatusGet */
#define ACK_CLEAREOCSTATUSGET 0x0B09

/**
   This is the acknowledgement for CMD_ClearEOCStatusGet.
*/
typedef struct ACK_ClearEOCStatusGet ACK_ClearEOCStatusGet_t;

/** Message ID for EVT_ClearEOCStatusGet */
#define EVT_CLEAREOCSTATUSGET 0x0B09

/**
   This autonomous message reports the Clear EOC status. It is sent only if the
   "Autonomous Status Message Control" was enabled for Tx and/or Rx direction with
   CMD_ClearEOC_Configure. If Tx direction is enabled, the message is generated when a
   Tx transmission is finished or failed. If Rx direction is enabled, the message is
   generated when the Rx status transitions from "Idle" to "Data Available" for
   retrieval by the host.
*/
typedef struct EVT_ClearEOCStatusGet EVT_ClearEOCStatusGet_t;

/** Message ID for CMD_BAT_TableEntriesGet */
#define CMD_BAT_TABLEENTRIESGET 0x1703

/**
   Requests information about the bit-allocation per subcarrier in VDSL mode. The
   values for all subcarriers of both directions can be retrieved.
   (For ADSL use CMD_ADSL_BAT_DS_Get and CMD_ADSL_BAT_US_Get).
*/
typedef struct CMD_BAT_TableEntriesGet CMD_BAT_TableEntriesGet_t;

/** Message ID for ACK_BAT_TableEntriesGet */
#define ACK_BAT_TABLEENTRIESGET 0x1703

/**
   Returns information about the bit-allocation per tone for the chosen range of
   subcarriers.(Section 7.5.1.29.1-2 of G997.1)
*/
typedef struct ACK_BAT_TableEntriesGet ACK_BAT_TableEntriesGet_t;

/** Message ID for CMD_GainTableEntriesGet */
#define CMD_GAINTABLEENTRIESGET 0x1903

/**
   Requests information about the Gains per subcarrier. The values for all subcarriers
   of both directions can be retrieved.
   (For ADSL use CMD_ADSL_GainTableDS_Get and CMD_ADSL_GainTableUS_Get).
*/
typedef struct CMD_GainTableEntriesGet CMD_GainTableEntriesGet_t;

/** Message ID for ACK_GainTableEntriesGet */
#define ACK_GAINTABLEENTRIESGET 0x1903

/**
   Returns information about the Gain-per-tone for the chosen range of
   subcarriers.(Section 7.5.1.29.3-4 of G997.1)
*/
typedef struct ACK_GainTableEntriesGet ACK_GainTableEntriesGet_t;

/** Message ID for CMD_SNR_NE_TableEntriesGet */
#define CMD_SNR_NE_TABLEENTRIESGET 0x0B03

/**
   The message requests information about the SNR per subcarrier available at the
   near-end side, which means for upstream direction at the CO side and for downstream
   direction at the CPE. It is the hosts responsibility to select the tone indices
   accordingly.
*/
typedef struct CMD_SNR_NE_TableEntriesGet CMD_SNR_NE_TableEntriesGet_t;

/** Message ID for ACK_SNR_NE_TableEntriesGet */
#define ACK_SNR_NE_TABLEENTRIESGET 0x0B03

/**
   Returns information about the SNR per subcarrier available at the near-end side,
   which means for upstream direction at the CO side and for downstream direction at
   the CPE.
*/
typedef struct ACK_SNR_NE_TableEntriesGet ACK_SNR_NE_TableEntriesGet_t;

/** Message ID for CMD_ADSL_BAT_DS_Get */
#define CMD_ADSL_BAT_DS_GET 0x070E

/**
   Requests information about the downstream bit-allocation per subcarrier in ADSL
   mode.
*/
typedef struct CMD_ADSL_BAT_DS_Get CMD_ADSL_BAT_DS_Get_t;

/** Message ID for ACK_ADSL_BAT_DS_Get */
#define ACK_ADSL_BAT_DS_GET 0x070E

/**
   Returns information about the downstream bit-allocation per tone for the chosen
   range of subcarriers.(Section 7.5.1.29.1 of G997.1)
*/
typedef struct ACK_ADSL_BAT_DS_Get ACK_ADSL_BAT_DS_Get_t;

/** Message ID for CMD_ADSL_BAT_US_Get */
#define CMD_ADSL_BAT_US_GET 0x060E

/**
   Requests information about the upstream bit-allocation per subcarrier in ADSL mode.
*/
typedef struct CMD_ADSL_BAT_US_Get CMD_ADSL_BAT_US_Get_t;

/** Message ID for ACK_ADSL_BAT_US_Get */
#define ACK_ADSL_BAT_US_GET 0x060E

/**
   Returns information about the upstream bit-allocation per tone for the chosen range
   of subcarriers.(Section 7.5.1.29.2 of G997.1)
*/
typedef struct ACK_ADSL_BAT_US_Get ACK_ADSL_BAT_US_Get_t;

/** Message ID for CMD_ADSL_GainTableDS_Get */
#define CMD_ADSL_GAINTABLEDS_GET 0x090E

/**
   Requests information about the Gains per subcarrier for the downstream direction.
*/
typedef struct CMD_ADSL_GainTableDS_Get CMD_ADSL_GainTableDS_Get_t;

/** Message ID for ACK_ADSL_GainTableDS_Get */
#define ACK_ADSL_GAINTABLEDS_GET 0x090E

/**
   Returns information about the Gain-per-tone for the chosen range of subcarriers for
   the downstream direction.(Section 7.5.1.29.3 of G997.1) as requested by
   CMD_ADSL_GainTableDS_Get.
*/
typedef struct ACK_ADSL_GainTableDS_Get ACK_ADSL_GainTableDS_Get_t;

/** Message ID for CMD_ADSL_GainTableUS_Get */
#define CMD_ADSL_GAINTABLEUS_GET 0x080E

/**
   Requests information about the Gains per subcarrier for the upstream direction.
*/
typedef struct CMD_ADSL_GainTableUS_Get CMD_ADSL_GainTableUS_Get_t;

/** Message ID for ACK_ADSL_GainTableUS_Get */
#define ACK_ADSL_GAINTABLEUS_GET 0x080E

/**
   Returns information about the Gain-per-tone for the chosen range of subcarriers for
   the upstream direction.(Section 7.5.1.29.4 of G997.1)
*/
typedef struct ACK_ADSL_GainTableUS_Get ACK_ADSL_GainTableUS_Get_t;

/** Message ID for CMD_ADSL_ExMarginReductionGet */
#define CMD_ADSL_EXMARGINREDUCTIONGET 0x2F03

/**
   Requests information about the Excess Margin Reduction needed for the fine gain
   calculation at the ATU_R.(See also ACK_ADSL_GainTableDS_Get).
*/
typedef struct CMD_ADSL_ExMarginReductionGet CMD_ADSL_ExMarginReductionGet_t;

/** Message ID for ACK_ADSL_ExMarginReductionGet */
#define ACK_ADSL_EXMARGINREDUCTIONGET 0x2F03

/**
   Returns information about the Excess Margin Reduction for the downstream direction.
*/
typedef struct ACK_ADSL_ExMarginReductionGet ACK_ADSL_ExMarginReductionGet_t;

/** Message ID for CMD_HlogDS_Get */
#define CMD_HLOGDS_GET 0x4A03

/**
   Requests information about the downstream HLOG information per subcarrier group
   (Section 7.5.1.26.6 of G997.1).
   During STEADY_STATE, the command can be used to request near-end data only (CPE).
   For the far-end HLOG to be provided via the EOC channel CMD_TestParamsFE_Request
   must be used.
   During loop diagnostic mode, the command shall be used to request both near-end and
   far-end data.
*/
typedef struct CMD_HlogDS_Get CMD_HlogDS_Get_t;

/** Message ID for ACK_HlogDS_Get */
#define ACK_HLOGDS_GET 0x4A03

/**
   Returns information about the downstream HLOG per subcarrier group for the chosen
   range. (Section 7.5.1.26.6 of G997.1)
*/
typedef struct ACK_HlogDS_Get ACK_HlogDS_Get_t;

/** Message ID for CMD_HlogUS_Get */
#define CMD_HLOGUS_GET 0x4B03

/**
   Requests information about the upstream HLOG informationper subcarrier group
   (Section 7.5.1.26.11 of G997.1).
   During STEADY_STATE, the command can be used to request near-end data only (CO). For
   the far-end HLOG to be provided via the EOC channel CMD_TestParamsFE_Request must be
   used.
   During loop diagnostic mode, the command is used to request both near-end and
   far-end data.
*/
typedef struct CMD_HlogUS_Get CMD_HlogUS_Get_t;

/** Message ID for ACK_HlogUS_Get */
#define ACK_HLOGUS_GET 0x4B03

/**
   Returns information about the upstream HLOG per subcarrier group for the chosen
   range. (Section 7.5.1.26.11 of G997.1)
*/
typedef struct ACK_HlogUS_Get ACK_HlogUS_Get_t;

/** Message ID for CMD_HlinDS_Get */
#define CMD_HLINDS_GET 0x4803

/**
   Requests information about the downstream HLIN information per subcarrier group.
   (Section 7.5.1.26.3 of G997.1).
   The HLIN data are available during loop diagnostic mode only.
*/
typedef struct CMD_HlinDS_Get CMD_HlinDS_Get_t;

/** Message ID for ACK_HlinDS_Get */
#define ACK_HLINDS_GET 0x4803

/**
   Returns information about the downstream HLIN per subcarrier group for the chosen
   range. (Section 7.5.1.26.3 of G997.1)
*/
typedef struct ACK_HlinDS_Get ACK_HlinDS_Get_t;

/** Message ID for CMD_HlinUS_Get */
#define CMD_HLINUS_GET 0x4903

/**
   Requests information about the upstream HLIN information per subcarrier group.
   (Section 7.5.1.26.9 of G997.1).
   The HLIN data are available in loop diagnostic mode only.
*/
typedef struct CMD_HlinUS_Get CMD_HlinUS_Get_t;

/** Message ID for ACK_HlinUS_Get */
#define ACK_HLINUS_GET 0x4903

/**
   Returns information about the upstream HLIN per subcarrier group for the chosen
   range. (Section 7.5.1.26.9 of G997.1)
*/
typedef struct ACK_HlinUS_Get ACK_HlinUS_Get_t;

/** Message ID for CMD_QLN_DS_Get */
#define CMD_QLN_DS_GET 0x4C03

/**
   Requests information about the downstream QLN information (QLNpsds) per subcarrier
   group (Section 7.5.1.27.3 of G997.1).
   During STEADY_STATE, the command can be used to request near-end data only (CPE).
   For the far-end QLN to be provided via the EOC channel the message
   CMD_TestParamsFE_Request must be used.
   During loop diagnostic mode, the command is used to request both near-end and
   far-end data.
*/
typedef struct CMD_QLN_DS_Get CMD_QLN_DS_Get_t;

/** Message ID for ACK_QLN_DS_Get */
#define ACK_QLN_DS_GET 0x4C03

/**
   Returns information about the QLN per subcarrier group for the chosen range.
   (Section 7.5.1.27.3 of G997.1)
*/
typedef struct ACK_QLN_DS_Get ACK_QLN_DS_Get_t;

/** Message ID for CMD_QLN_US_Get */
#define CMD_QLN_US_GET 0x4D03

/**
   Requests information about the upstream QLN information upstream (QLNpsus) per
   subcarrier group. (Section 7.5.1.27.6 of G997.1).
   During STEADY_STATE, the command can be used to request near-end data only (CO). For
   the far-end QLN to be provided via the EOC channel the message
   CMD_TestParamsFE_Request must be used.
   During loop diagnostic mode, the command can be used to request both near-end and
   far-end data.
*/
typedef struct CMD_QLN_US_Get CMD_QLN_US_Get_t;

/** Message ID for ACK_QLN_US_Get */
#define ACK_QLN_US_GET 0x4D03

/**
   Returns information about the QLN per subcarrier group for the chosen range.
   (Section 7.5.1.27.6 of G997.1)
*/
typedef struct ACK_QLN_US_Get ACK_QLN_US_Get_t;

/** Message ID for CMD_SNR_DS_Get */
#define CMD_SNR_DS_GET 0x5503

/**
   Requests information about the downstream SNR per subcarrier group (Section
   7.5.1.28.3 of G997.1)
   The message is for loop diagnostic mode only to retrieve NE and FE data! During
   STEADY_STATE use CMD_SNR_NE_TableEntriesGet and CMD_TestParamsFE_Request instead.
*/
typedef struct CMD_SNR_DS_Get CMD_SNR_DS_Get_t;

/** Message ID for ACK_SNR_DS_Get */
#define ACK_SNR_DS_GET 0x5503

/**
   Returns information about the SNR per subcarrier group for the chosen range.
   (Section 7.5.1.28.3 of G997.1)
*/
typedef struct ACK_SNR_DS_Get ACK_SNR_DS_Get_t;

/** Message ID for CMD_SNR_US_Get */
#define CMD_SNR_US_GET 0x4E03

/**
   Requests information about the upstream SNR per subcarrier group (Section 7.5.1.28.6
   of G997.1).
   The message is for loop diagnostic mode only to retrieve NE or FE data! During
   STEADY_STATE use CMD_SNR_NE_TableEntriesGet and CMD_TestParamsFE_Request instead.
*/
typedef struct CMD_SNR_US_Get CMD_SNR_US_Get_t;

/** Message ID for ACK_SNR_US_Get */
#define ACK_SNR_US_GET 0x4E03

/**
   Returns information about the upstream SNR per subcarrier group for the chosen
   range. (Section 7.5.1.28.6 of G997.1)
*/
typedef struct ACK_SNR_US_Get ACK_SNR_US_Get_t;

/** Message ID for CMD_TestParamsAuxDS_Get */
#define CMD_TESTPARAMSAUXDS_GET 0x4F03

/**
   Requests test parameter related information for the downstream direction: The HLIN
   scaling factor (HLINSCds), the subcarrier group size "G" and the measurement times
   for HLOGpsds, QLNpsds, SNRpsds.
   (Sections 7.5.1.26.1/2/4/5, 7.5.1.27.1/2 and 7.5.1.28.1/2 of G997.1)
   During STEADY_STATE, the command can be used to request near-end data only (CPE).
   For the far-end data to be provided via the EOC channel the message
   CMD_TestParamsFE_Request must be applied.
   During loop diagnostic mode, the command is used to request both near-end and
   far-end data.
*/
typedef struct CMD_TestParamsAuxDS_Get CMD_TestParamsAuxDS_Get_t;

/** Message ID for ACK_TestParamsAuxDS_Get */
#define ACK_TESTPARAMSAUXDS_GET 0x4F03

/**
   Provides the test-parameter related information as requested by
   CMD_TestParamsAuxDS_Get: The HLIN scaling factor (HLINSCds), the subcarrier group
   size "G" and the measurement times for HLOGpsds, QLNpsds, SNRpsds.
   (Sections 7.5.1.26.1/2/4/5, 7.5.1.27.1/2 and 7.5.1.28.1/2 of G997.1)
*/
typedef struct ACK_TestParamsAuxDS_Get ACK_TestParamsAuxDS_Get_t;

/** Message ID for CMD_TestParamsAuxUS_Get */
#define CMD_TESTPARAMSAUXUS_GET 0x5003

/**
   Requests test parameter related information for the upstream direction: The HLIN
   scaling factor (HLINSCus), the subcarrier group size "G" and the measurement times
   for HLOGpsus, QLNpsus, SNRpsus.
   (Sections 7.5.1.26.7/8/10/11, 7.5.1.27.4/5 and 7.5.1.28.4/5 of G997.1)
   During STEADY_STATE, the command can be used to request near-end data only (CO). For
   the far-end data to be provided via the EOC channel the message
   CMD_TestParamsFE_Request must be applied.
   During loop diagnostic mode, the command can be used to request both near-end and
   far-end data.
*/
typedef struct CMD_TestParamsAuxUS_Get CMD_TestParamsAuxUS_Get_t;

/** Message ID for ACK_TestParamsAuxUS_Get */
#define ACK_TESTPARAMSAUXUS_GET 0x5003

/**
   Provides the test-parameter related information as requested by
   CMD_TestParamsAuxUS_Get: The HLIN scaling factor (HLINSCus), the subcarrier group
   size "G" and the measurement times for HLOGpsus, QLNpsus, SNRpsus.
*/
typedef struct ACK_TestParamsAuxUS_Get ACK_TestParamsAuxUS_Get_t;

/** Message ID for CMD_TestParamsFE_Request */
#define CMD_TESTPARAMSFE_REQUEST 0x0849

/**
   This message is used to trigger retrieval of the far-end PMD Test Parameters Hlog,
   SNR and QLN via the EOC channel during Showtime. If StartIndex or EndIndex do not
   contain values in a valid range then no autonomous message will be sent by the
   firmware. Once the retrieval process is started, it can also be aborted with this
   message via the control parameter. The status of the parameter retrieval process can
   be polled via the CMD_TestParamsFE_Poll message. Once the parameters are retrieved,
   they are provided with the EVT_PMD_TestParamsGet message.
*/
typedef struct CMD_TestParamsFE_Request CMD_TestParamsFE_Request_t;

/** Message ID for ACK_TestParamsFE_Request */
#define ACK_TESTPARAMSFE_REQUEST 0x0849

/**
   This is the acknowledgement for CMD_TestParamsFE_Request.
*/
typedef struct ACK_TestParamsFE_Request ACK_TestParamsFE_Request_t;

/** Message ID for CMD_TestParamsFE_Poll */
#define CMD_TESTPARAMSFE_POLL 0x0809

/**
   This message polls the status of the far end parameter retrieval process.
*/
typedef struct CMD_TestParamsFE_Poll CMD_TestParamsFE_Poll_t;

/** Message ID for ACK_TestParamsFE_Poll */
#define ACK_TESTPARAMSFE_POLL 0x0809

/**
   This is the acknowledgement for CMD_TestParamsFE_Poll.
*/
typedef struct ACK_TestParamsFE_Poll ACK_TestParamsFE_Poll_t;

/** Message ID for EVT_PMD_TestParamsGet */
#define EVT_PMD_TESTPARAMSGET 0x5803

/**
   This event message provides the far-end data after the far end data retrieval
   process. This message is a result of the CMD_TestParamsFE_Request message.
*/
typedef struct EVT_PMD_TestParamsGet EVT_PMD_TestParamsGet_t;

/** Message ID for CMD_OLR_RxStatusGet */
#define CMD_OLR_RXSTATUSGET 0x5F03

/**
   Requests the OLR status information on bit swaps, DRR and SRA requests for the
   receive direction.
*/
typedef struct CMD_OLR_RxStatusGet CMD_OLR_RxStatusGet_t;

/** Message ID for ACK_OLR_RxStatusGet */
#define ACK_OLR_RXSTATUSGET 0x5F03

/**
   Reports the OLR status information for the receive direction as requestey by
   CMD_OLR_RxStatusGet.
*/
typedef struct ACK_OLR_RxStatusGet ACK_OLR_RxStatusGet_t;

/** Message ID for CMD_OLR_TxStatusGet */
#define CMD_OLR_TXSTATUSGET 0x6003

/**
   Requests the OLR status information on bit swaps, DRR and SRA requests for the
   transmit direction.
*/
typedef struct CMD_OLR_TxStatusGet CMD_OLR_TxStatusGet_t;

/** Message ID for ACK_OLR_TxStatusGet */
#define ACK_OLR_TXSTATUSGET 0x6003

/**
   Reports the OLR status information for the transmit direction as requestey by
   CMD_OLR_TxStatusGet.
*/
typedef struct ACK_OLR_TxStatusGet ACK_OLR_TxStatusGet_t;

/** Message ID for CMD_CRC_ErrorRequest */
#define CMD_CRC_ERRORREQUEST 0x0C49

/**
   Triggers the insertion of corrupt CRC’s into the at either the VTU-R or VTU-C. The
   message controls the EOC "Diagnostic" command to start and stop the corrupt CRC
   transmission. (Section 11.2.3.2 of G993.2 [3]). The EOC "Diagnostic" command is sent
   by the CO only for triggering corrupt CRC’s.
*/
typedef struct CMD_CRC_ErrorRequest CMD_CRC_ErrorRequest_t;

/** Message ID for ACK_CRC_ErrorRequest */
#define ACK_CRC_ERRORREQUEST 0x0C49

/**
   This is the acknowledgement for CMD_CRC_ErrorRequest.
*/
typedef struct ACK_CRC_ErrorRequest ACK_CRC_ErrorRequest_t;

/** Message ID for CMD_CRC_ErrorStatusGet */
#define CMD_CRC_ERRORSTATUSGET 0x0D09

/**
   Requests the status of corrupt CRC insertion, initiated by CMD_CRC_ErrorRequest.
*/
typedef struct CMD_CRC_ErrorStatusGet CMD_CRC_ErrorStatusGet_t;

/** Message ID for ACK_CRC_ErrorStatusGet */
#define ACK_CRC_ERRORSTATUSGET 0x0D09

/**
   Provides the status of corrupt CRC insertion. Acknowledgement for
   CMD_CRC_ErrorStatusGet.
*/
typedef struct ACK_CRC_ErrorStatusGet ACK_CRC_ErrorStatusGet_t;

/** Message ID for CMD_DebugTrailControl */
#define CMD_DEBUGTRAILCONTROL 0x5643

/**
   Without affecting normal operation, certain types of data may be logged to a
   dedicated data buffer to be read out by the management system. This message is used
   to specify the type of data to be logged and the point in time at which to stop
   logging.
*/
typedef struct CMD_DebugTrailControl CMD_DebugTrailControl_t;

/** Message ID for ACK_DebugTrailControl */
#define ACK_DEBUGTRAILCONTROL 0x5643

/**
   This is the acknowledgement for CMD_DebugTrailControl.
*/
typedef struct ACK_DebugTrailControl ACK_DebugTrailControl_t;

/** Message ID for CMD_RingbufferDataGet */
#define CMD_RINGBUFFERDATAGET 0x0703

/**
   This message is used to retrieve debug trace data with the debug trace system. The
   debug trace system needs to be actived with CMD_DebugTrailControl, before this
   message can be used.
*/
typedef struct CMD_RingbufferDataGet CMD_RingbufferDataGet_t;

/** Message ID for ACK_RingbufferDataGet */
#define ACK_RINGBUFFERDATAGET 0x0703

/**
   This is the acknowledgement for CMD_RingbufferDataGet.
*/
typedef struct ACK_RingbufferDataGet ACK_RingbufferDataGet_t;

/** Message ID for CMD_RingbufferStatusGet */
#define CMD_RINGBUFFERSTATUSGET 0x5703

/**
   This message is used to retrieve the status of the debug trace system. The debug
   trace system needs to be actived with CMD_DebugTrailControl, before this message can
   be used.
*/
typedef struct CMD_RingbufferStatusGet CMD_RingbufferStatusGet_t;

/** Message ID for ACK_RingbufferStatusGet */
#define ACK_RINGBUFFERSTATUSGET 0x5703

/**
   Provides the Debug Trail Status requested by CMD_RingbufferStatusGet.
   The Debug Trail Index, Buffer Size, and Wrap Count are used to determine the
   location of valid data within the Debug Buffer to retrieve them with
   CMD_RingbufferDataGet.
   Note: See example below.
*/
typedef struct ACK_RingbufferStatusGet ACK_RingbufferStatusGet_t;

/** Message ID for CMD_SELT_Configure */
#define CMD_SELT_CONFIGURE 0x1048

/**
   Configures the parameters for single-ended line testing (SELT).
*/
typedef struct CMD_SELT_Configure CMD_SELT_Configure_t;

/** Message ID for ACK_SELT_Configure */
#define ACK_SELT_CONFIGURE 0x1048

/**
   Acknowledgement for CMD_SELT_Configure.
*/
typedef struct ACK_SELT_Configure ACK_SELT_Configure_t;

/** Message ID for CMD_SELT_QLN_Get */
#define CMD_SELT_QLN_GET 0x0020

/**
   The message requests the results of the quiet-line noise (QLN) measurement which is
   part of the SELT test, if enabled with CMD_SELT_Configure.
*/
typedef struct CMD_SELT_QLN_Get CMD_SELT_QLN_Get_t;

/** Message ID for ACK_SELT_QLN_Get */
#define ACK_SELT_QLN_GET 0x0020

/**
   Returns information about the QLN per subcarrier resulting from the SELT test.
*/
typedef struct ACK_SELT_QLN_Get ACK_SELT_QLN_Get_t;

/** Message ID for CMD_SELT_UER_Get */
#define CMD_SELT_UER_GET 0x0120

/**
   The message requests the results of the Uncalibrated Echo Response (UER) measurement
   which is part of the SELT test, if enabled with CMD_SELT_Configure.
*/
typedef struct CMD_SELT_UER_Get CMD_SELT_UER_Get_t;

/** Message ID for ACK_SELT_UER_Get */
#define ACK_SELT_UER_GET 0x0120

/**
   Returns information about the UER per subcarrier resulting from the SELT test.
*/
typedef struct ACK_SELT_UER_Get ACK_SELT_UER_Get_t;

/**
   Requests information about the current state of the modem state-machine.
   The command can be sent in all states of the modem state machine (see Figure2).
*/
struct CMD_ModemFSM_StateGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the current state of the modem state-machine.
*/
struct ACK_ModemFSM_StateGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Status */
   DSL_uint16_t ModemState;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Status */
   DSL_uint16_t ModemState;
#endif
} __PACKED__ ;


/**
   Returns information about the current state of the modem state-machine. This message
   is sent autonomously, if the modem enters a new state and reporting is enabled for
   that state (see CMD_ModemFSM_EventConfigure).
*/
struct EVT_ModemFSM_StateGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Status */
   DSL_uint16_t ModemState;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Status */
   DSL_uint16_t ModemState;
#endif
} __PACKED__ ;


/**
   Autonomous message indicating that the modem online code was reached and that
   initialization was completed. The management entity may not perform any
   query-response messaging until the EVT_ModemReady message is received. Its
   generation cannot be disabled. If it is not received this indicates a problem,
   possibly with the boot sequence. If the message is received, it contains a result
   code that may also indicate that an error was detected by the firmware during
   initialization.
*/
struct EVT_ModemReady
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Ready Result Code */
   DSL_uint16_t MR_ResCode;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Modem Ready Result Code */
   DSL_uint16_t MR_ResCode;
#endif
} __PACKED__ ;


/**
   Controls state transitions of the modem state-machine.
*/
struct CMD_ModemFSM_StateSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 12;
   /** Link Control */
   DSL_bf16_t LinkControl : 4;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Control */
   DSL_bf16_t LinkControl : 4;
   /** Reserved */
   DSL_bf16_t Res00 : 12;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_ModemFSM_StateSet
*/
struct ACK_ModemFSM_StateSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Changes the DSL mode of a line between VDSL and ADSL. After applying this message, a
   reboot with the selected FW takes place. This command is only valid in boot modes
   0x8 and 0x9 and ignored otherwise.
   Note: This message needs to be applied for changing the DSL mode only, not for
   every line initialization.
   Note: Before applying this message, the VINAX device driver writes the wanted DSL
   mode to a DFE HW register which is evaluated during the boot process.
   (PDSCR register, Bit PYPRG1, "0"=VDSL)
   Note: The parameter "DLS Mode Boot Selection" is needed for a common ADSL/VDSL
   FW. Otherwise, this parameter is not evaluated.
*/
struct CMD_DSL_ModeModify
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 15;
   /** DSL Mode Boot Selection */
   DSL_bf16_t DSL_Mode : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** DSL Mode Boot Selection */
   DSL_bf16_t DSL_Mode : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 15;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_DSL_ModeModify
*/
struct ACK_DSL_ModeModify
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports failure information after entering FAIL state. This message is sent
   autonomously without host request.
*/
struct ALM_ModemFSM_FailReasonGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** System Sub Error Code */
   DSL_uint8_t SubErrorCode;
   /** System Error Code */
   DSL_uint8_t ErrorCode;
   /** Failure State Information */
   DSL_uint16_t FW_FailCode;
   /** Reserved */
   DSL_uint16_t Res00[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** System Error Code */
   DSL_uint8_t ErrorCode;
   /** System Sub Error Code */
   DSL_uint8_t SubErrorCode;
   /** Failure State Information */
   DSL_uint16_t FW_FailCode;
   /** Reserved */
   DSL_uint16_t Res00[8];
#endif
} __PACKED__ ;


/**
   Requests failure information after entering FAIL state.
*/
struct CMD_ModemFSM_FailReasonGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns failure information after entering FAIL state.
*/
struct ACK_ModemFSM_FailReasonGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** System Sub Error Code */
   DSL_uint8_t SubErrorCode;
   /** System Error Code */
   DSL_uint8_t ErrorCode;
   /** Failure State Information */
   DSL_uint16_t FW_FailCode;
   /** Reserved */
   DSL_uint16_t Res00[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** System Error Code */
   DSL_uint8_t ErrorCode;
   /** System Sub Error Code */
   DSL_uint8_t SubErrorCode;
   /** Failure State Information */
   DSL_uint16_t FW_FailCode;
   /** Reserved */
   DSL_uint16_t Res00[8];
#endif
} __PACKED__ ;


/**
   Configuration of options for the modem state machine.
*/
struct CMD_ModemFSM_OptionsSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 11;
   /** US Virtual Noise Support */
   DSL_bf16_t E4 : 1;
   /** DS Virtual Noise Support */
   DSL_bf16_t E3 : 1;
   /** Loop Diagnostic Mode Control */
   DSL_bf16_t E2 : 1;
   /** Reserved 0 */
   DSL_bf16_t E1 : 1;
   /** Automatic Re-Start Control */
   DSL_bf16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Automatic Re-Start Control */
   DSL_bf16_t E0 : 1;
   /** Reserved 0 */
   DSL_bf16_t E1 : 1;
   /** Loop Diagnostic Mode Control */
   DSL_bf16_t E2 : 1;
   /** DS Virtual Noise Support */
   DSL_bf16_t E3 : 1;
   /** US Virtual Noise Support */
   DSL_bf16_t E4 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 11;
#endif
} __PACKED__ ;
 

/**
   Acknowledgement to CMD_ModemFSM_OptionsSet (configuration of automatic re-start).
*/
struct ACK_ModemFSM_OptionsSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets options for the modem state-machine. Enables/Disables EVENT messages (EVT) for
   specific state transitions. The EVT message is generated after entering the
   particular state listed in the parameter description of the enable bits.
*/
struct CMD_ModemFSM_EventConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** Enable Bit 14 */
   DSL_bf16_t E14 : 1;
   /** Enable Bit 13 */
   DSL_bf16_t E13 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
   /** Enable Bit 11 */
   DSL_bf16_t E11 : 1;
   /** Enable Bit 10 */
   DSL_bf16_t E10 : 1;
   /** Enable Bit 9 */
   DSL_bf16_t E9 : 1;
   /** Enable Bit 8 */
   DSL_bf16_t E8 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 1;
   /** Enable Bit 6 */
   DSL_bf16_t E6 : 1;
   /** Enable Bit 5 */
   DSL_bf16_t E5 : 1;
   /** Enable Bit 4 */
   DSL_bf16_t E4 : 1;
   /** Enable Bit 3 */
   DSL_bf16_t E3 : 1;
   /** Enable Bit 2 */
   DSL_bf16_t E2 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 1;
   /** Enable Bit 0 */
   DSL_bf16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit 0 */
   DSL_bf16_t E0 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** Enable Bit 2 */
   DSL_bf16_t E2 : 1;
   /** Enable Bit 3 */
   DSL_bf16_t E3 : 1;
   /** Enable Bit 4 */
   DSL_bf16_t E4 : 1;
   /** Enable Bit 5 */
   DSL_bf16_t E5 : 1;
   /** Enable Bit 6 */
   DSL_bf16_t E6 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
   /** Enable Bit 8 */
   DSL_bf16_t E8 : 1;
   /** Enable Bit 9 */
   DSL_bf16_t E9 : 1;
   /** Enable Bit 10 */
   DSL_bf16_t E10 : 1;
   /** Enable Bit 11 */
   DSL_bf16_t E11 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 1;
   /** Enable Bit 13 */
   DSL_bf16_t E13 : 1;
   /** Enable Bit 14 */
   DSL_bf16_t E14 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 1;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_ModemFSM_EventConfigure. (Enabling/Disabling EVENT messages
   (EVTs) for specific state transitions.)
*/
struct ACK_ModemFSM_EventConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Enables/Disables the generation of ALARM messages (ALM) for specific near-end line
   failures. If the corresponding Enable bit for a line failure is set, then the modem
   firmware will send an autonomous message ALM_LineFailureNE_Get if this failure
   occurs.
*/
struct CMD_FailuresNE_AlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 2;
   /** Enable Bit 13 */
   DSL_bf16_t E13 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** Enable Bit3 */
   DSL_bf16_t E3 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 1;
   /** Enable Bit1 */
   DSL_bf16_t E1 : 1;
   /** Enable Bit0 */
   DSL_bf16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit0 */
   DSL_bf16_t E0 : 1;
   /** Enable Bit1 */
   DSL_bf16_t E1 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** Enable Bit3 */
   DSL_bf16_t E3 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** Enable Bit 13 */
   DSL_bf16_t E13 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 2;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_FailuresNE_AlarmConfigure.
*/
struct ACK_FailuresNE_AlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Enables/Disables the generation of ALARM messages (ALM) for specific far-end line
   failures. If the corresponding Enable bit for a line failure is set, then the modem
   firmware will send an autonomous message ALM_LineFailureFE_Get if this failure
   occurs.
*/
struct CMD_FailuresFE_AlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 12;
   /** Enable Bit3 */
   DSL_bf16_t E3 : 1;
   /** Enable Bit2 */
   DSL_bf16_t E2 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
   /** Enable Bit0 */
   DSL_bf16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit0 */
   DSL_bf16_t E0 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** Enable Bit2 */
   DSL_bf16_t E2 : 1;
   /** Enable Bit3 */
   DSL_bf16_t E3 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 12;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_FailuresFE_AlarmConfigure.
*/
struct ACK_FailuresFE_AlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configures re-initialization triggers for near-end failure conditions in modem state
   STEADY STATE transmission (see Figure1). This command is accepted in RESET state
   only.
*/
struct CMD_ReInitNE_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 2;
   /** Enable Bit 13 for Re-init Trigger */
   DSL_bf16_t E13 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 2;
   /** Enable Bit 10 Re-init Trigger */
   DSL_bf16_t E10 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 6;
   /** Enable Bit 1 for Re-init Trigger */
   DSL_bf16_t E3 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 1;
   /** Enable Bit 1 for Re-init Trigger */
   DSL_bf16_t E1 : 1;
   /** Enable Bit 0 for Re-init Trigger */
   DSL_bf16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit 0 for Re-init Trigger */
   DSL_bf16_t E0 : 1;
   /** Enable Bit 1 for Re-init Trigger */
   DSL_bf16_t E1 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** Enable Bit 1 for Re-init Trigger */
   DSL_bf16_t E3 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 6;
   /** Enable Bit 10 Re-init Trigger */
   DSL_bf16_t E10 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 2;
   /** Enable Bit 13 for Re-init Trigger */
   DSL_bf16_t E13 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 2;
#endif
} __PACKED__ ;


/**
   Acknowledgment to CMD_ReInitNE_Configure (Configuration of re-initialization
   triggers for near-end failures).
*/
struct ACK_ReInitNE_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configures Re-initialization triggers for Far-End failure conditions in modem state
   STEADY STATE.
*/
struct CMD_ReInitFE_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 12;
   /** Enable Bit 3 for Re-init Trigger */
   DSL_bf16_t E3 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 2;
   /** Enable Bit 0 for Re-init Trigger */
   DSL_bf16_t E0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable Bit 0 for Re-init Trigger */
   DSL_bf16_t E0 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 2;
   /** Enable Bit 3 for Re-init Trigger */
   DSL_bf16_t E3 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 12;
#endif
} __PACKED__ ;


/**
   Acknowledgment to CMD_ReInitFE_Configure (Configuration of Re-initialization
   triggers for far-end failures).
*/
struct ACK_ReInitFE_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests information about the loaded FW image. This info is used e.g. for swapping
   between ADSL and VDSL.
*/
struct CMD_FW_ImageInfoGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the information about the FW image requested by ACK_FW_ImageInfoGet
*/
struct ACK_FW_ImageInfoGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
   /** Site */
   DSL_bf16_t imageType : 1;
   /** DSL mode */
   DSL_bf16_t dslMode : 1;
   /** Site */
   DSL_bf16_t location : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Site */
   DSL_bf16_t location : 1;
   /** DSL mode */
   DSL_bf16_t dslMode : 1;
   /** Site */
   DSL_bf16_t imageType : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
#endif
} __PACKED__ ;


/**
   Configuration of the VTU Transmission System Enabling (XTSE).Configures the
   transmission system coding types to be supported on the line (VDSL flavour and annex
   support).
   (References: Section 7.3.1.1.1 of G.997.1 and G.994.1 Amd4 [6] Tables "Standard
   information field – SPar(1) coding")
*/
struct CMD_XTSE_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ADSL Mode-Bit15: G.992.5, Annex A */
   DSL_bf16_t A15 : 1;
   /** ADSL Mode-Bit14: G.992.5, Annex B */
   DSL_bf16_t A14 : 1;
   /** ADSL Mode-Bit13: G.992.3, Annex M */
   DSL_bf16_t A13 : 1;
   /** ADSL Mode-Bit12: G.992.3, Annex L, US Mask2 */
   DSL_bf16_t A12 : 1;
   /** ADSL Mode-Bit11: G.992.3, Annex L, US Mask1 */
   DSL_bf16_t A11 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** ADSL Mode-Bit9: G.992.3, Annex B */
   DSL_bf16_t A9 : 1;
   /** ADSL Mode-Bit8: G.992.3, Annex A */
   DSL_bf16_t A8 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 4;
   /** ADSL Mode-Bit3: G.992.1, Annex B */
   DSL_bf16_t A3 : 1;
   /** ADSL Mode-Bit2: G.992.1, Annex A */
   DSL_bf16_t A2 : 1;
   /** ADSL Mode-Bit1: G.992.5, Annex M */
   DSL_bf16_t A1 : 1;
   /** ADSL Mode-Bit0: T1.413 */
   DSL_bf16_t A0 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 13;
   /** VDSL Mode-Bit2: VDSL2 */
   DSL_bf16_t V2 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 1;
   /** VDSL Mode-Bit0: VDSL1 */
   DSL_bf16_t V0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ADSL Mode-Bit0: T1.413 */
   DSL_bf16_t A0 : 1;
   /** ADSL Mode-Bit1: G.992.5, Annex M */
   DSL_bf16_t A1 : 1;
   /** ADSL Mode-Bit2: G.992.1, Annex A */
   DSL_bf16_t A2 : 1;
   /** ADSL Mode-Bit3: G.992.1, Annex B */
   DSL_bf16_t A3 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 4;
   /** ADSL Mode-Bit8: G.992.3, Annex A */
   DSL_bf16_t A8 : 1;
   /** ADSL Mode-Bit9: G.992.3, Annex B */
   DSL_bf16_t A9 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
   /** ADSL Mode-Bit11: G.992.3, Annex L, US Mask1 */
   DSL_bf16_t A11 : 1;
   /** ADSL Mode-Bit12: G.992.3, Annex L, US Mask2 */
   DSL_bf16_t A12 : 1;
   /** ADSL Mode-Bit13: G.992.3, Annex M */
   DSL_bf16_t A13 : 1;
   /** ADSL Mode-Bit14: G.992.5, Annex B */
   DSL_bf16_t A14 : 1;
   /** ADSL Mode-Bit15: G.992.5, Annex A */
   DSL_bf16_t A15 : 1;
   /** VDSL Mode-Bit0: VDSL1 */
   DSL_bf16_t V0 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 1;
   /** VDSL Mode-Bit2: VDSL2 */
   DSL_bf16_t V2 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 13;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_XTSE_Configure.
*/
struct ACK_XTSE_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Controls the downstream band usage.
*/
struct CMD_BandControl_DS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Downstream Bands */
   DSL_uint8_t NumBandsDS;
   /** Band Descriptor DS */
   VNX_ToneIndex_t band[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Downstream Bands */
   DSL_uint8_t NumBandsDS;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Band Descriptor DS */
   VNX_ToneIndex_t band[8];
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_BandControl_DS_Set.
*/
struct ACK_BandControl_DS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Controls the upstream band usage.
   For CO side only, if used to configure normal modem operation in state RESET (and
   not for DMTscope tests). Then the data is transmitted from CO to the CPE during
   initialization (in GHS and FULL_INITIALIZATION state).
   If used for DMTscope tests without handshake and training phases it must be applied
   at the CPE directly.
*/
struct CMD_BandControl_US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Upstream Bands */
   DSL_uint8_t NumBandsUS;
   /** Band Descriptor US */
   VNX_ToneIndex_t band[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Upstream Bands */
   DSL_uint8_t NumBandsUS;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Band Descriptor US */
   VNX_ToneIndex_t band[8];
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_BandControl_US_Set.
*/
struct ACK_BandControl_US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets the RFI bands to be notched out in the PSD.
   To delete all already configured RFI bands, apply the message with NumRfiBands=1 and
   RfiBand=(0,0).
*/
struct CMD_RFI_BandControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of RFI Bands */
   DSL_uint8_t NumRfiBands;
   /** RFI Band Descriptor */
   VNX_ToneIndex_t RfiBand[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of RFI Bands */
   DSL_uint8_t NumRfiBands;
   /** Reserved */
   DSL_uint8_t Res00;
   /** RFI Band Descriptor */
   VNX_ToneIndex_t RfiBand[16];
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_RFI_BandControlSet.
*/
struct ACK_RFI_BandControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configures the minimum rate of the message based overhead. (Section 7.3.1.5 of
   G.997.1)
   For VDSL1, the minimum overhead rate refers to the EOC overhead rate.
*/
struct CMD_OH_RateSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** MSGMINds, Minimum OH Rate DS */
   DSL_uint16_t minOH_RateDS ;
   /** MSGMINus, Minimum OH Rate US */
   DSL_uint16_t minOH_RateUS ;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** MSGMINds, Minimum OH Rate DS */
   DSL_uint16_t minOH_RateDS ;
   /** MSGMINus, Minimum OH Rate US */
   DSL_uint16_t minOH_RateUS ;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_OH_RateSet.
*/
struct ACK_OH_RateSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configuration of PSD and Power parameters.
   For CO side only unless used for DMTscope tests! If used to configure normal modem
   operation in state RESET (and not for DMTscope tests), the data is transmitted from
   CO to the CPE during initialization. If used for DMTscope tests without handshake
   and training phases it is applicable for both sides.
*/
struct CMD_PSD_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Power/PSD Configuration Parameter: MAXNOMPSDds */
   DSL_uint16_t MAXNOMPSDds;
   /** Power/PSD Configuration Parameter: MAXNOMPSDus */
   DSL_uint16_t MAXNOMPSDus;
   /** Power/PSD Configuration Parameter: MAXNOMATPds */
   DSL_uint16_t MAXNOMATPds;
   /** Power/PSD Configuration Parameters */
   DSL_uint16_t MAXNOMATPus;
   /** Reserved */
   DSL_uint16_t Res00;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Power/PSD Configuration Parameter: MAXNOMPSDds */
   DSL_uint16_t MAXNOMPSDds;
   /** Power/PSD Configuration Parameter: MAXNOMPSDus */
   DSL_uint16_t MAXNOMPSDus;
   /** Power/PSD Configuration Parameter: MAXNOMATPds */
   DSL_uint16_t MAXNOMATPds;
   /** Power/PSD Configuration Parameters */
   DSL_uint16_t MAXNOMATPus;
   /** Reserved */
   DSL_uint16_t Res00;
#endif
} __PACKED__ ;


/**
   For CO side only unless used for DMTscope tests!
   Acknowledgement for the message CMD_PSD_Set.
*/
struct ACK_PSD_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Controls the transmit signal at the VTU-O via transmit PSD breakpoints (along with
   the MAXNOMPSD).These PSD breakpoints shape the PSD level relative to the MAXNOMPSD.
*/
struct CMD_PSD_BreakpointsTxDS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Breakpoint Descriptor TxPSDds */
   VNX_PSDbreak_t breakpoint[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Breakpoint Descriptor TxPSDds */
   VNX_PSDbreak_t breakpoint[32];
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_PSD_BreakpointsTxDS_Set.
*/
struct ACK_PSD_BreakpointsTxDS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Specifies the maximum upstream transmit PSD by means of breakpoints. Values are set
   at the VTU-O and transferred to the VTU-R in the O-SIGNATURE message (except if used
   for DMTscope). See chapter 12.4.4 in G.993.1 [2] and chapter 12.2.4.2.1 in G.993.2
   [3].
   If used for DMTscope tests without handshake and training phases it must be applied
   at the VTU-R directly.
*/
struct CMD_PSD_BreakpointsTxUS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Breakpoint Descriptor TxPSDus */
   VNX_PSDbreak_t breakpoint[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Breakpoint Descriptor TxPSDus */
   VNX_PSDbreak_t breakpoint[32];
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_PSD_BreakpointsTxUS_Set.
*/
struct ACK_PSD_BreakpointsTxUS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   For VDSL1 only. In VDSL2 use CMD_PSD_BreakpointsRxRef2US_Set.
   Specifies the Upstream Receive Reference PSD by means of breakpoints. It is used for
   upstream power back-off (UPBO). Values are set at the VTU-O and transferred to the
   VTU-R in the O-SIGNATURE message (See chapter 12.4.4 in G.993.1 [2]).
*/
struct CMD_PSD_BreakpointsRxRefUS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Breakpoint Descriptor RxRef PSD */
   VNX_PSDbreak_t breakpoint[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Breakpoint Descriptor RxRef PSD */
   VNX_PSDbreak_t breakpoint[32];
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_PSD_BreakpointsRxRefUS_Set.
*/
struct ACK_PSD_BreakpointsRxRefUS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Specifies the VDSL2 Upstream Receive Reference PSD in the (a, b) format (Section
   7.3.1.2.13 of G997.1). Contains the parameters to compute the reference PSD that
   shall be used for the calculation of upstream power back-off (UPBO). One couple of
   parameters (a, b) is defined per upstream band (except US0). Values are set at the
   VTU-O and transferred to the VTU-R in the O-SIGNATURE message (See chapter 12.2.4 in
   G.993.2 [3]).
*/
struct CMD_PSD_BreakpointsRxRef2US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Bands */
   DSL_uint8_t numOfBands;
   /** (a, b) UPBO RxRef2 PSD Descriptor */
   VNX_RxRef2PSD_t a_b_set[7];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Bands */
   DSL_uint8_t numOfBands;
   /** Reserved */
   DSL_uint8_t Res00;
   /** (a, b) UPBO RxRef2 PSD Descriptor */
   VNX_RxRef2PSD_t a_b_set[7];
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_PSD_BreakpointsRxRef2US_Set.
*/
struct ACK_PSD_BreakpointsRxRef2US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Controls the Upstream Power Back-Off (UPBO) mode of operation. There are 2 modes: an
   "UPBO standard mode" ( 7.3.1.2.13 of G997.1) and an "UPBO boost mode", which
   calculates the LOSS(kl0, f) parameter in a different way (see Parameter 2 below).
*/
struct CMD_PBO_Control
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** UPBO Boost Mode Enable */
   DSL_bf16_t UPBOBoost : 1;
   /** Force CO-MIB Electrical Loop Length */
   DSL_bf16_t UPBOKO : 1;
   /** UPBOKL */
   DSL_uint16_t UPBOKL;
   /** kl0_start */
   DSL_uint16_t kl0_start;
   /** kl0_stop */
   DSL_uint16_t kl0_stop;
   /** FREFstart */
   DSL_uint16_t FREFstart;
   /** FREFstop */
   DSL_uint16_t FREFstop;
   /** KLREFstart */
   DSL_uint16_t KLREFstart;
   /** KLREFstop */
   DSL_uint16_t KLREFstop;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Force CO-MIB Electrical Loop Length */
   DSL_bf16_t UPBOKO : 1;
   /** UPBO Boost Mode Enable */
   DSL_bf16_t UPBOBoost : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** UPBOKL */
   DSL_uint16_t UPBOKL;
   /** kl0_start */
   DSL_uint16_t kl0_start;
   /** kl0_stop */
   DSL_uint16_t kl0_stop;
   /** FREFstart */
   DSL_uint16_t FREFstart;
   /** FREFstop */
   DSL_uint16_t FREFstop;
   /** KLREFstart */
   DSL_uint16_t KLREFstart;
   /** KLREFstop */
   DSL_uint16_t KLREFstop;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_PBO_Control.
*/
struct ACK_PBO_Control
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the electrical loop length estimate kl0.
*/
struct CMD_UPBO_KL0Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the data requested by CMD_UPBO_KL0Get.
   (Section 7.5.1.23 of G.997.1 [7])
*/
struct ACK_UPBO_KL0Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Kl0 Estimate VTU-O */
   DSL_uint16_t kl0_EstimO;
   /** Kl0 Estimate VTU-R */
   DSL_uint16_t kl0_EstimR;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Kl0 Estimate VTU-O */
   DSL_uint16_t kl0_EstimO;
   /** Kl0 Estimate VTU-R */
   DSL_uint16_t kl0_EstimR;
#endif
} __PACKED__ ;


/**
   Enables for each VDSL2 Profile an adaptive PSD optimization (ceiling) algorithm.
   Currently only required for Profile 8b due to the high transmit power.
*/
struct CMD_PSD_CeilingDS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Enable PSD Ceiling Optimization for Profile 30a */
   DSL_bf16_t PSDCeil7 : 1;
   /** Enable PSD Ceiling Optimization for Profile 17a */
   DSL_bf16_t PSDCeil6 : 1;
   /** Enable PSD Ceiling Optimization for Profile 12b */
   DSL_bf16_t PSDCeil5 : 1;
   /** Enable PSD Ceiling Optimization for Profile 12a */
   DSL_bf16_t PSDCeil4 : 1;
   /** Enable PSD Ceiling Optimization for Profile 8d */
   DSL_bf16_t PSDCeil3 : 1;
   /** Enable PSD Ceiling Optimization for Profile 8c */
   DSL_bf16_t PSDCeil2 : 1;
   /** Enable PSD Ceiling Optimization for Profile 8b */
   DSL_bf16_t PSDCeil1 : 1;
   /** Enable PSD Ceiling Optimization for Profile 8a */
   DSL_bf16_t PSDCeil0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Enable PSD Ceiling Optimization for Profile 8a */
   DSL_bf16_t PSDCeil0 : 1;
   /** Enable PSD Ceiling Optimization for Profile 8b */
   DSL_bf16_t PSDCeil1 : 1;
   /** Enable PSD Ceiling Optimization for Profile 8c */
   DSL_bf16_t PSDCeil2 : 1;
   /** Enable PSD Ceiling Optimization for Profile 8d */
   DSL_bf16_t PSDCeil3 : 1;
   /** Enable PSD Ceiling Optimization for Profile 12a */
   DSL_bf16_t PSDCeil4 : 1;
   /** Enable PSD Ceiling Optimization for Profile 12b */
   DSL_bf16_t PSDCeil5 : 1;
   /** Enable PSD Ceiling Optimization for Profile 17a */
   DSL_bf16_t PSDCeil6 : 1;
   /** Enable PSD Ceiling Optimization for Profile 30a */
   DSL_bf16_t PSDCeil7 : 1;
   /** Reserved */
   DSL_uint8_t Res00;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_PSD_CeilingDS_Set.
*/
struct ACK_PSD_CeilingDS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configuration of noise margin parameters.
*/
struct CMD_NoiseMarginSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Noise Margin Configuration Parameter: TARSNRMds */
   DSL_uint16_t TARSNRMds;
   /** Noise Margin Configuration Parameter: TARSNRMus */
   DSL_uint16_t TARSNRMus;
   /** Noise Margin Configuration Parameter: MAXSNRMds */
   DSL_uint16_t MAXSNRMds;
   /** Noise Margin Configuration Parameter: MAXSNRMus */
   DSL_uint16_t MAXSNRMus;
   /** Noise Margin Configuration Parameter: MINSNRMds */
   DSL_uint16_t MINSNRMds;
   /** Noise Margin Configuration Parameter: MINSNRMus */
   DSL_uint16_t MINSNRMus;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Noise Margin Configuration Parameter: TARSNRMds */
   DSL_uint16_t TARSNRMds;
   /** Noise Margin Configuration Parameter: TARSNRMus */
   DSL_uint16_t TARSNRMus;
   /** Noise Margin Configuration Parameter: MAXSNRMds */
   DSL_uint16_t MAXSNRMds;
   /** Noise Margin Configuration Parameter: MAXSNRMus */
   DSL_uint16_t MAXSNRMus;
   /** Noise Margin Configuration Parameter: MINSNRMds */
   DSL_uint16_t MINSNRMds;
   /** Noise Margin Configuration Parameter: MINSNRMus */
   DSL_uint16_t MINSNRMus;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_NoiseMarginSet.
*/
struct ACK_NoiseMarginSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Set the TSSI breakpoints for PSD shaping for ADSL in downstream direction.
*/
struct CMD_TssiDS_ADSL_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPt;
   /** TSSI Breakpoints */
   VNX_TSSIbreak_t tssiBreakpt[60];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPt;
   /** Reserved */
   DSL_uint8_t Res00;
   /** TSSI Breakpoints */
   VNX_TSSIbreak_t tssiBreakpt[60];
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_TssiDS_ADSL_Set.
*/
struct ACK_TssiDS_ADSL_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Set the TSSI breakpoints for PSD shaping for ADSL in upstream direction.
*/
struct CMD_TssiUS_ADSL_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPt;
   /** TSSI Breakpoints */
   VNX_TSSIbreak_t tssiBreakpt[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPt;
   /** Reserved */
   DSL_uint8_t Res00;
   /** TSSI Breakpoints */
   VNX_TSSIbreak_t tssiBreakpt[4];
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_TssiUS_ADSL_Set.
*/
struct ACK_TssiUS_ADSL_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Attention: To be applied after TSSI configuration!
   This configures the ADSL downstream carrier mask per tone.
   The carrier masking must be applied after TSSI configuration with
   CMD_TssiDS_ADSL_Set, because it is meant for masking, i.e. switching off carriers
   but not for switching on carriers with are suppressed via the TSSI breakpoints.
   The message has a fix length to support all ADSL modes, in ADSL and ADSL2 the values
   for the subcarrier indices >255 are unused ("Don’t care").
*/
struct CMD_ADSL_CarMaskDS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Carrier Mask Bitfield */
   DSL_uint16_t mask[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Carrier Mask Bitfield */
   DSL_uint16_t mask[32];
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ADSL_CarMaskDS_Set.
*/
struct ACK_ADSL_CarMaskDS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Attention: To be applied after TSSI configuration!
   This configures the ADSL upstream carrier mask per tone.
   The carrier masking must be applied after TSSI configuration with
   CMD_TssiUS_ADSL_Set, because it is meant for masking, i.e. switching off carriers
   but not for switching them on if suppressed via the TSSI breakpoints.
*/
struct CMD_ADSL_CarMaskUS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Carrier Mask Bitfield */
   DSL_uint16_t mask[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Carrier Mask Bitfield */
   DSL_uint16_t mask[4];
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ADSL_CarMaskUS_Set.
*/
struct ACK_ADSL_CarMaskUS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This configures the ADSL downstream powercutback for the different ADSL flavours.
*/
struct CMD_ADSL_PCB_DS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Default Minimum Power Cutback */
   DSL_uint16_t pcbdef;
   /** ADSL1 Minimum Power Cutback */
   DSL_uint16_t pcbadsl1;
   /** ADSL2 Minimum Power Cutback for Annex A/B/I/J */
   DSL_uint16_t pcbadsl2;
   /** ADSL2 Minimum Power Cutback for Annex L Upstream Mask 1 */
   DSL_uint16_t pcbadsl2l1;
   /** ADSL2 Minimum Power Cutback for Annex L Upstream Mask 2 */
   DSL_uint16_t pcbadsl2l2;
   /** ADSL2 Minimum Power Cutback for Annex M */
   DSL_uint16_t pcbadsl2m;
   /** ADSL2+ Minimum Power Cutback for Annex A/B/I/J */
   DSL_uint16_t pcbadsl2p;
   /** ADSL2+ Minimum Power Cutback for Annex M */
   DSL_uint16_t pcbadsl2pm;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Default Minimum Power Cutback */
   DSL_uint16_t pcbdef;
   /** ADSL1 Minimum Power Cutback */
   DSL_uint16_t pcbadsl1;
   /** ADSL2 Minimum Power Cutback for Annex A/B/I/J */
   DSL_uint16_t pcbadsl2;
   /** ADSL2 Minimum Power Cutback for Annex L Upstream Mask 1 */
   DSL_uint16_t pcbadsl2l1;
   /** ADSL2 Minimum Power Cutback for Annex L Upstream Mask 2 */
   DSL_uint16_t pcbadsl2l2;
   /** ADSL2 Minimum Power Cutback for Annex M */
   DSL_uint16_t pcbadsl2m;
   /** ADSL2+ Minimum Power Cutback for Annex A/B/I/J */
   DSL_uint16_t pcbadsl2p;
   /** ADSL2+ Minimum Power Cutback for Annex M */
   DSL_uint16_t pcbadsl2pm;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ADSL_PCB_DS_Set.
*/
struct ACK_ADSL_PCB_DS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This configures the ADSL upstream power cutback for the different ADSL flavours.
*/
struct CMD_ADSL_PCB_US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Default Minimum Power Cutback */
   DSL_uint16_t pcbdef;
   /** ADSL1 Minimum Power Cutback */
   DSL_uint16_t pcbadsl1;
   /** ADSL2 Minimum Power Cutback for Annex A/B/I/J */
   DSL_uint16_t pcbadsl2;
   /** ADSL2 Minimum Power Cutback for Annex L Upstream Mask 1 */
   DSL_uint16_t pcbadsl2l1;
   /** ADSL2 Minimum Power Cutback for Annex L Upstream Mask 2 */
   DSL_uint16_t pcbadsl2l2;
   /** ADSL2 Minimum Power Cutback for Annex M */
   DSL_uint16_t pcbadsl2m;
   /** ADSL2+ Minimum Power Cutback for Annex A/B/I/J */
   DSL_uint16_t pcbadsl2p;
   /** ADSL2+ Minimum Power Cutback for Annex M */
   DSL_uint16_t pcbadsl2pm;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Default Minimum Power Cutback */
   DSL_uint16_t pcbdef;
   /** ADSL1 Minimum Power Cutback */
   DSL_uint16_t pcbadsl1;
   /** ADSL2 Minimum Power Cutback for Annex A/B/I/J */
   DSL_uint16_t pcbadsl2;
   /** ADSL2 Minimum Power Cutback for Annex L Upstream Mask 1 */
   DSL_uint16_t pcbadsl2l1;
   /** ADSL2 Minimum Power Cutback for Annex L Upstream Mask 2 */
   DSL_uint16_t pcbadsl2l2;
   /** ADSL2 Minimum Power Cutback for Annex M */
   DSL_uint16_t pcbadsl2m;
   /** ADSL2+ Minimum Power Cutback for Annex A/B/I/J */
   DSL_uint16_t pcbadsl2p;
   /** ADSL2+ Minimum Power Cutback for Annex M */
   DSL_uint16_t pcbadsl2pm;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ADSL_PCB_US_Set.
*/
struct ACK_ADSL_PCB_US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets parameters for downstream bearer channel 0. (Chapters 7.3.2.1-5 of G.997.1
   [9])
*/
struct CMD_BearerCh0_DS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
   /** Reserved (STM) */
   DSL_bf16_t Res01 : 1;
   /** ATM Configuration Control */
   DSL_bf16_t ATMControl : 1;
   /** PTM Configuration Control */
   DSL_bf16_t PTMControl : 1;
   /** Minimum Data Rate BC0 PTM DS */
   DSL_uint16_t minRate0_PTMds;
   /** Maximum Data Rate BC0 PTM DS */
   DSL_uint16_t maxRate0_PTMds;
   /** Minimum Reserved Data Rate BC0 PTM DS */
   DSL_uint16_t minResRate0_PTMds;
   /** Reserved */
   DSL_uint8_t Res02;
   /** Maximum Interleaving Delay BC0 PTM DS */
   DSL_uint8_t maxDelay0_PTMds;
   /** Reserved */
   DSL_bf16_t Res03 : 6;
   /** "SHORT PACKETS" OPTION BC0 PTM DS */
   DSL_bf16_t ShortPacket_PTMds : 1;
   /** "PRE-EMPTION" OPTION BC0 PTM DS */
   DSL_bf16_t Preempt_PTMds : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 6;
   /** Maximum BER BC0 PTM DS */
   DSL_bf16_t BER_PTMds : 2;
   /** Reserved */
   DSL_bf16_t Res05 : 4;
   /** Minimum INP BC0 PTM DS */
   DSL_bf16_t minINP_PTMds : 12;
   /** Minimum Data Rate BC0 ATM DS */
   DSL_uint16_t minRate0_ATMds;
   /** Maximum Data Rate BC0 ATM DS */
   DSL_uint16_t maxRate0_ATMds;
   /** Minimum Reserved Data Rate BC0 ATM DS */
   DSL_uint16_t minResRate0_ATMds;
   /** Reserved */
   DSL_uint8_t Res06;
   /** Maximum Interleaving Delay BC0 ATM DS */
   DSL_uint8_t maxDelay0_ATMds;
   /** Reserved */
   DSL_bf16_t Res07 : 14;
   /** Maximum BER BC0 ATM DS */
   DSL_bf16_t BER_ATMds : 2;
   /** Reserved */
   DSL_bf16_t Res08 : 4;
   /** Minimum INP BC0 ATM DS */
   DSL_bf16_t minINP_ATMds : 12;
   /** Reserved */
   DSL_uint16_t Res09[6];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PTM Configuration Control */
   DSL_bf16_t PTMControl : 1;
   /** ATM Configuration Control */
   DSL_bf16_t ATMControl : 1;
   /** Reserved (STM) */
   DSL_bf16_t Res00 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 13;
   /** Minimum Data Rate BC0 PTM DS */
   DSL_uint16_t minRate0_PTMds;
   /** Maximum Data Rate BC0 PTM DS */
   DSL_uint16_t maxRate0_PTMds;
   /** Minimum Reserved Data Rate BC0 PTM DS */
   DSL_uint16_t minResRate0_PTMds;
   /** Maximum Interleaving Delay BC0 PTM DS */
   DSL_uint8_t maxDelay0_PTMds;
   /** Reserved */
   DSL_uint8_t Res02;
   /** Maximum BER BC0 PTM DS */
   DSL_bf16_t BER_PTMds : 2;
   /** Reserved */
   DSL_bf16_t Res03 : 6;
   /** "PRE-EMPTION" OPTION BC0 PTM DS */
   DSL_bf16_t Preempt_PTMds : 1;
   /** "SHORT PACKETS" OPTION BC0 PTM DS */
   DSL_bf16_t ShortPacket_PTMds : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 6;
   /** Minimum INP BC0 PTM DS */
   DSL_bf16_t minINP_PTMds : 12;
   /** Reserved */
   DSL_bf16_t Res05 : 4;
   /** Minimum Data Rate BC0 ATM DS */
   DSL_uint16_t minRate0_ATMds;
   /** Maximum Data Rate BC0 ATM DS */
   DSL_uint16_t maxRate0_ATMds;
   /** Minimum Reserved Data Rate BC0 ATM DS */
   DSL_uint16_t minResRate0_ATMds;
   /** Maximum Interleaving Delay BC0 ATM DS */
   DSL_uint8_t maxDelay0_ATMds;
   /** Reserved */
   DSL_uint8_t Res06;
   /** Maximum BER BC0 ATM DS */
   DSL_bf16_t BER_ATMds : 2;
   /** Reserved */
   DSL_bf16_t Res07 : 14;
   /** Minimum INP BC0 ATM DS */
   DSL_bf16_t minINP_ATMds : 12;
   /** Reserved */
   DSL_bf16_t Res08 : 4;
   /** Reserved */
   DSL_uint16_t Res09[6];
#endif
} __PACKED__ ;
 
/**
   Acknowledgement for CMD_BearerCh0_DS_Set. (Configuration of bearer channel 0).
*/
struct ACK_BearerCh0_DS_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets parameters for upstream bearer channel 0. (Chapters 7.3.2.1-5 of G.997.1 [9])
*/
struct CMD_BearerCh0_US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
   /** Reserved (STM) */
   DSL_bf16_t Res01 : 1;
   /** ATM Configuration Control */
   DSL_bf16_t ATMControl : 1;
   /** PTM Configuration Control */
   DSL_bf16_t PTMControl : 1;
   /** Minimum Data Rate BC0 PTM US */
   DSL_uint16_t minRate0_PTMus;
   /** Maximum Data Rate BC0 PTM US */
   DSL_uint16_t maxRate0_PTMus;
   /** Minimum Reserved Data Rate BC0 PTM US */
   DSL_uint16_t minResRate0_PTMus;
   /** Reserved */
   DSL_uint8_t Res02;
   /** Maximum Interleaving Delay BC0 PTM US */
   DSL_uint8_t maxDelay0_PTMus;
   /** Reserved */
   DSL_bf16_t Res03 : 6;
   /** "SHORT PACKETS" OPTION BC0 PTM US */
   DSL_bf16_t ShortPacket_PTMus : 1;
   /** "PRE-EMPTION" OPTION BC0 PTM US */
   DSL_bf16_t Preempt_PTMus : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 6;
   /** Maximum BER BC0 PTM US */
   DSL_bf16_t BER_PTMus : 2;
   /** Reserved */
   DSL_bf16_t Res05 : 4;
   /** Minimum INP BC0 PTM US */
   DSL_bf16_t minINP_PTMus : 12;
   /** Minimum Data Rate BC0 ATM US */
   DSL_uint16_t minRate0_ATMus;
   /** Maximum Data Rate BC0 ATM US */
   DSL_uint16_t maxRate0_ATMus;
   /** Minimum Reserved Data Rate BC0 ATM US */
   DSL_uint16_t minResRate0_ATMus;
   /** Reserved */
   DSL_uint8_t Res06;
   /** Maximum Interleaving Delay BC0 ATM US */
   DSL_uint8_t maxDelay0_ATMus;
   /** Reserved */
   DSL_bf16_t Res07 : 14;
   /** Maximum BER BC0 ATM US */
   DSL_bf16_t BER_ATMus : 2;
   /** Reserved */
   DSL_bf16_t Res08 : 4;
   /** Minimum INP BC0 ATM US */
   DSL_bf16_t minINP_ATMus : 12;
   /** Reserved */
   DSL_uint16_t Res09[6];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PTM Configuration Control */
   DSL_bf16_t PTMControl : 1;
   /** ATM Configuration Control */
   DSL_bf16_t ATMControl : 1;
   /** Reserved (STM) */
   DSL_bf16_t Res00 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 13;
   /** Minimum Data Rate BC0 PTM US */
   DSL_uint16_t minRate0_PTMus;
   /** Maximum Data Rate BC0 PTM US */
   DSL_uint16_t maxRate0_PTMus;
   /** Minimum Reserved Data Rate BC0 PTM US */
   DSL_uint16_t minResRate0_PTMus;
   /** Maximum Interleaving Delay BC0 PTM US */
   DSL_uint8_t maxDelay0_PTMus;
   /** Reserved */
   DSL_uint8_t Res02;
   /** Maximum BER BC0 PTM US */
   DSL_bf16_t BER_PTMus : 2;
   /** Reserved */
   DSL_bf16_t Res03 : 6;
   /** "PRE-EMPTION" OPTION BC0 PTM US */
   DSL_bf16_t Preempt_PTMus : 1;
   /** "SHORT PACKETS" OPTION BC0 PTM US */
   DSL_bf16_t ShortPacket_PTMus : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 6;
   /** Minimum INP BC0 PTM US */
   DSL_bf16_t minINP_PTMus : 12;
   /** Reserved */
   DSL_bf16_t Res05 : 4;
   /** Minimum Data Rate BC0 ATM US */
   DSL_uint16_t minRate0_ATMus;
   /** Maximum Data Rate BC0 ATM US */
   DSL_uint16_t maxRate0_ATMus;
   /** Minimum Reserved Data Rate BC0 ATM US */
   DSL_uint16_t minResRate0_ATMus;
   /** Maximum Interleaving Delay BC0 ATM US */
   DSL_uint8_t maxDelay0_ATMus;
   /** Reserved */
   DSL_uint8_t Res06;
   /** Maximum BER BC0 ATM US */
   DSL_bf16_t BER_ATMus : 2;
   /** Reserved */
   DSL_bf16_t Res07 : 14;
   /** Minimum INP BC0 ATM US */
   DSL_bf16_t minINP_ATMus : 12;
   /** Reserved */
   DSL_bf16_t Res08 : 4;
   /** Reserved */
   DSL_uint16_t Res09[6];
#endif
} __PACKED__ ; 

/**
   Acknowledgement for CMD_BearerCh0_US_Set. (Configuration of bearer channel 0).
*/
struct ACK_BearerCh0_US_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configures the bearer channel to TPS-TC mapping for both directions as ATM.
   Note: The value written with this message will be made the default value starting
   with the end-of-July 06 release so that it is not necessary to apply it
   explicitely then.
*/
struct CMD_ADSL_TransportModeSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Physical Transport Mode */
   DSL_uint16_t mode;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Physical Transport Mode */
   DSL_uint16_t mode;
#endif
} __PACKED__ ;


/**
   Acknowledgment for message CMD_ADSL_TransportModeSet.
*/
struct ACK_ADSL_TransportModeSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Controls certain funtions of the chip set.
   Attention: 1. The default values are different for VDSL and ADSL. 2. VDSL uses
   the Parameters 3 to 4 only.
*/
struct CMD_AlgorithmControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 11;
   /** ADSL-Only Algorithm Control, Bit 4 */
   DSL_bf16_t AAC4 : 1;
   /** Reserved */
   DSL_bf16_t AAC3 : 1;
   /** ADSL-Only Algorithm Control, Bit 2 */
   DSL_bf16_t AAC2 : 1;
   /** ADSL-Only Algorithm Control, Bit 1 */
   DSL_bf16_t AAC1 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 7;
   /** Pilot Tone Selection Control, Bit8 */
   DSL_bf16_t IAC8 : 1;
   /** Initialization Algorithm Control, Bit7 */
   DSL_bf16_t IAC7 : 1;
   /** Initialization Algorithm Control, Bit6 */
   DSL_bf16_t IAC6 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 1;
   /** Initialization Algorithm Control, Bit4 */
   DSL_bf16_t IAC4 : 1;
   /** Initialization Algorithm Control, Bit3 */
   DSL_bf16_t IAC3 : 1;
   /** Initialization Algorithm Control, Bit2 */
   DSL_bf16_t IAC2 : 1;
   /** Initialization Algorithm Control, Bit1 */
   DSL_bf16_t IAC1 : 1;
   /** Initialization Algorithm Control, Bit0 */
   DSL_bf16_t IAC0 : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 5;
   /** Steady-State Algorithm Control, Bit10 */
   DSL_bf16_t SAC10 : 1;
   /** Steady-State Algorithm Control, Bit9 */
   DSL_bf16_t SAC9 : 1;
   /** Steady-State Algorithm Control, Bit8 */
   DSL_bf16_t SAC8 : 1;
   /** Steady-State Algorithm Control, Bit7 */
   DSL_bf16_t SAC7 : 1;
   /** Steady-State Algorithm Control, Bit6 */
   DSL_bf16_t SAC6 : 1;
   /** Reserved */
   DSL_bf16_t Res05 : 1;
   /** Steady-State Algorithm Control, Bit4 */
   DSL_bf16_t SAC4 : 1;
   /** Steady-State Algorithm Control, Bit3 */
   DSL_bf16_t SAC3 : 1;
   /** Steady-State Algorithm Control, Bit2 */
   DSL_bf16_t SAC2 : 1;
   /** Steady-State Algorithm Control, Bit1 */
   DSL_bf16_t SAC1 : 1;
   /** Steady-State Algorithm Control, Bit0 */
   DSL_bf16_t SAC0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** ADSL-Only Algorithm Control, Bit 1 */
   DSL_bf16_t AAC1 : 1;
   /** ADSL-Only Algorithm Control, Bit 2 */
   DSL_bf16_t AAC2 : 1;
   /** Reserved */
   DSL_bf16_t AAC3 : 1;
   /** ADSL-Only Algorithm Control, Bit 4 */
   DSL_bf16_t AAC4 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 11;
   /** Initialization Algorithm Control, Bit0 */
   DSL_bf16_t IAC0 : 1;
   /** Initialization Algorithm Control, Bit1 */
   DSL_bf16_t IAC1 : 1;
   /** Initialization Algorithm Control, Bit2 */
   DSL_bf16_t IAC2 : 1;
   /** Initialization Algorithm Control, Bit3 */
   DSL_bf16_t IAC3 : 1;
   /** Initialization Algorithm Control, Bit4 */
   DSL_bf16_t IAC4 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 1;
   /** Initialization Algorithm Control, Bit6 */
   DSL_bf16_t IAC6 : 1;
   /** Initialization Algorithm Control, Bit7 */
   DSL_bf16_t IAC7 : 1;
   /** Pilot Tone Selection Control, Bit8 */
   DSL_bf16_t IAC8 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 7;
   /** Steady-State Algorithm Control, Bit0 */
   DSL_bf16_t SAC0 : 1;
   /** Steady-State Algorithm Control, Bit1 */
   DSL_bf16_t SAC1 : 1;
   /** Steady-State Algorithm Control, Bit2 */
   DSL_bf16_t SAC2 : 1;
   /** Steady-State Algorithm Control, Bit3 */
   DSL_bf16_t SAC3 : 1;
   /** Steady-State Algorithm Control, Bit4 */
   DSL_bf16_t SAC4 : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 1;
   /** Steady-State Algorithm Control, Bit6 */
   DSL_bf16_t SAC6 : 1;
   /** Steady-State Algorithm Control, Bit7 */
   DSL_bf16_t SAC7 : 1;
   /** Steady-State Algorithm Control, Bit8 */
   DSL_bf16_t SAC8 : 1;
   /** Steady-State Algorithm Control, Bit9 */
   DSL_bf16_t SAC9 : 1;
   /** Steady-State Algorithm Control, Bit10 */
   DSL_bf16_t SAC10 : 1;
   /** Reserved */
   DSL_bf16_t Res05 : 5;
#endif
} __PACKED__ ;
 

/**
   Acknowledgement for message CMD_AlgorithmControlSet.
*/
struct ACK_AlgorithmControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message controls the digital high-pass filters in the VINAX.
*/
struct CMD_DigitalFilterSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HP Filter Operation Mode */
   DSL_uint16_t OpMode;
   /** HP Filter Control */
   DSL_uint16_t filterControl;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HP Filter Operation Mode */
   DSL_uint16_t OpMode;
   /** HP Filter Control */
   DSL_uint16_t filterControl;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_DigitalFilterSet.
*/
struct ACK_DigitalFilterSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Selection of the used hybrid.
*/
struct CMD_HybridSelect
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hybrid Type Selection */
   DSL_uint16_t HybridType;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hybrid Type Selection */
   DSL_uint16_t HybridType;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_HybridSelect.
*/
struct ACK_HybridSelect
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets transmit and receive gain value for the gain calibration.
*/
struct CMD_GainCalibrationSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Path Gain Calibration */
   DSL_uint16_t gainTx;
   /** Receive Path Gain Calibration */
   DSL_uint16_t gainRx;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Path Gain Calibration */
   DSL_uint16_t gainTx;
   /** Receive Path Gain Calibration */
   DSL_uint16_t gainRx;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_GainCalibrationSet.
*/
struct ACK_GainCalibrationSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command is used to configure the power level threshold at which the Peak to
   Average Ratio Reduction (PARR) algorithm is enabled. The PARR is operating when the
   transmit power level is above the threshold.
   Only valid if PARR was switched on with CMD_AlgorithmControlSet.
*/
struct CMD_PARR_ThreshSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Power Threshold */
   DSL_uint16_t powerThresh;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Power Threshold */
   DSL_uint16_t powerThresh;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_PARR_ThreshSet.
*/
struct ACK_PARR_ThreshSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The command selects PARR tones to be used by the Peak to Average Ratio Reduction
   (PARR) algorithm. Only valid if PARR was enabled with CMD_AlgorithmControlSet.
   Note: Care must be taken in selecting the parameters.  A poor selection will
   result in decreased performance!
*/
struct CMD_PARR_ToneSelect
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PARR Scaling Exponent */
   DSL_int16_t scaleExp;
   /** PARR Ceiling (dB above RMS) */
   DSL_uint16_t ceil;
   /** PARR Scaling */
   DSL_int16_t scale;
   /** Number of PARR Tones */
   DSL_uint16_t numOfTones;
   /** PARR Tone */
   DSL_uint16_t parrTone[20];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** PARR Scaling Exponent */
   DSL_int16_t scaleExp;
   /** PARR Ceiling (dB above RMS) */
   DSL_uint16_t ceil;
   /** PARR Scaling */
   DSL_int16_t scale;
   /** Number of PARR Tones */
   DSL_uint16_t numOfTones;
   /** PARR Tone */
   DSL_uint16_t parrTone[20];
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_PARR_ToneSelect.
*/
struct ACK_PARR_ToneSelect
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Controls the amount of de-interleaver memory claimed by the CO US receiver. This is
   necessary for interoperability reasons.
   For known far-end modems, it is advantageous to use as much as possible of the
   de-interleaving memory available in the HW.  For interoperability purposes with
   unknown CPEs, the US memory usage is limited to a configurable part, which is
   dependant on profile and data rate.
   Note: EXAMPLE:G.993.2 requires 64 kB of total interleaving and de-interleaving
   memory (altogether for CO+CPE) for band plans 8 a-d. The Vinax supports 128
   kB in total, corresponding to 64 kB for each direction (CO+CPE), thus at
   the CO there are 32 kB available for de-interleaving.Known CPE: CO uses all
   32 kB of the available HW memory for de-interleaving.Unknown CPE: The
   memory required at the CO for de-interleaving is profile and data rate
   dependant.
   Note: The VDSL2 standard defines the interleaver/de-interleaver memory as a
   single sized block. i.e. its definition assumes that the single memory
   block can be shared by the VTU's interleaver and de-interleaver at any
   ratio as long as the total memory (interleaver + de-interleaver) does not
   exceed the memory requirements from the standard. The interleaver setting
   is controlled by the CO. The CO estimates how much memory is needed for the
   US de-interleaving, based on it specifies to the CPE how much
   de-interleaving memory it can use for the DS. The CPE then chooses the
   interleaver depth and other framing parameters based on the memory
   restrictions provided by the CO.
*/
struct CMD_US_DeinterleaverMemPartition
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** De-Interleaver Memory for known CPE */
   DSL_uint16_t memKnownCPE;
   /** De-Interleaver Memory for unknown CPE */
   DSL_uint16_t memUnknownCPE;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** De-Interleaver Memory for known CPE */
   DSL_uint16_t memKnownCPE;
   /** De-Interleaver Memory for unknown CPE */
   DSL_uint16_t memUnknownCPE;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_US_DeinterleaverMemPartition.
*/
struct ACK_US_DeinterleaverMemPartition
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message controls the ACE filter in the AFE’s receive part.
*/
struct CMD_ACE_FilterSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ACE Filter Control */
   DSL_uint16_t filterControl;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ACE Filter Control */
   DSL_uint16_t filterControl;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_ACE_FilterSet.
*/
struct ACK_ACE_FilterSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reads the configured ACE filter mode.
*/
struct CMD_ACE_FilterGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the configured ACE filter mode as requested by CMD_ACE_FilterGet.
*/
struct ACK_ACE_FilterGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** ACE Filter Mode */
   DSL_uint8_t filterMode;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ACE Filter Mode */
   DSL_uint8_t filterMode;
   /** Reserved */
   DSL_uint8_t Res00;
#endif
} __PACKED__ ;


/**
   Performs some miscellaneous chip set configurations.
*/
struct CMD_Misc_ConfigSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 15;
   /** Clock Gating Control */
   DSL_bf16_t clockGating : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Clock Gating Control */
   DSL_bf16_t clockGating : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 15;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for ACK_Misc_ConfigSet.
*/
struct ACK_Misc_ConfigSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reads back the parameter values configured with CMD_AlgorithmControlSet.
*/
struct CMD_AlgorithmControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the information requested by CMD_AlgorithmControlGet.
*/
struct ACK_AlgorithmControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Algorithm Control Parameter Status */
   DSL_uint16_t Readback[3];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Algorithm Control Parameter Status */
   DSL_uint16_t Readback[3];
#endif
} __PACKED__ ;


/**
   Reads back the settings for the digital high-pass filters as configured with
   CMD_DigitalFilterSet.
*/
struct CMD_DigitalFilterGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the configured settings of the digital high-pass filters as requested by
   CMD_DigitalFilterGet.
*/
struct ACK_DigitalFilterGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Digital Filter Parameter Status */
   DSL_uint16_t Readback[2];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Digital Filter Parameter Status */
   DSL_uint16_t Readback[2];
#endif
} __PACKED__ ;


/**
   Reads back the selected hybrid as configured with CMD_HybridSelect.
*/
struct CMD_HybridSelectionGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the used hybrid as requested by CMD_HybridSelectionGet.
*/
struct ACK_HybridSelectionGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hybrid Status */
   DSL_uint16_t Readback;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Hybrid Status */
   DSL_uint16_t Readback;
#endif
} __PACKED__ ;


/**
   Selects one or more groups of tones to be used during the handshake phase. (See
   G.994.1 Amendment 2 [5], Table 1/G.994.1)
*/
struct CMD_HS_ToneGroupSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 7;
   /** Tone Group 8: CAR_SET_V43I */
   DSL_bf16_t Group8 : 1;
   /** Tone Group 7: CAR_SET_V43P */
   DSL_bf16_t Group7 : 1;
   /** Tone Group 6: CAR_SET_V43 */
   DSL_bf16_t Group6 : 1;
   /** Tone Group 5: CAR_SET_J43 */
   DSL_bf16_t Group5 : 1;
   /** Tone Group 4: CAR_SET_C43 */
   DSL_bf16_t Group4 : 1;
   /** Tone Group 3: CAR_SET_B43C */
   DSL_bf16_t Group3 : 1;
   /** Tone Group 2: CAR_SET_B43 */
   DSL_bf16_t Group2 : 1;
   /** Tone Group 1: CAR_SET_A43C */
   DSL_bf16_t Group1 : 1;
   /** Tone Group 0: CAR_SET_A43 */
   DSL_bf16_t Group0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Tone Group 0: CAR_SET_A43 */
   DSL_bf16_t Group0 : 1;
   /** Tone Group 1: CAR_SET_A43C */
   DSL_bf16_t Group1 : 1;
   /** Tone Group 2: CAR_SET_B43 */
   DSL_bf16_t Group2 : 1;
   /** Tone Group 3: CAR_SET_B43C */
   DSL_bf16_t Group3 : 1;
   /** Tone Group 4: CAR_SET_C43 */
   DSL_bf16_t Group4 : 1;
   /** Tone Group 5: CAR_SET_J43 */
   DSL_bf16_t Group5 : 1;
   /** Tone Group 6: CAR_SET_V43 */
   DSL_bf16_t Group6 : 1;
   /** Tone Group 7: CAR_SET_V43P */
   DSL_bf16_t Group7 : 1;
   /** Tone Group 8: CAR_SET_V43I */
   DSL_bf16_t Group8 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 7;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_HS_ToneGroupSet.
*/
struct ACK_HS_ToneGroupSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Configures the profile settings to be used for the handshake phase. (See G.994.1
   Amendment 4 [6], NPAR(3) coding)
*/
struct CMD_HS_VDSL2ProfileControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** 30a, Profile Support Bit 7 */
   DSL_bf16_t profileSup7 : 1;
   /** 17a, Profile Support Bit 6 */
   DSL_bf16_t profileSup6 : 1;
   /** 12b, Profile Support Bit 5 */
   DSL_bf16_t profileSup5 : 1;
   /** 12a, Profile Support Bit 4 */
   DSL_bf16_t profileSup4 : 1;
   /** 8d, Profile Support Bit 3 */
   DSL_bf16_t profileSup3 : 1;
   /** 8c, Profile Support Bit 2 */
   DSL_bf16_t profileSup2 : 1;
   /** 8b, Profile Support Bit 1 */
   DSL_bf16_t profileSup1 : 1;
   /** 8a, Profile Support Bit 0 */
   DSL_bf16_t profileSup0 : 1;
   /** Reserved */
   DSL_uint8_t Res01;
   /** 30a, Profile Preferred Bit 7 */
   DSL_bf16_t profilePref7 : 1;
   /** 17a, Profile Preferred Bit 6 */
   DSL_bf16_t profilePref6 : 1;
   /** 12b, Profile Preferred Bit 5 */
   DSL_bf16_t profilePref5 : 1;
   /** 12a, Profile Preferred Bit 4 */
   DSL_bf16_t profilePref4 : 1;
   /** 8d, Profile Preferred Bit 3 */
   DSL_bf16_t profilePref3 : 1;
   /** 8c, Profile Preferred Bit 2 */
   DSL_bf16_t profilePref2 : 1;
   /** 8b, Profile Preferred Bit 1 */
   DSL_bf16_t profilePref1 : 1;
   /** 8a, Profile Preferred Bit 0 */
   DSL_bf16_t profilePref0 : 1;
   /** ADLU-56, Annex A US0 PSDs Supported Bit 15 */
   DSL_bf16_t A_US0PsdSup15 : 1;
   /** ADLU-52, Annex A US0 PSDs Supported Bit 14 */
   DSL_bf16_t A_US0PsdSup14 : 1;
   /** ADLU-48, Annex A US0 PSDs Supported Bit 13 */
   DSL_bf16_t A_US0PsdSup13 : 1;
   /** ADLU-44, Annex A US0 PSDs Supported Bit 12 */
   DSL_bf16_t A_US0PsdSup12 : 1;
   /** ADLU-40, Annex A US0 PSDs Supported Bit 11 */
   DSL_bf16_t A_US0PsdSup11 : 1;
   /** ADLU-36, Annex A US0 PSDs Supported Bit 10 */
   DSL_bf16_t A_US0PsdSup10 : 1;
   /** ADLU-32, Annex A US0 PSDs Supported Bit 9 */
   DSL_bf16_t A_US0PsdSup9 : 1;
   /** EU-64, Annex A US0 PSDs Supported Bit 8 */
   DSL_bf16_t A_US0PsdSup8 : 1;
   /** EU-60, Annex A US0 PSDs Supported Bit 7 */
   DSL_bf16_t A_US0PsdSup7 : 1;
   /** EU-56, Annex A US0 PSDs Supported Bit 6 */
   DSL_bf16_t A_US0PsdSup6 : 1;
   /** EU-52, Annex A US0 PSDs Supported Bit 5 */
   DSL_bf16_t A_US0PsdSup5 : 1;
   /** EU-48, Annex A US0 PSDs Supported Bit 4 */
   DSL_bf16_t A_US0PsdSup4 : 1;
   /** EU-44, Annex A US0 PSDs Supported Bit 3 */
   DSL_bf16_t A_US0PsdSup3 : 1;
   /** EU-40, Annex A US0 PSDs Supported Bit 2 */
   DSL_bf16_t A_US0PsdSup2 : 1;
   /** EU-36, Annex A US0 PSDs Supported Bit 1 */
   DSL_bf16_t A_US0PsdSup1 : 1;
   /** EU-32, Annex A US0 PSDs Supported Bit 0 */
   DSL_bf16_t A_US0PsdSup0 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 12;
   /** 17a Profile, Annex A US0 PSDs Supported Bit 19 */
   DSL_bf16_t A_US0PsdSup19 : 1;
   /** 12b Profile, Annex A US0 PSDs Supported Bit 18 */
   DSL_bf16_t A_US0PsdSup18 : 1;
   /** ADLU-64, Annex A US0 PSDs Supported Bit 17 */
   DSL_bf16_t A_US0PsdSup17 : 1;
   /** ADLU-60, Annex A US0 PSDs Supported Bit 16 */
   DSL_bf16_t A_US0PsdSup16 : 1;
   /** ADLU-56, Annex A US0 PSDs Preferred Bit 15 */
   DSL_bf16_t A_US0PsdPref15 : 1;
   /** ADLU-52, Annex A US0 PSDs Preferred Bit 14 */
   DSL_bf16_t A_US0PsdPref14 : 1;
   /** ADLU-48, Annex A US0 PSDs Preferred Bit 13 */
   DSL_bf16_t A_US0PsdPref13 : 1;
   /** ADLU-44, Annex A US0 PSDs Preferred Bit 12 */
   DSL_bf16_t A_US0PsdPref12 : 1;
   /** ADLU-40, Annex A US0 PSDs Preferred Bit 11 */
   DSL_bf16_t A_US0PsdPref11 : 1;
   /** ADLU-36, Annex A US0 PSDs Preferred Bit 10 */
   DSL_bf16_t A_US0PsdPref10 : 1;
   /** ADLU-32, Annex A US0 PSDs Preferred Bit 9 */
   DSL_bf16_t A_US0PsdPref9 : 1;
   /** EU-64, Annex A US0 PSDs Preferred Bit 8 */
   DSL_bf16_t A_US0PsdPref8 : 1;
   /** EU-60, Annex A US0 PSDs Preferred Bit 7 */
   DSL_bf16_t A_US0PsdPref7 : 1;
   /** EU-56, Annex A US0 PSDs Preferred Bit 6 */
   DSL_bf16_t A_US0PsdPref6 : 1;
   /** EU-52, Annex A US0 PSDs Preferred Bit 5 */
   DSL_bf16_t A_US0PsdPref5 : 1;
   /** EU-48, Annex A US0 PSDs Preferred Bit 4 */
   DSL_bf16_t A_US0PsdPref4 : 1;
   /** EU-44, Annex A US0 PSDs Preferred Bit 3 */
   DSL_bf16_t A_US0PsdPref3 : 1;
   /** EU-40, Annex A US0 PSDs Preferred Bit 2 */
   DSL_bf16_t A_US0PsdPref2 : 1;
   /** EU-36, Annex A US0 PSDs Preferred Bit 1 */
   DSL_bf16_t A_US0PsdPref1 : 1;
   /** EU-32, Annex A US0 PSDs Preferred Bit 0 */
   DSL_bf16_t A_US0PsdPref0 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 12;
   /** 17a Profile, Annex A US0 PSDs Preferred Bit 19 */
   DSL_bf16_t A_US0PsdPref19 : 1;
   /** 12b Profile, Annex A US0 PSDs Preferred Bit 18 */
   DSL_bf16_t A_US0PsdPref18 : 1;
   /** ADLU-64, Annex A US0 PSDs Preferred Bit 17 */
   DSL_bf16_t A_US0PsdPref17 : 1;
   /** ADLU-60, Annex A US0 PSDs Preferred Bit 16 */
   DSL_bf16_t A_US0PsdPref16 : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 11;
   /** 17a Profile, Annex B US0 PSDs Supported Bit 4 */
   DSL_bf16_t B_US0PsdSup4 : 1;
   /** 12b Profile, Annex B US0 PSDs Supported Bit 3 */
   DSL_bf16_t B_US0PsdSup3 : 1;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Supported Bit 2 */
   DSL_bf16_t B_US0PsdSup2 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Supported Bit 1 */
   DSL_bf16_t B_US0PsdSup1 : 1;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Supported Bit 0 */
   DSL_bf16_t B_US0PsdSup0 : 1;
   /** Reserved */
   DSL_bf16_t Res05 : 13;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Preferred Bit 2 */
   DSL_bf16_t B_US0PsdPref2 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Preferred Bit 1 */
   DSL_bf16_t B_US0PsdPref1 : 1;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Preferred Bit 0 */
   DSL_bf16_t B_US0PsdPref0 : 1;
   /** Reserved for Annex C US0 PSDs Supported */
   DSL_uint16_t Res06;
   /** Reserved for Annex C US0 PSDs Preferred */
   DSL_uint16_t Res07;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** 8a, Profile Support Bit 0 */
   DSL_bf16_t profileSup0 : 1;
   /** 8b, Profile Support Bit 1 */
   DSL_bf16_t profileSup1 : 1;
   /** 8c, Profile Support Bit 2 */
   DSL_bf16_t profileSup2 : 1;
   /** 8d, Profile Support Bit 3 */
   DSL_bf16_t profileSup3 : 1;
   /** 12a, Profile Support Bit 4 */
   DSL_bf16_t profileSup4 : 1;
   /** 12b, Profile Support Bit 5 */
   DSL_bf16_t profileSup5 : 1;
   /** 17a, Profile Support Bit 6 */
   DSL_bf16_t profileSup6 : 1;
   /** 30a, Profile Support Bit 7 */
   DSL_bf16_t profileSup7 : 1;
   /** Reserved */
   DSL_uint8_t Res00;
   /** 8a, Profile Preferred Bit 0 */
   DSL_bf16_t profilePref0 : 1;
   /** 8b, Profile Preferred Bit 1 */
   DSL_bf16_t profilePref1 : 1;
   /** 8c, Profile Preferred Bit 2 */
   DSL_bf16_t profilePref2 : 1;
   /** 8d, Profile Preferred Bit 3 */
   DSL_bf16_t profilePref3 : 1;
   /** 12a, Profile Preferred Bit 4 */
   DSL_bf16_t profilePref4 : 1;
   /** 12b, Profile Preferred Bit 5 */
   DSL_bf16_t profilePref5 : 1;
   /** 17a, Profile Preferred Bit 6 */
   DSL_bf16_t profilePref6 : 1;
   /** 30a, Profile Preferred Bit 7 */
   DSL_bf16_t profilePref7 : 1;
   /** Reserved */
   DSL_uint8_t Res01;
   /** EU-32, Annex A US0 PSDs Supported Bit 0 */
   DSL_bf16_t A_US0PsdSup0 : 1;
   /** EU-36, Annex A US0 PSDs Supported Bit 1 */
   DSL_bf16_t A_US0PsdSup1 : 1;
   /** EU-40, Annex A US0 PSDs Supported Bit 2 */
   DSL_bf16_t A_US0PsdSup2 : 1;
   /** EU-44, Annex A US0 PSDs Supported Bit 3 */
   DSL_bf16_t A_US0PsdSup3 : 1;
   /** EU-48, Annex A US0 PSDs Supported Bit 4 */
   DSL_bf16_t A_US0PsdSup4 : 1;
   /** EU-52, Annex A US0 PSDs Supported Bit 5 */
   DSL_bf16_t A_US0PsdSup5 : 1;
   /** EU-56, Annex A US0 PSDs Supported Bit 6 */
   DSL_bf16_t A_US0PsdSup6 : 1;
   /** EU-60, Annex A US0 PSDs Supported Bit 7 */
   DSL_bf16_t A_US0PsdSup7 : 1;
   /** EU-64, Annex A US0 PSDs Supported Bit 8 */
   DSL_bf16_t A_US0PsdSup8 : 1;
   /** ADLU-32, Annex A US0 PSDs Supported Bit 9 */
   DSL_bf16_t A_US0PsdSup9 : 1;
   /** ADLU-36, Annex A US0 PSDs Supported Bit 10 */
   DSL_bf16_t A_US0PsdSup10 : 1;
   /** ADLU-40, Annex A US0 PSDs Supported Bit 11 */
   DSL_bf16_t A_US0PsdSup11 : 1;
   /** ADLU-44, Annex A US0 PSDs Supported Bit 12 */
   DSL_bf16_t A_US0PsdSup12 : 1;
   /** ADLU-48, Annex A US0 PSDs Supported Bit 13 */
   DSL_bf16_t A_US0PsdSup13 : 1;
   /** ADLU-52, Annex A US0 PSDs Supported Bit 14 */
   DSL_bf16_t A_US0PsdSup14 : 1;
   /** ADLU-56, Annex A US0 PSDs Supported Bit 15 */
   DSL_bf16_t A_US0PsdSup15 : 1;
   /** ADLU-60, Annex A US0 PSDs Supported Bit 16 */
   DSL_bf16_t A_US0PsdSup16 : 1;
   /** ADLU-64, Annex A US0 PSDs Supported Bit 17 */
   DSL_bf16_t A_US0PsdSup17 : 1;
   /** 12b Profile, Annex A US0 PSDs Supported Bit 18 */
   DSL_bf16_t A_US0PsdSup18 : 1;
   /** 17a Profile, Annex A US0 PSDs Supported Bit 19 */
   DSL_bf16_t A_US0PsdSup19 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 12;
   /** EU-32, Annex A US0 PSDs Preferred Bit 0 */
   DSL_bf16_t A_US0PsdPref0 : 1;
   /** EU-36, Annex A US0 PSDs Preferred Bit 1 */
   DSL_bf16_t A_US0PsdPref1 : 1;
   /** EU-40, Annex A US0 PSDs Preferred Bit 2 */
   DSL_bf16_t A_US0PsdPref2 : 1;
   /** EU-44, Annex A US0 PSDs Preferred Bit 3 */
   DSL_bf16_t A_US0PsdPref3 : 1;
   /** EU-48, Annex A US0 PSDs Preferred Bit 4 */
   DSL_bf16_t A_US0PsdPref4 : 1;
   /** EU-52, Annex A US0 PSDs Preferred Bit 5 */
   DSL_bf16_t A_US0PsdPref5 : 1;
   /** EU-56, Annex A US0 PSDs Preferred Bit 6 */
   DSL_bf16_t A_US0PsdPref6 : 1;
   /** EU-60, Annex A US0 PSDs Preferred Bit 7 */
   DSL_bf16_t A_US0PsdPref7 : 1;
   /** EU-64, Annex A US0 PSDs Preferred Bit 8 */
   DSL_bf16_t A_US0PsdPref8 : 1;
   /** ADLU-32, Annex A US0 PSDs Preferred Bit 9 */
   DSL_bf16_t A_US0PsdPref9 : 1;
   /** ADLU-36, Annex A US0 PSDs Preferred Bit 10 */
   DSL_bf16_t A_US0PsdPref10 : 1;
   /** ADLU-40, Annex A US0 PSDs Preferred Bit 11 */
   DSL_bf16_t A_US0PsdPref11 : 1;
   /** ADLU-44, Annex A US0 PSDs Preferred Bit 12 */
   DSL_bf16_t A_US0PsdPref12 : 1;
   /** ADLU-48, Annex A US0 PSDs Preferred Bit 13 */
   DSL_bf16_t A_US0PsdPref13 : 1;
   /** ADLU-52, Annex A US0 PSDs Preferred Bit 14 */
   DSL_bf16_t A_US0PsdPref14 : 1;
   /** ADLU-56, Annex A US0 PSDs Preferred Bit 15 */
   DSL_bf16_t A_US0PsdPref15 : 1;
   /** ADLU-60, Annex A US0 PSDs Preferred Bit 16 */
   DSL_bf16_t A_US0PsdPref16 : 1;
   /** ADLU-64, Annex A US0 PSDs Preferred Bit 17 */
   DSL_bf16_t A_US0PsdPref17 : 1;
   /** 12b Profile, Annex A US0 PSDs Preferred Bit 18 */
   DSL_bf16_t A_US0PsdPref18 : 1;
   /** 17a Profile, Annex A US0 PSDs Preferred Bit 19 */
   DSL_bf16_t A_US0PsdPref19 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 12;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Supported Bit 0 */
   DSL_bf16_t B_US0PsdSup0 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Supported Bit 1 */
   DSL_bf16_t B_US0PsdSup1 : 1;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Supported Bit 2 */
   DSL_bf16_t B_US0PsdSup2 : 1;
   /** 12b Profile, Annex B US0 PSDs Supported Bit 3 */
   DSL_bf16_t B_US0PsdSup3 : 1;
   /** 17a Profile, Annex B US0 PSDs Supported Bit 4 */
   DSL_bf16_t B_US0PsdSup4 : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 11;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Preferred Bit 0 */
   DSL_bf16_t B_US0PsdPref0 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Preferred Bit 1 */
   DSL_bf16_t B_US0PsdPref1 : 1;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Preferred Bit 2 */
   DSL_bf16_t B_US0PsdPref2 : 1;
   /** Reserved */
   DSL_bf16_t Res05 : 13;
   /** Reserved for Annex C US0 PSDs Supported */
   DSL_uint16_t Res06;
   /** Reserved for Annex C US0 PSDs Preferred */
   DSL_uint16_t Res07;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_HS_VDSL2ProfileControl.
*/
struct ACK_HS_VDSL2ProfileControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the VDSL2 far-end Standard Info (See G.994.1 Amendment 4 [6], NPAR(3)
   coding).
*/
struct CMD_HS_StandardInfoFE_VDSL2Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the VDSL2 far-end info as requested by CMD_HS_StandardInfoFE_VDSL2Get (see
   G.994.1 Amendment 4 [6], NPAR(3) coding).
*/
struct ACK_HS_StandardInfoFE_VDSL2Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint8_t Res01;
   /** 30a, Profile Support Bit 7 */
   DSL_uint32_t profileSup7 : 1;
   /** 17a, Profile Support Bit 6 */
   DSL_uint32_t profileSup6 : 1;
   /** 12b, Profile Support Bit 5 */
   DSL_uint32_t profileSup5 : 1;
   /** 12a, Profile Support Bit 4 */
   DSL_uint32_t profileSup4 : 1;
   /** 8d, Profile Support Bit 3 */
   DSL_uint32_t profileSup3 : 1;
   /** 8c, Profile Support Bit 2 */
   DSL_uint32_t profileSup2 : 1;
   /** 8b, Profile Support Bit 1 */
   DSL_uint32_t profileSup1 : 1;
   /** 8a, Profile Support Bit 0 */
   DSL_uint32_t profileSup0 : 1;
   /** ADLU-56, Annex A US0 PSDs Supported Bit 15 */
   DSL_uint32_t A_US0PsdSup15 : 1;
   /** ADLU-52, Annex A US0 PSDs Supported Bit 14 */
   DSL_uint32_t A_US0PsdSup14 : 1;
   /** ADLU-48, Annex A US0 PSDs Supported Bit 13 */
   DSL_uint32_t A_US0PsdSup13 : 1;
   /** ADLU-44, Annex A US0 PSDs Supported Bit 12 */
   DSL_uint32_t A_US0PsdSup12 : 1;
   /** ADLU-40, Annex A US0 PSDs Supported Bit 11 */
   DSL_uint32_t A_US0PsdSup11 : 1;
   /** ADLU-36, Annex A US0 PSDs Supported Bit 10 */
   DSL_uint32_t A_US0PsdSup10 : 1;
   /** ADLU-32, Annex A US0 PSDs Supported Bit 9 */
   DSL_uint32_t A_US0PsdSup9 : 1;
   /** EU-64, Annex A US0 PSDs Supported Bit 8 */
   DSL_uint32_t A_US0PsdSup8 : 1;
   /** EU-60, Annex A US0 PSDs Supported Bit 7 */
   DSL_uint32_t A_US0PsdSup7 : 1;
   /** EU-56, Annex A US0 PSDs Supported Bit 6 */
   DSL_uint32_t A_US0PsdSup6 : 1;
   /** EU-52, Annex A US0 PSDs Supported Bit 5 */
   DSL_uint32_t A_US0PsdSup5 : 1;
   /** EU-48, Annex A US0 PSDs Supported Bit 4 */
   DSL_uint32_t A_US0PsdSup4 : 1;
   /** EU-44, Annex A US0 PSDs Supported Bit 3 */
   DSL_uint32_t A_US0PsdSup3 : 1;
   /** EU-40, Annex A US0 PSDs Supported Bit 2 */
   DSL_uint32_t A_US0PsdSup2 : 1;
   /** EU-36, Annex A US0 PSDs Supported Bit 1 */
   DSL_uint32_t A_US0PsdSup1 : 1;
   /** EU-32, Annex A US0 PSDs Supported Bit 0 */
   DSL_uint32_t A_US0PsdSup0 : 1;
   /** Reserved */
   DSL_uint32_t Res02 : 10;
   DSL_uint32_t A_US0PsdSup21 : 1;
   DSL_uint32_t A_US0PsdSup20 : 1;
   /** 17a Profile, Annex A US0 PSDs Supported Bit 19 */
   DSL_uint32_t A_US0PsdSup19 : 1;
   /** 12b Profile, Annex A US0 PSDs Supported Bit 18 */
   DSL_uint32_t A_US0PsdSup18 : 1;
   /** ADLU-64, Annex A US0 PSDs Supported Bit 17 */
   DSL_uint32_t A_US0PsdSup17 : 1;
   /** ADLU-60, Annex A US0 PSDs Supported Bit 16 */
   DSL_uint32_t A_US0PsdSup16 : 1;
   /** Reserved */
   DSL_uint32_t Res03 : 11;
   /** 17a Profile, Annex B US0 PSDs Supported Bit 4 */
   DSL_uint32_t B_US0PsdSup4 : 1;
   /** 12b Profile, Annex B US0 PSDs Supported Bit 3 */
   DSL_uint32_t B_US0PsdSup3 : 1;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Supported Bit 2 */
   DSL_uint32_t B_US0PsdSup2 : 1;
   /** US0 In 138 to 276 kHz, Annex B US0 PSDs Supported Bit 1 */
   DSL_uint32_t B_US0PsdSup1 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Supported Bit 0 */
   DSL_uint32_t B_US0PsdSup0 : 1;
   /** Reserved */
   DSL_uint32_t Res04 : 2;
   DSL_uint32_t C_US0PsdSup13 : 1;
   DSL_uint32_t C_US0PsdSup12 : 1;
   DSL_uint32_t Res05 : 6;
   DSL_uint32_t C_US0PsdSup5 : 1;
   DSL_uint32_t C_US0PsdSup4 : 1;
   DSL_uint32_t Res06 : 2;
   DSL_uint32_t C_US0PsdSup1 : 1;
   DSL_uint32_t C_US0PsdSup0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** 8a, Profile Support Bit 0 */
   DSL_uint32_t profileSup0 : 1;
   /** 8b, Profile Support Bit 1 */
   DSL_uint32_t profileSup1 : 1;
   /** 8c, Profile Support Bit 2 */
   DSL_uint32_t profileSup2 : 1;
   /** 8d, Profile Support Bit 3 */
   DSL_uint32_t profileSup3 : 1;
   /** 12a, Profile Support Bit 4 */
   DSL_uint32_t profileSup4 : 1;
   /** 12b, Profile Support Bit 5 */
   DSL_uint32_t profileSup5 : 1;
   /** 17a, Profile Support Bit 6 */
   DSL_uint32_t profileSup6 : 1;
   /** 30a, Profile Support Bit 7 */
   DSL_uint32_t profileSup7 : 1;
   /** Reserved */
   DSL_uint8_t Res01;
   /** EU-32, Annex A US0 PSDs Supported Bit 0 */
   DSL_uint32_t A_US0PsdSup0 : 1;
   /** EU-36, Annex A US0 PSDs Supported Bit 1 */
   DSL_uint32_t A_US0PsdSup1 : 1;
   /** EU-40, Annex A US0 PSDs Supported Bit 2 */
   DSL_uint32_t A_US0PsdSup2 : 1;
   /** EU-44, Annex A US0 PSDs Supported Bit 3 */
   DSL_uint32_t A_US0PsdSup3 : 1;
   /** EU-48, Annex A US0 PSDs Supported Bit 4 */
   DSL_uint32_t A_US0PsdSup4 : 1;
   /** EU-52, Annex A US0 PSDs Supported Bit 5 */
   DSL_uint32_t A_US0PsdSup5 : 1;
   /** EU-56, Annex A US0 PSDs Supported Bit 6 */
   DSL_uint32_t A_US0PsdSup6 : 1;
   /** EU-60, Annex A US0 PSDs Supported Bit 7 */
   DSL_uint32_t A_US0PsdSup7 : 1;
   /** EU-64, Annex A US0 PSDs Supported Bit 8 */
   DSL_uint32_t A_US0PsdSup8 : 1;
   /** ADLU-32, Annex A US0 PSDs Supported Bit 9 */
   DSL_uint32_t A_US0PsdSup9 : 1;
   /** ADLU-36, Annex A US0 PSDs Supported Bit 10 */
   DSL_uint32_t A_US0PsdSup10 : 1;
   /** ADLU-40, Annex A US0 PSDs Supported Bit 11 */
   DSL_uint32_t A_US0PsdSup11 : 1;
   /** ADLU-44, Annex A US0 PSDs Supported Bit 12 */
   DSL_uint32_t A_US0PsdSup12 : 1;
   /** ADLU-48, Annex A US0 PSDs Supported Bit 13 */
   DSL_uint32_t A_US0PsdSup13 : 1;
   /** ADLU-52, Annex A US0 PSDs Supported Bit 14 */
   DSL_uint32_t A_US0PsdSup14 : 1;
   /** ADLU-56, Annex A US0 PSDs Supported Bit 15 */
   DSL_uint32_t A_US0PsdSup15 : 1;
   /** ADLU-60, Annex A US0 PSDs Supported Bit 16 */
   DSL_uint32_t A_US0PsdSup16 : 1;
   /** ADLU-64, Annex A US0 PSDs Supported Bit 17 */
   DSL_uint32_t A_US0PsdSup17 : 1;
   /** 12b Profile, Annex A US0 PSDs Supported Bit 18 */
   DSL_uint32_t A_US0PsdSup18 : 1;
   /** 17a Profile, Annex A US0 PSDs Supported Bit 19 */
   DSL_uint32_t A_US0PsdSup19 : 1;
   DSL_uint32_t A_US0PsdSup20 : 1;
   DSL_uint32_t A_US0PsdSup21 : 1;
   /** Reserved */
   DSL_uint32_t Res02 : 10;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Supported Bit 0 */
   DSL_bf16_t B_US0PsdSup0 : 1;
   /** US0 In 138 to 276 kHz, Annex B US0 PSDs Supported Bit 1 */
   DSL_bf16_t B_US0PsdSup1 : 1;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Supported Bit 2 */
   DSL_bf16_t B_US0PsdSup2 : 1;
   /** 12b Profile, Annex B US0 PSDs Supported Bit 3 */
   DSL_bf16_t B_US0PsdSup3 : 1;
   /** 17a Profile, Annex B US0 PSDs Supported Bit 4 */
   DSL_bf16_t B_US0PsdSup4 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 11;
   DSL_uint32_t C_US0PsdSup0 : 1;
   DSL_uint32_t C_US0PsdSup1 : 1;
   DSL_uint32_t Res06 : 2;
   DSL_uint32_t C_US0PsdSup4 : 1;
   DSL_uint32_t C_US0PsdSup5 : 1;
   DSL_uint32_t Res05 : 6;
   DSL_uint32_t C_US0PsdSup12 : 1;
   DSL_uint32_t C_US0PsdSup13 : 1;
#endif
} __PACKED__ ;


/**
   Requests the actual selected VDSL2 Profile (See G.994.1 Amendment 4 [6], NPAR(3)
   coding).
*/
struct CMD_HS_SelectedProfileVDSL2Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the actual selected VDSL2 Profile as requested by
   CMD_HS_SelectedProfileVDSL2Get (see G.994.1 Amendment 4 [6], NPAR(3) coding).
*/
struct ACK_HS_SelectedProfileVDSL2Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint8_t Res01;
   /** 30a, Profile Selected Bit 7 */
   DSL_uint32_t profileSel7 : 1;
   /** 17a, Profile Selected Bit 6 */
   DSL_uint32_t profileSel6 : 1;
   /** 12b, Profile Selected Bit 5 */
   DSL_uint32_t profileSel5 : 1;
   /** 12a, Profile Selected Bit 4 */
   DSL_uint32_t profileSel4 : 1;
   /** 8d, Profile Selected Bit 3 */
   DSL_uint32_t profileSel3 : 1;
   /** 8c, Profile Selected Bit 2 */
   DSL_uint32_t profileSel2 : 1;
   /** 8b, Profile Selected Bit 1 */
   DSL_uint32_t profileSel1 : 1;
   /** 8a, Profile Selected Bit 0 */
   DSL_uint32_t profileSel0 : 1;
   /** ADLU-56, Annex A US0 PSDs Selected Bit  15 */
   DSL_uint32_t A_US0PsdSel15 : 1;
   /** ADLU-52, Annex A US0 PSDs Selected Bit  14 */
   DSL_uint32_t A_US0PsdSel14 : 1;
   /** ADLU-48, Annex A US0 PSDs Selected Bit  13 */
   DSL_uint32_t A_US0PsdSel13 : 1;
   /** ADLU-44, Annex A US0 PSDs Selected Bit  12 */
   DSL_uint32_t A_US0PsdSel12 : 1;
   /** ADLU-40, Annex A US0 PSDs Selected Bit  11 */
   DSL_uint32_t A_US0PsdSel11 : 1;
   /** ADLU-36, Annex A US0 PSDs Selected Bit  10 */
   DSL_uint32_t A_US0PsdSel10 : 1;
   /** ADLU-32, Annex A US0 PSDs Selected Bit  9 */
   DSL_uint32_t A_US0PsdSel9 : 1;
   /** EU-64, Annex A US0 PSDs Selected Bit  8 */
   DSL_uint32_t A_US0PsdSel8 : 1;
   /** EU-60, Annex A US0 PSDs Selected Bit  7 */
   DSL_uint32_t A_US0PsdSel7 : 1;
   /** EU-56, Annex A US0 PSDs Selected Bit  6 */
   DSL_uint32_t A_US0PsdSel6 : 1;
   /** EU-52, Annex A US0 PSDs Selected Bit  5 */
   DSL_uint32_t A_US0PsdSel5 : 1;
   /** EU-48, Annex A US0 PSDs Selected Bit  4 */
   DSL_uint32_t A_US0PsdSel4 : 1;
   /** EU-44, Annex A US0 PSDs Selected Bit  3 */
   DSL_uint32_t A_US0PsdSel3 : 1;
   /** EU-40, Annex A US0 PSDs Selected Bit  2 */
   DSL_uint32_t A_US0PsdSel2 : 1;
   /** EU-36, Annex A US0 PSDs Selected Bit  1 */
   DSL_uint32_t A_US0PsdSel1 : 1;
   /** EU-32, Annex A US0 PSDs Selected Bit  0 */
   DSL_uint32_t A_US0PsdSel0 : 1;
   /** Reserved */
   DSL_uint32_t Res02 : 10;
   /** ADLU-128 Annex A US0 PSDs Selected Bit 21 */
   DSL_uint32_t A_US0PsdSel21 : 1;
   /** EU-128, Annex A US0 PSDs Selected Bit 20 */
   DSL_uint32_t A_US0PsdSel20 : 1;
   /** Reserved */
   DSL_uint32_t Res03 : 2;
   /** ADLU-64, Annex A US0 PSDs Selected Bit  17 */
   DSL_uint32_t A_US0PsdSel17 : 1;
   /** ADLU-60, Annex A US0 PSDs Selected Bit  16 */
   DSL_uint32_t A_US0PsdSel16 : 1;
   /** Reserved */
   DSL_uint32_t Res04 : 13;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Selected Bit 2 */
   DSL_uint32_t B_US0PsdSel2 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Selected Bit 1 */
   DSL_uint32_t B_US0PsdSel1 : 1;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Selected Bit 0 */
   DSL_uint32_t B_US0PsdSel0 : 1;
   /** Reserved */
   DSL_uint32_t Res05 : 10;
   /** US0 In 25 to 276 kHz, Annex C US0 PSDs Selected Bit 5 */
   DSL_uint32_t C_US0PsdSel5 : 1;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Selected Bit 4 */
   DSL_uint32_t C_US0PsdSel4 : 1;
   /** Reserved */
   DSL_uint32_t Res06 : 2;
   /** US0 In 25 to 276kHz, Annex C US0 PSDs Selected Bit 1 */
   DSL_uint32_t C_US0PsdSel1 : 1;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Selected Bit 0 */
   DSL_uint32_t C_US0PsdSel0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** 8a, Profile Selected Bit 0 */
   DSL_uint32_t profileSel0 : 1;
   /** 8b, Profile Selected Bit 1 */
   DSL_uint32_t profileSel1 : 1;
   /** 8c, Profile Selected Bit 2 */
   DSL_uint32_t profileSel2 : 1;
   /** 8d, Profile Selected Bit 3 */
   DSL_uint32_t profileSel3 : 1;
   /** 12a, Profile Selected Bit 4 */
   DSL_uint32_t profileSel4 : 1;
   /** 12b, Profile Selected Bit 5 */
   DSL_uint32_t profileSel5 : 1;
   /** 17a, Profile Selected Bit 6 */
   DSL_uint32_t profileSel6 : 1;
   /** 30a, Profile Selected Bit 7 */
   DSL_uint32_t profileSel7 : 1;
   /** Reserved */
   DSL_uint8_t Res01;
   /** EU-32, Annex A US0 PSDs Selected Bit  0 */
   DSL_uint32_t A_US0PsdSel0 : 1;
   /** EU-36, Annex A US0 PSDs Selected Bit  1 */
   DSL_uint32_t A_US0PsdSel1 : 1;
   /** EU-40, Annex A US0 PSDs Selected Bit  2 */
   DSL_uint32_t A_US0PsdSel2 : 1;
   /** EU-44, Annex A US0 PSDs Selected Bit  3 */
   DSL_uint32_t A_US0PsdSel3 : 1;
   /** EU-48, Annex A US0 PSDs Selected Bit  4 */
   DSL_uint32_t A_US0PsdSel4 : 1;
   /** EU-52, Annex A US0 PSDs Selected Bit  5 */
   DSL_uint32_t A_US0PsdSel5 : 1;
   /** EU-56, Annex A US0 PSDs Selected Bit  6 */
   DSL_uint32_t A_US0PsdSel6 : 1;
   /** EU-60, Annex A US0 PSDs Selected Bit  7 */
   DSL_uint32_t A_US0PsdSel7 : 1;
   /** EU-64, Annex A US0 PSDs Selected Bit  8 */
   DSL_uint32_t A_US0PsdSel8 : 1;
   /** ADLU-32, Annex A US0 PSDs Selected Bit  9 */
   DSL_uint32_t A_US0PsdSel9 : 1;
   /** ADLU-36, Annex A US0 PSDs Selected Bit  10 */
   DSL_uint32_t A_US0PsdSel10 : 1;
   /** ADLU-40, Annex A US0 PSDs Selected Bit  11 */
   DSL_uint32_t A_US0PsdSel11 : 1;
   /** ADLU-44, Annex A US0 PSDs Selected Bit  12 */
   DSL_uint32_t A_US0PsdSel12 : 1;
   /** ADLU-48, Annex A US0 PSDs Selected Bit  13 */
   DSL_uint32_t A_US0PsdSel13 : 1;
   /** ADLU-52, Annex A US0 PSDs Selected Bit  14 */
   DSL_uint32_t A_US0PsdSel14 : 1;
   /** ADLU-56, Annex A US0 PSDs Selected Bit  15 */
   DSL_uint32_t A_US0PsdSel15 : 1;
   /** ADLU-60, Annex A US0 PSDs Selected Bit  16 */
   DSL_uint32_t A_US0PsdSel16 : 1;
   /** ADLU-64, Annex A US0 PSDs Selected Bit  17 */
   DSL_uint32_t A_US0PsdSel17 : 1;
   /** Reserved */
   DSL_uint32_t Res02 : 2;
   /** EU-128, Annex A US0 PSDs Selected Bit 20 */
   DSL_uint32_t A_US0PsdSel20 : 1;
   /** ADLU-128 Annex A US0 PSDs Selected Bit 21 */
   DSL_uint32_t A_US0PsdSel21 : 1;
   /** Reserved */
   DSL_uint32_t Res03 : 10;
   /** US0 In 25 to 138 kHz, Annex B US0 PSDs Selected Bit 0 */
   DSL_uint32_t B_US0PsdSel0 : 1;
   /** US0 In 25 to 276 kHz, Annex B US0 PSDs Selected Bit 1 */
   DSL_uint32_t B_US0PsdSel1 : 1;
   /** US0 In 120 to 276 kHz, Annex B US0 PSDs Selected Bit 2 */
   DSL_uint32_t B_US0PsdSel2 : 1;
   /** Reserved */
   DSL_uint32_t Res04 : 13;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Selected Bit 0 */
   DSL_uint32_t C_US0PsdSel0 : 1;
   /** US0 In 25 to 276kHz, Annex C US0 PSDs Selected Bit 1 */
   DSL_uint32_t C_US0PsdSel1 : 1;
   /** Reserved */
   DSL_uint32_t Res05 : 2;
   /** US0 In 25 to 138 kHz, Annex C US0 PSDs Selected Bit 4 */
   DSL_uint32_t C_US0PsdSel4 : 1;
   /** US0 In 25 to 276 kHz, Annex C US0 PSDs Selected Bit 5 */
   DSL_uint32_t C_US0PsdSel5 : 1;
   /** Reserved */
   DSL_uint32_t Res06 : 10;
#endif
} __PACKED__ ;

/**
   Requests the handshake information about the far-end operating mode (See G.994.1 [4]
   and Amendments 1-4 [5]-[8], standard information fields- SPAR(1) coding).
*/
struct CMD_HS_StandardInfoFE_SPAR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the handshake information about the far-end operating mode (See G.994.1 [4]
   and Amendments 1-4 [5]-[8], Standard information field - SPAR(1) coding). The bit
   numbering per octet below follows the standard (starting with "1").
*/
struct ACK_HS_StandardInfoFE_SPAR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** Octet 2, Bit 7: Committee T1 enhanced SHDSL */
   DSL_bf16_t Oct2B7 : 1;
   /** Octet 2, Bit 6: ETSI SCM VDSL */
   DSL_bf16_t Oct2B6 : 1;
   /** Octet 2, Bit 5: ETSI MCM VDSL */
   DSL_bf16_t Oct2B5 : 1;
   /** Octet 2, Bit 4: T1E1 Trial-use SCM VDSL */
   DSL_bf16_t Oct2B4 : 1;
   /** Octet 2, Bit 3: T1E1 Trial-use MCM VDSL */
   DSL_bf16_t Oct2B3 : 1;
   /** Octet 2, Bit 2: G991.2 Annex B/G */
   DSL_bf16_t Oct2B2 : 1;
   /** Octet 2, Bit 1: G991.2 Annex A/F */
   DSL_bf16_t Oct2B1 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
   /** Octet 1, Bit 7: G992.1 Annex I */
   DSL_bf16_t Oct1B7 : 1;
   /** Octet 1, Bit 6: G992.1 Annex H */
   DSL_bf16_t Oct1B6 : 1;
   /** Octet 1, Bit 5: G992.2 Annex C */
   DSL_bf16_t Oct1B5 : 1;
   /** Octet 1, Bit 4: G992.2 Annex A/B */
   DSL_bf16_t Oct1B4 : 1;
   /** Octet 1, Bit 3: G992.1 Annex C */
   DSL_bf16_t Oct1B3 : 1;
   /** Octet 1, Bit 2: G992.1 Annex B */
   DSL_bf16_t Oct1B2 : 1;
   /** Octet 1, Bit 1: G992.1 Annex A */
   DSL_bf16_t Oct1B1 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 1;
   /** Octet 4, Bit 7: IEEE 802.3ah 2BASE-TS */
   DSL_bf16_t Oct4B7 : 1;
   /** Octet 4, Bit 6: IEEE 802.3ah 2BASE-TL */
   DSL_bf16_t Oct4B6 : 1;
   /** Octet 4, Bit 5: G992.5 Annex J */
   DSL_bf16_t Oct4B5 : 1;
   /** Octet 4, Bit 4: G992.3 Annex M */
   DSL_bf16_t Oct4B4 : 1;
   /** Octet 4, Bit 3: G992.5 Annex I */
   DSL_bf16_t Oct4B3 : 1;
   /** Octet 4, Bit 2: G992.5 Annex B */
   DSL_bf16_t Oct4B2 : 1;
   /** Octet 4, Bit 1: G992.5 Annex A */
   DSL_bf16_t Oct4B1 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 1;
   /** Octet 3, Bit 7: G992.3 Annex C */
   DSL_bf16_t Oct3B7 : 1;
   /** Octet 3, Bit 6: G992.4 Annex I */
   DSL_bf16_t Oct3B6 : 1;
   /** Octet 3, Bit 5: G992.4 Annex A */
   DSL_bf16_t Oct3B5 : 1;
   /** Octet 3, Bit 4: G992.3 Annex J */
   DSL_bf16_t Oct3B4 : 1;
   /** Octet 3, Bit 3: G992.3 Annex I */
   DSL_bf16_t Oct3B3 : 1;
   /** Octet 3, Bit 2: G992.3 Annex B */
   DSL_bf16_t Oct3B2 : 1;
   /** Octet 3, Bit 1: G992.3 Annex A/L */
   DSL_bf16_t Oct3B1 : 1;
   /** Reserved */
   DSL_uint8_t Res04;
   /** Reserved */
   DSL_bf16_t Res05 : 2;
   /** Octet 5, Bit 5: G993.2 */
   DSL_bf16_t Oct5B6 : 1;
   /** Octet 5, Bit 4: G992.5 Annex C */
   DSL_bf16_t Oct5B5 : 1;
   /** Octet 5, Bit 3: Variable Silence Period */
   DSL_bf16_t Oct5B4 : 1;
   /** Octet 5, Bit 2: G992.1 Annex I / T1E1 TRQ 12 */
   DSL_bf16_t Oct5B3 : 1;
   /** Octet 5, Bit 1: G993.1 / ANSI T1.424 */
   DSL_bf16_t Oct5B2 : 1;
   /** Octet 5, Bit 0: G992.5 Annex M */
   DSL_bf16_t Oct5B1 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Octet 1, Bit 1: G992.1 Annex A */
   DSL_bf16_t Oct1B1 : 1;
   /** Octet 1, Bit 2: G992.1 Annex B */
   DSL_bf16_t Oct1B2 : 1;
   /** Octet 1, Bit 3: G992.1 Annex C */
   DSL_bf16_t Oct1B3 : 1;
   /** Octet 1, Bit 4: G992.2 Annex A/B */
   DSL_bf16_t Oct1B4 : 1;
   /** Octet 1, Bit 5: G992.2 Annex C */
   DSL_bf16_t Oct1B5 : 1;
   /** Octet 1, Bit 6: G992.1 Annex H */
   DSL_bf16_t Oct1B6 : 1;
   /** Octet 1, Bit 7: G992.1 Annex I */
   DSL_bf16_t Oct1B7 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** Octet 2, Bit 1: G991.2 Annex A/F */
   DSL_bf16_t Oct2B1 : 1;
   /** Octet 2, Bit 2: G991.2 Annex B/G */
   DSL_bf16_t Oct2B2 : 1;
   /** Octet 2, Bit 3: T1E1 Trial-use MCM VDSL */
   DSL_bf16_t Oct2B3 : 1;
   /** Octet 2, Bit 4: T1E1 Trial-use SCM VDSL */
   DSL_bf16_t Oct2B4 : 1;
   /** Octet 2, Bit 5: ETSI MCM VDSL */
   DSL_bf16_t Oct2B5 : 1;
   /** Octet 2, Bit 6: ETSI SCM VDSL */
   DSL_bf16_t Oct2B6 : 1;
   /** Octet 2, Bit 7: Committee T1 enhanced SHDSL */
   DSL_bf16_t Oct2B7 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
   /** Octet 3, Bit 1: G992.3 Annex A/L */
   DSL_bf16_t Oct3B1 : 1;
   /** Octet 3, Bit 2: G992.3 Annex B */
   DSL_bf16_t Oct3B2 : 1;
   /** Octet 3, Bit 3: G992.3 Annex I */
   DSL_bf16_t Oct3B3 : 1;
   /** Octet 3, Bit 4: G992.3 Annex J */
   DSL_bf16_t Oct3B4 : 1;
   /** Octet 3, Bit 5: G992.4 Annex A */
   DSL_bf16_t Oct3B5 : 1;
   /** Octet 3, Bit 6: G992.4 Annex I */
   DSL_bf16_t Oct3B6 : 1;
   /** Octet 3, Bit 7: G992.3 Annex C */
   DSL_bf16_t Oct3B7 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 1;
   /** Octet 4, Bit 1: G992.5 Annex A */
   DSL_bf16_t Oct4B1 : 1;
   /** Octet 4, Bit 2: G992.5 Annex B */
   DSL_bf16_t Oct4B2 : 1;
   /** Octet 4, Bit 3: G992.5 Annex I */
   DSL_bf16_t Oct4B3 : 1;
   /** Octet 4, Bit 4: G992.3 Annex M */
   DSL_bf16_t Oct4B4 : 1;
   /** Octet 4, Bit 5: G992.5 Annex J */
   DSL_bf16_t Oct4B5 : 1;
   /** Octet 4, Bit 6: IEEE 802.3ah 2BASE-TL */
   DSL_bf16_t Oct4B6 : 1;
   /** Octet 4, Bit 7: IEEE 802.3ah 2BASE-TS */
   DSL_bf16_t Oct4B7 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 1;
   /** Octet 5, Bit 0: G992.5 Annex M */
   DSL_bf16_t Oct5B1 : 1;
   /** Octet 5, Bit 1: G993.1 / ANSI T1.424 */
   DSL_bf16_t Oct5B2 : 1;
   /** Octet 5, Bit 2: G992.1 Annex I / T1E1 TRQ 12 */
   DSL_bf16_t Oct5B3 : 1;
   /** Octet 5, Bit 3: Variable Silence Period */
   DSL_bf16_t Oct5B4 : 1;
   /** Octet 5, Bit 4: G992.5 Annex C */
   DSL_bf16_t Oct5B5 : 1;
   /** Octet 5, Bit 5: G993.2 */
   DSL_bf16_t Oct5B6 : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 2;
   /** Reserved */
   DSL_uint8_t Res05;
#endif
} __PACKED__ ;

/**
   Sets the in System Vendor ID at the xTU-C. (Section 7.4.3 of G997.1 [7])
*/
struct CMD_SysVendorID_O_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorID_O_Set.
*/
struct ACK_SysVendorID_O_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets the in System Vendor ID at the xTU-R. (Section 7.4.4 of G997.1 [7])
*/
struct CMD_SysVendorID_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorID_R_Set.
*/
struct ACK_SysVendorID_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets the in system vendor version number at the xTU-C. (Section 7.4.5 of G997.1
   [7])
*/
struct CMD_SysVendorVersionNum_O_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorVersionNum_O_Set.
*/
struct ACK_SysVendorVersionNum_O_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets the in system vendor version number at the xTU-R. (Section 7.4.6 of G997.1
   [7])
*/
struct CMD_SysVendorVersionNum_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorVersionNum_R_Set.
*/
struct ACK_SysVendorVersionNum_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   For CO side only!
   Sets the in system vendor version number at the xTU-C. (Section 7.4.7 of G997.1
   [7])
*/
struct CMD_SysVendorSerialNum_O_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorSerialNum_O_Set.
*/
struct ACK_SysVendorSerialNum_O_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets the in system vendor serial number at the xTU-R. (Section 7.4.8 of G997.1 [7])
*/
struct CMD_SysVendorSerialNum_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_SysVendorSerialNum_R_Set.
*/
struct ACK_SysVendorSerialNum_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets auxiliary inventory information at the VTU-O (11.2.3.2.4 of G993.2 [3]).
*/
struct CMD_AuxInventoryInfo_O_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_AuxInventoryInfo_O_Set.
*/
struct ACK_AuxInventoryInfo_O_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets auxiliary inventory information at the VTU-R (11.2.3.2.4 of G993.2 [3]).
*/
struct CMD_AuxInventoryInfo_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-R Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-R Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_AuxInventoryInfo_R_Set.
*/
struct ACK_AuxInventoryInfo_R_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the xTU-C Vendor ID. (Section 7.4.1 of G997.1 [7])
*/
struct CMD_VendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the xTU-C Vendor ID as requested by CMD_VendorID_O_Get.
*/
struct ACK_VendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C G.994.1 Vendor ID */
   DSL_uint16_t vendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C G.994.1 Vendor ID */
   DSL_uint16_t vendorID[4];
#endif
} __PACKED__ ;


/**
   Requests the xTU-R Vendor ID. (Section 7.4.2 of G997.1 [7])
*/
struct CMD_VendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the xTU-R Vendor ID as requested by CMD_VendorID_R_Get.
*/
struct ACK_VendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R G.994.1 Vendor ID */
   DSL_uint16_t vendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R G.994.1 Vendor ID */
   DSL_uint16_t vendorID[4];
#endif
} __PACKED__ ;


/**
   Requests the xTU-C System Vendor ID. (Section 7.4.3 of G997.1 [7])
*/
struct CMD_SysVendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-C System Vendor ID as requested by CMD_SysVendorID_O_Get.
*/
struct ACK_SysVendorID_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#endif
} __PACKED__ ;


/**
   Requests the xTU-R System Vendor ID. (Section 7.4.4 of G997.1 [7])
*/
struct CMD_SysVendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-R System Vendor ID as requested by CMD_SysVendorID_R_Get.
*/
struct ACK_SysVendorID_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor ID */
   DSL_uint16_t sysVendorID[4];
#endif
} __PACKED__ ;


/**
   Requests the xTU-C system vendor version number. (Section 7.4.5 of G997.1 [7])
*/
struct CMD_SysVendorVersionNumO_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-C system vendor version number as requested by
   CMD_SysVendorVersionNumO_Get.
*/
struct ACK_SysVendorVersionNum_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#endif
} __PACKED__ ;


/**
   Requests the xTU-R system vendor version number. (Section 7.4.6 of G997.1 [7])
*/
struct CMD_SysVendorVersionNum_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-R system vendor version number as requested by
   CMD_SysVendorVersionNum_R_Get.
*/
struct ACK_SysVendorVersionNum_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Version Number */
   DSL_uint16_t versionNum[8];
#endif
} __PACKED__ ;


/**
   Requests the xTU-C system vendor serial number. (Section 7.4.7 of G997.1 [7])
*/
struct CMD_SysVendorSerialNum_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-C system vendor serial number as requested by
   CMD_SysVendorSerialNum_O_Get.
*/
struct ACK_SysVendorSerialNum_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-C System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#endif
} __PACKED__ ;


/**
   Requests the xTU-R system vendor serial number. (Section 7.4.8 of G997.1 [7])
*/
struct CMD_SysVendorSerialNum_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the xTU-R system vendor version number as requested by
   CMD_SysVendorSerialNum_R_Get.
*/
struct ACK_SysVendorSerialNum_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** xTU-R System Vendor Serial Number */
   DSL_uint16_t serialNum[16];
#endif
} __PACKED__ ;


/**
   Requests VTU-O auxiliary inventory information (11.2.3.2.4 of G993.2 [3]).
*/
struct CMD_AuxInventoryInfo_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the VTU-O auxiliary inventory information as requested by
   CMD_AuxInventoryInfo_O_Get.
*/
struct ACK_AuxInventoryInfo_O_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#endif
} __PACKED__ ;


/**
   Requests VTU-R auxiliary inventory information (11.2.3.2.4 of G993.2 [3]).
*/
struct CMD_AuxInventoryInfo_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the VTU-R auxiliary inventory information as requested by
   CMD_AuxInventoryInfo_R_Get.
*/
struct ACK_AuxInventoryInfo_R_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** VTU-O Auxiliary Inventory Information */
   DSL_uint16_t auxInfo[32];
#endif
} __PACKED__ ;


/**
   Requests the transmission system in use on the line (DSL flavour).
   (Section 7.5.1.1. of G.997.1 [7])
*/
struct CMD_XTSE_StatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the transmission system mode in use as requested by CMD_XTSE_StatusGet.
*/
struct ACK_XTSE_StatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ADSL Mode-Bit15: G.992.5, Annex A */
   DSL_uint32_t A15 : 1;
   /** ADSL Mode-Bit14: G.992.5, Annex B */
   DSL_uint32_t A14 : 1;
   /** ADSL Mode-Bit13: G.992.3, Annex M */
   DSL_uint32_t A13 : 1;
   /** ADSL Mode-Bit12: G.992.3, Annex L, US Mask2 */
   DSL_uint32_t A12 : 1;
   /** ADSL Mode-Bit11: G.992.3, Annex L, US Mask1 */
   DSL_uint32_t A11 : 1;
   /** Reserved */
   DSL_uint32_t Res00 : 1;
   /** ADSL Mode-Bit9: G.992.3, Annex B */
   DSL_uint32_t A9 : 1;
   /** ADSL Mode-Bit8: G.992.3, Annex A */
   DSL_uint32_t A8 : 1;
   /** Reserved */
   DSL_uint32_t Res01 : 4;
   /** ADSL Mode-Bit3: G.992.1, Annex B */
   DSL_uint32_t A3 : 1;
   /** ADSL Mode-Bit2: G.992.1, Annex A */
   DSL_uint32_t A2 : 1;
   /** ADSL Mode-Bit1: G.992.5, Annex M */
   DSL_uint32_t A1 : 1;
   /** ADSL Mode-Bit0: T1.413 */
   DSL_uint32_t A0 : 1;
   /** Reserved */
   DSL_uint32_t Res02 : 13;
   /** VDSL Mode-Bit2: VDSL2 */
   DSL_uint32_t V2 : 1;
   /** Reserved */
   DSL_uint32_t Res03 : 1;
   /** VDSL Mode-Bit0: VDSL1 */
   DSL_uint32_t V0 : 1;
   /** Reserved */
   DSL_uint32_t Res04 : 14;
   /** Line Power Management State */
   DSL_uint32_t LxState : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** ADSL Mode-Bit0: T1.413 */
   DSL_uint32_t A0 : 1;
   /** ADSL Mode-Bit1: G.992.5, Annex M */
   DSL_uint32_t A1 : 1;
   /** ADSL Mode-Bit2: G.992.1, Annex A */
   DSL_uint32_t A2 : 1;
   /** ADSL Mode-Bit3: G.992.1, Annex B */
   DSL_uint32_t A3 : 1;
   /** Reserved */
   DSL_uint32_t Res00 : 4;
   /** ADSL Mode-Bit8: G.992.3, Annex A */
   DSL_uint32_t A8 : 1;
   /** ADSL Mode-Bit9: G.992.3, Annex B */
   DSL_uint32_t A9 : 1;
   /** Reserved */
   DSL_uint32_t Res01 : 1;
   /** ADSL Mode-Bit11: G.992.3, Annex L, US Mask1 */
   DSL_uint32_t A11 : 1;
   /** ADSL Mode-Bit12: G.992.3, Annex L, US Mask2 */
   DSL_uint32_t A12 : 1;
   /** ADSL Mode-Bit13: G.992.3, Annex M */
   DSL_uint32_t A13 : 1;
   /** ADSL Mode-Bit14: G.992.5, Annex B */
   DSL_uint32_t A14 : 1;
   /** ADSL Mode-Bit15: G.992.5, Annex A */
   DSL_uint32_t A15 : 1;
   /** VDSL Mode-Bit0: VDSL1 */
   DSL_uint32_t V0 : 1;
   /** Reserved */
   DSL_uint32_t Res02 : 1;
   /** VDSL Mode-Bit2: VDSL2 */
   DSL_uint32_t V2 : 1;
   /** Reserved */
   DSL_uint32_t Res03 : 13;
   /** Line Power Management State */
   DSL_uint32_t LxState : 2;
   /** Reserved */
   DSL_uint32_t Res04 : 14;
#endif
} __PACKED__ ;


/**
   Reads the used downstream band configuration.
   Note: The reported used DS band configuration reflects the MEDLEYds set as
   defined in the O-PRM message of the channel discovery phase according to
   G.993.2.
*/
struct CMD_BandControl_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_BandControl_DS_Get.
*/
struct ACK_BandControl_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Downstream Bands */
   DSL_uint8_t NumBandsDS;
   /** Band Descriptor DS */
   VNX_ToneIndex_t band[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Downstream Bands */
   DSL_uint8_t NumBandsDS;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Band Descriptor DS */
   VNX_ToneIndex_t band[8];
#endif
} __PACKED__ ;


/**
   Reads the used upstream band configuration.
   Note: The reported used US band configuration reflects the MEDLEYus set as
   defined in the R-PRM message of the channel discovery phase in G.993.2.
*/
struct CMD_BandControl_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_BandControl_US_Get.
*/
struct ACK_BandControl_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Upstream Bands */
   DSL_uint8_t NumBandsDS;
   /** Band Descriptor US */
   VNX_ToneIndex_t band[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Upstream Bands */
   DSL_uint8_t NumBandsDS;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Band Descriptor US */
   VNX_ToneIndex_t band[8];
#endif
} __PACKED__ ;


/**
   Reads the RFI band configuration.
*/
struct CMD_RFI_BandControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_RFI_BandControlGet.
*/
struct ACK_RFI_BandControlGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of RFI Bands */
   DSL_uint8_t NumRfiBands;
   /** RFI Band Descriptor */
   VNX_ToneIndex_t RfiBand[16];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of RFI Bands */
   DSL_uint8_t NumRfiBands;
   /** Reserved */
   DSL_uint8_t Res00;
   /** RFI Band Descriptor */
   VNX_ToneIndex_t RfiBand[16];
#endif
} __PACKED__ ;


/**
   This message requests data about the line status (like noise margin) for downstream
   direction.
*/
struct CMD_LineStatusDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message delivers the line status data for downstream direction requested by
   CMD_LineStatusDS_Get.
*/
struct ACK_LineStatusDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Downstream LATN (for ADSL only) */
   DSL_uint16_t LATNds;
   /** Downstream SATN (for ADSL only) */
   DSL_uint16_t SATNds;
   /** Downstream SNRM */
   DSL_uint16_t SNRMds;
   /** Downstream ATTNDR, LSW */
   DSL_uint16_t ATTNDRds_LSW;
   /** Downstream ATTNDR, MSW */
   DSL_uint16_t ATTNDRds_MSW;
   /** Downstream ACTATP */
   DSL_int16_t ACTATPds;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Reserved */
   DSL_bf16_t Res02 : 7;
   /** Virtual Noise Status DS */
   DSL_bf16_t VNstatus_ds : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 7;
   /** TCM Status DS, LP0 */
   DSL_bf16_t TCMstatus_ds : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Downstream LATN (for ADSL only) */
   DSL_uint16_t LATNds;
   /** Downstream SATN (for ADSL only) */
   DSL_uint16_t SATNds;
   /** Downstream SNRM */
   DSL_uint16_t SNRMds;
   /** Downstream ATTNDR, LSW */
   DSL_uint16_t ATTNDRds_LSW;
   /** Downstream ATTNDR, MSW */
   DSL_uint16_t ATTNDRds_MSW;
   /** Downstream ACTATP */
   DSL_int16_t ACTATPds;
   /** Reserved */
   DSL_uint16_t Res01;
   /** TCM Status DS, LP0 */
   DSL_bf16_t TCMstatus_ds : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 7;
   /** Virtual Noise Status DS */
   DSL_bf16_t VNstatus_ds : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 7;
#endif
} __PACKED__ ;
 

/**
   This message requests data about the upstream line status (like noise margin).
*/
struct CMD_LineStatusUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message delivers the line status data for upstream direction requested by
   CMD_LineStatusUS_Get.
*/
struct ACK_LineStatusUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Upstream LATN (for ADSL only) */
   DSL_uint16_t LATNus;
   /** Upstream SATN (for ADSL only) */
   DSL_uint16_t SATNus;
   /** Upstream SNRM */
   DSL_uint16_t SNRMus;
   /** Upstream ATTNDR, LSW */
   DSL_uint16_t ATTNDRus_LSW;
   /** Upstream ATTNDR, MSW */
   DSL_uint16_t ATTNDRus_MSW;
   /** Upstream ACTATP */
   DSL_int16_t ACTATPus;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Reserved */
   DSL_bf16_t Res02 : 7;
   /** Virtual Noise Status US */
   DSL_bf16_t VNstatus_us : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 7;
   /** TCM Status US, LP0 */
   DSL_bf16_t TCMstatus_us : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Upstream LATN (for ADSL only) */
   DSL_uint16_t LATNus;
   /** Upstream SATN (for ADSL only) */
   DSL_uint16_t SATNus;
   /** Upstream SNRM */
   DSL_uint16_t SNRMus;
   /** Upstream ATTNDR, LSW */
   DSL_uint16_t ATTNDRus_LSW;
   /** Upstream ATTNDR, MSW */
   DSL_uint16_t ATTNDRus_MSW;
   /** Upstream ACTATP */
   DSL_int16_t ACTATPus;
   /** Reserved */
   DSL_uint16_t Res01;
   /** TCM Status US, LP0 */
   DSL_bf16_t TCMstatus_us : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 7;
   /** Virtual Noise Status US */
   DSL_bf16_t VNstatus_us : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 7;
#endif
} __PACKED__ ;
 

/**
   This message requests data about the line status (noise margin, line attenuation and
   signal attenuation) for downstream direction in a "per band" granularity.
*/
struct CMD_LineStatusPerBandDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message delivers the line status data (line attenuation, signal attenuation and
   noise margin) for downstream direction in a "per band" granularity as requested by
   CMD_LineStatusPerBandDS_Get.
*/
struct ACK_LineStatusPerBandDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Line Status Descriptor DS */
   VNX_LineStatus_pb_t LineStatus_pb_ds[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Line Status Descriptor DS */
   VNX_LineStatus_pb_t LineStatus_pb_ds[8];
#endif
} __PACKED__ ;


/**
   This message requests data about the line status (noise margin, line - and signal
   attenuation) for upstream direction in a "per band" granularity.
*/
struct CMD_LineStatusPerBandUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message delivers the line status data (line attenuation, signal attenuation and
   noise margin) for upstream direction in a "per band" granularity as requested by
   CMD_LineStatusPerBandUS_Get.
*/
struct ACK_LineStatusPerBandUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Line Status Descriptor US */
   VNX_LineStatus_pb_t LineStatus_pb_us[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Line Status Descriptor US */
   VNX_LineStatus_pb_t LineStatus_pb_us[8];
#endif
} __PACKED__ ;


/**
   Requests the breakpoints of the actual PSD for the downstream direction.
*/
struct CMD_PSD_BreakpointsDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the breakpoints of the actual PSD for the downstream direction as requested
   by CMD_PSD_BreakpointsDS_Get.
*/
struct ACK_PSD_BreakpointsDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Breakpoint Descriptor DS PSD */
   VNX_PSDbreak_t breakpoint[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Breakpoint Descriptor DS PSD */
   VNX_PSDbreak_t breakpoint[32];
#endif
} __PACKED__ ;


/**
   Requests the breakpoints of the actual PSD for the upstream direction.
*/
struct CMD_PSD_BreakpointsUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the breakpoints of the actual PSD for the upstream direction as requested
   by CMD_PSD_BreakpointsUS_Get.
*/
struct ACK_PSD_BreakpointsUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Breakpoint Descriptor US PSD */
   VNX_PSDbreak_t breakpoint[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Number of Breakpoints */
   DSL_uint8_t NumBreakPts;
   /** Reserved */
   DSL_uint8_t Res00;
   /** Breakpoint Descriptor US PSD */
   VNX_PSDbreak_t breakpoint[32];
#endif
} __PACKED__ ;


/**
   The Downstream TSSI Breakpoints are specified by the transmitter (VTU-O) and
   communicated to the receiver (VTU-R) via the O-PRM message.   The TSSI Breakpoints
   are used to shape the O-P-TEQ signal. This message provides read only access to the
   DS TSSI Breakpoints. The DS TSSI Breakpoints are not configurable via messages.
*/
struct CMD_TssiDS_VDSL2_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement of CMD_TssiDS_VDSL2_Get.
*/
struct ACK_TssiDS_VDSL2_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data from the DS TSSI breakpoint buffer */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data from the DS TSSI breakpoint buffer */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   The Upstream TSSI Breakpoints are specified by the transmitter (VTU-R) and
   communicated to the receiver (VTU-O) via the R-PRM message.   The TSSI Breakpoints
   are used to shape the R-P-TEQ signal.
   Note: The TSSI Breakpoints can be read only but are not configurable via
   messages.
*/
struct CMD_TssiUS_VDSL2_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement of CMD_TssiUS_VDSL2_Get.
*/
struct ACK_TssiUS_VDSL2_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data from the US TSSI breakpoint buffer */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data from the US TSSI breakpoint buffer */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   Reads the actual power cutback value.
*/
struct CMD_ADSL_PCB_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the actual power cutback vaues requested with CMD_ADSL_PCB_Get.
*/
struct ACK_ADSL_PCB_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Downstream Power Cutback */
   DSL_uint16_t dspcb;
   /** Upstream Power Cutback */
   DSL_uint16_t uspcb;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Downstream Power Cutback */
   DSL_uint16_t dspcb;
   /** Upstream Power Cutback */
   DSL_uint16_t uspcb;
#endif
} __PACKED__ ;


/**
   Requests status information for the downstream bearer channels.
*/
struct CMD_BearerChsDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers status information for the downstream bearer channels.
*/
struct ACK_BearerChsDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Net Data Rate DS for LP0, LSW */
   DSL_uint16_t DRdsLP0_LSW;
   /** Net Data Rate DS for LP0, MSW */
   DSL_uint16_t DRdsLP0_MSW;
   /** Reserved */
   DSL_uint16_t Res00[2];
   /** Reserved */
   DSL_uint16_t Res01[4];
   /** Actual Interleaving Delay DS, LP0 */
   DSL_uint16_t delay_dsLP0;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Actual Interleaver Depth DS, LP0 */
   DSL_uint16_t intlDepth_dsLP0;
   /** Reserved */
   DSL_uint16_t Res03;
   /** Reserved */
   DSL_bf16_t Res04 : 7;
   /** TCM Status DS, LP0 */
   DSL_bf16_t TCMstatus_dsLP0 : 1;
   /** Parity Bytes DS, LP0 */
   DSL_bf16_t parity_dsLP0 : 8;
   /** Reserved */
   DSL_uint16_t Res05;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Net Data Rate DS for LP0, LSW */
   DSL_uint16_t DRdsLP0_LSW;
   /** Net Data Rate DS for LP0, MSW */
   DSL_uint16_t DRdsLP0_MSW;
   /** Reserved */
   DSL_uint16_t Res00[2];
   /** Reserved */
   DSL_uint16_t Res01[4];
   /** Actual Interleaving Delay DS, LP0 */
   DSL_uint16_t delay_dsLP0;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Actual Interleaver Depth DS, LP0 */
   DSL_uint16_t intlDepth_dsLP0;
   /** Reserved */
   DSL_uint16_t Res03;
   /** Parity Bytes DS, LP0 */
   DSL_bf16_t parity_dsLP0 : 8;
   /** TCM Status DS, LP0 */
   DSL_bf16_t TCMstatus_dsLP0 : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 7;
   /** Reserved */
   DSL_uint16_t Res05;
#endif
} __PACKED__ ;


/**
   Requests status information for the upstream bearer channels.
*/
struct CMD_BearerChsUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers status information for the upstream bearer channels.
*/
struct ACK_BearerChsUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Net Data Rate US for LP0, LSW */
   DSL_uint16_t DRusLP0_LSW;
   /** Net Data Rate US for LP0, MSW */
   DSL_uint16_t DRusLP0_MSW;
   /** Reserved */
   DSL_uint16_t Res00[2];
   /** Reserved */
   DSL_uint16_t Res01[4];
   /** Actual Interleaving Delay US, LP0 */
   DSL_uint16_t delay_usLP0;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Actual Interleaver Depth US, LP0 */
   DSL_uint16_t intlDepth_usLP0;
   /** Reserved */
   DSL_uint16_t Res03;
   /** Reserved */
   DSL_bf16_t Res04 : 7;
   /** TCM Status US, LP0 */
   DSL_bf16_t TCMstatus_usLP0 : 1;
   /** Parity Bytes US, LP0 */
   DSL_bf16_t parity_usLP0 : 8;
   /** Reserved */
   DSL_uint16_t Res05;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Net Data Rate US for LP0, LSW */
   DSL_uint16_t DRusLP0_LSW;
   /** Net Data Rate US for LP0, MSW */
   DSL_uint16_t DRusLP0_MSW;
   /** Reserved */
   DSL_uint16_t Res00[2];
   /** Reserved */
   DSL_uint16_t Res01[4];
   /** Actual Interleaving Delay US, LP0 */
   DSL_uint16_t delay_usLP0;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Actual Interleaver Depth US, LP0 */
   DSL_uint16_t intlDepth_usLP0;
   /** Reserved */
   DSL_uint16_t Res03;
   /** Parity Bytes US, LP0 */
   DSL_bf16_t parity_usLP0 : 8;
   /** TCM Status US, LP0 */
   DSL_bf16_t TCMstatus_usLP0 : 1;
   /** Reserved */
   DSL_bf16_t Res04 : 7;
   /** Reserved */
   DSL_uint16_t Res05;
#endif
} __PACKED__ ;


/**
   Requests basic VDSL1 frame setting data for the downstream direction.
*/
struct CMD_FrameData1DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers basic VDSL1 framing parameters for the downstream direction requested by
   CMD_FrameData1DS_Get.These parameters are covered in sections 8.5 & 8.4 of G.993.1
   [2].
*/
struct ACK_FrameData1DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data Rate LP0 DS */
   DSL_uint16_t rateLP0ds;
   /** Reserved */
   DSL_uint16_t Res00;
   /** RS-Codeword Size "N" LP0 DS */
   DSL_uint16_t cwSizeLP0ds;
   /** Reserved */
   DSL_uint16_t Res01;
   /** RS Check Bytes "R" LP0 DS */
   DSL_uint16_t RS_ParBytesLP0ds;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Interleaver Block Length "I", LP0 DS */
   DSL_uint16_t IntlBlockLenLP0ds;
   /** Reserved */
   DSL_uint16_t Res03;
   /** Interleaving Depth Parameter "M" LP0 DS */
   DSL_uint16_t IntlDepthLP0ds;
   /** Reserved */
   DSL_uint16_t Res04;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data Rate LP0 DS */
   DSL_uint16_t rateLP0ds;
   /** Reserved */
   DSL_uint16_t Res00;
   /** RS-Codeword Size "N" LP0 DS */
   DSL_uint16_t cwSizeLP0ds;
   /** Reserved */
   DSL_uint16_t Res01;
   /** RS Check Bytes "R" LP0 DS */
   DSL_uint16_t RS_ParBytesLP0ds;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Interleaver Block Length "I", LP0 DS */
   DSL_uint16_t IntlBlockLenLP0ds;
   /** Reserved */
   DSL_uint16_t Res03;
   /** Interleaving Depth Parameter "M" LP0 DS */
   DSL_uint16_t IntlDepthLP0ds;
   /** Reserved */
   DSL_uint16_t Res04;
#endif
} __PACKED__ ;


/**
   Requests basic VDSL1 frame setting data for the upstream direction.
*/
struct CMD_FrameData1US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers basic VDSL1 frame setting data for the upstream direction requested by
   CMD_FrameData1US_Get. These parameters are covered in sections 8.5 & 8.4 of G.993.1
   [2].
*/
struct ACK_FrameData1US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data Rate LP0 US */
   DSL_uint16_t rateLP0us;
   /** Reserved */
   DSL_uint16_t Res00;
   /** RS-Codeword Size "N" LP0 US */
   DSL_uint16_t cwSizeLP0us;
   /** Reserved */
   DSL_uint16_t Res01;
   /** RS Check Bytes "R" LP0 US */
   DSL_uint16_t RS_ParBytesLP0us;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Interleaver Block Length "I", LP0 US */
   DSL_uint16_t IntlBlockLenLP0us;
   /** Reserved */
   DSL_uint16_t Res03;
   /** Interleaving Depth Parameter "M" LP0 US */
   DSL_uint16_t IntlDepthLP0us;
   /** Reserved */
   DSL_uint16_t Res04;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data Rate LP0 US */
   DSL_uint16_t rateLP0us;
   /** Reserved */
   DSL_uint16_t Res00;
   /** RS-Codeword Size "N" LP0 US */
   DSL_uint16_t cwSizeLP0us;
   /** Reserved */
   DSL_uint16_t Res01;
   /** RS Check Bytes "R" LP0 US */
   DSL_uint16_t RS_ParBytesLP0us;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Interleaver Block Length "I", LP0 US */
   DSL_uint16_t IntlBlockLenLP0us;
   /** Reserved */
   DSL_uint16_t Res03;
   /** Interleaving Depth Parameter "M" LP0 US */
   DSL_uint16_t IntlDepthLP0us;
   /** Reserved */
   DSL_uint16_t Res04;
#endif
} __PACKED__ ;


/**
   Requests an extended set of VDSL1 framing parameters for the downstream direction.
*/
struct CMD_FrameDataExt1DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers an extended set of VDSL1 framing parameters for the downstream direction
   requested by CMD_FrameDataExt1DS_Get. (Sections 8.5 & 8.4 of G.993.1 [2])
*/
struct ACK_FrameDataExt1DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00[2];
   /** Data Bytes per Frame "U" LP0 DS */
   DSL_uint16_t U_LP0ds;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Dummy Bytes "Dz" LP0 DS */
   DSL_uint16_t Dz_LP0ds;
   /** Reserved */
   DSL_uint16_t Res02;
   /** OH Bytes "E" LP0 DS */
   DSL_uint16_t E_LP0ds;
   /** Reserved */
   DSL_uint16_t Res03;
   /** RS Dummy Bytes LP0 DS */
   DSL_uint16_t Drs_LP0ds;
   /** Reserved */
   DSL_uint16_t Res04;
   /** RS Info Bytes "K" LP0 DS */
   DSL_uint16_t K_LP0ds;
   /** Reserved */
   DSL_uint16_t Res05;
   /** RS Check Bytes "R" LP0 DS */
   DSL_uint16_t R_LP0ds;
   /** Reserved */
   DSL_uint16_t Res06;
   /** RS Codeword Size "N" LP0 DS */
   DSL_uint16_t N_LP0ds;
   /** Reserved */
   DSL_uint16_t Res07;
   /** Bytes per Symbol "P" LP0 DS */
   DSL_uint16_t P_LP0ds;
   /** Reserved */
   DSL_uint16_t Res08;
   /** Interleaving Depth Parameter "M" LP0 DS */
   DSL_uint16_t M_LP0ds;
   /** Reserved */
   DSL_uint16_t Res09;
   /** Interleaver Block Length "I", LP0 DS */
   DSL_uint16_t I_LP0ds;
   /** Reserved */
   DSL_uint16_t Res10;
   /** Framing Parameter "q", LP0 DS */
   DSL_uint16_t q_LP0ds;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Interleaving Depth "D" LP0 DS */
   DSL_uint16_t D_LP0ds;
   /** Reserved */
   DSL_uint16_t Res12;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00[2];
   /** Data Bytes per Frame "U" LP0 DS */
   DSL_uint16_t U_LP0ds;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Dummy Bytes "Dz" LP0 DS */
   DSL_uint16_t Dz_LP0ds;
   /** Reserved */
   DSL_uint16_t Res02;
   /** OH Bytes "E" LP0 DS */
   DSL_uint16_t E_LP0ds;
   /** Reserved */
   DSL_uint16_t Res03;
   /** RS Dummy Bytes LP0 DS */
   DSL_uint16_t Drs_LP0ds;
   /** Reserved */
   DSL_uint16_t Res04;
   /** RS Info Bytes "K" LP0 DS */
   DSL_uint16_t K_LP0ds;
   /** Reserved */
   DSL_uint16_t Res05;
   /** RS Check Bytes "R" LP0 DS */
   DSL_uint16_t R_LP0ds;
   /** Reserved */
   DSL_uint16_t Res06;
   /** RS Codeword Size "N" LP0 DS */
   DSL_uint16_t N_LP0ds;
   /** Reserved */
   DSL_uint16_t Res07;
   /** Bytes per Symbol "P" LP0 DS */
   DSL_uint16_t P_LP0ds;
   /** Reserved */
   DSL_uint16_t Res08;
   /** Interleaving Depth Parameter "M" LP0 DS */
   DSL_uint16_t M_LP0ds;
   /** Reserved */
   DSL_uint16_t Res09;
   /** Interleaver Block Length "I", LP0 DS */
   DSL_uint16_t I_LP0ds;
   /** Reserved */
   DSL_uint16_t Res10;
   /** Framing Parameter "q", LP0 DS */
   DSL_uint16_t q_LP0ds;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Interleaving Depth "D" LP0 DS */
   DSL_uint16_t D_LP0ds;
   /** Reserved */
   DSL_uint16_t Res12;
#endif
} __PACKED__ ;


/**
   Requests an extended set of VDSL1 framing parameters for the upstream direction.
*/
struct CMD_FrameDataExt1US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers an extended set of VDSL1 framing parameters for the upstream direction
   requested by CMD_FrameDataExt1US_Get. (Sections 8.5 & 8.4 of G.993.1 [2])
*/
struct ACK_FrameDataExt1US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00[2];
   /** Data Bytes per Frame "U" LP0 US */
   DSL_uint16_t U_LP0us;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Dummy Bytes "Dz" LP0 US */
   DSL_uint16_t Dz_LP0us;
   /** Reserved */
   DSL_uint16_t Res02;
   /** OH Bytes LP0 "E" US */
   DSL_uint16_t E_LP0us;
   /** Reserved */
   DSL_uint16_t Res03;
   /** RS Dummy Bytes LP0 US */
   DSL_uint16_t Drs_LP0us;
   /** Reserved */
   DSL_uint16_t Res04;
   /** RS Info Bytes "K" LP0 US */
   DSL_uint16_t K_LP0us;
   /** Reserved */
   DSL_uint16_t Res05;
   /** RS Check Bytes "R" LP0 US */
   DSL_uint16_t R_LP0us;
   /** Reserved */
   DSL_uint16_t Res06;
   /** RS Codeword Size "N" LP0 US */
   DSL_uint16_t N_LP0us;
   /** Reserved */
   DSL_uint16_t Res07;
   /** Bytes per Symbol "P" LP0 US */
   DSL_uint16_t P_LP0us;
   /** Reserved */
   DSL_uint16_t Res08;
   /** Interleaving Depth Parameter "M" LP0 US */
   DSL_uint16_t M_LP0us;
   /** Reserved */
   DSL_uint16_t Res09;
   /** Interleaver Block Length "I", LP0 US */
   DSL_uint16_t I_LP0us;
   /** Reserved */
   DSL_uint16_t Res10;
   /** Framing Parameter "q", LP0 US */
   DSL_uint16_t q_LP0us;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Interleaving Depth "D" LP0 US */
   DSL_uint16_t D_LP0us;
   /** Reserved */
   DSL_uint16_t Res12;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00[2];
   /** Data Bytes per Frame "U" LP0 US */
   DSL_uint16_t U_LP0us;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Dummy Bytes "Dz" LP0 US */
   DSL_uint16_t Dz_LP0us;
   /** Reserved */
   DSL_uint16_t Res02;
   /** OH Bytes LP0 "E" US */
   DSL_uint16_t E_LP0us;
   /** Reserved */
   DSL_uint16_t Res03;
   /** RS Dummy Bytes LP0 US */
   DSL_uint16_t Drs_LP0us;
   /** Reserved */
   DSL_uint16_t Res04;
   /** RS Info Bytes "K" LP0 US */
   DSL_uint16_t K_LP0us;
   /** Reserved */
   DSL_uint16_t Res05;
   /** RS Check Bytes "R" LP0 US */
   DSL_uint16_t R_LP0us;
   /** Reserved */
   DSL_uint16_t Res06;
   /** RS Codeword Size "N" LP0 US */
   DSL_uint16_t N_LP0us;
   /** Reserved */
   DSL_uint16_t Res07;
   /** Bytes per Symbol "P" LP0 US */
   DSL_uint16_t P_LP0us;
   /** Reserved */
   DSL_uint16_t Res08;
   /** Interleaving Depth Parameter "M" LP0 US */
   DSL_uint16_t M_LP0us;
   /** Reserved */
   DSL_uint16_t Res09;
   /** Interleaver Block Length "I", LP0 US */
   DSL_uint16_t I_LP0us;
   /** Reserved */
   DSL_uint16_t Res10;
   /** Framing Parameter "q", LP0 US */
   DSL_uint16_t q_LP0us;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Interleaving Depth "D" LP0 US */
   DSL_uint16_t D_LP0us;
   /** Reserved */
   DSL_uint16_t Res12;
#endif
} __PACKED__ ;


/**
   Requests an extended set of VDSL2 framing parameters for the downstream direction.
*/
struct CMD_FrameDataExt2DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers an extended set of VDSL2 framing parameters for the downstream direction
   requested by CMD_FrameDataExt2DS_Get. (Sections 7.5.2.6 of G.997.1 and 9.5 & 9.6 of
   G.993.2 [3])
*/
struct ACK_FrameDataExt2DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bytes "Bp" from BC0 per MDF LP0 DS */
   DSL_uint16_t B0_LP0ds;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Reserved */
   DSL_uint16_t Res02;
   /** MDFs per OH Sub-Frame "Tp" LP0 DS */
   DSL_uint16_t Tp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res03;
   /** OH Bytes per OH sub-frame "Gp" LP0 DS */
   DSL_uint16_t Gp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res04;
   /** OH Frames per OH Superframe "Fp" LP0 DS */
   DSL_uint16_t Fp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res05;
   /** MDFs per RS-Codeword "Mp" LP0 DS */
   DSL_uint16_t Mp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res06;
   /** Bits per Symbol "Lp" LP0 DS */
   DSL_uint16_t Lp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res07;
   /** RS Check Bytes "Rp" LP0 DS */
   DSL_uint16_t Rp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res08;
   /** Interleaver Block Length "Ip", LP0 DS */
   DSL_uint16_t Ip_LP0ds;
   /** Reserved */
   DSL_uint16_t Res09;
   /** Interleaving Depth "Dp" LP0 DS */
   DSL_uint16_t Dp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res10;
   /** OH Frame Period "PERp", LP0 DS */
   DSL_uint16_t PERp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Line Data Rate DS for LP0, LSW */
   DSL_uint16_t DRp_LP0ds_LSW;
   /** Line Data Rate DS for LP0, MSW */
   DSL_uint16_t DRp_LP0ds_MSW;
   /** Reserved */
   DSL_uint16_t Res12;
   /** Reserved */
   DSL_uint16_t Res13;
   /** RS Codeword Size "N_fecp" LP0 DS */
   DSL_uint16_t N_fecp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res14;
   /** Framing Parameter "q", LP0 DS */
   DSL_uint16_t q_LP0ds;
   /** Reserved */
   DSL_uint16_t Res15;
   /** OH Bytes per OH FrameLP0 "SEQp" DS */
   DSL_uint16_t SEQp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res16;
   /** OH sub-frames "Up" per OH frame LP0 DS */
   DSL_uint16_t Up_LP0ds;
   /** Reserved */
   DSL_uint16_t Res17;
   /** Bytes per OH Frame LP0 DS, LSW */
   DSL_uint16_t PERBp_LP0ds_LSW;
   /** Bytes per OH Frame LP0 DS, MSW */
   DSL_uint16_t PERBp_LP0ds_MSW;
   /** Reserved */
   DSL_uint16_t Res18;
   /** Reserved */
   DSL_uint16_t Res19;
   /** Bytes in MSG OH "MSGc" LP0 DS */
   DSL_uint16_t MSGc_LP0ds;
   /** Reserved */
   DSL_uint16_t Res20;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bytes "Bp" from BC0 per MDF LP0 DS */
   DSL_uint16_t B0_LP0ds;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Reserved */
   DSL_uint16_t Res02;
   /** MDFs per OH Sub-Frame "Tp" LP0 DS */
   DSL_uint16_t Tp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res03;
   /** OH Bytes per OH sub-frame "Gp" LP0 DS */
   DSL_uint16_t Gp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res04;
   /** OH Frames per OH Superframe "Fp" LP0 DS */
   DSL_uint16_t Fp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res05;
   /** MDFs per RS-Codeword "Mp" LP0 DS */
   DSL_uint16_t Mp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res06;
   /** Bits per Symbol "Lp" LP0 DS */
   DSL_uint16_t Lp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res07;
   /** RS Check Bytes "Rp" LP0 DS */
   DSL_uint16_t Rp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res08;
   /** Interleaver Block Length "Ip", LP0 DS */
   DSL_uint16_t Ip_LP0ds;
   /** Reserved */
   DSL_uint16_t Res09;
   /** Interleaving Depth "Dp" LP0 DS */
   DSL_uint16_t Dp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res10;
   /** OH Frame Period "PERp", LP0 DS */
   DSL_uint16_t PERp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Line Data Rate DS for LP0, LSW */
   DSL_uint16_t DRp_LP0ds_LSW;
   /** Line Data Rate DS for LP0, MSW */
   DSL_uint16_t DRp_LP0ds_MSW;
   /** Reserved */
   DSL_uint16_t Res12;
   /** Reserved */
   DSL_uint16_t Res13;
   /** RS Codeword Size "N_fecp" LP0 DS */
   DSL_uint16_t N_fecp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res14;
   /** Framing Parameter "q", LP0 DS */
   DSL_uint16_t q_LP0ds;
   /** Reserved */
   DSL_uint16_t Res15;
   /** OH Bytes per OH FrameLP0 "SEQp" DS */
   DSL_uint16_t SEQp_LP0ds;
   /** Reserved */
   DSL_uint16_t Res16;
   /** OH sub-frames "Up" per OH frame LP0 DS */
   DSL_uint16_t Up_LP0ds;
   /** Reserved */
   DSL_uint16_t Res17;
   /** Bytes per OH Frame LP0 DS, LSW */
   DSL_uint16_t PERBp_LP0ds_LSW;
   /** Bytes per OH Frame LP0 DS, MSW */
   DSL_uint16_t PERBp_LP0ds_MSW;
   /** Reserved */
   DSL_uint16_t Res18;
   /** Reserved */
   DSL_uint16_t Res19;
   /** Bytes in MSG OH "MSGc" LP0 DS */
   DSL_uint16_t MSGc_LP0ds;
   /** Reserved */
   DSL_uint16_t Res20;
#endif
} __PACKED__ ;


/**
   Requests an extended set of VDSL2 framing parameters for the upstream direction.
*/
struct CMD_FrameDataExt2US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers an extended set of VDSL2 framing parameters for the upstream direction
   requested by CMD_FrameDataExt2US_Get. (Sections 7.5.2.6 of G.997.1 and 9.5 & 9.6 of
   G.993.2 [3])
*/
struct ACK_FrameDataExt2US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bytes "Bp" from BC0 per MDF LP0 US */
   DSL_uint16_t B0_LP0us;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Reserved */
   DSL_uint16_t Res02;
   /** MDFs per OH Sub-Frame "Tp" LP0 US */
   DSL_uint16_t Tp_LP0us;
   /** Reserved */
   DSL_uint16_t Res03;
   /** OH Bytes per OH sub-frame "Gp" LP0 US */
   DSL_uint16_t Gp_LP0us;
   /** Reserved */
   DSL_uint16_t Res04;
   /** OH Frames per OH Superframe "Fp" LP0 US */
   DSL_uint16_t Fp_LP0us;
   /** Reserved */
   DSL_uint16_t Res05;
   /** MDFs per RS-Codeword "Mp" LP0 US */
   DSL_uint16_t Mp_LP0us;
   /** Reserved */
   DSL_uint16_t Res06;
   /** Bits per Symbol "Lp" LP0 US */
   DSL_uint16_t Lp_LP0us;
   /** Reserved */
   DSL_uint16_t Res07;
   /** RS Check Bytes "Rp" LP0 US */
   DSL_uint16_t Rp_LP0us;
   /** Reserved */
   DSL_uint16_t Res08;
   /** Interleaver Block Length "Ip", LP0 US */
   DSL_uint16_t Ip_LP0us;
   /** Reserved */
   DSL_uint16_t Res09;
   /** Interleaving Depth "Dp" LP0 US */
   DSL_uint16_t Dp_LP0us;
   /** Reserved */
   DSL_uint16_t Res10;
   /** OH Frame Period "PERp", LP0 US */
   DSL_uint16_t PERp_LP0us;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Line Data Rate US for LP0, LSW */
   DSL_uint16_t DRp_LP0us_LSW;
   /** Line Data Rate US for LP0, MSW */
   DSL_uint16_t DRp_LP0us_MSW;
   /** Reserved */
   DSL_uint16_t Res12;
   /** Reserved */
   DSL_uint16_t Res13;
   /** RS Codeword Size "N_fecp" LP0 US */
   DSL_uint16_t N_fecp_LP0us;
   /** Reserved */
   DSL_uint16_t Res14;
   /** Framing Parameter "q", LP0 US */
   DSL_uint16_t q_LP0us;
   /** Reserved */
   DSL_uint16_t Res15;
   /** OH Bytes per OH FrameLP0 "SEQp" US */
   DSL_uint16_t SEQp_LP0us;
   /** Reserved */
   DSL_uint16_t Res16;
   /** OH sub-frames "Up" per OH frame LP0 US */
   DSL_uint16_t Up_LP0us;
   /** Reserved */
   DSL_uint16_t Res17;
   /** Bytes per OH Frame LP0 US, LSW */
   DSL_uint16_t PERBp_LP0us_LSW;
   /** Bytes per OH Frame LP0 US, MSW */
   DSL_uint16_t PERBp_LP0us_MSW;
   /** Reserved */
   DSL_uint16_t Res18;
   /** Reserved */
   DSL_uint16_t Res19;
   /** Bytes in MSG OH "MSGc" LP0 US */
   DSL_uint16_t MSGc_LP0us;
   /** Reserved */
   DSL_uint16_t Res20;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bytes "Bp" from BC0 per MDF LP0 US */
   DSL_uint16_t B0_LP0us;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Reserved */
   DSL_uint16_t Res02;
   /** MDFs per OH Sub-Frame "Tp" LP0 US */
   DSL_uint16_t Tp_LP0us;
   /** Reserved */
   DSL_uint16_t Res03;
   /** OH Bytes per OH sub-frame "Gp" LP0 US */
   DSL_uint16_t Gp_LP0us;
   /** Reserved */
   DSL_uint16_t Res04;
   /** OH Frames per OH Superframe "Fp" LP0 US */
   DSL_uint16_t Fp_LP0us;
   /** Reserved */
   DSL_uint16_t Res05;
   /** MDFs per RS-Codeword "Mp" LP0 US */
   DSL_uint16_t Mp_LP0us;
   /** Reserved */
   DSL_uint16_t Res06;
   /** Bits per Symbol "Lp" LP0 US */
   DSL_uint16_t Lp_LP0us;
   /** Reserved */
   DSL_uint16_t Res07;
   /** RS Check Bytes "Rp" LP0 US */
   DSL_uint16_t Rp_LP0us;
   /** Reserved */
   DSL_uint16_t Res08;
   /** Interleaver Block Length "Ip", LP0 US */
   DSL_uint16_t Ip_LP0us;
   /** Reserved */
   DSL_uint16_t Res09;
   /** Interleaving Depth "Dp" LP0 US */
   DSL_uint16_t Dp_LP0us;
   /** Reserved */
   DSL_uint16_t Res10;
   /** OH Frame Period "PERp", LP0 US */
   DSL_uint16_t PERp_LP0us;
   /** Reserved */
   DSL_uint16_t Res11;
   /** Line Data Rate US for LP0, LSW */
   DSL_uint16_t DRp_LP0us_LSW;
   /** Line Data Rate US for LP0, MSW */
   DSL_uint16_t DRp_LP0us_MSW;
   /** Reserved */
   DSL_uint16_t Res12;
   /** Reserved */
   DSL_uint16_t Res13;
   /** RS Codeword Size "N_fecp" LP0 US */
   DSL_uint16_t N_fecp_LP0us;
   /** Reserved */
   DSL_uint16_t Res14;
   /** Framing Parameter "q", LP0 US */
   DSL_uint16_t q_LP0us;
   /** Reserved */
   DSL_uint16_t Res15;
   /** OH Bytes per OH FrameLP0 "SEQp" US */
   DSL_uint16_t SEQp_LP0us;
   /** Reserved */
   DSL_uint16_t Res16;
   /** OH sub-frames "Up" per OH frame LP0 US */
   DSL_uint16_t Up_LP0us;
   /** Reserved */
   DSL_uint16_t Res17;
   /** Bytes per OH Frame LP0 US, LSW */
   DSL_uint16_t PERBp_LP0us_LSW;
   /** Bytes per OH Frame LP0 US, MSW */
   DSL_uint16_t PERBp_LP0us_MSW;
   /** Reserved */
   DSL_uint16_t Res18;
   /** Reserved */
   DSL_uint16_t Res19;
   /** Bytes in MSG OH "MSGc" LP0 US */
   DSL_uint16_t MSGc_LP0us;
   /** Reserved */
   DSL_uint16_t Res20;
#endif
} __PACKED__ ;


/**
   Requests ADSL framing parameters for the downstream direction.
*/
struct CMD_ADSL_FrameDataDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the ADSL framing parameters for the downstream direction requested by
   CMD_ADSL_FrameDataDS_Get. (Sections 7.5 & 7.6 of G.992.3 [1])
*/
struct ACK_ADSL_FrameDataDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" LP0 DS */
   DSL_uint16_t Rp;
   /** Symbols per RS-Codeword "Sp" LP0 DS */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" LP0 DS */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" LP0 DS */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" LP0 DS */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" LP0 DS */
   DSL_uint16_t Lp;
   /** TCM Bits LP0 DS */
   DSL_uint16_t TCM;
   /** Framing Mode LP0 DS */
   DSL_uint16_t fraMode;
   /** "Tp" LP0 DS */
   DSL_uint16_t Tp;
   /** "SEQp" LP0 DS */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" DS */
   DSL_uint16_t MSGc;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" LP0 DS */
   DSL_uint16_t Rp;
   /** Symbols per RS-Codeword "Sp" LP0 DS */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" LP0 DS */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" LP0 DS */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" LP0 DS */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" LP0 DS */
   DSL_uint16_t Lp;
   /** TCM Bits LP0 DS */
   DSL_uint16_t TCM;
   /** Framing Mode LP0 DS */
   DSL_uint16_t fraMode;
   /** "Tp" LP0 DS */
   DSL_uint16_t Tp;
   /** "SEQp" LP0 DS */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" DS */
   DSL_uint16_t MSGc;
#endif
} __PACKED__ ;


/**
   Requests ADSL framing parameters for the upstream direction.
*/
struct CMD_ADSL_FrameDataUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Delivers the ADSL framing parameters for the upstream direction requested by
   CMD_ADSL_FrameDataDS_Get. (Sections 7.5 & 7.6 of G.992.3 [1])
*/
struct ACK_ADSL_FrameDataUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" LP0 US */
   DSL_uint16_t Rp_LP0us;
   /** Symbols per RS-Codeword "Sp" LP0 US */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" LP0 US */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" LP0 US */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" LP0 US */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" LP0 US */
   DSL_uint16_t Lp;
   /** TCM Bits LP0 US */
   DSL_uint16_t TCM;
   /** Framing Mode LP0 US */
   DSL_uint16_t fraMode;
   /** "Tp" LP0 US */
   DSL_uint16_t Tp;
   /** "SEQp" LP0 US */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" US */
   DSL_uint16_t MSGc;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** RS Check Bytes "Rp" LP0 US */
   DSL_uint16_t Rp_LP0us;
   /** Symbols per RS-Codeword "Sp" LP0 US */
   DSL_uint16_t Sp;
   /** Interleaving Depth "Dp" LP0 US */
   DSL_uint16_t Dp;
   /** Bytes per MDF "Kp" LP0 US */
   DSL_uint16_t Kp;
   /** MDFs per RS-Codeword "Mp" LP0 US */
   DSL_uint16_t Mp;
   /** Bits per Symbol "Lp" LP0 US */
   DSL_uint16_t Lp;
   /** TCM Bits LP0 US */
   DSL_uint16_t TCM;
   /** Framing Mode LP0 US */
   DSL_uint16_t fraMode;
   /** "Tp" LP0 US */
   DSL_uint16_t Tp;
   /** "SEQp" LP0 US */
   DSL_uint16_t SEQp;
   /** Bytes in MSG-OH "MSGc" US */
   DSL_uint16_t MSGc;
#endif
} __PACKED__ ;


/**
   Controls the behaviour of the performance counters.
*/
struct CMD_TR1ControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TR1 Period */
   DSL_uint16_t period;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** TR1 EVT Enable */
   DSL_bf16_t TR1_EVT_Enable : 1;
   /** TR1 Enable */
   DSL_bf16_t TR1enable : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 13;
   /** TR1 Control */
   DSL_bf16_t TR1Control : 3;
   /** TR1 Adjust */
   DSL_uint16_t TR1Adjust;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TR1 Period */
   DSL_uint16_t period;
   /** TR1 Enable */
   DSL_bf16_t TR1enable : 1;
   /** TR1 EVT Enable */
   DSL_bf16_t TR1_EVT_Enable : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** TR1 Control */
   DSL_bf16_t TR1Control : 3;
   /** Reserved */
   DSL_bf16_t Res01 : 13;
   /** TR1 Adjust */
   DSL_uint16_t TR1Adjust;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_TR1ControlSet.
*/
struct ACK_TR1ControlSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Autonomous message indicating the expiry of a TR1 period.
*/
struct EVT_TR1Expiry
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
#endif
} __PACKED__ ;


/**
   The message configures the thresholds for the "current period" PMD counters. If any
   of the configured thresholds is met within a 15-min TR1 period, an autonomous
   message ALM_TR1CounterThreshHit is sent to the host, provided the ALM generation was
   enabled for it with CMD_TR1ThreshAlarmConfigure.
   (See section 7.3.1.8 of G997.1 [7])
*/
struct CMD_G997_PMD_StatsThreshSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Threshold FECS NE */
   DSL_uint16_t threshFECS_NE;
   /** Threshold ES NE */
   DSL_uint16_t threshES_NE;
   /** Threshold SES NE */
   DSL_uint16_t threshSES_NE;
   /** Threshold LOSS NE */
   DSL_uint16_t threshLOSS_NE;
   /** Threshold UAS NE */
   DSL_uint16_t threshUAS_NE;
   /** Threshold CRC NE LP0, LSW */
   DSL_uint16_t threshCVI_NE_LSW;
   /** Threshold CRC NE LP0, MSW */
   DSL_uint16_t threshCVI_NE_MSW;
   /** Threshold FEC NE LP0, LSW */
   DSL_uint16_t threshECI_NE_LSW;
   /** Threshold FEC NE LP0, MSW */
   DSL_uint16_t threshECI_NE_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Threshold FECS FE */
   DSL_uint16_t threshFECS_FE;
   /** Threshold ES FE */
   DSL_uint16_t threshES_FE;
   /** Threshold SES FE */
   DSL_uint16_t threshSES_FE;
   /** Threshold LOSS FE */
   DSL_uint16_t threshLOSS_FE;
   /** Threshold UAS FE */
   DSL_uint16_t threshUAS_FE;
   /** Threshold CRC FE LP0, LSW */
   DSL_uint16_t threshCVI_FE_LSW;
   /** Threshold CRC FE LP0, MSW */
   DSL_uint16_t threshCVI_FE_MSW;
   /** Threshold FEC FE LP0, LSW */
   DSL_uint16_t threshECI_FE_LSW;
   /** Threshold FEC FE LP0, MSW */
   DSL_uint16_t threshECI_FE_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Threshold FECS NE */
   DSL_uint16_t threshFECS_NE;
   /** Threshold ES NE */
   DSL_uint16_t threshES_NE;
   /** Threshold SES NE */
   DSL_uint16_t threshSES_NE;
   /** Threshold LOSS NE */
   DSL_uint16_t threshLOSS_NE;
   /** Threshold UAS NE */
   DSL_uint16_t threshUAS_NE;
   /** Threshold CRC NE LP0, LSW */
   DSL_uint16_t threshCVI_NE_LSW;
   /** Threshold CRC NE LP0, MSW */
   DSL_uint16_t threshCVI_NE_MSW;
   /** Threshold FEC NE LP0, LSW */
   DSL_uint16_t threshECI_NE_LSW;
   /** Threshold FEC NE LP0, MSW */
   DSL_uint16_t threshECI_NE_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Threshold FECS FE */
   DSL_uint16_t threshFECS_FE;
   /** Threshold ES FE */
   DSL_uint16_t threshES_FE;
   /** Threshold SES FE */
   DSL_uint16_t threshSES_FE;
   /** Threshold LOSS FE */
   DSL_uint16_t threshLOSS_FE;
   /** Threshold UAS FE */
   DSL_uint16_t threshUAS_FE;
   /** Threshold CRC FE LP0, LSW */
   DSL_uint16_t threshCVI_FE_LSW;
   /** Threshold CRC FE LP0, MSW */
   DSL_uint16_t threshCVI_FE_MSW;
   /** Threshold FEC FE LP0, LSW */
   DSL_uint16_t threshECI_FE_LSW;
   /** Threshold FEC FE LP0, MSW */
   DSL_uint16_t threshECI_FE_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_G997_PMD_StatsThreshSet.
*/
struct ACK_G997_PMD_StatsThreshSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the configured threshold values for the PMD counters.
*/
struct CMD_G997_PMD_StatsThreshGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message reports the thresholds configured for the "current period" PMD counters.
   (see also )
*/
struct ACK_G997_PMD_StatsThreshGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Threshold FECS NE */
   DSL_uint16_t threshFECS_NE;
   /** Threshold ES NE */
   DSL_uint16_t threshES_NE;
   /** Threshold SES NE */
   DSL_uint16_t threshSES_NE;
   /** Threshold LOSS NE */
   DSL_uint16_t threshLOSS_NE;
   /** Threshold UAS NE */
   DSL_uint16_t threshUAS_NE;
   /** Threshold CRC NE LP0, LSW */
   DSL_uint16_t threshCVI_NE_LSW;
   /** Threshold CRC NE LP0, MSW */
   DSL_uint16_t threshCVI_NE_MSW;
   /** Threshold FEC NE LP0, LSW */
   DSL_uint16_t threshECI_NE_LSW;
   /** Threshold FEC NE LP0, MSW */
   DSL_uint16_t threshECI_NE_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Threshold FECS FE */
   DSL_uint16_t threshFECS_FE;
   /** Threshold ES FE */
   DSL_uint16_t threshES_FE;
   /** Threshold SES FE */
   DSL_uint16_t threshSES_FE;
   /** Threshold LOSS FE */
   DSL_uint16_t threshLOSS_FE;
   /** Threshold UAS FE */
   DSL_uint16_t threshUAS_FE;
   /** Threshold CRC FE LP0, LSW */
   DSL_uint16_t threshCVI_FE_LSW;
   /** Threshold CRC FE LP0, MSW */
   DSL_uint16_t threshCVI_FE_MSW;
   /** Threshold FEC FE LP0, LSW */
   DSL_uint16_t threshECI_FE_LSW;
   /** Threshold FEC FE LP0, MSW */
   DSL_uint16_t threshECI_FE_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Threshold FECS NE */
   DSL_uint16_t threshFECS_NE;
   /** Threshold ES NE */
   DSL_uint16_t threshES_NE;
   /** Threshold SES NE */
   DSL_uint16_t threshSES_NE;
   /** Threshold LOSS NE */
   DSL_uint16_t threshLOSS_NE;
   /** Threshold UAS NE */
   DSL_uint16_t threshUAS_NE;
   /** Threshold CRC NE LP0, LSW */
   DSL_uint16_t threshCVI_NE_LSW;
   /** Threshold CRC NE LP0, MSW */
   DSL_uint16_t threshCVI_NE_MSW;
   /** Threshold FEC NE LP0, LSW */
   DSL_uint16_t threshECI_NE_LSW;
   /** Threshold FEC NE LP0, MSW */
   DSL_uint16_t threshECI_NE_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Threshold FECS FE */
   DSL_uint16_t threshFECS_FE;
   /** Threshold ES FE */
   DSL_uint16_t threshES_FE;
   /** Threshold SES FE */
   DSL_uint16_t threshSES_FE;
   /** Threshold LOSS FE */
   DSL_uint16_t threshLOSS_FE;
   /** Threshold UAS FE */
   DSL_uint16_t threshUAS_FE;
   /** Threshold CRC FE LP0, LSW */
   DSL_uint16_t threshCVI_FE_LSW;
   /** Threshold CRC FE LP0, MSW */
   DSL_uint16_t threshCVI_FE_MSW;
   /** Threshold FEC FE LP0, LSW */
   DSL_uint16_t threshECI_FE_LSW;
   /** Threshold FEC FE LP0, MSW */
   DSL_uint16_t threshECI_FE_MSW;
#endif
} __PACKED__ ;


/**
   Enables/Disables the generation of ALARM messages (ALM) for specific performance
   counters with TR1 threshold supervision. If the corresponding Enable bit for a
   counter is set, then the modem firmware will send an autonomous message
   ALM_TR1CounterThreshHitif the "current period" counter meets its configured
   threshold.
*/
struct CMD_TR1ThreshAlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** NE Enable Bit15 */
   DSL_bf16_t NE15 : 1;
   /** NE Enable Bit14 */
   DSL_bf16_t NE14 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 7;
   /** NE Enable Bit6 */
   DSL_bf16_t NE6 : 1;
   /** NE Enable Bit5 */
   DSL_bf16_t NE5 : 1;
   /** NE Enable Bit4 */
   DSL_bf16_t NE4 : 1;
   /** NE Enable Bit3 */
   DSL_bf16_t NE3 : 1;
   /** NE Enable Bit2 */
   DSL_bf16_t NE2 : 1;
   /** NE Enable Bit1 */
   DSL_bf16_t NE1 : 1;
   /** NE Enable Bit0 */
   DSL_bf16_t NE0 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** FE Enable Bit6 */
   DSL_bf16_t FE6 : 1;
   /** FE Enable Bit5 */
   DSL_bf16_t FE5 : 1;
   /** FE Enable Bit4 */
   DSL_bf16_t FE4 : 1;
   /** FE Enable Bit3 */
   DSL_bf16_t FE3 : 1;
   /** FE Enable Bit2 */
   DSL_bf16_t FE2 : 1;
   /** FE Enable Bit1 */
   DSL_bf16_t FE1 : 1;
   /** FE Enable Bit0 */
   DSL_bf16_t FE0 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 8;
   /** Near-End HEC-P Latency Path 1 */
   DSL_bf16_t hecp_l1 : 1;
   /** Near-End CD-P Latency Path 1 */
   DSL_bf16_t cdp_l1 : 1;
   /** Near-End CU-P Latency Path 1 */
   DSL_bf16_t cup_l1 : 1;
   /** Near-End IBE-P Latency Path 1 */
   DSL_bf16_t ibep_l1 : 1;
   /** Near-End HEC-P Latency Path 0 */
   DSL_bf16_t hecp_l0 : 1;
   /** Near-End CD-P Latency Path 0 */
   DSL_bf16_t cdp_l0 : 1;
   /** Near-End CU-P Latency Path 0 */
   DSL_bf16_t cup_l0 : 1;
   /** Near-End IBE-P Latency Path 0 */
   DSL_bf16_t ibep_l0 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 8;
   /** Far-End HEC-PFE Latency Path 1 */
   DSL_bf16_t hecpfe_l1 : 1;
   /** Far-End CD-PFE Latency Path 1 */
   DSL_bf16_t cdpfe_l1 : 1;
   /** Far-End CU-PFE Latency Path 1 */
   DSL_bf16_t cupfe_l1 : 1;
   /** Far-End IBE-PFE Latency Path 1 */
   DSL_bf16_t ibepfe_l1 : 1;
   /** Far-End HEC-PFE Latency Path 0 */
   DSL_bf16_t hecpfe_l0 : 1;
   /** Far-End CD-PFE Latency Path 0 */
   DSL_bf16_t cdpfe_l0 : 1;
   /** Far-End CU-PFE Latency Path 0 */
   DSL_bf16_t cupfe_l0 : 1;
   /** Far-End IBE-PFE Latency Path 0 */
   DSL_bf16_t ibepfe_l0 : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** NE Enable Bit0 */
   DSL_bf16_t NE0 : 1;
   /** NE Enable Bit1 */
   DSL_bf16_t NE1 : 1;
   /** NE Enable Bit2 */
   DSL_bf16_t NE2 : 1;
   /** NE Enable Bit3 */
   DSL_bf16_t NE3 : 1;
   /** NE Enable Bit4 */
   DSL_bf16_t NE4 : 1;
   /** NE Enable Bit5 */
   DSL_bf16_t NE5 : 1;
   /** NE Enable Bit6 */
   DSL_bf16_t NE6 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 7;
   /** NE Enable Bit14 */
   DSL_bf16_t NE14 : 1;
   /** NE Enable Bit15 */
   DSL_bf16_t NE15 : 1;
   /** FE Enable Bit0 */
   DSL_bf16_t FE0 : 1;
   /** FE Enable Bit1 */
   DSL_bf16_t FE1 : 1;
   /** FE Enable Bit2 */
   DSL_bf16_t FE2 : 1;
   /** FE Enable Bit3 */
   DSL_bf16_t FE3 : 1;
   /** FE Enable Bit4 */
   DSL_bf16_t FE4 : 1;
   /** FE Enable Bit5 */
   DSL_bf16_t FE5 : 1;
   /** FE Enable Bit6 */
   DSL_bf16_t FE6 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** Near-End IBE-P Latency Path 0 */
   DSL_bf16_t ibep_l0 : 1;
   /** Near-End CU-P Latency Path 0 */
   DSL_bf16_t cup_l0 : 1;
   /** Near-End CD-P Latency Path 0 */
   DSL_bf16_t cdp_l0 : 1;
   /** Near-End HEC-P Latency Path 0 */
   DSL_bf16_t hecp_l0 : 1;
   /** Near-End IBE-P Latency Path 1 */
   DSL_bf16_t ibep_l1 : 1;
   /** Near-End CU-P Latency Path 1 */
   DSL_bf16_t cup_l1 : 1;
   /** Near-End CD-P Latency Path 1 */
   DSL_bf16_t cdp_l1 : 1;
   /** Near-End HEC-P Latency Path 1 */
   DSL_bf16_t hecp_l1 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 8;
   /** Far-End IBE-PFE Latency Path 0 */
   DSL_bf16_t ibepfe_l0 : 1;
   /** Far-End CU-PFE Latency Path 0 */
   DSL_bf16_t cupfe_l0 : 1;
   /** Far-End CD-PFE Latency Path 0 */
   DSL_bf16_t cdpfe_l0 : 1;
   /** Far-End HEC-PFE Latency Path 0 */
   DSL_bf16_t hecpfe_l0 : 1;
   /** Far-End IBE-PFE Latency Path 1 */
   DSL_bf16_t ibepfe_l1 : 1;
   /** Far-End CU-PFE Latency Path 1 */
   DSL_bf16_t cupfe_l1 : 1;
   /** Far-End CD-PFE Latency Path 1 */
   DSL_bf16_t cdpfe_l1 : 1;
   /** Far-End HEC-PFE Latency Path 1 */
   DSL_bf16_t hecpfe_l1 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 8;
#endif
} __PACKED__ ;


/**
   Acknowledgement to CMD_TR1ThreshAlarmConfigure.
*/
struct ACK_TR1ThreshAlarmConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Autonomous message that reports when TR1 thresholds are reached. It is sent if at
   least one of the "current TR1 period" performance counters with enabled threshold
   supervision met its configured threshold.
*/
struct ALM_TR1CounterThreshHit
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TC CRC NE */
   DSL_bf16_t NE15 : 1;
   /** TC CV NE */
   DSL_bf16_t NE14 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 7;
   /** FEC NE */
   DSL_bf16_t NE6 : 1;
   /** CRC NE */
   DSL_bf16_t NE5 : 1;
   /** UAS NE */
   DSL_bf16_t NE4 : 1;
   /** LOSS NE */
   DSL_bf16_t NE3 : 1;
   /** SES NE */
   DSL_bf16_t NE2 : 1;
   /** ES NE */
   DSL_bf16_t NE1 : 1;
   /** FECS NE */
   DSL_bf16_t NE0 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** FEC (ECI) FE */
   DSL_bf16_t FE6 : 1;
   /** FEC (CVI) FE */
   DSL_bf16_t FE5 : 1;
   /** UAS FE */
   DSL_bf16_t FE4 : 1;
   /** LOSS FE */
   DSL_bf16_t FE3 : 1;
   /** SES FE */
   DSL_bf16_t FE2 : 1;
   /** ES FE */
   DSL_bf16_t FE1 : 1;
   /** FECS FE */
   DSL_bf16_t FE0 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 8;
   /** Near-End HEC-P Latency Path 1 */
   DSL_bf16_t hecp_l1 : 1;
   /** Near-End CD-P Latency Path 1 */
   DSL_bf16_t cdp_l1 : 1;
   /** Near-End CU-P Latency Path 1 */
   DSL_bf16_t cup_l1 : 1;
   /** Near-End IBE-P Latency Path 1 */
   DSL_bf16_t ibep_l1 : 1;
   /** Near-End HEC-P Latency Path 0 */
   DSL_bf16_t hecp_l0 : 1;
   /** Near-End CD-P Latency Path 0 */
   DSL_bf16_t cdp_l0 : 1;
   /** Near-End CU-P Latency Path 0 */
   DSL_bf16_t cup_l0 : 1;
   /** Near-End IBE-P Latency Path 0 */
   DSL_bf16_t ibep_l0 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 8;
   /** Far-End HEC-PFE Latency Path 1 */
   DSL_bf16_t hecpfe_l1 : 1;
   /** Far-End CD-PFE Latency Path 1 */
   DSL_bf16_t cdpfe_l1 : 1;
   /** Far-End CU-PFE Latency Path 1 */
   DSL_bf16_t cupfe_l1 : 1;
   /** Far-End IBE-PFE Latency Path 1 */
   DSL_bf16_t ibepfe_l1 : 1;
   /** Far-End HEC-PFE Latency Path 0 */
   DSL_bf16_t hecpfe_l0 : 1;
   /** Far-End CD-PFE Latency Path 0 */
   DSL_bf16_t cdpfe_l0 : 1;
   /** Far-End CU-PFE Latency Path 0 */
   DSL_bf16_t cupfe_l0 : 1;
   /** Far-End IBE-PFE Latency Path 0 */
   DSL_bf16_t ibepfe_l0 : 1;
   /** Time Stamp */
   DSL_uint16_t timestamp;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** FECS NE */
   DSL_bf16_t NE0 : 1;
   /** ES NE */
   DSL_bf16_t NE1 : 1;
   /** SES NE */
   DSL_bf16_t NE2 : 1;
   /** LOSS NE */
   DSL_bf16_t NE3 : 1;
   /** UAS NE */
   DSL_bf16_t NE4 : 1;
   /** CRC NE */
   DSL_bf16_t NE5 : 1;
   /** FEC NE */
   DSL_bf16_t NE6 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 7;
   /** TC CV NE */
   DSL_bf16_t NE14 : 1;
   /** TC CRC NE */
   DSL_bf16_t NE15 : 1;
   /** FECS FE */
   DSL_bf16_t FE0 : 1;
   /** ES FE */
   DSL_bf16_t FE1 : 1;
   /** SES FE */
   DSL_bf16_t FE2 : 1;
   /** LOSS FE */
   DSL_bf16_t FE3 : 1;
   /** UAS FE */
   DSL_bf16_t FE4 : 1;
   /** FEC (CVI) FE */
   DSL_bf16_t FE5 : 1;
   /** FEC (ECI) FE */
   DSL_bf16_t FE6 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** Near-End IBE-P Latency Path 0 */
   DSL_bf16_t ibep_l0 : 1;
   /** Near-End CU-P Latency Path 0 */
   DSL_bf16_t cup_l0 : 1;
   /** Near-End CD-P Latency Path 0 */
   DSL_bf16_t cdp_l0 : 1;
   /** Near-End HEC-P Latency Path 0 */
   DSL_bf16_t hecp_l0 : 1;
   /** Near-End IBE-P Latency Path 1 */
   DSL_bf16_t ibep_l1 : 1;
   /** Near-End CU-P Latency Path 1 */
   DSL_bf16_t cup_l1 : 1;
   /** Near-End CD-P Latency Path 1 */
   DSL_bf16_t cdp_l1 : 1;
   /** Near-End HEC-P Latency Path 1 */
   DSL_bf16_t hecp_l1 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 8;
   /** Far-End IBE-PFE Latency Path 0 */
   DSL_bf16_t ibepfe_l0 : 1;
   /** Far-End CU-PFE Latency Path 0 */
   DSL_bf16_t cupfe_l0 : 1;
   /** Far-End CD-PFE Latency Path 0 */
   DSL_bf16_t cdpfe_l0 : 1;
   /** Far-End HEC-PFE Latency Path 0 */
   DSL_bf16_t hecpfe_l0 : 1;
   /** Far-End IBE-PFE Latency Path 1 */
   DSL_bf16_t ibepfe_l1 : 1;
   /** Far-End CU-PFE Latency Path 1 */
   DSL_bf16_t cupfe_l1 : 1;
   /** Far-End CD-PFE Latency Path 1 */
   DSL_bf16_t cdpfe_l1 : 1;
   /** Far-End HEC-PFE Latency Path 1 */
   DSL_bf16_t hecpfe_l1 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 8;
   /** Time Stamp */
   DSL_uint16_t timestamp;
#endif
} __PACKED__ ;


/**
   Requests the threshold exceedance status of the TR1 performance counters.
*/
struct CMD_TR1CounterThreshHit
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports which TR1 performance counters reached or exceeded their individually
   configurable thresholds.
*/
struct ACK_TR1CounterThreshHit
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** TC CRC NE */
   DSL_bf16_t NE15 : 1;
   /** TC CV NE */
   DSL_bf16_t NE14 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 7;
   /** FEC NE */
   DSL_bf16_t NE6 : 1;
   /** CRC NE */
   DSL_bf16_t NE5 : 1;
   /** UAS NE */
   DSL_bf16_t NE4 : 1;
   /** LOSS NE */
   DSL_bf16_t NE3 : 1;
   /** SES NE */
   DSL_bf16_t NE2 : 1;
   /** ES NE */
   DSL_bf16_t NE1 : 1;
   /** FECS NE */
   DSL_bf16_t NE0 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** FEC (ECI) FE */
   DSL_bf16_t FE6 : 1;
   /** FEC (CVI) FE */
   DSL_bf16_t FE5 : 1;
   /** UAS FE */
   DSL_bf16_t FE4 : 1;
   /** LOSS FE */
   DSL_bf16_t FE3 : 1;
   /** SES FE */
   DSL_bf16_t FE2 : 1;
   /** ES FE */
   DSL_bf16_t FE1 : 1;
   /** FECS FE */
   DSL_bf16_t FE0 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 8;
   /** Near-End HEC-P Latency Path 1 */
   DSL_bf16_t hecp_l1 : 1;
   /** Near-End CD-P Latency Path 1 */
   DSL_bf16_t cdp_l1 : 1;
   /** Near-End CU-P Latency Path 1 */
   DSL_bf16_t cup_l1 : 1;
   /** Near-End IBE-P Latency Path 1 */
   DSL_bf16_t ibep_l1 : 1;
   /** Near-End HEC-P Latency Path 0 */
   DSL_bf16_t hecp_l0 : 1;
   /** Near-End CD-P Latency Path 0 */
   DSL_bf16_t cdp_l0 : 1;
   /** Near-End CU-P Latency Path 0 */
   DSL_bf16_t cup_l0 : 1;
   /** Near-End IBE-P Latency Path 0 */
   DSL_bf16_t ibep_l0 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 8;
   /** Far-End HEC-PFE Latency Path 1 */
   DSL_bf16_t hecpfe_l1 : 1;
   /** Far-End CD-PFE Latency Path 1 */
   DSL_bf16_t cdpfe_l1 : 1;
   /** Far-End CU-PFE Latency Path 1 */
   DSL_bf16_t cupfe_l1 : 1;
   /** Far-End IBE-PFE Latency Path 1 */
   DSL_bf16_t ibepfe_l1 : 1;
   /** Far-End HEC-PFE Latency Path 0 */
   DSL_bf16_t hecpfe_l0 : 1;
   /** Far-End CD-PFE Latency Path 0 */
   DSL_bf16_t cdpfe_l0 : 1;
   /** Far-End CU-PFE Latency Path 0 */
   DSL_bf16_t cupfe_l0 : 1;
   /** Far-End IBE-PFE Latency Path 0 */
   DSL_bf16_t ibepfe_l0 : 1;
   /** Time Stamp */
   DSL_uint16_t timestamp;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** FECS NE */
   DSL_bf16_t NE0 : 1;
   /** ES NE */
   DSL_bf16_t NE1 : 1;
   /** SES NE */
   DSL_bf16_t NE2 : 1;
   /** LOSS NE */
   DSL_bf16_t NE3 : 1;
   /** UAS NE */
   DSL_bf16_t NE4 : 1;
   /** CRC NE */
   DSL_bf16_t NE5 : 1;
   /** FEC NE */
   DSL_bf16_t NE6 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 7;
   /** TC CV NE */
   DSL_bf16_t NE14 : 1;
   /** TC CRC NE */
   DSL_bf16_t NE15 : 1;
   /** FECS FE */
   DSL_bf16_t FE0 : 1;
   /** ES FE */
   DSL_bf16_t FE1 : 1;
   /** SES FE */
   DSL_bf16_t FE2 : 1;
   /** LOSS FE */
   DSL_bf16_t FE3 : 1;
   /** UAS FE */
   DSL_bf16_t FE4 : 1;
   /** FEC (CVI) FE */
   DSL_bf16_t FE5 : 1;
   /** FEC (ECI) FE */
   DSL_bf16_t FE6 : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** Near-End IBE-P Latency Path 0 */
   DSL_bf16_t ibep_l0 : 1;
   /** Near-End CU-P Latency Path 0 */
   DSL_bf16_t cup_l0 : 1;
   /** Near-End CD-P Latency Path 0 */
   DSL_bf16_t cdp_l0 : 1;
   /** Near-End HEC-P Latency Path 0 */
   DSL_bf16_t hecp_l0 : 1;
   /** Near-End IBE-P Latency Path 1 */
   DSL_bf16_t ibep_l1 : 1;
   /** Near-End CU-P Latency Path 1 */
   DSL_bf16_t cup_l1 : 1;
   /** Near-End CD-P Latency Path 1 */
   DSL_bf16_t cdp_l1 : 1;
   /** Near-End HEC-P Latency Path 1 */
   DSL_bf16_t hecp_l1 : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 8;
   /** Far-End IBE-PFE Latency Path 0 */
   DSL_bf16_t ibepfe_l0 : 1;
   /** Far-End CU-PFE Latency Path 0 */
   DSL_bf16_t cupfe_l0 : 1;
   /** Far-End CD-PFE Latency Path 0 */
   DSL_bf16_t cdpfe_l0 : 1;
   /** Far-End HEC-PFE Latency Path 0 */
   DSL_bf16_t hecpfe_l0 : 1;
   /** Far-End IBE-PFE Latency Path 1 */
   DSL_bf16_t ibepfe_l1 : 1;
   /** Far-End CU-PFE Latency Path 1 */
   DSL_bf16_t cupfe_l1 : 1;
   /** Far-End CD-PFE Latency Path 1 */
   DSL_bf16_t cdpfe_l1 : 1;
   /** Far-End HEC-PFE Latency Path 1 */
   DSL_bf16_t hecpfe_l1 : 1;
   /** Reserved */
   DSL_bf16_t Res03 : 8;
   /** Time Stamp */
   DSL_uint16_t timestamp;
#endif
} __PACKED__ ;


/**
   This autonomous message indicates near-end line failures.
*/
struct ALM_LineFailureNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 2;
   /** ESE Failure NE */
   DSL_bf16_t ESE_NE : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** LOM Failure NE */
   DSL_bf16_t LOM_NE : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 1;
   /** LOF Failure NE */
   DSL_bf16_t LOF_NE : 1;
   /** LOS Failure NE */
   DSL_bf16_t LOS_NE : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS Failure NE */
   DSL_bf16_t LOS_NE : 1;
   /** LOF Failure NE */
   DSL_bf16_t LOF_NE : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** LOM Failure NE */
   DSL_bf16_t LOM_NE : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 9;
   /** ESE Failure NE */
   DSL_bf16_t ESE_NE : 1;
   /** Reserved */
   DSL_bf16_t Res02 : 2;
#endif
} __PACKED__ ;


/**
   Requests the near-end line failure status.
*/
struct CMD_LineFailureNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the near-end line failure status requested by the command
   CMD_LineFailureNE_Get.
*/
struct ACK_LineFailureNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** PRO Failure NE */
   DSL_bf16_t PRO_NE : 1;
   /** ESE Failure NE */
   DSL_bf16_t ESE_NE : 1;
   /** LOL Failure NE */
   DSL_bf16_t LOL_NE : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 8;
   /** LOM Failure NE */
   DSL_bf16_t LOM_NE : 1;
   /** LPR Failure NE */
   DSL_bf16_t LPR_NE : 1;
   /** LOF Failure NE */
   DSL_bf16_t LOF_NE : 1;
   /** LOS Failure NE */
   DSL_bf16_t LOS_NE : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS Failure NE */
   DSL_bf16_t LOS_NE : 1;
   /** LOF Failure NE */
   DSL_bf16_t LOF_NE : 1;
   /** LPR Failure NE */
   DSL_bf16_t LPR_NE : 1;
   /** LOM Failure NE */
   DSL_bf16_t LOM_NE : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 8;
   /** LOL Failure NE */
   DSL_bf16_t LOL_NE : 1;
   /** ESE Failure NE */
   DSL_bf16_t ESE_NE : 1;
   /** PRO Failure NE */
   DSL_bf16_t PRO_NE : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
#endif
} __PACKED__ ;


/**
   This autonomous message indicates far-end line failures.
*/
struct ALM_LineFailureFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 12;
   /** LOM Failure FE */
   DSL_bf16_t LOM_FE : 1;
   /** LPR Failure FE */
   DSL_bf16_t LPR_FE : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 1;
   /** LOS Failure FE */
   DSL_bf16_t LOS_FE : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS Failure FE */
   DSL_bf16_t LOS_FE : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 1;
   /** LPR Failure FE */
   DSL_bf16_t LPR_FE : 1;
   /** LOM Failure FE */
   DSL_bf16_t LOM_FE : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 12;
#endif
} __PACKED__ ;


/**
   Requests the far-end line failure status.
*/
struct CMD_LineFailureFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the far-end line failure status requested by the command
   CMD_LineFailureFE_Get.
*/
struct ACK_LineFailureFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 2;
   /** ESE Failure FE */
   DSL_bf16_t ESE_FE : 1;
   /** LOL Failure FE */
   DSL_bf16_t LOL_FE : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 8;
   /** LOM Failure FE */
   DSL_bf16_t LOM_FE : 1;
   /** LPR Failure FE */
   DSL_bf16_t LPR_FE : 1;
   /** LOF Failure FE */
   DSL_bf16_t LOF_FE : 1;
   /** LOS Failure FE */
   DSL_bf16_t LOS_FE : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** LOS Failure FE */
   DSL_bf16_t LOS_FE : 1;
   /** LOF Failure FE */
   DSL_bf16_t LOF_FE : 1;
   /** LPR Failure FE */
   DSL_bf16_t LPR_FE : 1;
   /** LOM Failure FE */
   DSL_bf16_t LOM_FE : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 8;
   /** LOL Failure FE */
   DSL_bf16_t LOL_FE : 1;
   /** ESE Failure FE */
   DSL_bf16_t ESE_FE : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 2;
#endif
} __PACKED__ ;


/**
   Requests near-end line performance counter values (Section 7.2.1.1 of G.997.1) for
   the current and for the previous 15-min-TR1 period.
*/
struct CMD_LinePerfCountNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides near-end line performance counter values requested by
   CMD_LinePerfCountNE_Get (Section 7.2.1.1 of G.997.1). Contains counter values for
   the current and for the previous 15-min-TR1 interval.
*/
struct ACK_LinePerfCountNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FECS Count, LSW, Current Period */
   DSL_uint16_t cntFECS_NE_LSW;
   /** Near-End FECS Count, MSW, Current Period */
   DSL_uint16_t cntFECS_NE_MSW;
   /** Near-End ES Count, LSW, Current Period */
   DSL_uint16_t cntES_NE_LSW;
   /** Near-End ES Count, MSW, Current Period */
   DSL_uint16_t cntES_NE_MSW;
   /** Near-End SES Count, LSW, Current Period */
   DSL_uint16_t cntSES_NE_LSW;
   /** Near-End SES Count, MSW, Current Period */
   DSL_uint16_t cntSES_NE_MSW;
   /** Near-End LOSS Count, LSW, Current Period */
   DSL_uint16_t cntLOSS_NE_LSW;
   /** Near-End LOSS Count, MSW, Current Period */
   DSL_uint16_t cntLOSS_NE_MSW;
   /** Near-End UAS Count, LSW, Current Period */
   DSL_uint16_t cntUAS_NE_LSW;
   /** Near-End UAS Count, MSW, Current Period */
   DSL_uint16_t cntUAS_NE_MSW;
   /** Near-End FECS Count, LSW, Previous Period */
   DSL_uint16_t cntFECS_NE_LSW_prev;
   /** Near-End FECS Count, MSW, Previous Period */
   DSL_uint16_t cntFECS_NE_MSW_prev;
   /** Near-End ES Count, LSW, Previous Period */
   DSL_uint16_t cntES_NE_LSW_prev;
   /** Near-End ES Count, MSW, Previous Period */
   DSL_uint16_t cntES_NE_MSW_prev;
   /** Near-End SES Count, LSW, Previous Period */
   DSL_uint16_t cntSES_NE_LSW_prev;
   /** Near-End SES Count, MSW, Previous Period */
   DSL_uint16_t cntSES_NE_MSW_prev;
   /** Near-End LOSS Count, LSW, Previous Period */
   DSL_uint16_t cntLOSS_NE_LSW_prev;
   /** Near-End LOSS Count, MSW, Previous Period */
   DSL_uint16_t cntLOSS_NE_MSW_prev;
   /** Near-End UAS Count, LSW, Previous Period */
   DSL_uint16_t cntUAS_NE_LSW_prev;
   /** Near-End UAS Count, MSW, Previous Period */
   DSL_uint16_t cntUAS_NE_MSW_prev;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FECS Count, LSW, Current Period */
   DSL_uint16_t cntFECS_NE_LSW;
   /** Near-End FECS Count, MSW, Current Period */
   DSL_uint16_t cntFECS_NE_MSW;
   /** Near-End ES Count, LSW, Current Period */
   DSL_uint16_t cntES_NE_LSW;
   /** Near-End ES Count, MSW, Current Period */
   DSL_uint16_t cntES_NE_MSW;
   /** Near-End SES Count, LSW, Current Period */
   DSL_uint16_t cntSES_NE_LSW;
   /** Near-End SES Count, MSW, Current Period */
   DSL_uint16_t cntSES_NE_MSW;
   /** Near-End LOSS Count, LSW, Current Period */
   DSL_uint16_t cntLOSS_NE_LSW;
   /** Near-End LOSS Count, MSW, Current Period */
   DSL_uint16_t cntLOSS_NE_MSW;
   /** Near-End UAS Count, LSW, Current Period */
   DSL_uint16_t cntUAS_NE_LSW;
   /** Near-End UAS Count, MSW, Current Period */
   DSL_uint16_t cntUAS_NE_MSW;
   /** Near-End FECS Count, LSW, Previous Period */
   DSL_uint16_t cntFECS_NE_LSW_prev;
   /** Near-End FECS Count, MSW, Previous Period */
   DSL_uint16_t cntFECS_NE_MSW_prev;
   /** Near-End ES Count, LSW, Previous Period */
   DSL_uint16_t cntES_NE_LSW_prev;
   /** Near-End ES Count, MSW, Previous Period */
   DSL_uint16_t cntES_NE_MSW_prev;
   /** Near-End SES Count, LSW, Previous Period */
   DSL_uint16_t cntSES_NE_LSW_prev;
   /** Near-End SES Count, MSW, Previous Period */
   DSL_uint16_t cntSES_NE_MSW_prev;
   /** Near-End LOSS Count, LSW, Previous Period */
   DSL_uint16_t cntLOSS_NE_LSW_prev;
   /** Near-End LOSS Count, MSW, Previous Period */
   DSL_uint16_t cntLOSS_NE_MSW_prev;
   /** Near-End UAS Count, LSW, Previous Period */
   DSL_uint16_t cntUAS_NE_LSW_prev;
   /** Near-End UAS Count, MSW, Previous Period */
   DSL_uint16_t cntUAS_NE_MSW_prev;
#endif
} __PACKED__ ;


/**
   Writes near-end line performance counter values (Section 7.2.1.1 of G.997.1) 
   for the current and for the previous 15-min-TR1 period.
*/
struct CMD_LinePerfCountNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End FECS Count, LSW, Current Period */
   DSL_uint16_t cntFECS_NE_LSW;
   /* Near-End FECS Count, MSW, Current Period */
   DSL_uint16_t cntFECS_NE_MSW;
   /* Near-End ES Count, LSW, Current Period */
   DSL_uint16_t cntES_NE_LSW;
   /* Near-End ES Count, MSW, Current Period */
   DSL_uint16_t cntES_NE_MSW;
   /* Near-End SES Count, LSW, Current Period */
   DSL_uint16_t cntSES_NE_LSW;
   /* Near-End SES Count, MSW, Current Period */
   DSL_uint16_t cntSES_NE_MSW;
   /* Near-End LOSS Count, LSW, Current Period */
   DSL_uint16_t cntLOSS_NE_LSW;
   /* Near-End LOSS Count, MSW, Current Period */
   DSL_uint16_t cntLOSS_NE_MSW;
   /* Near-End UAS Count, LSW, Current Period */
   DSL_uint16_t cntUAS_NE_LSW;
   /* Near-End UAS Count, MSW, Current Period */
   DSL_uint16_t cntUAS_NE_MSW;
   /* Near-End FECS Count, LSW, Previous Period */
   DSL_uint16_t cntFECS_NE_LSW_prev;
   /* Near-End FECS Count, MSW, Previous Period */
   DSL_uint16_t cntFECS_NE_MSW_prev;
   /* Near-End ES Count, LSW, Previous Period */
   DSL_uint16_t cntES_NE_LSW_prev;
   /* Near-End ES Count, MSW, Previous Period */
   DSL_uint16_t cntES_NE_MSW_prev;
   /* Near-End SES Count, LSW, Previous Period */
   DSL_uint16_t cntSES_NE_LSW_prev;
   /* Near-End SES Count, MSW, Previous Period */
   DSL_uint16_t cntSES_NE_MSW_prev;
   /* Near-End LOSS Count, LSW, Previous Period */
   DSL_uint16_t cntLOSS_NE_LSW_prev;
   /* Near-End LOSS Count, MSW, Previous Period */
   DSL_uint16_t cntLOSS_NE_MSW_prev;
   /* Near-End UAS Count, LSW, Previous Period */
   DSL_uint16_t cntUAS_NE_LSW_prev;
   /* Near-End UAS Count, MSW, Previous Period */
   DSL_uint16_t cntUAS_NE_MSW_prev;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End FECS Count, LSW, Current Period */
   DSL_uint16_t cntFECS_NE_LSW;
   /* Near-End FECS Count, MSW, Current Period */
   DSL_uint16_t cntFECS_NE_MSW;
   /* Near-End ES Count, LSW, Current Period */
   DSL_uint16_t cntES_NE_LSW;
   /* Near-End ES Count, MSW, Current Period */
   DSL_uint16_t cntES_NE_MSW;
   /* Near-End SES Count, LSW, Current Period */
   DSL_uint16_t cntSES_NE_LSW;
   /* Near-End SES Count, MSW, Current Period */
   DSL_uint16_t cntSES_NE_MSW;
   /* Near-End LOSS Count, LSW, Current Period */
   DSL_uint16_t cntLOSS_NE_LSW;
   /* Near-End LOSS Count, MSW, Current Period */
   DSL_uint16_t cntLOSS_NE_MSW;
   /* Near-End UAS Count, LSW, Current Period */
   DSL_uint16_t cntUAS_NE_LSW;
   /* Near-End UAS Count, MSW, Current Period */
   DSL_uint16_t cntUAS_NE_MSW;
   /* Near-End FECS Count, LSW, Previous Period */
   DSL_uint16_t cntFECS_NE_LSW_prev;
   /* Near-End FECS Count, MSW, Previous Period */
   DSL_uint16_t cntFECS_NE_MSW_prev;
   /* Near-End ES Count, LSW, Previous Period */
   DSL_uint16_t cntES_NE_LSW_prev;
   /* Near-End ES Count, MSW, Previous Period */
   DSL_uint16_t cntES_NE_MSW_prev;
   /* Near-End SES Count, LSW, Previous Period */
   DSL_uint16_t cntSES_NE_LSW_prev;
   /* Near-End SES Count, MSW, Previous Period */
   DSL_uint16_t cntSES_NE_MSW_prev;
   /* Near-End LOSS Count, LSW, Previous Period */
   DSL_uint16_t cntLOSS_NE_LSW_prev;
   /* Near-End LOSS Count, MSW, Previous Period */
   DSL_uint16_t cntLOSS_NE_MSW_prev;
   /* Near-End UAS Count, LSW, Previous Period */
   DSL_uint16_t cntUAS_NE_LSW_prev;
   /* Near-End UAS Count, MSW, Previous Period */
   DSL_uint16_t cntUAS_NE_MSW_prev;
#endif
} __PACKED__;

/**
   Provides near-end line performance counter values requested by CMD_LinePerfCountNE_Set 
   (Section 7.2.1.1 of G.997.1). Contains counter values for the current and for 
   the previous 15-min-TR1 interval.
*/
struct ACK_LinePerfCountNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length*/
   DSL_uint16_t Length;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length*/
   DSL_uint16_t Length;
#endif
} __PACKED__;

/**
   Requests far-end line performance counter values (Section 7.2.1.2 of G.997.1) for
   the current and for the previous 15-min-TR1 period.
*/
struct CMD_LinePerfCountFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides far-end line performance counter values requested by
   CMD_LinePerfCountFE_Get (Section 7.2.1.2 of G.997.1). Contains counter values for
   the current and for the previous 15-min-TR1 interval.
*/
struct ACK_LinePerfCountFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End FECS Count, LSW, Current Period */
   DSL_uint16_t cntFECS_FE_LSW;
   /** Far-End FECS Count, MSW, Current Period */
   DSL_uint16_t cntFECS_FE_MSW;
   /** Far-End ES Count, LSW, Current Period */
   DSL_uint16_t cntES_FE_LSW;
   /** Far-End ES Count, MSW, Current Period */
   DSL_uint16_t cntES_FE_MSW;
   /** Far-End SES Count, LSW, Current Period */
   DSL_uint16_t cntSES_FE_LSW;
   /** Far-End SES Count, MSW, Current Period */
   DSL_uint16_t cntSES_FE_MSW;
   /** Far-End LOSS Count, LSW, Current Period */
   DSL_uint16_t cntLOSS_FE_LSW;
   /** Far-End LOSS Count, MSW, Current Period */
   DSL_uint16_t cntLOSS_FE_MSW;
   /** Far-End UAS Count, LSW, Current Period */
   DSL_uint16_t cntUAS_FE_LSW;
   /** Far-End UAS Count, MSW, Current Period */
   DSL_uint16_t cntUAS_FE_MSW;
   /** Far-End FECS Count, LSW, Previous Period */
   DSL_uint16_t cntFECS_FE_LSW_prev;
   /** Far-End FECS Count, MSW, Previous Period */
   DSL_uint16_t cntFECS_FE_MSW_prev;
   /** Far-End ES Count, LSW, Previous Period */
   DSL_uint16_t cntES_FE_LSW_prev;
   /** Far-End ES Count, MSW, Previous Period */
   DSL_uint16_t cntES_FE_MSW_prev;
   /** Far-End SES Count, LSW, Previous Period */
   DSL_uint16_t cntSES_FE_LSW_prev;
   /** Far-End SES Count, MSW, Previous Period */
   DSL_uint16_t cntSES_FE_MSW_prev;
   /** Far-End LOSS Count, LSW, Previous Period */
   DSL_uint16_t cntLOSS_FE_LSW_prev;
   /** Far-End LOSS Count, MSW, Previous Period */
   DSL_uint16_t cntLOSS_FE_MSW_prev;
   /** Far-End UAS Count, LSW, Previous Period */
   DSL_uint16_t cntUAS_FE_LSW_prev;
   /** Far-End UAS Count, MSW, Previous Period */
   DSL_uint16_t cntUAS_FE_MSW_prev;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End FECS Count, LSW, Current Period */
   DSL_uint16_t cntFECS_FE_LSW;
   /** Far-End FECS Count, MSW, Current Period */
   DSL_uint16_t cntFECS_FE_MSW;
   /** Far-End ES Count, LSW, Current Period */
   DSL_uint16_t cntES_FE_LSW;
   /** Far-End ES Count, MSW, Current Period */
   DSL_uint16_t cntES_FE_MSW;
   /** Far-End SES Count, LSW, Current Period */
   DSL_uint16_t cntSES_FE_LSW;
   /** Far-End SES Count, MSW, Current Period */
   DSL_uint16_t cntSES_FE_MSW;
   /** Far-End LOSS Count, LSW, Current Period */
   DSL_uint16_t cntLOSS_FE_LSW;
   /** Far-End LOSS Count, MSW, Current Period */
   DSL_uint16_t cntLOSS_FE_MSW;
   /** Far-End UAS Count, LSW, Current Period */
   DSL_uint16_t cntUAS_FE_LSW;
   /** Far-End UAS Count, MSW, Current Period */
   DSL_uint16_t cntUAS_FE_MSW;
   /** Far-End FECS Count, LSW, Previous Period */
   DSL_uint16_t cntFECS_FE_LSW_prev;
   /** Far-End FECS Count, MSW, Previous Period */
   DSL_uint16_t cntFECS_FE_MSW_prev;
   /** Far-End ES Count, LSW, Previous Period */
   DSL_uint16_t cntES_FE_LSW_prev;
   /** Far-End ES Count, MSW, Previous Period */
   DSL_uint16_t cntES_FE_MSW_prev;
   /** Far-End SES Count, LSW, Previous Period */
   DSL_uint16_t cntSES_FE_LSW_prev;
   /** Far-End SES Count, MSW, Previous Period */
   DSL_uint16_t cntSES_FE_MSW_prev;
   /** Far-End LOSS Count, LSW, Previous Period */
   DSL_uint16_t cntLOSS_FE_LSW_prev;
   /** Far-End LOSS Count, MSW, Previous Period */
   DSL_uint16_t cntLOSS_FE_MSW_prev;
   /** Far-End UAS Count, LSW, Previous Period */
   DSL_uint16_t cntUAS_FE_LSW_prev;
   /** Far-End UAS Count, MSW, Previous Period */
   DSL_uint16_t cntUAS_FE_MSW_prev;
#endif
} __PACKED__ ;

/**
   Writes far-end line performance counter values (Section 7.2.1.2 of G.997.1)
   for the current and for the previous 15-min-TR1 period.
*/
struct CMD_LinePerfCountFE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Far-End FECS Count, LSW, Current Period */
   DSL_uint16_t cntFECS_FE_LSW;
   /* Far-End FECS Count, MSW, Current Period */
   DSL_uint16_t cntFECS_FE_MSW;
   /* Far-End ES Count, LSW, Current Period */
   DSL_uint16_t cntES_FE_LSW;
   /* Far-End ES Count, MSW, Current Period */
   DSL_uint16_t cntES_FE_MSW;
   /* Far-End SES Count, LSW, Current Period */
   DSL_uint16_t cntSES_FE_LSW;
   /* Far-End SES Count, MSW, Current Period */
   DSL_uint16_t cntSES_FE_MSW;
   /* Far-End LOSS Count, LSW, Current Period */
   DSL_uint16_t cntLOSS_FE_LSW;
   /* Far-End LOSS Count, MSW, Current Period */
   DSL_uint16_t cntLOSS_FE_MSW;
   /* Far-End UAS Count, LSW, Current Period */
   DSL_uint16_t cntUAS_FE_LSW;
   /* Far-End UAS Count, MSW, Current Period */
   DSL_uint16_t cntUAS_FE_MSW;
   /* Far-End FECS Count, LSW, Previous Period */
   DSL_uint16_t cntFECS_FE_LSW_prev;
   /* Far-End FECS Count, MSW, Previous Period */
   DSL_uint16_t cntFECS_FE_MSW_prev;
   /* Far-End ES Count, LSW, Previous Period */
   DSL_uint16_t cntES_FE_LSW_prev;
   /* Far-End ES Count, MSW, Previous Period */
   DSL_uint16_t cntES_FE_MSW_prev;
   /* Far-End SES Count, LSW, Previous Period */
   DSL_uint16_t cntSES_FE_LSW_prev;
   /* Far-End SES Count, MSW, Previous Period */
   DSL_uint16_t cntSES_FE_MSW_prev;
   /* Far-End LOSS Count, LSW, Previous Period */
   DSL_uint16_t cntLOSS_FE_LSW_prev;
   /* Far-End LOSS Count, MSW, Previous Period */
   DSL_uint16_t cntLOSS_FE_MSW_prev;
   /* Far-End UAS Count, LSW, Previous Period */
   DSL_uint16_t cntUAS_FE_LSW_prev;
   /* Far-End UAS Count, MSW, Previous Period */
   DSL_uint16_t cntUAS_FE_MSW_prev;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Far-End FECS Count, LSW, Current Period */
   DSL_uint16_t cntFECS_FE_LSW;
   /* Far-End FECS Count, MSW, Current Period */
   DSL_uint16_t cntFECS_FE_MSW;
   /* Far-End ES Count, LSW, Current Period */
   DSL_uint16_t cntES_FE_LSW;
   /* Far-End ES Count, MSW, Current Period */
   DSL_uint16_t cntES_FE_MSW;
   /* Far-End SES Count, LSW, Current Period */
   DSL_uint16_t cntSES_FE_LSW;
   /* Far-End SES Count, MSW, Current Period */
   DSL_uint16_t cntSES_FE_MSW;
   /* Far-End LOSS Count, LSW, Current Period */
   DSL_uint16_t cntLOSS_FE_LSW;
   /* Far-End LOSS Count, MSW, Current Period */
   DSL_uint16_t cntLOSS_FE_MSW;
   /* Far-End UAS Count, LSW, Current Period */
   DSL_uint16_t cntUAS_FE_LSW;
   /* Far-End UAS Count, MSW, Current Period */
   DSL_uint16_t cntUAS_FE_MSW;
   /* Far-End FECS Count, LSW, Previous Period */
   DSL_uint16_t cntFECS_FE_LSW_prev;
   /* Far-End FECS Count, MSW, Previous Period */
   DSL_uint16_t cntFECS_FE_MSW_prev;
   /* Far-End ES Count, LSW, Previous Period */
   DSL_uint16_t cntES_FE_LSW_prev;
   /* Far-End ES Count, MSW, Previous Period */
   DSL_uint16_t cntES_FE_MSW_prev;
   /* Far-End SES Count, LSW, Previous Period */
   DSL_uint16_t cntSES_FE_LSW_prev;
   /* Far-End SES Count, MSW, Previous Period */
   DSL_uint16_t cntSES_FE_MSW_prev;
   /* Far-End LOSS Count, LSW, Previous Period */
   DSL_uint16_t cntLOSS_FE_LSW_prev;
   /* Far-End LOSS Count, MSW, Previous Period */
   DSL_uint16_t cntLOSS_FE_MSW_prev;
   /* Far-End UAS Count, LSW, Previous Period */
   DSL_uint16_t cntUAS_FE_LSW_prev;
   /* Far-End UAS Count, MSW, Previous Period */
   DSL_uint16_t cntUAS_FE_MSW_prev;
#endif
} __PACKED__;

/**
   Acknowledgement for the message CMD_LinePerfCountFE_Set.
*/
struct ACK_LinePerfCountFE_Set
{
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
} __PACKED__;


/**
   Requests near-end line performance counter values (Section 7.2.1.1 of G.997.1). 
   These are wrap-around counters which are not affected by the TR1-period.
   Only supported in the ADSL mode
*/
struct CMD_LinePerfCountNE_NoTR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides near-end line performance counter values requested by 
   CMD_LinePerfCountNE_NoTR1Get (Section 7.2.1.1 of G.997.1). 
   These are wrap-around counters which are not affected by the TR1-period.
*/
struct ACK_LinePerfCountNE_NoTR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Parameter 0 */
   DSL_uint16_t Index;
   /* Parameter 1 */
   DSL_uint16_t Length;
   /* Near-End FECS Count, LSW */
   DSL_uint16_t cntFECS_LSW;
   /* Near-End FECS Count, MSW */
   DSL_uint16_t cntFECS_MSW;
   /* Near-End ES Count, LSW */
   DSL_uint16_t cntES_LSW;
   /* Near-End ES Count, MSW */
   DSL_uint16_t cntES_MSW;
   /* Near-End SES Count, LSW */
   DSL_uint16_t cntSES_LSW;
   /* Near-End SES Count, LSW */
   DSL_uint16_t cntSES_MSW;
   /* Near-End LOSS Count, LSW */
   DSL_uint16_t cntLOSS_LSW;
   /* Near-End LOSS Count, LSW */
   DSL_uint16_t cntLOSS_MSW;
   /* Parameter 10 to 11 */
   DSL_uint16_t Res[2];
#else
   /* Parameter 0 */
   DSL_uint16_t Index;
   /* Parameter 1 */
   DSL_uint16_t Length;
   /* Parameter 2 */
   DSL_uint16_t cntFECS_LSW;
   /* Parameter 3 */
   DSL_uint16_t cntFECS_MSW;
   /* Parameter 4 */
   DSL_uint16_t cntES_LSW;
   /* Parameter 5 */
   DSL_uint16_t cntES_MSW;
   /* Parameter 6 */
   DSL_uint16_t cntSES_LSW;
   /* Parameter 7 */
   DSL_uint16_t cntSES_MSW;
   /* Parameter 8 */
   DSL_uint16_t cntLOSS_LSW;
   /* Parameter 9 */
   DSL_uint16_t cntLOSS_MSW;
   /* Parameter 10 to 11 */
   DSL_uint16_t Res[2];
#endif
} __PACKED__ ;

/**
   Writes near-end CRC counter values. (Section 7.2.2.1.1 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.
*/
struct CMD_CRC_StatsNE_NoTR1Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /** Near-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /** Near-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
#endif
} __PACKED__ ;


/**
   Acknowledgement for messages CMD_CRC_StatsNE_NoTR1Set.
*/
struct ACK_CRC_StatsNE_NoTR1Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;

/**
   Writes near-end FEC counter values (Section 7.2.2.1.2 of G997.1). These are
   wrap-around counters which are not affected by the TR1-period.
*/
struct CMD_FEC_StatsNE_NoTR1Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /** Near-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /** Reserved */
   DSL_uint16_t Res1[2];
   /** Reserved */
   DSL_uint16_t Res2[8];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /** Near-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /** Reserved */
   DSL_uint16_t Res1[2];
   /** Reserved */
   DSL_uint16_t Res2[8];
#endif
} __PACKED__ ;


/**
   Acknowledgement for messages CMD_FEC_StatsNE_NoTR1Set.
*/
struct ACK_FEC_StatsNE_NoTR1Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;

/**
   Writes near-end line performance counter values (Section 7.2.1.1 of G.997.1). These
   are wrap-around counters which are not affected by the TR1-period.
*/
struct CMD_LinePerfCountNE_NoTR1Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FECS Count, LSW */
   DSL_uint16_t cntFECS_LSW;
   /** Near-End FECS Count, MSW */
   DSL_uint16_t cntFECS_MSW;
   /** Near-End ES Count, LSW */
   DSL_uint16_t cntES_LSW;
   /** Near-End ES Count, MSW */
   DSL_uint16_t cntES_MSW;
   /** Near-End SES Count, LSW */
   DSL_uint16_t cntSES_LSW;
   /** Near-End SES Count, MSW */
   DSL_uint16_t cntSES_MSW;
   /** Near-End LOSS Count, LSW */
   DSL_uint16_t cntLOSS_LSW;
   /** Near-End LOSS Count, MSW */
   DSL_uint16_t cntLOSS_MSW;
   /** Near-End UAS Count, LSW */
   DSL_uint16_t cntUAS_LSW;
   /** Near-End UAS Count, MSW */
   DSL_uint16_t cntUAS_MSW;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FECS Count, LSW */
   DSL_uint16_t cntFECS_LSW;
   /** Near-End FECS Count, MSW */
   DSL_uint16_t cntFECS_MSW;
   /** Near-End ES Count, LSW */
   DSL_uint16_t cntES_LSW;
   /** Near-End ES Count, MSW */
   DSL_uint16_t cntES_MSW;
   /** Near-End SES Count, LSW */
   DSL_uint16_t cntSES_LSW;
   /** Near-End SES Count, MSW */
   DSL_uint16_t cntSES_MSW;
   /** Near-End LOSS Count, LSW */
   DSL_uint16_t cntLOSS_LSW;
   /** Near-End LOSS Count, MSW */
   DSL_uint16_t cntLOSS_MSW;
   /** Near-End UAS Count, LSW */
   DSL_uint16_t cntUAS_LSW;
   /** Near-End UAS Count, MSW */
   DSL_uint16_t cntUAS_MSW;
#endif
} __PACKED__ ;


/**
   Acknowledgement for messages CMD_LinePerfCountNE_NoTR1Set.
*/
struct ACK_LinePerfCountNE_NoTR1Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;

/**
   Requests line initialization performance counter values (Section 7.2.1.3 of G.997.1)
   for the current and previous 15-min-TR1 period.
*/
struct CMD_LineInitCountGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides line initialization performance counter values requested by
   CMD_LineInitCountGet (Section 7.2.1.3 of G.997.1). Contains counter values for the
   current and for the previous 15-min-TR1 interval. These counters are not reset at
   link re-initialization.
*/
struct ACK_LineInitCountGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Full Init Count LSW, Current Period */
   DSL_uint16_t cntInit_LSW;
   /** Full Init Count MSW, Current Period */
   DSL_uint16_t cntInit_MSW;
   /** Failed Full Init Count LSW, Current Period */
   DSL_uint16_t cntInitFail_LSW;
   /** Failed Full Init Count MSW, Current Period */
   DSL_uint16_t cntInitFail_MSW;
   /** Reserved */
   DSL_uint16_t Res1[4];
   /** Full Init Count LSW, Previous Period */
   DSL_uint16_t cntInit_LSW_prev;
   /** Full Init Count MSW, Previous Period */
   DSL_uint16_t cntInit_MSW_prev;
   /** Failed Full Init Count LSW, Previous Period */
   DSL_uint16_t cntInitFail_LSW_prev;
   /** Failed Full Init Count MSW, Previous Period */
   DSL_uint16_t cntInitFail_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res2[4];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Full Init Count LSW, Current Period */
   DSL_uint16_t cntInit_LSW;
   /** Full Init Count MSW, Current Period */
   DSL_uint16_t cntInit_MSW;
   /** Failed Full Init Count LSW, Current Period */
   DSL_uint16_t cntInitFail_LSW;
   /** Failed Full Init Count MSW, Current Period */
   DSL_uint16_t cntInitFail_MSW;
   /** Reserved */
   DSL_uint16_t Res1[4];
   /** Full Init Count LSW, Previous Period */
   DSL_uint16_t cntInit_LSW_prev;
   /** Full Init Count MSW, Previous Period */
   DSL_uint16_t cntInit_MSW_prev;
   /** Failed Full Init Count LSW, Previous Period */
   DSL_uint16_t cntInitFail_LSW_prev;
   /** Failed Full Init Count MSW, Previous Period */
   DSL_uint16_t cntInitFail_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res2[4];
#endif
} __PACKED__ ;

/**
   Writes line initialization performance counter values (Section 7.2.1.3 of G.997.1)
   for the current and previous 15-min-TR1 period.
*/
struct CMD_LineInitCountSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Full Init Count LSW, Current Period */
   DSL_uint16_t cntInit_LSW;
   /* Full Init Count MSW, Current Period */
   DSL_uint16_t cntInit_MSW;
   /* Failed Full Init Count LSW, Current Period */
   DSL_uint16_t cntInitFail_LSW;
   /* Failed Full Init Count MSW, Current Period */
   DSL_uint16_t cntInitFail_MSW;
   /* Reserved */
   DSL_uint16_t Res1[4];
   /* Full Init Count LSW, Previous Period */
   DSL_uint16_t cntInit_LSW_prev;
   /* Full Init Count MSW, Previous Period */
   DSL_uint16_t cntInit_MSW_prev;
   /* Failed Full Init Count LSW, Previous Period */
   DSL_uint16_t cntInitFail_LSW_prev;
   /* Failed Full Init Count MSW, Previous Period */
   DSL_uint16_t cntInitFail_MSW_prev;
   /* Reserved */
   DSL_uint16_t Res2[4];
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Full Init Count LSW, Current Period */
   DSL_uint16_t cntInit_LSW;
   /* Full Init Count MSW, Current Period */
   DSL_uint16_t cntInit_MSW;
   /* Failed Full Init Count LSW, Current Period */
   DSL_uint16_t cntInitFail_LSW;
   /* Failed Full Init Count MSW, Current Period */
   DSL_uint16_t cntInitFail_MSW;
   /* Reserved */
   DSL_uint16_t Res1[4];
   /* Full Init Count LSW, Previous Period */
   DSL_uint16_t cntInit_LSW_prev;
   /* Full Init Count MSW, Previous Period */
   DSL_uint16_t cntInit_MSW_prev;
   /* Failed Full Init Count LSW, Previous Period */
   DSL_uint16_t cntInitFail_LSW_prev;
   /* Failed Full Init Count MSW, Previous Period */
   DSL_uint16_t cntInitFail_MSW_prev;
   /* Reserved */
   DSL_uint16_t Res2[4];
#endif
} __PACKED__;

/**
   Acknowledgement for the message CMD_LineInitCountSet.
*/
struct ACK_LineInitCountSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
#endif
} __PACKED__;

/**
   Requests near-end CRC counter values.
*/
struct CMD_CRC_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns near-end CRC counter values. (Section 7.2.2.1.1 of G997.1)
*/
struct ACK_CRC_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End CRC Count LP0, LSW, Current Period */
   DSL_uint16_t cntCVI_LSW;
   /** Near-End CRC Count LP0, MSW, Current Period */
   DSL_uint16_t cntCVI_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Near-End CRC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntCVI_LSW_prev;
   /** Near-End CRC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntCVI_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Reserved */
   DSL_uint16_t Res03;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End CRC Count LP0, LSW, Current Period */
   DSL_uint16_t cntCVI_LSW;
   /** Near-End CRC Count LP0, MSW, Current Period */
   DSL_uint16_t cntCVI_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Near-End CRC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntCVI_LSW_prev;
   /** Near-End CRC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntCVI_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Reserved */
   DSL_uint16_t Res03;
#endif
} __PACKED__ ;

/**
   Writes the near-end CRC counter values (Section 7.2.2.1.1 of G997.1)
*/
struct CMD_CRC_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End CRC Count LP0, LSW, Current Period */
   DSL_uint16_t cntCVI_LSW;
   /* Near-End CRC Count LP0, MSW, Current Period */
   DSL_uint16_t cntCVI_MSW;
   /* Near-End CRC Count LP1, LSW, Current Period */
   DSL_uint16_t cntCVF_LSW;
   /* Near-End CRC Count LP1, MSW, Current Period */
   DSL_uint16_t cntCVF_MSW;
   /* Near-End CRC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntCVI_LSW_prev;
   /* Near-End CRC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntCVI_MSW_prev;
   /* Near-End CRC Count LP1, LSW, Previous Period */
   DSL_uint16_t cntCVF_LSW_prev;
   /* Near-End CRC Count LP1, MSW, Previous Period */
   DSL_uint16_t cntCVF_MSW_prev;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End CRC Count LP0, LSW, Current Period */
   DSL_uint16_t cntCVI_LSW;
   /* Near-End CRC Count LP0, MSW, Current Period */
   DSL_uint16_t cntCVI_MSW;
   /* Near-End CRC Count LP1, LSW, Current Period */
   DSL_uint16_t cntCVF_LSW;
   /* Near-End CRC Count LP1, MSW, Current Period */
   DSL_uint16_t cntCVF_MSW;
   /* Near-End CRC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntCVI_LSW_prev;
   /* Near-End CRC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntCVI_MSW_prev;
   /* Near-End CRC Count LP1, LSW, Previous Period */
   DSL_uint16_t cntCVF_LSW_prev;
   /* Near-End CRC Count LP1, MSW, Previous Period */
   DSL_uint16_t cntCVF_MSW_prev;
#endif
} __PACKED__ ;

/**
   Acknowledgement for the message CMD_CRC_StatsNE_Set.
*/
struct ACK_CRC_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__;

/**
   Requests near-end CRC counter values. These are wrap-around counters which 
   are not affected by the TR1-period.
*/
struct CMD_CRC_StatsNE_NoTR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;

/**
   Returns near-end CRC counter values. (Section 7.2.2.1.1 of G997.1). 
   These are wrap-around counters which are not affected by the TR1-period.
*/
struct ACK_CRC_StatsNE_NoTR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /* Near-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /* Parameter 4 */
   DSL_uint16_t Res00;
   /* Parameter 5 */
   DSL_uint16_t Res01;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End CRC Count LP0, LSW */
   DSL_uint16_t cntCVI_LSW;
   /* Near-End CRC Count LP0, MSW */
   DSL_uint16_t cntCVI_MSW;
   /* Parameter 4 */
   DSL_uint16_t Res00;
   /* Parameter 5 */
   DSL_uint16_t Res01;
#endif
} __PACKED__ ;

/**
   Requests far-end CRC counter values.
*/
struct CMD_CRC_StatsFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns far-end CRC counter values. (Section 7.2.2.2.1 of G997.1)
*/
struct ACK_CRC_StatsFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End CRC Count LP0, LSW, Current Period */
   DSL_uint16_t cntCVI_LSW;
   /** Far-End CRC Count LP0, MSW, Current Period */
   DSL_uint16_t cntCVI_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Far-End CRC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntCVI_LSW_prev;
   /** Far-End CRC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntCVI_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Reserved */
   DSL_uint16_t Res03;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End CRC Count LP0, LSW, Current Period */
   DSL_uint16_t cntCVI_LSW;
   /** Far-End CRC Count LP0, MSW, Current Period */
   DSL_uint16_t cntCVI_MSW;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01;
   /** Far-End CRC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntCVI_LSW_prev;
   /** Far-End CRC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntCVI_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res02;
   /** Reserved */
   DSL_uint16_t Res03;
#endif
} __PACKED__ ;

/**
   Writes the far-end CRC counter values (Section 7.2.2.2.1 of G997.1)
*/
struct CMD_CRC_StatsFE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Far-End CRC Count LP0, LSW, Current Period */
   DSL_uint16_t cntCVI_LSW;
   /* Far-End CRC Count LP0, MSW, Current Period */
   DSL_uint16_t cntCVI_MSW;
   /* Far-End CRC Count LP1, LSW, Current Period */
   DSL_uint16_t cntCVF_LSW;
   /* Far-End CRC Count LP1, MSW, Current Period */
   DSL_uint16_t cntCVF_MSW;
   /* Far-End CRC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntCVI_LSW_prev;
   /* Far-End CRC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntCVI_MSW_prev;
   /* Far-End CRC Count LP1, LSW, Previous Period */
   DSL_uint16_t cntCVF_LSW_prev;
   /* Far-End CRC Count LP1, MSW, Previous Period */
   DSL_uint16_t cntCVF_MSW_prev;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Far-End CRC Count LP0, LSW, Current Period */
   DSL_uint16_t cntCVI_LSW;
   /* Far-End CRC Count LP0, MSW, Current Period */
   DSL_uint16_t cntCVI_MSW;
   /* Far-End CRC Count LP1, LSW, Current Period */
   DSL_uint16_t cntCVF_LSW;
   /* Far-End CRC Count LP1, MSW, Current Period */
   DSL_uint16_t cntCVF_MSW;
   /* Far-End CRC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntCVI_LSW_prev;
   /* Far-End CRC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntCVI_MSW_prev;
   /* Far-End CRC Count LP1, LSW, Previous Period */
   DSL_uint16_t cntCVF_LSW_prev;
   /* Far-End CRC Count LP1, MSW, Previous Period */
   DSL_uint16_t cntCVF_MSW_prev;
#endif
} __PACKED__ ;

struct ACK_CRC_StatsFE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;

/**
   Requests near-end FEC counter values.
*/
struct CMD_FEC_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns near-end FEC counter values. (Section 7.2.2.1.2 of G997.1)
*/
struct ACK_FEC_StatsNE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FEC Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_LSW;
   /** Near-End FEC Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_MSW;
   /** Reserved */
   DSL_uint16_t Res1[2];
   /** Near-End FEC Uncorrectable Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_NoCorrMSW;
   /** Reserved */
   DSL_uint16_t Res2[2];
   /** Near-End Total RS CW Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_CW_LSW;
   /** Near-End Total RS CW Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_CW_MSW;
   /** Reserved */
   DSL_uint16_t Res3[2];
   /** Near-End FEC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_LSW_prev;
   /** Near-End FEC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res4[2];
   /** Near-End FEC Uncorrectable Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrLSW_prev;
   /** Near-End FEC Uncorrectable Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrMSW_prev;
   /** Reserved */
   DSL_uint16_t Res5[2];
   /** Near-End Total RS CW Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_CW_LSW_prev;
   /** Near-End Total RS CW Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_CW_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res6[2];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Near-End FEC Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_LSW;
   /** Near-End FEC Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_MSW;
   /** Reserved */
   DSL_uint16_t Res1[2];
   /** Near-End FEC Uncorrectable Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_NoCorrLSW;
   /** Near-End FEC Uncorrectable Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_NoCorrMSW;
   /** Reserved */
   DSL_uint16_t Res2[2];
   /** Near-End Total RS CW Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_CW_LSW;
   /** Near-End Total RS CW Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_CW_MSW;
   /** Reserved */
   DSL_uint16_t Res3[2];
   /** Near-End FEC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_LSW_prev;
   /** Near-End FEC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res4[2];
   /** Near-End FEC Uncorrectable Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrLSW_prev;
   /** Near-End FEC Uncorrectable Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrMSW_prev;
   /** Reserved */
   DSL_uint16_t Res5[2];
   /** Near-End Total RS CW Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_CW_LSW_prev;
   /** Near-End Total RS CW Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_CW_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res6[2];
#endif
} __PACKED__ ;

/**
   Writes near-end FEC counter values. (Section 7.2.2.1.2 of G997.1).
*/
struct CMD_FEC_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End FEC Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_LSW;
   /* Near-End FEC Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_MSW;
   /* Near-End FEC Count LP1, LSW, Current Period */
   DSL_uint16_t cntFEC_LSW;
   /* Near-End FEC Count LP1, MSW, Current Period */
   DSL_uint16_t cntFEC_MSW;
   /* Near-End FEC Uncorrectable Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_NoCorrLSW;
   /* Near-End FEC Uncorrectable Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_NoCorrMSW;
   /* Near-End FEC Uncorrectable Count LP1, LSW, Current Period */
   DSL_uint16_t cntECF_NoCorrLSW;
   /* Near-End FEC Uncorrectable Count LP1, MSW, Current Period */
   DSL_uint16_t cntECF_NoCorrMSW;
   /* Near-End Total RS CW Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_CW_LSW;
   /* Near-End Total RS CW Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_CW_MSW;
   /* Near-End Total RS CW Count LP1, LSW, Current Period */
   DSL_uint16_t cntECF_CW_LSW;
   /* Near-End Total RS CW Count LP1, MSW, Current Period */
   DSL_uint16_t cntECF_CW_MSW;
   /* Near-End FEC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_LSW_prev;
   /* Near-End FEC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_MSW_prev;
   /* Near-End FEC Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_LSW_prev;
   /* Near-End FEC Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_MSW_prev;
   /* Near-End FEC Uncorrectable Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrLSW_prev;
   /* Near-End FEC Uncorrectable Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrMSW_prev;
   /* Near-End FEC Uncorrectable Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_NoCorrLSW_prev;
   /* Near-End FEC Uncorrectable Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_NoCorrMSW_prev;
   /* Near-End Total RS CW Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_CW_LSW_prev;
   /* Near-End Total RS CW Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_CW_MSW_prev;
   /* Near-End Total RS CW Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_CW_LSW_prev;
   /* Near-End Total RS CW Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_CW_MSW_prev;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End FEC Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_LSW;
   /* Near-End FEC Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_MSW;
   /* Near-End FEC Count LP1, LSW, Current Period */
   DSL_uint16_t cntFEC_LSW;
   /* Near-End FEC Count LP1, MSW, Current Period */
   DSL_uint16_t cntFEC_MSW;
   /* Near-End FEC Uncorrectable Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_NoCorrLSW;
   /* Near-End FEC Uncorrectable Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_NoCorrMSW;
   /* Near-End FEC Uncorrectable Count LP1, LSW, Current Period */
   DSL_uint16_t cntECF_NoCorrLSW;
   /* Near-End FEC Uncorrectable Count LP1, MSW, Current Period */
   DSL_uint16_t cntECF_NoCorrMSW;
   /* Near-End Total RS CW Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_CW_LSW;
   /* Near-End Total RS CW Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_CW_MSW;
   /* Near-End Total RS CW Count LP1, LSW, Current Period */
   DSL_uint16_t cntECF_CW_LSW;
   /* Near-End Total RS CW Count LP1, MSW, Current Period */
   DSL_uint16_t cntECF_CW_MSW;
   /* Near-End FEC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_LSW_prev;
   /* Near-End FEC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_MSW_prev;
   /* Near-End FEC Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_LSW_prev;
   /* Near-End FEC Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_MSW_prev;
   /* Near-End FEC Uncorrectable Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrLSW_prev;
   /* Near-End FEC Uncorrectable Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrMSW_prev;
   /* Near-End FEC Uncorrectable Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_NoCorrLSW_prev;
   /* Near-End FEC Uncorrectable Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_NoCorrMSW_prev;
   /* Near-End Total RS CW Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_CW_LSW_prev;
   /* Near-End Total RS CW Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_CW_MSW_prev;
   /* Near-End Total RS CW Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_CW_LSW_prev;
   /* Near-End Total RS CW Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_CW_MSW_prev;
#endif
} __PACKED__ ;

/**
   Acknowledgement for the message CMD_FEC_StatsNE_Set.
*/
struct ACK_FEC_StatsNE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;

/**
   Requests near-end FEC counter values. These are wrap-around counters which 
   are not affected by the TR1-period.
*/
struct CMD_FEC_StatsNE_NoTR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;

/**
   Returns near-end FEC counter values (Section 7.2.2.1.2 of G997.1). 
   These are wrap-around counters which are not affected by the TR1-period.
*/
struct ACK_FEC_StatsNE_NoTR1Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /* Near-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /* Parameter 4 to 5 */
   DSL_uint16_t Res1[2];
   /* Parameter 6 to 13 */
   DSL_uint16_t Res2[8];
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Near-End FEC Count LP0, LSW */
   DSL_uint16_t cntECI_LSW;
   /* Near-End FEC Count LP0, MSW */
   DSL_uint16_t cntECI_MSW;
   /* Parameter 4 to 5 */
   DSL_uint16_t Res1[2];
   /* Parameter 6 to 13 */
   DSL_uint16_t Res2[8];
#endif
} __PACKED__ ;

/**
   Requests far-end FEC counter values.
*/
struct CMD_FEC_StatsFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns the far-end FEC counter values requested by CMD_FEC_StatsFE_Get.(Section
   7.2.2.2.2 of G997.1)
*/
struct ACK_FEC_StatsFE_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End FEC Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_LSW;
   /** Far-End FEC Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_MSW;
   /** Reserved */
   DSL_uint16_t Res1[2];
   /** Far-End FEC Uncorrectable Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_NoCorrLSW;
   /** Far-End FEC Uncorrectable Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_NoCorrMSW;
   /** Reserved */
   DSL_uint16_t Res2[2];
   /** Far-End Total RS CW Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_CW_LSW;
   /** Far-End Total RS CW Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_CW_MSW;
   /** Reserved */
   DSL_uint16_t Res3[2];
   /** Far-End FEC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_LSW_prev;
   /** Far-End FEC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res4[2];
   /** Far-End FEC Uncorrectable Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrLSW_prev;
   /** Far-End FEC Uncorrectable Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrMSW_prev;
   /** Reserved */
   DSL_uint16_t Res5[2];
   /** Far-End Total RS CW Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_CW_LSW_prev;
   /** Far-End Total RS CW Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_CW_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res6[2];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-End FEC Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_LSW;
   /** Far-End FEC Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_MSW;
   /** Reserved */
   DSL_uint16_t Res1[2];
   /** Far-End FEC Uncorrectable Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_NoCorrLSW;
   /** Far-End FEC Uncorrectable Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_NoCorrMSW;
   /** Reserved */
   DSL_uint16_t Res2[2];
   /** Far-End Total RS CW Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_CW_LSW;
   /** Far-End Total RS CW Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_CW_MSW;
   /** Reserved */
   DSL_uint16_t Res3[2];
   /** Far-End FEC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_LSW_prev;
   /** Far-End FEC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res4[2];
   /** Far-End FEC Uncorrectable Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrLSW_prev;
   /** Far-End FEC Uncorrectable Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_NoCorrMSW_prev;
   /** Reserved */
   DSL_uint16_t Res5[2];
   /** Far-End Total RS CW Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_CW_LSW_prev;
   /** Far-End Total RS CW Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_CW_MSW_prev;
   /** Reserved */
   DSL_uint16_t Res6[2];
#endif
} __PACKED__ ;

/**
   Writes far-end FEC counter values. (Section 7.2.2.2.2 of G997.1)
*/
struct CMD_FEC_StatsFE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Far-End FEC Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_LSW;
   /* Far-End FEC Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_MSW;
   /* Far-End FEC Count LP1, LSW, Current Period */
   DSL_uint16_t cntFEC_LSW;
   /* Far-End FEC Count LP1, MSW, Current Period */
   DSL_uint16_t cntFEC_MSW;
   /* Reserved */
   DSL_uint16_t Res0[4];
   /* Far-End Total RS CW Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_CW_LSW;
   /* Far-End Total RS CW Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_CW_MSW;
   /* Far-End Total RS CW Count LP1, LSW, Current Period */
   DSL_uint16_t cntECF_CW_LSW;
   /* Far-End Total RS CW Count LP1, MSW, Current Period */
   DSL_uint16_t cntECF_CW_MSW;
   /* Far-End FEC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_LSW_prev;
   /* Far-End FEC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_MSW_prev;
   /* Far-End FEC Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_LSW_prev;
   /* Far-End FEC Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_MSW_prev;
   /* Reserved */
   DSL_uint16_t Res1[4];
   /* Far-End Total RS CW Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_CW_LSW_prev;
   /* Far-End Total RS CW Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_CW_MSW_prev;
   /* Far-End Total RS CW Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_CW_LSW_prev;
   /* Far-End Total RS CW Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_CW_MSW_prev;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
   /* Far-End FEC Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_LSW;
   /* Far-End FEC Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_MSW;
   /* Far-End FEC Count LP1, LSW, Current Period */
   DSL_uint16_t cntFEC_LSW;
   /* Far-End FEC Count LP1, MSW, Current Period */
   DSL_uint16_t cntFEC_MSW;
   /* Reserved */
   DSL_uint16_t Res0[4];
   /* Far-End Total RS CW Count LP0, LSW, Current Period */
   DSL_uint16_t cntECI_CW_LSW;
   /* Far-End Total RS CW Count LP0, MSW, Current Period */
   DSL_uint16_t cntECI_CW_MSW;
   /* Far-End Total RS CW Count LP1, LSW, Current Period */
   DSL_uint16_t cntECF_CW_LSW;
   /* Far-End Total RS CW Count LP1, MSW, Current Period */
   DSL_uint16_t cntECF_CW_MSW;
   /* Far-End FEC Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_LSW_prev;
   /* Far-End FEC Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_MSW_prev;
   /* Far-End FEC Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_LSW_prev;
   /* Far-End FEC Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_MSW_prev;
   /* Reserved */
   DSL_uint16_t Res1[4];
   /* Far-End Total RS CW Count LP0, LSW, Previous Period */
   DSL_uint16_t cntECI_CW_LSW_prev;
   /* Far-End Total RS CW Count LP0, MSW, Previous Period */
   DSL_uint16_t cntECI_CW_MSW_prev;
   /* Far-End Total RS CW Count LP1, LSW, Previous Period */
   DSL_uint16_t cntECF_CW_LSW_prev;
   /* Far-End Total RS CW Count LP1, MSW, Previous Period */
   DSL_uint16_t cntECF_CW_MSW_prev;
#endif
} __PACKED__ ;

/**
   Acknowledgement for the message CMD_FEC_StatsFE_Set.
*/
struct ACK_FEC_StatsFE_Set
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
#else
   /* Index */
   DSL_uint16_t Index;
   /* Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;

/**
   Controls the "Set-Time" and "Read-Time" EOC messages to either synchronize or read
   the time of the xTU-R.
   Note: Modifying the Time does not affect the TR1 supervision period and the
   TR1-counters.
*/
struct CMD_TimeControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
   /** Reserved */
   DSL_bf16_t Res1 : 1;
   /** Status of Set Time */
   DSL_bf16_t TimeControl : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Status of Set Time */
   DSL_bf16_t TimeControl : 2;
   /** Reserved */
   DSL_bf16_t Res1 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_TimeControl.
*/
struct ACK_TimeControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Sets the time.
   To synchronize the time of the xTU-C and xTU-R, the xTU-C sets the far-end time
   parameter and then triggers the sending of the EOC time command to transfer the
   configured value to the opposite side using CMD_TimeControl.
   Note: Modifying the Time does not affect the TR1 supervision period and the
   TR1-counters.
*/
struct CMD_TimeSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** FE Time */
   VNX_TIME_t timeFE;
   /** NE Time */
   VNX_TIME_t timeNE;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_uint16_t Res00;
   /** NE Time */
   VNX_TIME_t timeNE;
   /** FE Time */
   VNX_TIME_t timeFE;
#endif
} __PACKED__ ;


/**
   Acknowledgement for message CMD_TimeSet.
*/
struct ACK_TimeSet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the status of the "Set Time and Read Time" EOC messages.
*/
struct CMD_TimeGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the status of the "Set Time and Read Time" EOC messages.
*/
struct ACK_TimeGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** Status of Read Time */
   DSL_bf16_t statusTimeMsg : 2;
   /** FE Time */
   VNX_TIME_t timeFE;
   /** NE Time */
   VNX_TIME_t timeNE;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Status of Read Time */
   DSL_bf16_t statusTimeMsg : 2;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** NE Time */
   VNX_TIME_t timeNE;
   /** FE Time */
   VNX_TIME_t timeFE;
#endif
} __PACKED__ ;


/**
   This message is used to configure the autonomous messaging related to Clear EOC
   transmission.
*/
struct CMD_ClearEOC_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
   /** Tx Autonomous Status Message Control */
   DSL_bf16_t TxEVTstatus : 1;
   /** Rx Autonomous Status Message Control */
   DSL_bf16_t RxEVTstatus : 1;
   /** Rx Autonomous Clear EOC Data Message Control */
   DSL_bf16_t RxEVTdata : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Rx Autonomous Clear EOC Data Message Control */
   DSL_bf16_t RxEVTdata : 1;
   /** Rx Autonomous Status Message Control */
   DSL_bf16_t RxEVTstatus : 1;
   /** Tx Autonomous Status Message Control */
   DSL_bf16_t TxEVTstatus : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ClearEOC_Configure.
*/
struct ACK_ClearEOC_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message is used to trigger the transmission of  Clear EOC messages that were
   placed into the Clear EOC transmit buffer before with CMD_ClearEOC_Write.
*/
struct CMD_ClearEOC_TxTrigger
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 15;
   /** Transmit Control Trigger */
   DSL_bf16_t txTrigger : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Control Trigger */
   DSL_bf16_t txTrigger : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 15;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ClearEOC_TxTrigger.
*/
struct ACK_ClearEOC_TxTrigger
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message is used to write data to the ClearEOC write buffer of type
   VNX_ClearEOC_t. When the buffer is filled, the transmission is started applying
   CMD_ClearEOC_TxTrigger. If the message to transmit is longer than the mailbox size,
   a sequence of writes to the ClearEOC buffer has to be done before the transmission
   is started with CMD_ClearEOC_TxTrigger. When autonomous Tx status messaging is
   activated via CMD_ClearEOC_Configure, then the finished transmission is indicated by
   EVT_ClearEOCStatusGet.
*/
struct CMD_ClearEOC_Write
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   This message is the acknowledgement for CMD_ClearEOC_Write.
*/
struct ACK_ClearEOC_Write
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message is used to read data from the ClearEOC buffer of type VNX_ClearEOC_t.
   The length of the actual Clear EOC message can be found in the buffer. Please refer
   to VNX_ClearEOC_t. The availability of data can either be checked via
   CMD_ClearEOCStatusGet in polling mode or it can be reported by an autonomous
   EVT_ClearEOCStatusGet message when data is received (to be enabled using
   CMD_ClearEOC_Configure).
*/
struct CMD_ClearEOC_Read
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message is the acknowledgement to CMD_ClearEOC_Read.
*/
struct ACK_ClearEOC_Read
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   This message is an autonomous message that is generated when ClearEOC data was
   received and autonomous Clear EOC data messaging has been activated via
   CMD_ClearEOC_Configure. If the ClearEOC data does not fit in one message, then a
   sequence of messages is generated. The ClearEOC buffer is of type VNX_ClearEOC_t.
*/
struct EVT_ClearEOC_Read
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   This message is used to retrieve the status of the clear eoc data transmission.
*/
struct CMD_ClearEOCStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_ClearEOCStatusGet.
*/
struct ACK_ClearEOCStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** Transmit Status */
   DSL_bf16_t txstat : 2;
   /** Reserved */
   DSL_bf16_t Res01 : 14;
   /** Receive Status */
   DSL_bf16_t rxstat : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Status */
   DSL_bf16_t txstat : 2;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** Receive Status */
   DSL_bf16_t rxstat : 2;
   /** Reserved */
   DSL_bf16_t Res01 : 14;
#endif
} __PACKED__ ;


/**
   This autonomous message reports the Clear EOC status. It is sent only if the
   "Autonomous Status Message Control" was enabled for Tx and/or Rx direction with
   CMD_ClearEOC_Configure. If Tx direction is enabled, the message is generated when a
   Tx transmission is finished or failed. If Rx direction is enabled, the message is
   generated when the Rx status transitions from "Idle" to "Data Available" for
   retrieval by the host.
*/
struct EVT_ClearEOCStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** Transmit Status */
   DSL_bf16_t txstat : 2;
   /** Reserved */
   DSL_bf16_t Res01 : 14;
   /** Receive Status */
   DSL_bf16_t rxstat : 2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Transmit Status */
   DSL_bf16_t txstat : 2;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** Receive Status */
   DSL_bf16_t rxstat : 2;
   /** Reserved */
   DSL_bf16_t Res01 : 14;
#endif
} __PACKED__ ;


/**
   Requests information about the bit-allocation per subcarrier in VDSL mode. The
   values for all subcarriers of both directions can be retrieved.
   (For ADSL use CMD_ADSL_BAT_DS_Get and CMD_ADSL_BAT_US_Get).
*/
struct CMD_BAT_TableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the bit-allocation per tone for the chosen range of
   subcarriers.(Section 7.5.1.29.1-2 of G997.1)
*/
struct ACK_BAT_TableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VNX_BAT_TableEntry_t BAT[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VNX_BAT_TableEntry_t BAT[128];
#endif
} __PACKED__ ;


/**
   Requests information about the Gains per subcarrier. The values for all subcarriers
   of both directions can be retrieved.
   (For ADSL use CMD_ADSL_GainTableDS_Get and CMD_ADSL_GainTableUS_Get).
*/
struct CMD_GainTableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the Gain-per-tone for the chosen range of
   subcarriers.(Section 7.5.1.29.3-4 of G997.1)
*/
struct ACK_GainTableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#endif
} __PACKED__ ;


/**
   The message requests information about the SNR per subcarrier available at the
   near-end side, which means for upstream direction at the CO side and for downstream
   direction at the CPE. It is the hosts responsibility to select the tone indices
   accordingly.
*/
struct CMD_SNR_NE_TableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the SNR per subcarrier available at the near-end side,
   which means for upstream direction at the CO side and for downstream direction at
   the CPE.
*/
struct ACK_SNR_NE_TableEntriesGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR */
   DSL_uint16_t SNRps[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR */
   DSL_uint16_t SNRps[128];
#endif
} __PACKED__ ;


/**
   Requests information about the downstream bit-allocation per subcarrier in ADSL
   mode.
*/
struct CMD_ADSL_BAT_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the downstream bit-allocation per tone for the chosen
   range of subcarriers.(Section 7.5.1.29.1 of G997.1)
*/
struct ACK_ADSL_BAT_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VNX_BAT_TableEntry_t BAT[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VNX_BAT_TableEntry_t BAT[128];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream bit-allocation per subcarrier in ADSL mode.
*/
struct CMD_ADSL_BAT_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the upstream bit-allocation per tone for the chosen range
   of subcarriers.(Section 7.5.1.29.2 of G997.1)
*/
struct ACK_ADSL_BAT_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VNX_BAT_TableEntry_t BAT[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Bit Allocation */
   VNX_BAT_TableEntry_t BAT[32];
#endif
} __PACKED__ ;


/**
   Requests information about the Gains per subcarrier for the downstream direction.
*/
struct CMD_ADSL_GainTableDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the Gain-per-tone for the chosen range of subcarriers for
   the downstream direction.(Section 7.5.1.29.3 of G997.1) as requested by
   CMD_ADSL_GainTableDS_Get.
*/
struct ACK_ADSL_GainTableDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#endif
} __PACKED__ ;


/**
   Requests information about the Gains per subcarrier for the upstream direction.
*/
struct CMD_ADSL_GainTableUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the Gain-per-tone for the chosen range of subcarriers for
   the upstream direction.(Section 7.5.1.29.4 of G997.1)
*/
struct ACK_ADSL_GainTableUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Gains */
   DSL_uint16_t Gains[128];
#endif
} __PACKED__ ;


/**
   Requests information about the Excess Margin Reduction needed for the fine gain
   calculation at the ATU_R.(See also ACK_ADSL_GainTableDS_Get).
*/
struct CMD_ADSL_ExMarginReductionGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the Excess Margin Reduction for the downstream direction.
*/
struct ACK_ADSL_ExMarginReductionGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Excess Margin Reduction */
   DSL_uint16_t eSnrmReduction;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Excess Margin Reduction */
   DSL_uint16_t eSnrmReduction;
#endif
} __PACKED__ ;


/**
   Requests information about the downstream HLOG information per subcarrier group
   (Section 7.5.1.26.6 of G997.1).
   During STEADY_STATE, the command can be used to request near-end data only (CPE).
   For the far-end HLOG to be provided via the EOC channel CMD_TestParamsFE_Request
   must be used.
   During loop diagnostic mode, the command shall be used to request both near-end and
   far-end data.
*/
struct CMD_HlogDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the downstream HLOG per subcarrier group for the chosen
   range. (Section 7.5.1.26.6 of G997.1)
*/
struct ACK_HlogDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLOG per SG: HLOGpsds */
   DSL_uint16_t HLOGpsds[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLOG per SG: HLOGpsds */
   DSL_uint16_t HLOGpsds[128];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream HLOG informationper subcarrier group
   (Section 7.5.1.26.11 of G997.1).
   During STEADY_STATE, the command can be used to request near-end data only (CO). For
   the far-end HLOG to be provided via the EOC channel CMD_TestParamsFE_Request must be
   used.
   During loop diagnostic mode, the command is used to request both near-end and
   far-end data.
*/
struct CMD_HlogUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the upstream HLOG per subcarrier group for the chosen
   range. (Section 7.5.1.26.11 of G997.1)
*/
struct ACK_HlogUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLOG per SG: HLOGpsds */
   DSL_uint16_t HLOGpsds[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLOG per SG: HLOGpsds */
   DSL_uint16_t HLOGpsds[128];
#endif
} __PACKED__ ;


/**
   Requests information about the downstream HLIN information per subcarrier group.
   (Section 7.5.1.26.3 of G997.1).
   The HLIN data are available during loop diagnostic mode only.
*/
struct CMD_HlinDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the downstream HLIN per subcarrier group for the chosen
   range. (Section 7.5.1.26.3 of G997.1)
*/
struct ACK_HlinDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN per SG: HLINpsds */
   VNX_HLIN_t HLINpsds[64];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN per SG: HLINpsds */
   VNX_HLIN_t HLINpsds[64];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream HLIN information per subcarrier group.
   (Section 7.5.1.26.9 of G997.1).
   The HLIN data are available in loop diagnostic mode only.
*/
struct CMD_HlinUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the upstream HLIN per subcarrier group for the chosen
   range. (Section 7.5.1.26.9 of G997.1)
*/
struct ACK_HlinUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN per SG: HLINpsus */
   VNX_HLIN_t HLINpsus[64];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN per SG: HLINpsus */
   VNX_HLIN_t HLINpsus[64];
#endif
} __PACKED__ ;


/**
   Requests information about the downstream QLN information (QLNpsds) per subcarrier
   group (Section 7.5.1.27.3 of G997.1).
   During STEADY_STATE, the command can be used to request near-end data only (CPE).
   For the far-end QLN to be provided via the EOC channel the message
   CMD_TestParamsFE_Request must be used.
   During loop diagnostic mode, the command is used to request both near-end and
   far-end data.
*/
struct CMD_QLN_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the QLN per subcarrier group for the chosen range.
   (Section 7.5.1.27.3 of G997.1)
*/
struct ACK_QLN_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN per SG: QLNpsds */
   VNX_QLN_NE_t QLNds[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN per SG: QLNpsds */
   VNX_QLN_NE_t QLNds[128];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream QLN information upstream (QLNpsus) per
   subcarrier group. (Section 7.5.1.27.6 of G997.1).
   During STEADY_STATE, the command can be used to request near-end data only (CO). For
   the far-end QLN to be provided via the EOC channel the message
   CMD_TestParamsFE_Request must be used.
   During loop diagnostic mode, the command can be used to request both near-end and
   far-end data.
*/
struct CMD_QLN_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the QLN per subcarrier group for the chosen range.
   (Section 7.5.1.27.6 of G997.1)
*/
struct ACK_QLN_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN per SG: QLNpsus */
   VNX_QLN_NE_t QLNus[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN per SG: QLNpsus */
   VNX_QLN_NE_t QLNus[128];
#endif
} __PACKED__ ;


/**
   Requests information about the downstream SNR per subcarrier group (Section
   7.5.1.28.3 of G997.1)
   The message is for loop diagnostic mode only to retrieve NE and FE data! During
   STEADY_STATE use CMD_SNR_NE_TableEntriesGet and CMD_TestParamsFE_Request instead.
*/
struct CMD_SNR_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the SNR per subcarrier group for the chosen range.
   (Section 7.5.1.28.3 of G997.1)
*/
struct ACK_SNR_DS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR per SG: SNRpsds */
   VNX_SNR_t SNRpsds[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR per SG: SNRpsds */
   VNX_SNR_t SNRpsds[128];
#endif
} __PACKED__ ;


/**
   Requests information about the upstream SNR per subcarrier group (Section 7.5.1.28.6
   of G997.1).
   The message is for loop diagnostic mode only to retrieve NE or FE data! During
   STEADY_STATE use CMD_SNR_NE_TableEntriesGet and CMD_TestParamsFE_Request instead.
*/
struct CMD_SNR_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the upstream SNR per subcarrier group for the chosen
   range. (Section 7.5.1.28.6 of G997.1)
*/
struct ACK_SNR_US_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR per SG: SNRpsus */
   VNX_SNR_t SNRpsus[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** SNR per SG: SNRpsus */
   VNX_SNR_t SNRpsus[128];
#endif
} __PACKED__ ;


/**
   Requests test parameter related information for the downstream direction: The HLIN
   scaling factor (HLINSCds), the subcarrier group size "G" and the measurement times
   for HLOGpsds, QLNpsds, SNRpsds.
   (Sections 7.5.1.26.1/2/4/5, 7.5.1.27.1/2 and 7.5.1.28.1/2 of G997.1)
   During STEADY_STATE, the command can be used to request near-end data only (CPE).
   For the far-end data to be provided via the EOC channel the message
   CMD_TestParamsFE_Request must be applied.
   During loop diagnostic mode, the command is used to request both near-end and
   far-end data.
*/
struct CMD_TestParamsAuxDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the test-parameter related information as requested by
   CMD_TestParamsAuxDS_Get: The HLIN scaling factor (HLINSCds), the subcarrier group
   size "G" and the measurement times for HLOGpsds, QLNpsds, SNRpsds.
   (Sections 7.5.1.26.1/2/4/5, 7.5.1.27.1/2 and 7.5.1.28.1/2 of G997.1)
*/
struct ACK_TestParamsAuxDS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN Scaling Factor "HLINSCds" */
   DSL_uint16_t HLINSC;
   /** HLIN Subcarrier Group Size DS "HLINGds" */
   DSL_uint16_t HLING;
   /** HLOG Measurement Time "HLOGMTds" */
   DSL_uint16_t HLOGMT;
   /** HLOG Subcarrier Group Size DS "HLOGGds" */
   DSL_uint16_t HLOGG;
   /** QLN Measurment Time "QLNMTds" */
   DSL_uint16_t QLNMT;
   /** QLN Subcarrier Group Size DS "QLNGds" */
   DSL_uint16_t QLNG;
   /** SNR Measurement Time "SNRMTds" */
   DSL_uint16_t SNRMT;
   /** SNR Subcarrier Group Size DS "SNRGds" */
   DSL_uint16_t SNRG;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN Scaling Factor "HLINSCds" */
   DSL_uint16_t HLINSC;
   /** HLIN Subcarrier Group Size DS "HLINGds" */
   DSL_uint16_t HLING;
   /** HLOG Measurement Time "HLOGMTds" */
   DSL_uint16_t HLOGMT;
   /** HLOG Subcarrier Group Size DS "HLOGGds" */
   DSL_uint16_t HLOGG;
   /** QLN Measurment Time "QLNMTds" */
   DSL_uint16_t QLNMT;
   /** QLN Subcarrier Group Size DS "QLNGds" */
   DSL_uint16_t QLNG;
   /** SNR Measurement Time "SNRMTds" */
   DSL_uint16_t SNRMT;
   /** SNR Subcarrier Group Size DS "SNRGds" */
   DSL_uint16_t SNRG;
#endif
} __PACKED__ ;


/**
   Requests test parameter related information for the upstream direction: The HLIN
   scaling factor (HLINSCus), the subcarrier group size "G" and the measurement times
   for HLOGpsus, QLNpsus, SNRpsus.
   (Sections 7.5.1.26.7/8/10/11, 7.5.1.27.4/5 and 7.5.1.28.4/5 of G997.1)
   During STEADY_STATE, the command can be used to request near-end data only (CO). For
   the far-end data to be provided via the EOC channel the message
   CMD_TestParamsFE_Request must be applied.
   During loop diagnostic mode, the command can be used to request both near-end and
   far-end data.
*/
struct CMD_TestParamsAuxUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the test-parameter related information as requested by
   CMD_TestParamsAuxUS_Get: The HLIN scaling factor (HLINSCus), the subcarrier group
   size "G" and the measurement times for HLOGpsus, QLNpsus, SNRpsus.
*/
struct ACK_TestParamsAuxUS_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN Scaling Factor "HLINSCus" */
   DSL_uint16_t HLINSC;
   /** HLIN Subcarrier Group Size DS "HLINGus" */
   DSL_uint16_t HLING;
   /** HLOG Measurement Time "HLOGMTus" */
   DSL_uint16_t HLOGMT;
   /** HLOG Subcarrier Group Size DS "HLOGGus" */
   DSL_uint16_t HLOGG;
   /** QLN Measurment Time "QLNMTus" */
   DSL_uint16_t QLNMT;
   /** QLN Subcarrier Group Size DS "QLNGus" */
   DSL_uint16_t QLNG;
   /** SNR Measurement Time "SNRMTus" */
   DSL_uint16_t SNRMT;
   /** SNR Subcarrier Group Size DS "SNRGus" */
   DSL_uint16_t SNRG;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** HLIN Scaling Factor "HLINSCus" */
   DSL_uint16_t HLINSC;
   /** HLIN Subcarrier Group Size DS "HLINGus" */
   DSL_uint16_t HLING;
   /** HLOG Measurement Time "HLOGMTus" */
   DSL_uint16_t HLOGMT;
   /** HLOG Subcarrier Group Size DS "HLOGGus" */
   DSL_uint16_t HLOGG;
   /** QLN Measurment Time "QLNMTus" */
   DSL_uint16_t QLNMT;
   /** QLN Subcarrier Group Size DS "QLNGus" */
   DSL_uint16_t QLNG;
   /** SNR Measurement Time "SNRMTus" */
   DSL_uint16_t SNRMT;
   /** SNR Subcarrier Group Size DS "SNRGus" */
   DSL_uint16_t SNRG;
#endif
} __PACKED__ ;


/**
   This message is used to trigger retrieval of the far-end PMD Test Parameters Hlog,
   SNR and QLN via the EOC channel during Showtime. If StartIndex or EndIndex do not
   contain values in a valid range then no autonomous message will be sent by the
   firmware. Once the retrieval process is started, it can also be aborted with this
   message via the control parameter. The status of the parameter retrieval process can
   be polled via the CMD_TestParamsFE_Poll message. Once the parameters are retrieved,
   they are provided with the EVT_PMD_TestParamsGet message.
*/
struct CMD_TestParamsFE_Request
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-end Retrieval Control */
   DSL_uint16_t Control;
   /** Subcarrier Group Start Index */
   DSL_uint16_t StartIndex;
   /** Subcarrier Group End Index */
   DSL_uint16_t EndIndex;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-end Retrieval Control */
   DSL_uint16_t Control;
   /** Subcarrier Group Start Index */
   DSL_uint16_t StartIndex;
   /** Subcarrier Group End Index */
   DSL_uint16_t EndIndex;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_TestParamsFE_Request.
*/
struct ACK_TestParamsFE_Request
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message polls the status of the far end parameter retrieval process.
*/
struct CMD_TestParamsFE_Poll
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_TestParamsFE_Poll.
*/
struct ACK_TestParamsFE_Poll
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-end retrieval status */
   DSL_uint16_t Status;
   /** Reserved1 */
   DSL_uint16_t Res1;
   /** Reserved2 */
   DSL_uint16_t Res2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Far-end retrieval status */
   DSL_uint16_t Status;
   /** Reserved1 */
   DSL_uint16_t Res1;
   /** Reserved2 */
   DSL_uint16_t Res2;
#endif
} __PACKED__ ;


/**
   This event message provides the far-end data after the far end data retrieval
   process. This message is a result of the CMD_TestParamsFE_Request message.
*/
struct EVT_PMD_TestParamsGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved1 */
   DSL_uint16_t Res1;
   /** Subcarrier Group Start Index */
   DSL_uint16_t StartIndex;
   /** Subcarrier Group End Index */
   DSL_uint16_t EndIndex;
   /** HLOG Measurement Time HLOGMT */
   DSL_uint16_t hlogTime;
   /** SNR Measurement Time SNRMT */
   DSL_uint16_t snrTime;
   /** QLN Measurment Time QLNMT */
   DSL_uint16_t qlnTime;
   /** Testparameter Result */
   VNX_TestParam_t TestPar[60];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved1 */
   DSL_uint16_t Res1;
   /** Subcarrier Group Start Index */
   DSL_uint16_t StartIndex;
   /** Subcarrier Group End Index */
   DSL_uint16_t EndIndex;
   /** HLOG Measurement Time HLOGMT */
   DSL_uint16_t hlogTime;
   /** SNR Measurement Time SNRMT */
   DSL_uint16_t snrTime;
   /** QLN Measurment Time QLNMT */
   DSL_uint16_t qlnTime;
   /** Testparameter Result */
   VNX_TestParam_t TestPar[60];
#endif
} __PACKED__ ;


/**
   Requests the OLR status information on bit swaps, DRR and SRA requests for the
   receive direction.
*/
struct CMD_OLR_RxStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the OLR status information for the receive direction as requestey by
   CMD_OLR_RxStatusGet.
*/
struct ACK_OLR_RxStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Rx Bit Swap Requests */
   DSL_uint16_t BitswapReqs;
   /** Rx Extended Bit Swap Requests */
   DSL_uint16_t ExtBitswapReqs;
   /** Rx Bit Swap UTC Responses */
   DSL_uint16_t BitswapUTCs;
   /** Rx "Bit Swaps Performed" Count */
   DSL_uint16_t BitswapsDone;
   /** Rx Bitswap Timeouts */
   DSL_uint16_t BitswapTimeOuts;
   /** Reserved */
   DSL_uint16_t Res00[10];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Rx Bit Swap Requests */
   DSL_uint16_t BitswapReqs;
   /** Rx Extended Bit Swap Requests */
   DSL_uint16_t ExtBitswapReqs;
   /** Rx Bit Swap UTC Responses */
   DSL_uint16_t BitswapUTCs;
   /** Rx "Bit Swaps Performed" Count */
   DSL_uint16_t BitswapsDone;
   /** Rx Bitswap Timeouts */
   DSL_uint16_t BitswapTimeOuts;
   /** Reserved */
   DSL_uint16_t Res00[10];
#endif
} __PACKED__ ;


/**
   Requests the OLR status information on bit swaps, DRR and SRA requests for the
   transmit direction.
*/
struct CMD_OLR_TxStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Reports the OLR status information for the transmit direction as requestey by
   CMD_OLR_TxStatusGet.
*/
struct ACK_OLR_TxStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Tx Bit Swap Requests */
   DSL_uint16_t BitswapReqs;
   /** Tx Extended Bit Swap Requests */
   DSL_uint16_t ExtBitswapReqs;
   /** Tx Bit Swap UTC Responses */
   DSL_uint16_t BitswapUTCs;
   /** Tx "Bit Swaps Performed" Count */
   DSL_uint16_t BitswapsDone;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01[10];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Tx Bit Swap Requests */
   DSL_uint16_t BitswapReqs;
   /** Tx Extended Bit Swap Requests */
   DSL_uint16_t ExtBitswapReqs;
   /** Tx Bit Swap UTC Responses */
   DSL_uint16_t BitswapUTCs;
   /** Tx "Bit Swaps Performed" Count */
   DSL_uint16_t BitswapsDone;
   /** Reserved */
   DSL_uint16_t Res00;
   /** Reserved */
   DSL_uint16_t Res01[10];
#endif
} __PACKED__ ;


/**
   Triggers the insertion of corrupt CRC’s into the at either the VTU-R or VTU-C. The
   message controls the EOC "Diagnostic" command to start and stop the corrupt CRC
   transmission. (Section 11.2.3.2 of G993.2 [3]). The EOC "Diagnostic" command is sent
   by the CO only for triggering corrupt CRC’s.
*/
struct CMD_CRC_ErrorRequest
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** Control for Corrupt RxCRCs (at the CPE) */
   DSL_bf16_t crcRx : 1;
   /** Control for Corrupt TxCRCs (at the CPE) */
   DSL_bf16_t crcTx : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Control for Corrupt TxCRCs (at the CPE) */
   DSL_bf16_t crcTx : 1;
   /** Control for Corrupt RxCRCs (at the CPE) */
   DSL_bf16_t crcRx : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_CRC_ErrorRequest.
*/
struct ACK_CRC_ErrorRequest
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the status of corrupt CRC insertion, initiated by CMD_CRC_ErrorRequest.
*/
struct CMD_CRC_ErrorStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the status of corrupt CRC insertion. Acknowledgement for
   CMD_CRC_ErrorStatusGet.
*/
struct ACK_CRC_ErrorStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
   /** Status of Corrupt RxCRCs (at the CPE) */
   DSL_bf16_t crcErrorsRx : 1;
   /** Status of Corrupt TxCRCs */
   DSL_bf16_t crcErrorsTx : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Status of Corrupt TxCRCs */
   DSL_bf16_t crcErrorsTx : 1;
   /** Status of Corrupt RxCRCs (at the CPE) */
   DSL_bf16_t crcErrorsRx : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 14;
#endif
} __PACKED__ ;


/**
   Without affecting normal operation, certain types of data may be logged to a
   dedicated data buffer to be read out by the management system. This message is used
   to specify the type of data to be logged and the point in time at which to stop
   logging.
*/
struct CMD_DebugTrailControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Log Trail Type */
   DSL_uint16_t trailType;
   /** Log Trail Subtype */
   DSL_uint16_t trailSubtype;
   /** Reserved */
   DSL_uint16_t Res00[4];
   /** Reserved */
   DSL_bf16_t Res01 : 15;
   /** Cyclic Update Control */
   DSL_bf16_t CyclicUpdate : 1;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Log Trail Type */
   DSL_uint16_t trailType;
   /** Log Trail Subtype */
   DSL_uint16_t trailSubtype;
   /** Reserved */
   DSL_uint16_t Res00[4];
   /** Cyclic Update Control */
   DSL_bf16_t CyclicUpdate : 1;
   /** Reserved */
   DSL_bf16_t Res01 : 15;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_DebugTrailControl.
*/
struct ACK_DebugTrailControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message is used to retrieve debug trace data with the debug trace system. The
   debug trace system needs to be actived with CMD_DebugTrailControl, before this
   message can be used.
*/
struct CMD_RingbufferDataGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This is the acknowledgement for CMD_RingbufferDataGet.
*/
struct ACK_RingbufferDataGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Trace Data from the Ringbuffer */
   DSL_uint16_t Data[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Trace Data from the Ringbuffer */
   DSL_uint16_t Data[128];
#endif
} __PACKED__ ;


/**
   This message is used to retrieve the status of the debug trace system. The debug
   trace system needs to be actived with CMD_DebugTrailControl, before this message can
   be used.
*/
struct CMD_RingbufferStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Provides the Debug Trail Status requested by CMD_RingbufferStatusGet.
   The Debug Trail Index, Buffer Size, and Wrap Count are used to determine the
   location of valid data within the Debug Buffer to retrieve them with
   CMD_RingbufferDataGet.
   Note: See example below.
*/
struct ACK_RingbufferStatusGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Index */
   DSL_uint16_t rbIndex;
   /** Buffer Size */
   DSL_uint16_t bufSize;
   /** Wrap Count */
   DSL_uint16_t wrapCnt;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Index */
   DSL_uint16_t rbIndex;
   /** Buffer Size */
   DSL_uint16_t bufSize;
   /** Wrap Count */
   DSL_uint16_t wrapCnt;
#endif
} __PACKED__ ;


/**
   Configures the parameters for single-ended line testing (SELT).
*/
struct CMD_SELT_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
   /** UER Format Control */
   DSL_bf16_t SELT2 : 1;
   /** UER Control */
   DSL_bf16_t SELT1 : 1;
   /** QLN Control */
   DSL_bf16_t SELT0 : 1;
   /** Initial magnitude */
   DSL_uint16_t toneLevel;
   /** Number of Frames for QLN */
   DSL_uint16_t avgNum_QLN;
   /** Number of Frames for UER */
   DSL_uint16_t avgNum_UER;
   /** First UER Tone */
   DSL_uint16_t tone1st_UER;
   /** Last UER Tone */
   DSL_uint16_t toneLast_UER;
   /** FV1, Fixed Value */
   DSL_uint16_t N_factor;
   /** SELT Version Tag */
   DSL_uint16_t versionTag;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLN Control */
   DSL_bf16_t SELT0 : 1;
   /** UER Control */
   DSL_bf16_t SELT1 : 1;
   /** UER Format Control */
   DSL_bf16_t SELT2 : 1;
   /** Reserved */
   DSL_bf16_t Res00 : 13;
   /** Initial magnitude */
   DSL_uint16_t toneLevel;
   /** Number of Frames for QLN */
   DSL_uint16_t avgNum_QLN;
   /** Number of Frames for UER */
   DSL_uint16_t avgNum_UER;
   /** First UER Tone */
   DSL_uint16_t tone1st_UER;
   /** Last UER Tone */
   DSL_uint16_t toneLast_UER;
   /** FV1, Fixed Value */
   DSL_uint16_t N_factor;
   /** SELT Version Tag */
   DSL_uint16_t versionTag;
#endif
} __PACKED__ ;


/**
   Acknowledgement for CMD_SELT_Configure.
*/
struct ACK_SELT_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   The message requests the results of the quiet-line noise (QLN) measurement which is
   part of the SELT test, if enabled with CMD_SELT_Configure.
*/
struct CMD_SELT_QLN_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the QLN per subcarrier resulting from the SELT test.
*/
struct ACK_SELT_QLN_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLNps */
   DSL_uint16_t QLNps[128];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** QLNps */
   DSL_uint16_t QLNps[128];
#endif
} __PACKED__ ;


/**
   The message requests the results of the Uncalibrated Echo Response (UER) measurement
   which is part of the SELT test, if enabled with CMD_SELT_Configure.
*/
struct CMD_SELT_UER_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Returns information about the UER per subcarrier resulting from the SELT test.
*/
struct ACK_SELT_UER_Get
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** UERps */
   VNX_UER_t UERps[32];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** UERps */
   VNX_UER_t UERps[32];
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
