#ifndef _MLD_H
#define _MLD_H




/* »’÷æ¥Ú”°∫Í */
#ifdef MLD_DEBUG
#define MLD_TRACE(fmt, args...)		printf("MLD " "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define MLD_TRACE(fmt, args...)
#endif

#define MAX_DEV_NAME_LEN 20


#define NODE_WANIF_NAME     "X_TWSZ-COM_ConnectedDeviceName"
#define NODE_WANIF_STAT     "ConnectionStatus"


#define MLD_PATH_PREFIX "InternetGatewayDevice.X_TWSZ-COM_MLD."
#define IGMP_SNOOPING_ENABLE "InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IGMPSnoop.Enabled"

#define NODE_MLD_PROXY "Proxy"
#define NODE_MLD_SNOOP "Snooping"

#define NODE_ENABLE "Enable"
#define NODE_ASSOCIATED_WAN_PATH "AssociatedWANConnection"
#define NODE_WANIF_STAT "ConnectionStatus"


#define WAN_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+"
#define WAN_PPPCONN_REGEX WAN_PATH_REGEX"\\.WANPPPConnection\\.[0-9]+\\.X_TWSZ-COM_IPv6Config+$"
#define WAN_IPCONN_REGEX WAN_PATH_REGEX"\\.WANIPConnection\\.[0-9]+\\.X_TWSZ-COM_IPv6Config+$"



#endif

