/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称 : nfmark.h
 文件描述 : skb->nfmark bit位划分

 修订记录 :
          1 创建 : pengyao
            日期 : 2010-12-07
            描述 :
**********************************************************************/
#ifndef __NFMARK_H__
#define __NFMARK_H__


/*********************************************************************
 *                              mark相关宏定义                       *
 *********************************************************************/

/*
skb->nfmark 32bit位划分:
                            1111    //0 - 3bit用于ClsIdx
                         111        //4 - 6bit用于L2/L3/FW/APP/POST分类
                        1           //7 bit用于上下行数据流标记
                    1111            //8 - 11bit用于bridge实例标记
                1111                //12 - 15bit用于dummyport设备编号标记
           11111                    //16 - 20bit用于策略路由标记
11111111111                         //21 - 31bit预留



0 - 3bit用于ClsIdx
4 - 6bit用于L2/L3/FW/APP/POST分类
7 bit用于上下行数据流标记

8 - 11bit用于bridge实例标记
12 - 15bit用于dummy设备编号标记
16 - 20bit用于策略路由标记
21 - 31bit预留
*/

/*ClsIdx占用的bit位*/
#define CLASSIDX_NFMARK_START_BIT         (0)
#define CLASSIDX_NFMARK_END_BIT           (3)

/*L2/L3/FW/APP/POST占用的bit位*/
#define TYPE_NFMARK_START_BIT             (4)
#define TYPE_NFMARK_END_BIT               (6)

/*上下行标记的bit位*/
#define UP_DOWN_NFMARK_BIT                (7)

/*bridge mark占用的bit位*/
#define VLAN_BRID_MARK_START_BIT          (8)
#define VLAN_BRID_MARK_END_BIT            (11)

/*dummyport设备编号占用的bit位*/
#define DUMMYPORTDEV_MARK_START_BIT       (12)
#define DUMMYPORTDEV_MARK_END_BIT         (15)

/*策略路由标记占用的bit位*/
#define PROUTE_MARK_START_BIT             (16)
#define PROUTE_MARK_END_BIT               (20)

/*ClsIdx对应的mask*/
#define CLASSIDX_NFMARK_MASK \
    ((0xFFFFFFFFUL<<CLASSIDX_NFMARK_START_BIT) - (0xFFFFFFFFUL<<(CLASSIDX_NFMARK_END_BIT + 1)))

/*L2/L3/FW/APP/POST对应的mask*/
#define TYPE_NFMARK_MASK \
    ((0xFFFFFFFFUL<<TYPE_NFMARK_START_BIT) - (0xFFFFFFFFUL<<(TYPE_NFMARK_END_BIT + 1)))


/*上下行mark标记mask*/
#define UP_DOWN_NFMARK_MASK     (0x1UL<<UP_DOWN_NFMARK_BIT)


/*bridge实例id对应的mask*/
#define VLAN_BRKEY_MARK_MASK \
    ((0XFFFFFFFFUL<<VLAN_BRID_MARK_START_BIT) - (0XFFFFFFFFUL<<(VLAN_BRID_MARK_END_BIT + 1)))

/*十进制bridge实例id转成:mask+mark表示*/
#define VLAN_GET_BRKEY_MARK(iMark) \
        (VLAN_BRKEY_MARK_MASK&(iMark<<VLAN_BRID_MARK_START_BIT))


/*dummyport设备编号对应的mask*/
#define DUMMYPORTDEV_MARK_MASK \
    ((0XFFFFFFFFUL<<DUMMYPORTDEV_MARK_START_BIT) - (0XFFFFFFFFUL<<(DUMMYPORTDEV_MARK_END_BIT + 1)))

/*十进制dummyport设备编号转成:mask+mark表示*/
#define DUMMYPORTDEV_MARK(iMark) \
        (DUMMYPORTDEV_MARK_MASK&(iMark<<DUMMYPORTDEV_MARK_START_BIT))


/*策略路由标记mark对应的mask*/
#define PROUTE_MARK_MASK \
    ((0XFFFFFFFFUL<<PROUTE_MARK_START_BIT) - (0XFFFFFFFFUL<<(PROUTE_MARK_END_BIT + 1)))

/* 策略路由mark 基准值*/
#define PROUTE_MARK_BASE  200

/*十进制策略路由标记转成:mask+mark表示，从skb->mark中取出时应加上PROUTE_MARK_BASE*/
#define PROUTE_MARK(iMark) \
        (PROUTE_MARK_MASK&((iMark - PROUTE_MARK_BASE)<<PROUTE_MARK_START_BIT))

/*从skb->mark中提取策略路由标记*/
#define GET_PROUTE_MARK_BY_MASK(mark) \
        (((PROUTE_MARK_MASK&(mark))>>PROUTE_MARK_START_BIT) + PROUTE_MARK_BASE)

/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/
#endif /* __NFMARK_H__ */

