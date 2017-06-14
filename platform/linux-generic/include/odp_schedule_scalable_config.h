/* Copyright (c) 2017, ARM Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#ifndef ODP_SCHEDULE_SCALABLE_CONFIG_H_
#define ODP_SCHEDULE_SCALABLE_CONFIG_H_

/*
 * Default scaling factor for the scheduler group
 *
 * This scaling factor is used when the application creates a scheduler
 * group with no worker threads.
 */
#define CONFIG_DEFAULT_XFACTOR 4

/*
 * Default weight (in events) for WRR in scalable scheduler
 *
 * This controls the per-queue weight for WRR between queues of the same
 * priority in the scalable scheduler
 * A higher value improves throughput while a lower value increases fairness
 * and thus likely decreases latency
 *
 * If WRR is undesired, set the value to ~0 which will use the largest possible
 * weight
 *
 * Note: an API for specifying this on a per-queue basis would be useful but is
 * not yet available
 */
#define CONFIG_WRR_WEIGHT 64

/*
 * Split queue producer/consumer metadata into separate cache lines.
 * This is beneficial on e.g. Cortex-A57 but not so much on A53.
 */
#define CONFIG_SPLIT_PRODCONS

/*
 * Use locks to protect queue (ring buffer) and scheduler state updates
 * On x86, this decreases overhead noticeably.
 */
#ifndef __ARM_ARCH
#define CONFIG_QSCHST_LOCK
/* Keep all ring buffer/qschst data together when using locks */
#undef CONFIG_SPLIT_PRODCONS
#endif

/*
 * Maximum number of ordered locks per queue.
 */
#define CONFIG_MAX_ORDERED_LOCKS_PER_QUEUE 2

#endif  /* ODP_SCHEDULE_SCALABLE_CONFIG_H_ */
