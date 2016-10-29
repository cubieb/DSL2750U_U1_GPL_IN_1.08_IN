/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : usblib_disk.h
 文件描述 : Hotplug标志处理函数


 函数列表 :


 修订记录 :
          1 创建 : 雷達
            日期 : 2008-5-12
            描述 :

**********************************************************************/
#ifndef __USBLIB_DISK_H__
#define __USBLIB_DISK_H__

/* fsid define */
#define DEFAULT_SECTOR_SIZE     512
#define MAX_SECTOR_SIZE 2048
#define SECTOR_SIZE     512     /* still used in osf/sgi/sun code */
#define MAXIMUM_PARTS   60

#define ACTIVE_FLAG     0x80
#define IS_ACTIVE(flag) ((flag) == ACTIVE_FLAG)

#define EXTENDED        0x05
#define WIN98_EXTENDED  0x0f
#define LINUX_PARTITION 0x81
#define LINUX_SWAP      0x82
#define LINUX_NATIVE    0x83
#define LINUX_EXTENDED  0x85
#define LINUX_LVM       0x8e
#define LINUX_RAID      0xfd
/* fsid */

typedef struct partition_info{
    struct partition_info *next;
    char *name; /* partitions name. */
    unsigned char ucboot_ind;         /* 0x80 - active */
    unsigned char ucsys_ind;          /* What partition type */
    unsigned long long ullpblocks;      /* partition's blocks */
    unsigned long long ullpodd;     /* odd flag on end */
}partinfo_t;

typedef struct{
    /* disk's content */
    char *name; /* disk name with full path */
    unsigned uheads, usectors, ucylinders;
    unsigned long long ullbytes;
    unsigned char ucpartsum;    /* total partitions */

    /* partitions's content */
    partinfo_t *part;
}diskinfo_container_t;

#define TDiskInfoContainer  diskinfo_container_t

/* disk info container member */
#define CNTNER_DSKINFO_SIZE (sizeof(diskinfo_container_t))
#define CNTNER_PARTINFO_SIZE    (sizeof(partinfo_t))        /* sizeof partinfo_t */
#define T_CNTNER_P(pcontainer)  ((diskinfo_container_t *)(pcontainer))
#define CNTNER_DSK_NAME(pcontainer) (T_CNTNER_P(pcontainer)->name)
#define CNTNER_DSK_HEADS(pcontainer)    (T_CNTNER_P(pcontainer)->uheads)
#define CNTNER_DSK_SECTORS(pcontainer)  (T_CNTNER_P(pcontainer)->usectors)
#define CNTNER_DSK_CYLINDERS(pcontainer)    (T_CNTNER_P(pcontainer)->ucylinders)
#define CNTNER_DSK_BYTES(pcontainer)    (T_CNTNER_P(pcontainer)->ullbytes)
#define CNTNER_DSK_PARTSUM(pcontainer)  (T_CNTNER_P(pcontainer)->ucpartsum)
#define CNTNER_DSK_PARTITIONS(pcontainer)   (T_CNTNER_P(pcontainer)->part)
#define T_PARTINFO_P(ppart)     ((partinfo_t *)(ppart))
#define CNTNER_PART_NEXT(ppart) (T_PARTINFO_P(ppart)->next)
#define CNTNER_PART_NAME(ppart) (T_PARTINFO_P(ppart)->name)
#define CNTNER_PART_ACTIVE_FLAG(ppart)  (T_PARTINFO_P(ppart)->ucboot_ind)
#define CNTNER_PART_FSTYPE(ppart)   (T_PARTINFO_P(ppart)->ucsys_ind)
#define CNTNER_PART_BLOCKS(ppart)   (T_PARTINFO_P(ppart)->ullpblocks)
#define CNTNER_PART_ODD(ppart)  (T_PARTINFO_P(ppart)->ullpodd)


extern const char *partition_type(unsigned char type);
extern int create_diskinfo_container(char *diskpath, diskinfo_container_t **ppcontainer);
extern void destroy_diskinfo_container(diskinfo_container_t *pcontainer);

#endif  /* __USBLIB_DISK_H__ */
