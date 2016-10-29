A. Files to use for PTMF/EIU/INI
================================

A.1. For release 3.x.x and pre 4.2.1
------------------------------------

For INI FPGA CPE 1.0:

	/iniapi/p2cpe/HostPtmfHwRegs.h
	/iniapi/p2cpe/HostPtmfApi.c
	/iniapi/p2cpe/HostPtmfUtils.c
	/iniapi/p2cpe/HostPtmfUtils.h

	/include/api/HostPtmfApi.h
	/include/api/HostPtmfUtils.h

For 16-port PTMF CO:

	/iniapi/p2co/HostPtmfHwRegs.h
	/iniapi/p2co/HostPtmfApi.c
	/iniapi/p2co/HostPtmfUtils.c

	/include/api/HostPtmfApi.h
	/include/api/HostPtmfUtils.h

A.2. For release post 4.2.1
---------------------------
For Ethernet Interface Unit of CPE 2.0:

	/iniapi/HostEnetPhyUtils.c
	/iniapi/HostPtmfApi.c
	/iniapi/HostPtmfDbg.c

	/iniapi/eiu/HostPtmfHwRegs.h
	/iniapi/eiu/HostPtmfUtils.c

	/include/api/HostPtmfApi.h
	/include/api/HostEnetPhyUtils.h
	/include/api/HostPtmfUtils.h

For 16-port PTMF CO:

	/iniapi/HostEnetPhyUtils.c
	/iniapi/HostPtmfApi.c
	/iniapi/HostPtmfDbg.c

	/iniapi/p2co/HostPtmfHwRegs.h
	/iniapi/p2co/HostPtmfUtils.c

	/include/api/HostPtmfApi.h
	/include/api/HostEnetPhyUtils.h
	/include/api/HostPtmfUtils.h