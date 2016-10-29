/**
 * @file ext_intf.h
 * @brief common interface for external communation
 * @author wangjiaming
 * @date 2012-02-16
 */
#ifndef _EXT_INTF_H
#define _EXT_INTF_H
#include "pcp.h"

void ext_intf_init();
void ext_intf_fd(int *max_fd, fd_set *rfds);
void ext_onotify(struct pcp_oper*);
void ext_iupdate(struct oper_head *oper_hd);
int ext_event(fd_set *r);
#endif
