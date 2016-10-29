#ifndef _GPON_CLI_H_
#define _GPON_CLI_H_

#define    GPON_GET_CONFIG              SIOCDEVPRIVATE+0
#define    GPON_GET_GEM_CONFIG                  1
#define    GPON_GET_USGEM_CONFIG                2
#define    GPON_GET_DSGEM_CONFIG                3
#define    GPON_GET_BCAST_GEM_CONFIG            4
#define    GPON_GET_TCONT_CONFIG                5
#define    GPON_GET_FDB_CONFIG                  6
#define    GPON_GET_AES_GEM_CONFIG              7
#define    GPON_GET_ALLOCID_CONFIG              8
#define    GPON_GET_BWMAP_CONFIG                9
#define    GPON_GET_DSGEM_CONFIG_ALL            10
#define    GPON_GET_USGEM_CONFIG_ALL            11
#define    GPON_GET_GEM_CONFIG_ALL		12
#define    GPON_GET_MCAST_FILTER_CONFIG		13

#define    GPON_GET_STATS               SIOCDEVPRIVATE+1
#define    GPON_GET_US_STATS                    1
#define    GPON_GET_DS_STATS                    2
#define    GPON_GET_OMCC_STATS                  3
#define    GPON_GET_PLOAM_STATS                 4
#define    GPON_GET_AES_STATS                   5
#define    GPON_GET_GIGE_STATS                  6
#define    GPON_GET_US_QM_STATS                 7
#define    GPON_GET_DS_FRAMER_STATS             8
#define    GPON_GET_GBIST_STATS                 9
#define    GPON_GET_DS_MCAST_FILTER_STATS       10
#define    GPON_GET_US_FDB_STATS                11
#define    GPON_GET_GEM_STATS                   12

#define    GPON_ADD_CONFIG              SIOCDEVPRIVATE+6
#define    GPON_ADD_UP_PORT                     1
#define    GPON_ADD_DOWN_PORT                   2
#define    GPON_ADD_TCONT                       3
#define    GPON_ADD_US_DEFAULT_GEM_PORT         4
#define    GPON_ADD_ONT                         5
#define    GPON_ADD_DS_MCAST_FILTER             6

#define    GPON_DEL_CONFIG              SIOCDEVPRIVATE+3

#define    GPON_DIAG_TEST               SIOCDEVPRIVATE+4

typedef struct {
        unsigned int cmd;  //sub command within an IOCTL option
        void         *bufPtr; //buffer send/receive data between CLI and device
}gponcmd_t;

typedef struct {
        int count;
} allocid_get_cfg_t;

typedef struct {
        int count;
        int valid_only;
} bwmap_get_cfg_t;

typedef struct {
        unsigned short gem_id;
        unsigned short vlan_id;
        unsigned int mac_address;
} gpon_ds_mcast_filter_cfg_t;

typedef struct {
        unsigned char gige_id;
        unsigned short gem_port_id;
} gpon_gem_port_t;


#endif

