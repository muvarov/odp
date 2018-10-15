/* Copyright (c) 2016-2018, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <signal.h>

#include <odp_api.h>
#include <odp/helper/odph_api.h>

#define POOL_NUM_PKT 100
#define POOL_SEG_LEN 1500
#define MAX_PKT_BURST 32
#define MAX_WORKERS 1

typedef struct {
	odp_pktio_t if0, if1;
	odp_pktin_queue_t if0in, if1in;
	odp_pktout_queue_t if0out, if1out;
	odph_ethaddr_t src, dst;
	int g_ret;
} global_data_t;

static global_data_t *global;

static odp_pktio_t create_pktio(const char *name, odp_pool_t pool,
				odp_pktin_queue_t *pktin,
				odp_pktout_queue_t *pktout)
{
	odp_pktio_param_t pktio_param;
	odp_pktin_queue_param_t in_queue_param;
	odp_pktout_queue_param_t out_queue_param;
	odp_pktio_t pktio;

	odp_pktio_param_init(&pktio_param);

	pktio = odp_pktio_open(name, pool, &pktio_param);
	if (pktio == ODP_PKTIO_INVALID) {
		printf("Failed to open %s\n", name);
		exit(1);
	}

	odp_pktin_queue_param_init(&in_queue_param);
	odp_pktout_queue_param_init(&out_queue_param);

	in_queue_param.op_mode = ODP_PKTIO_OP_MT_UNSAFE;

	if (odp_pktin_queue_config(pktio, &in_queue_param)) {
		printf("Failed to config input queue for %s\n", name);
		exit(1);
	}

	out_queue_param.op_mode = ODP_PKTIO_OP_MT_UNSAFE;

	if (odp_pktout_queue_config(pktio, &out_queue_param)) {
		printf("Failed to config output queue for %s\n", name);
		exit(1);
	}

	if (odp_pktin_queue(pktio, pktin, 1) != 1) {
		printf("pktin queue query failed for %s\n", name);
		exit(1);
	}
	if (odp_pktout_queue(pktio, pktout, 1) != 1) {
		printf("pktout queue query failed for %s\n", name);
		exit(1);
	}
	return pktio;
}

static int run_worker(void *arg ODP_UNUSED)
{
	odp_packet_t pkt_tbl[MAX_PKT_BURST];
	int pkts, sent, tx_drops, i;
	int total_pkts = 0;
	uint64_t wait_time = odp_pktin_wait_time(2 * ODP_TIME_SEC_IN_NS);

	if (odp_pktio_start(global->if0)) {
		printf("unable to start input interface\n");
		exit(1);
	}
	printf("started input interface\n");
	if (odp_pktio_start(global->if1)) {
		printf("unable to start output interface\n");
		exit(1);
	}
	printf("started output interface\n");
	printf("started all\n");

	while (1) {
		pkts = odp_pktin_recv_tmo(global->if0in, pkt_tbl, MAX_PKT_BURST,
					  wait_time);
		if (odp_unlikely(pkts <= 0)) {
			printf("recv tmo!\n");
			break;
		}

		for (i = 0; i < pkts; i++) {
			odp_packet_t pkt = pkt_tbl[i];
			odph_ethhdr_t *eth;

			if (odp_unlikely(!odp_packet_has_eth(pkt))) {
				printf("warning: packet has no eth header\n");
				return 0;
			}
			eth = (odph_ethhdr_t *)odp_packet_l2_ptr(pkt, NULL);
			eth->src = global->src;
			eth->dst = global->dst;
		}
		sent = odp_pktout_send(global->if1out, pkt_tbl, pkts);
		if (sent < 0)
			sent = 0;
		total_pkts += sent;
		tx_drops = pkts - sent;
		if (odp_unlikely(tx_drops))
			odp_packet_free_multi(&pkt_tbl[sent], tx_drops);
	}

	printf("Total send packets: %d\n", total_pkts);

	if (total_pkts < 10)
		global->g_ret = -1;

	return 0;
}

int main(int argc, char **argv)
{
	odp_pool_t pool;
	odp_pool_param_t params;
	odp_cpumask_t cpumask;
	odph_odpthread_t thd[MAX_WORKERS];
	odp_instance_t instance;
	odph_odpthread_params_t thr_params;
	odp_shm_t shm;
	int ret;

	/* let helper collect its own arguments (e.g. --odph_proc) */
	argc = odph_parse_options(argc, argv);

	if (argc < 3) {
		printf("Too few arguments (%i).\n"
		       "Two interface names needed as arguments.\n", argc);
		exit(0);
	}

	if (odp_init_global(&instance, NULL, NULL)) {
		printf("Error: ODP global init failed.\n");
		exit(1);
	}

	if (odp_init_local(instance, ODP_THREAD_CONTROL)) {
		printf("Error: ODP local init failed.\n");
		exit(1);
	}

	/* Reserve memory for args from shared mem */
	shm = odp_shm_reserve("_appl_global_data", sizeof(global_data_t),
			      ODP_CACHE_LINE_SIZE, 0);
	global = odp_shm_addr(shm);
	if (global == NULL) {
		printf("Error: shared mem alloc failed.\n");
		exit(EXIT_FAILURE);
	}

	memset(global, 0, sizeof(global_data_t));

	/* Create packet pool */
	odp_pool_param_init(&params);
	params.pkt.seg_len = POOL_SEG_LEN;
	params.pkt.len     = POOL_SEG_LEN;
	params.pkt.num     = POOL_NUM_PKT;
	params.type        = ODP_POOL_PACKET;

	pool = odp_pool_create("packet pool", &params);

	if (pool == ODP_POOL_INVALID) {
		printf("Error: packet pool create failed.\n");
		exit(1);
	}

	global->if0 = create_pktio(argv[1], pool, &global->if0in,
				   &global->if0out);
	global->if1 = create_pktio(argv[2], pool, &global->if1in,
				   &global->if1out);

	odp_cpumask_default_worker(&cpumask, MAX_WORKERS);

	memset(&thr_params, 0, sizeof(thr_params));
	thr_params.start    = run_worker;
	thr_params.arg      = NULL;
	thr_params.thr_type = ODP_THREAD_WORKER;
	thr_params.instance = instance;

	odph_odpthreads_create(thd, &cpumask, &thr_params);
	odph_odpthreads_join(thd);

	ret = global->g_ret;

	if (odp_pool_destroy(pool)) {
		printf("Error: pool destroy\n");
		exit(EXIT_FAILURE);
	}

	if (odp_shm_free(shm)) {
		printf("Error: shm free global data\n");
		exit(EXIT_FAILURE);
	}

	if (odp_term_local()) {
		printf("Error: term local\n");
		exit(EXIT_FAILURE);
	}

	if (odp_term_global(instance)) {
		printf("Error: term global\n");
		exit(EXIT_FAILURE);
	}

	return ret;
}
