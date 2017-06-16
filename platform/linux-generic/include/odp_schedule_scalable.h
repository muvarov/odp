/* Copyright (c) 2017, ARM Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#ifndef ODP_SCHEDULE_SCALABLE_H
#define ODP_SCHEDULE_SCALABLE_H

#include <odp/api/align.h>
#include <odp/api/schedule.h>
#include <odp/api/ticketlock.h>

#include <odp_schedule_scalable_config.h>
#include <odp_schedule_scalable_ordered.h>
#include <odp_llqueue.h>

/*
 * ODP_SCHED_PRIO_HIGHEST/NORMAL/LOWEST/DEFAULT are compile time
 * constants, but not ODP_SCHED_PRIO_NUM. The current API for this
 * is odp_schedule_num_prio(). The other schedulers also define
 * this internally as NUM_PRIO.
 */
#define ODP_SCHED_PRIO_NUM  8

typedef struct {
	union {
		struct {
			struct llqueue llq;
			uint32_t prio;
		};
		char line[ODP_CACHE_LINE_SIZE];
	};
} sched_queue_t ODP_ALIGNED_CACHE;

#define TICKET_INVALID (uint16_t)(~0U)

typedef struct {
	int32_t numevts;
	uint16_t wrr_budget;
	uint8_t cur_ticket;
	uint8_t nxt_ticket;
} qschedstate_t ODP_ALIGNED(sizeof(uint64_t));

typedef uint32_t ringidx_t;

#ifdef CONFIG_SPLIT_PRODCONS
#define SPLIT_PC ODP_ALIGNED_CACHE
#else
#define SPLIT_PC
#endif

#define ODP_NO_SCHED_QUEUE (ODP_SCHED_SYNC_ORDERED + 1)

typedef struct {
	struct llnode node;  /* must be first */
	sched_queue_t *schedq;
#ifdef CONFIG_QSCHST_LOCK
	odp_ticketlock_t qschlock;
#endif
	qschedstate_t qschst;
	uint16_t pop_deficit;
	uint16_t qschst_type;
	ringidx_t prod_read SPLIT_PC;
	ringidx_t prod_write;
	ringidx_t prod_mask;
	odp_buffer_hdr_t **prod_ring;
	ringidx_t cons_write SPLIT_PC;
	ringidx_t cons_read;
	reorder_window_t *rwin;
	void *user_ctx;
#ifdef CONFIG_SPLIT_PRODCONS
	odp_buffer_hdr_t **cons_ring;
	ringidx_t cons_mask;
	uint16_t cons_type;
#else
#define cons_mask prod_mask
#define cons_ring prod_ring
#define cons_type qschst_type
#endif
} sched_elem_t ODP_ALIGNED_CACHE;

/* Number of scheduling groups */
#define MAX_SCHED_GROUP (sizeof(sched_group_mask_t) * CHAR_BIT)

typedef bitset_t sched_group_mask_t;

typedef struct {
	/* Threads currently associated with the sched group */
	bitset_t thr_actual[ODP_SCHED_PRIO_NUM] ODP_ALIGNED_CACHE;
	bitset_t thr_wanted;
	/* Used to spread queues over schedq's */
	uint32_t xcount[ODP_SCHED_PRIO_NUM];
	/* Number of schedq's per prio */
	uint32_t xfactor;
	char name[ODP_SCHED_GROUP_NAME_LEN];
	/* ODP_SCHED_PRIO_NUM * xfactor. Must be last. */
	sched_queue_t schedq[1] ODP_ALIGNED_CACHE;
} sched_group_t;

/* Number of reorder contexts per thread */
#define TS_RVEC_SIZE 16

typedef struct {
	/* Atomic queue currently being processed or NULL */
	sched_elem_t *atomq;
	/* Current reorder context or NULL */
	reorder_context_t *rctx;
	uint8_t pause;
	uint8_t out_of_order;
	uint8_t tidx;
	uint8_t pad;
	uint32_t dequeued; /* Number of events dequeued from atomic queue */
	uint16_t pktin_next; /* Next pktin tag to poll */
	uint16_t pktin_poll_cnts;
	uint16_t ticket; /* Ticket for atomic queue or TICKET_INVALID */
	uint16_t num_schedq;
	uint16_t sg_sem; /* Set when sg_wanted is modified by other thread */
#define SCHEDQ_PER_THREAD (MAX_SCHED_GROUP * ODP_SCHED_PRIO_NUM)
	sched_queue_t *schedq_list[SCHEDQ_PER_THREAD];
	/* Current sched_group membership */
	sched_group_mask_t sg_actual[ODP_SCHED_PRIO_NUM];
	/* Future sched_group membership. */
	sched_group_mask_t sg_wanted[ODP_SCHED_PRIO_NUM];
	bitset_t priv_rvec_free;
	/* Bitset of free entries in rvec[] */
	bitset_t rvec_free ODP_ALIGNED_CACHE;
	/* Reordering contexts to allocate from */
	reorder_context_t rvec[TS_RVEC_SIZE] ODP_ALIGNED_CACHE;
} sched_scalable_thread_state_t ODP_ALIGNED_CACHE;

void sched_update_enq(sched_elem_t *q, uint32_t actual);
void sched_update_enq_sp(sched_elem_t *q, uint32_t actual);
sched_queue_t *schedq_from_sched_group(odp_schedule_group_t grp, uint32_t prio);
void sched_group_xcount_dec(odp_schedule_group_t grp, uint32_t prio);

#endif  /* ODP_SCHEDULE_SCALABLE_H */
