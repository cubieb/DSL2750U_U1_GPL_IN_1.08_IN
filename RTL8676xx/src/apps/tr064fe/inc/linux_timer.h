/*****************************************************************************
//
//  Copyright (c) 2006  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of:
//          Broadcom Corporation
//          16215 Alton Parkway
//          Irvine, California 92619
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
******************************************************************************
//
//  Filename:       linux_timer.h
//
******************************************************************************/

#ifndef _linux_timer_h_
#define _linux_timer_h_

#if defined(linux)


timer_t enqueue_event(struct itimerspec *value, event_callback_t func, void *arg);

void init_event_queue(int);
int timer_create(clockid_t         clock_id, /* clock ID (always CLOCK_REALTIME) */
                 struct sigevent * evp,      /* user event handler */
                 timer_t *         pTimer    /* ptr to return value */);
int timer_settime
(
    timer_t                   timerid, /* timer ID */
    int                       flags,   /* absolute or relative */
    const struct itimerspec * value,   /* time to be set */
    struct itimerspec *       ovalue   /* previous time set (NULL=no result) */
);

int timer_connect(timer_t     timerid, /* timer ID timer*/
                  void (*routine)(timer_t, int), /* user routine */
                  int arg      /* user argument */);

int timer_delete(timer_t timerid /* timer ID */);

#endif /* linux */

#endif /* linux_timer_h_ */
