/* Copyright (C) 2006 Ikanos Communications
 * See IKANOS_PROP_LICENSE.txt for license information. */

#ifndef __QOS_UTIL_H
#define __QOS_UTIL_H

#define  AP_QOS_UTILS_FILE    "/proc/qos/qos_config"
#define  QOS_CONFIG_MAP       0x1
#define  QOS_CONFIG_MARK      0x2
#define  QOS_CONFIG_PRIORITY  0x3
#define  QOS_LIST_CONFIG      0x4
#define  QOS_CONFIG_REMARK    0x5
#define  QOS_FLUSH_CONFIG     0x6
#define  QOS_DEBUG_CONFIG     0x7


#define QOS_INSUFF_PARAMS			0x1
#define QOS_MORE_PARAMS				0x2
#define QOS_ONLY_DIGITS       0x3
#define QOS_ONLY_INFO         0x4
#define QOS_INVAL_PRIORITY    0x5
#define QOS_INVAL_FLUSH       0x6
#define QOS_INVAL_CMD         0x7

#define NEXT_ARG() do { argv++; if (--argc <= 0) qos_explain(QOS_INSUFF_PARAMS); } while(0)
#define FINISH_ARG() do { argv++; if (--argc != 0) qos_explain(QOS_MORE_PARAMS); } while(0)

#endif
