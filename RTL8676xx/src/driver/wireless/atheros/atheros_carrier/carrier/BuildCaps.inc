#
# defaults
#
ifndef ATH_CAP_SUPERG_FF
export ATH_CAP_SUPERG_FF=1
endif

ifndef ATH_CAP_DYNTURBO
export ATH_CAP_DYNTURBO=0
endif

ifndef ATH_CAP_SUPERG_COMP
export ATH_CAP_SUPERG_COMP=0
endif

ifndef ATH_CAP_XR
export ATH_CAP_XR=1
endif

ifndef ATH_CAP_TX99
export ATH_CAP_TX99=1
endif

ifndef ATH_CAP_CWM
export ATH_CAP_CWM=1
endif

ifndef ATH_CHAINMASK_SELECT
export ATH_CHAINMASK_SELECT=1
endif

ifndef ATH_FORCE_PPM
export ATH_FORCE_PPM=1
endif

ifndef ATH_FORCE_BIAS
export ATH_FORCE_BIAS=1
endif

ifndef AR5416_EMULATION
export AR5416_EMULATION=1
endif

# Multicast Tunnelling with IGMP Snooping
ifndef ATH_MC_TUNNEL
export ATH_MC_TUNNEL=1

endif

ifndef AR9100
export AR9100=0
endif

ifndef ATH_NO_DEBUG
IEEE80211_DEBUG=1
endif

#
# directives
#
ifneq ($(strip ${ATH_CAP_SUPERG_FF}),0)
COPTS+= -DATH_SUPERG_FF=1
endif

ifneq ($(strip ${ATH_CAP_DYNTURBO}),0)
COPTS+= -DATH_SUPERG_DYNTURBO=1 -DATH_TURBO_SCAN=1
endif

ifneq ($(strip ${ATH_CAP_SUPERG_COMP}),0)
COPTS+= -DATH_SUPERG_COMP=1
endif

ifneq ($(strip ${ATH_CAP_XR}),0)
COPTS+= -DATH_SUPERG_XR=1
endif

ifneq ($(strip ${ATH_CAP_CWM}),0)
COPTS+= -DATH_CWM=1
endif

ifneq ($(strip ${ATH_CAP_TX99}),0)
COPTS+= -DATH_TX99_DIAG=1
endif

ifneq ($(strip ${ATH_CHAINMASK_SELECT}),0)
COPTS+= -DATH_CHAINMASK_SELECT=1
endif

ifneq ($(strip ${ATH_FORCE_PPM}),0)
COPTS+= -DATH_FORCE_PPM=1
endif

ifneq ($(strip ${ATH_FORCE_BIAS}),0)
COPTS+= -DATH_FORCE_BIAS=1
endif

ifneq ($(strip ${AR5416_EMULATION}),0)
COPTS+= -DAR5416_EMULATION=1
endif

# Multicast Tunnelling with IGMP Snooping
ifneq ($(strip ${ATH_MC_TUNNEL}),0)
COPTS+= -DATH_MC_TUNNEL=1
endif

ifneq ($(strip ${AR9100}),0)
COPTS+= -DAR9100=1
endif

ifeq ($(strip ${IEEE80211_DEBUG}),1)
COPTS+= -DIEEE80211_DEBUG=1
endif

COPTS+= -DDEBUG_PKTLOG
COPTS+= -DATH_FULL_APSCAN=1
