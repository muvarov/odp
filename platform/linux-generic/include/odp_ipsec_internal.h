/* Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:	BSD-3-Clause
 */

/**
 * @file
 *
 * ODP internal IPsec routines
 */

#ifndef ODP_IPSEC_INTERNAL_H_
#define ODP_IPSEC_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <odp/api/std_types.h>
#include <odp/api/plat/strong_types.h>

#include <odp/api/byteorder.h>
#include <odp/api/ipsec.h>
#include <odp/api/ticketlock.h>

/** @ingroup odp_ipsec
 *  @{
 */

typedef ODP_HANDLE_T(ipsec_status_t);

#define ODP_IPSEC_STATUS_INVALID \
	_odp_cast_scalar(ipsec_status_t, 0xffffffff)

typedef struct ipsec_sa_s ipsec_sa_t;

/**
 * @internal Get ipsec_status handle from event
 *
 * Converts an ODP_EVENT_IPSEC_STATUS type event to an IPsec status event.
 *
 * @param ev   Event handle
 *
 * @return IPsec status handle
 *
 * @see odp_event_type()
 */
ipsec_status_t _odp_ipsec_status_from_event(odp_event_t ev);

/**
 * @internal Free IPsec status event
 *
 * Frees the ipsec_status into the ipsec_status pool it was allocated from.
 *
 * @param res           IPsec status handle
 */
void _odp_ipsec_status_free(ipsec_status_t status);

/**
 * @internal Send ODP_IPSEC_STATUS event
 *
 * Sends the ipsec_status event using provided information
 *
 * @param queue         destination queue
 * @param id            status id
 * @param sa            SA respective to the operation
 * @param result        status value
 * @param warn          generated warning
 *
 * @retval 0 on success
 * @retval <0 on failure
 */
int _odp_ipsec_status_send(odp_queue_t queue,
			   odp_ipsec_status_id_t id,
			   odp_ipsec_sa_t sa,
			   int result,
			   odp_ipsec_warn_t warn);

#define IPSEC_MAX_IV_LEN	32   /**< Maximum IV length in bytes */

#define IPSEC_MAX_SALT_LEN	4    /**< Maximum salt length in bytes */

/**
 * Maximum number of available SAs
 */
#define ODP_CONFIG_IPSEC_SAS	8

struct ipsec_sa_s {
	odp_atomic_u32_t state ODP_ALIGNED_CACHE;

	uint32_t	ipsec_sa_idx;
	odp_ipsec_sa_t	ipsec_sa_hdl;

	odp_ipsec_protocol_t proto;
	uint32_t	spi;

	odp_ipsec_mode_t mode;

	/* Limits */
	uint64_t soft_limit_bytes;
	uint64_t soft_limit_packets;
	uint64_t hard_limit_bytes;
	uint64_t hard_limit_packets;

	/* Statistics for soft/hard expiration */
	odp_atomic_u64_t bytes;
	odp_atomic_u64_t packets;

	odp_crypto_session_t session;
	void		*context;
	odp_queue_t	queue;

	uint32_t	icv_len;
	uint32_t	esp_iv_len;
	uint32_t	esp_block_len;

	uint8_t		salt[IPSEC_MAX_SALT_LEN];
	uint32_t	salt_length;

	unsigned	dec_ttl : 1;
	unsigned	copy_dscp : 1;
	unsigned	copy_df : 1;

	union {
		struct {
			odp_ipsec_lookup_mode_t lookup_mode;
			odp_u32be_t	lookup_dst_ip;
		} in;

		struct {
			odp_u32be_t	tun_src_ip;
			odp_u32be_t	tun_dst_ip;

			/* 32-bit from which low 16 are used */
			odp_atomic_u32_t tun_hdr_id;
			odp_atomic_u32_t seq;

			uint8_t		tun_ttl;
			uint8_t		tun_dscp;
			uint8_t		tun_df;
		} out;
	};
};

/**
 * IPSEC Security Association (SA) lookup parameters
 */
typedef struct odp_ipsec_sa_lookup_s {
	/** IPSEC protocol: ESP or AH */
	odp_ipsec_protocol_t proto;

	/** SPI value */
	uint32_t spi;

	/* FIXME: IPv4 vs IPv6 */

	/** IP destination address (NETWORK ENDIAN) */
	void    *dst_addr;
} ipsec_sa_lookup_t;

/**
 * Obtain SA reference
 */
ipsec_sa_t *_odp_ipsec_sa_use(odp_ipsec_sa_t sa);

/**
 * Release SA reference
 */
void _odp_ipsec_sa_unuse(ipsec_sa_t *ipsec_sa);

/**
 * Lookup SA corresponding to inbound packet pkt
 */
ipsec_sa_t *_odp_ipsec_sa_lookup(const ipsec_sa_lookup_t *lookup);

/**
 * Update SA usage statistics, filling respective status for the packet.
 *
 * @retval <0 if hard limits were breached
 */
int _odp_ipsec_sa_update_stats(ipsec_sa_t *ipsec_sa, uint32_t len,
			       odp_ipsec_op_status_t *status);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
