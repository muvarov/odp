/* Copyright (c) 2014-2018, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */


/**
 * @file
 *
 * ODP timeout descriptor - implementation internal
 */

#ifndef ODP_TIMER_INTERNAL_H_
#define ODP_TIMER_INTERNAL_H_

#include <odp/api/align.h>
#include <odp/api/debug.h>
#include <odp_buffer_internal.h>
#include <odp_pool_internal.h>
#include <odp/api/timer.h>
#include <odp_global_data.h>

/* Minimum number of scheduling rounds between checking timer pools. */
#define CONFIG_TIMER_RUN_RATELIMIT_ROUNDS 1

/**
 * Internal Timeout header
 */
typedef struct {
	/* common buffer header */
	odp_buffer_hdr_t buf_hdr;

	/* Requested expiration time */
	uint64_t expiration;
	/* User ptr inherited from parent timer */
	void *user_ptr;
	/* Parent timer */
	odp_timer_t timer;
} odp_timeout_hdr_t;

unsigned _timer_run(void);

/* Static inline wrapper to minimize modification of schedulers. */
static inline unsigned timer_run(void)
{
	return odp_global_rw->inline_timers ? _timer_run() : 0;
}

#endif
