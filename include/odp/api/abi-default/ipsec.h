/* Copyright (c) 2016-2018, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

/**
 * @file
 *
 * ODP IPSEC API - platform specific types
 */

#ifndef ODP_ABI_IPSEC_H_
#define ODP_ABI_IPSEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <odp/api/std_types.h>

/** @internal Dummy type for strong typing */
typedef struct { char dummy; /**< @internal Dummy */ } _odp_abi_ipsec_sa_t;

/** @ingroup odp_ipsec
 *  @{
 */

typedef _odp_abi_ipsec_sa_t *odp_ipsec_sa_t;

#define ODP_IPSEC_SA_INVALID ((odp_ipsec_sa_t)0)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
