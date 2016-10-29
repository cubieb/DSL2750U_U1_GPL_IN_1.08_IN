
#ifndef _WANCOMMON_H
#define _WANCOMMON_H

#define GetCommonLinkProperties       DefaultAction

#define NewTotalBytesSent             DefaultAction
#define GetTotalBytesReceived         DefaultAction
#define GetTotalPacketsSent           DefaultAction
#define GetTotalPacketsReceived       DefaultAction
#define GetActiveConnection           DefaultAction
#define GetTotalBytesSent             DefaultAction
#define GetEnabledForInternet         DefaultAction

#define VAR_WANAccessType                   0
#define VAR_Layer1UpstreamMaxBitRate        1
#define VAR_Layer1DownstreamMaxBitRate      2
#define VAR_PhysicalLinkStatus              3
#define VAR_EnabledForInternet              4
#define VAR_TotalBytesSent                  5
#define VAR_TotalBytesReceived              6
#define VAR_TotalPacketsSent                7
#define VAR_TotalPacketsReceived            8

int WANCommonInterfaceConfig_GetVar(struct Service *psvc, int varindex);
int SetEnabledForInternet(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* _WANCOMMON_H */
