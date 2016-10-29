#ifndef _DEVICECONFIGPARAMS_H
#define _DEVICECONFIGPARAMS_H

#define VAR_PersistenData    0
#define VAR_ConfigFile       1
#define VAR_AStatus          2
#define VAR_AUUID            3


int DeviceConfig_Init(PService, service_state_t);
int DeviceConfig_GetVar(struct Service *psvc, int varindex);
int Tr64FactoryReset(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int Tr64Reboot(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
#endif /* _DEVICECONFIGPARAMS_H */
