/*=========================================================================
Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
文件名称 : flash_layout.h
文件描述 : flash image布局的定义


修订记录 :
         1 创建 : 轩光磊
           日期 : 2008-9-12
           描述 :

         2 修订: 轩光磊
            日期: 2009-3-16
            描述: 优化条目的操作

         3 修订: 夏超仁
         	 日期: 2011-08-08
         	 描述: 重新对文件尾部定义,以简化升级流程


=========================================================================*/
#ifndef _FLASH_LAYOUT_H_
#define _FLASH_LAYOUT_H_

/*=========================================================================
	底层配置信息中产品名称、版本信息、
	image类别和序列号字符串长度的定义
=========================================================================*/

#define PRODUCT_NAME_LEN			32
#define TBS_VERSION_NAME_LEN		32
#define IMAGE_TYPES_NAME_LEN		8
#define BOARD_ID_NAME_LEN			16
#define ITEM_DATA_LEN               255

/* Bootloader固定占用64KB大小,u-boot.bin */
#define	BOOTCODE_LENGTH		( 0x10000 )

#define SYSTEM_CONFIG_VERSION		"TBSCFG01"
#define IMAGE_TAIL_VERSION			"TBSTLV01"
#define	VERSION_LEN					12 
/*====================================================================
	系统配置信息数据结构，数据项在打包时初始化
====================================================================*/
struct sys_cfg
{
    unsigned char cfg_version[VERSION_LEN];	/*配置信息版本*/
    unsigned int first_kernel_offset;
    unsigned int first_kernel_len;
    unsigned int first_rootfs_offset;
    unsigned int first_rootfs_len;
    unsigned long first_image_checksum;
    unsigned int second_kernel_offset;
    unsigned int second_kernel_len;
    unsigned int second_rootfs_offset;
    unsigned int second_rootfs_len;
    unsigned long second_image_checksum;
    unsigned int ip;
    unsigned char mac[6];
    unsigned char endian;
    unsigned char boot_img_flag;		/*0: 系统引导image1；1: 系统引导image 2*/
    unsigned char board_id[BOARD_ID_NAME_LEN];			/*客户电子签名*/
    unsigned char product[PRODUCT_NAME_LEN];				/*方案标识符*/
    unsigned char version[TBS_VERSION_NAME_LEN];			/*版本信息*/
    unsigned char tag[4];						/*  底层配置块标识符'sysc'  */
    unsigned char sn[20];
};

/*====================================================================
	wan侧配置信息数据结构，数据项在升级时初始化
====================================================================*/
typedef struct
{
    unsigned int version;
    unsigned int connection_type; //WAN连接类型：DHCP, PPPOE
    unsigned int ip;
    unsigned int subnet_mask;
    unsigned int gateway_ip;
    unsigned int vlan;
    unsigned int dns;
    unsigned int dns2;
    char pppoe_account[64];  //PPPOE用户名
    char pppoe_password[64]; //PPPOE密码
    unsigned int pppoe_auth_method;  //PPPOE认证类型
}
wan_eth_ipv4_config_t;

/*====================================================================
	升级配置信息数据结构，数据项在升级时初始化
====================================================================*/
typedef struct
{
    char url[512];       //升级URL
    char account[64];    //用户名
    char password[64];   //密码
}
upg_config_t;


/*====================================================================
	image尾部信息数据结构，数据项在打包时初始化, 整合原有头部
====================================================================*/
struct image_tail
{
    unsigned int bootloader_len;							/*bootloader长度*/
    unsigned int config_block_offset;					/*配置区偏移地址*/
    unsigned int config_block_len;						/*配置区长度*/
    unsigned int first_kernel_offset;					/*小image内核偏移地址*/
    unsigned int first_kernel_len;						/*小image内核长度*/
    unsigned int first_rootfs_offset;					/*小image根分区地址*/
    unsigned int first_rootfs_len;						/*小image根分区长度*/
    unsigned long first_image_checksum;					/*小imageCRC值*/
    unsigned int second_kernel_offset;					/*大image内核地址*/
    unsigned int second_kernel_len;						/*大image内核长度*/
    unsigned int second_rootfs_offset;					/*大image根分区地址*/
    unsigned int second_rootfs_len;						/*大image根分区长度*/
    unsigned long second_image_checksum;					/*大imageCRC值*/
    unsigned int boot_img_flag;		/*0: 系统引导image1；1: 系统引导image 2*/
    unsigned char board_id[BOARD_ID_NAME_LEN];			/*客户电子签名*/
    unsigned char version[TBS_VERSION_NAME_LEN];			/*版本信息*/
    unsigned char product[PRODUCT_NAME_LEN];				/*方案标识符*/
    unsigned int image_len;								/*文件长度*/
    unsigned int img_type; 								/*文件类型标识符*/
    unsigned char tail_version[VERSION_LEN];				/*尾部信息版本*/
    unsigned long image_checksum;						    /*image文件校验和*/
};

struct update_parameters {
	struct image_tail tail;
	unsigned int cfg_sector;
	unsigned int f_s_sector;
	unsigned int f_e_sector;
	unsigned int s_s_sector;
	unsigned int s_e_sector;
	unsigned long dest_addr;
	int          size;
	int syscfg_version;
};	


/*====================================================================
	文件类型标示符定义，用于判断接收的文件类型。
====================================================================*/

typedef enum {
    TYPE_UNKNOWN = 0,      /* 未知文件类型 */
    TYPE_SINGLE_IMG = 1,      /* 单image的IMG格式文件 */
    TYPE_SINGLE_BIN = 2,      /* 单image的BIN格式文件 */
    TYPE_DUAL_IMG = 3,      /* 双image的IMG格式文件 */
    TYPE_DUAL_BIN = 4,      /* 双image的BIN格式文件 */
    TYPE_BACKUP_IMG = 5,      /* 大小image的IMG格式文件 */
    TYPE_BACKUP_BIN = 6,      /* 大小image的BIN格式文件 */
    TYPE_BACKUP_FIMG = 7,     /* 大小image的大IMG格式文件 */
    TYPE_BACKUP_BIMG = 8,     /* 大小image的小IMG格式文件 */
    TYPE_BOOTLOADER = 9,      /* Bootloader */
    TYPE_VIRTUAL_DIMG = 10     /* 虚拟新版本双IMAGE文件 */
}image_type;


/*=========================================================================
	配置信息条目数据结构
=========================================================================*/

typedef struct
{
    unsigned short	crc;
    unsigned short	len;
    unsigned char	avail;
}
__attribute__ ((packed)) item_hdr_t;

typedef struct
{
    item_hdr_t	hdr;
    unsigned char	data[];				/* 数据*/
}
item_t;

#define		ITEM_HEAD_LEN				(sizeof( item_hdr_t))
#define		ITEM_SIZE(data_len)			((ITEM_HEAD_LEN + (data_len)) + ((ITEM_HEAD_LEN + (data_len)) % 2))
#define		CONFIG_MARK		            "OK"
#define		CONFIG_MARK_LEN	            (strlen(CONFIG_MARK))
#define		ITEM_UNAVAIL	            0x00
#define		ITEM_AVAIL		            0x11
#define		ITEM_BAD		            0x22
#define		ITEM_NULL		            0xFF


/*=========================================================================
	此数据结构用来应层接口做ioctl时与底层进行数据交互
=========================================================================*/

typedef struct
{
    unsigned char *name;
    unsigned char item_error;
    unsigned long len;
    unsigned char *data;				/* 数据*/
}
item_app_t;

/*=========================================================================
	系统中使用的条目名称，条目名称最好用宏定义放在这里
	便于管理和维护
=========================================================================*/

#define		LLCONFIG_NAME			"llconfig"
#define		WLAN_NAME				"wlan_cfg"
#define		TBS_APP_CFG			    "tbs_app_cfg"
#define		TR069_EVENT			    "tr069_event"
#define		PPPOE_SSID			    "pppoe_ssid"
#define     BOOT_TIMES              "boot_times"
#define     TBS_BIT_FLAG            "tbs_bit_flag"
#define     TBS_DEFCFG_PATH         "def_cfg_path"
#define     TBS_USERCFG_ITEM        "user_cfg_item"
#define     TBS_USERCFG_PREFIX		"tbs_app_cfg_"
#define		BACKUP_NET_CFG			"backup_net_cfg"
#define		TR069_UPG   			"tr069_upg"
#define     TBS_SNMP_EVENT		    "tbs_snmp"
#define     WLAN_PASSWORD           "wlan_password"
#define     TELNET_PASSWORD         "telnet_password"
#define     CUR_TELNET_PASS         "cur_telnet_pass"
#define     WLAN_SSID               "wlan_ssid"

/*=========================================================================
	在对条目操作时返回的错误码
=========================================================================*/

enum item_error
{
    ERROR_ITEM_OK = 0,
    ERROR_ITEM_MTD,
    ERROR_ITEM_IOCTRL,
    ERROR_ITEM_MALLOC,
    ERROR_ITEM_NOT_FIND,
    ERROR_ITEM_CRC,
    ERROR_ITEM_BIG,
    ERROR_ITEM_REPEAT_OK,
    ERROR_ITEM_REPEAT_FAIL,
    ERROR_CONFIG_LOST,
    ERROR_FLASH_BUSY
};

/*=========================================================================
	在对文件升级操作时返回的错误码
=========================================================================*/
enum upate_erros
{
	ERROR_OK ,
	ERROR_CRC ,
	ERROR_IMG_SIZE ,
	ERROR_PRODUCT ,
	ERROR_IMG_TYPE,
	ERROR_NO_MEM
};
/*=========================================================================
	应用层获取和保存条目的接口
=========================================================================*/

int app_item_get( void *data , char *item_name , unsigned short *len );		/* 获取相应条目信息*/
int app_item_save( void *data , char *item_name , unsigned short len );		/* 保存新条目*/

/*=========================================================================
	底层获取和保存条目的接口
=========================================================================*/
int item_get( void *data , char *item_name , unsigned short *len );		/* 获取相应条目信息*/
int item_save( void *data , char *item_name , unsigned short len );		/* 保存新条目*/


/*=========================================================================
	应用层获取MAC址址接口
=========================================================================*/

typedef struct
{
    int id;
    int	offset;
    unsigned char mac[6];
}
mac_t;

enum device_mac
{
    LAN_MAC = 0,
    WAN_MAC,
    WLAN_MAC,
    USB_MAC
};

int tbs_read_mac(int id, int offset, unsigned char *mac);
int app_tbs_read_mac(int id, int offset, unsigned char *mac);
int file_check(void *data, unsigned int len, int force_flag, void *ptr);
int flash_update(void *data, unsigned int len, int flag, void *tail);
int	is_sysdata(void *sys_data);

/*=========================================================================
	应用层调用DUAL IMAGE同步接口
=========================================================================*/
int app_dual_image_sync(void);
#endif  /*_FLASH_LAYOUT_H_*/

