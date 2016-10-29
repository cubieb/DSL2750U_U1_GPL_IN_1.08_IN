#include "rtk_voip.h"
#include "RtcpReceiverXR.h"

static void RtcpRx_ReportBlock_DLRR( uint32 sid, 
							RtcpXRReportBlockHeader *pReportBlock )
{
}

void RtcpRx_readXR (uint32 sid, RtcpHeader* head)
{
	uint32 total_length = head ->length * 4;	// in bytes   
	uint32 offset = 0;
	uint8 *middle = ( uint8 * )head;
	RtcpXRReportBlockHeader *pReportBlock;
	
	offset = sizeof( RtcpHeader ) + 4;	// +4 for SSRC 
	
	while( offset < total_length ) {
		
		pReportBlock = ( RtcpXRReportBlockHeader * )( middle + offset );
		
		switch( pReportBlock ->blockType ) {
		case rtcpXRBT_LossRLE:
		case rtcpXRBT_DuplicateRLE:
		case rtcpXRBT_PacketReceiptTimes:
		case rtcpXRBT_ReceiverReferenceTime:
			break;
			
		case rtcpXRBT_DLRR:
			RtcpRx_ReportBlock_DLRR( sid, pReportBlock );
			break;
			
		case rtcpXRBT_StatisticsSummary:
		case rtcpXRBT_VoIPMetrics:
			break;
			
		default:
			printk( "Bad XR block report type\n" );
			break;
		}
				
		offset += pReportBlock ->blockLength * 4 + 4;	// +4 for BT header 
	}
}


