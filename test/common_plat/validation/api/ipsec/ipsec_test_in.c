/* Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include "config.h"

#include "ipsec.h"

#include "test_vectors.h"

static void test_in_ah_sha256(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, true, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_ah_sha256_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_ah_sha256_tun(void)
{
	odp_ipsec_tunnel_param_t tunnel = {};
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, true, 123, &tunnel,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_ah_tun_sha256_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_ah_sha256_tun_notun(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, true, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_ah_tun_sha256_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0_ipip },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_esp_null_sha256(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_esp_null_sha256_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_esp_aes_cbc_null(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 123, NULL,
			    ODP_CIPHER_ALG_AES_CBC, &key_a5_128,
			    ODP_AUTH_ALG_NULL, NULL,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_esp_aes_cbc_null_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_esp_aes_cbc_sha256(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 123, NULL,
			    ODP_CIPHER_ALG_AES_CBC, &key_a5_128,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_esp_aes_cbc_sha256_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_lookup_ah_sha256(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, true, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_ah_sha256_1,
		.lookup = 1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_lookup_esp_null_sha256(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_esp_null_sha256_1,
		.lookup = 1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_esp_null_sha256_tun(void)
{
	odp_ipsec_tunnel_param_t tunnel = {};
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 123, &tunnel,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_esp_tun_null_sha256_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_icmp_0 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_ah_esp_pkt(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	/* This test will not work properly inbound inline mode.
	 * test_in_lookup_ah_esp_pkt will be used instead. */
	if (suite_context.inbound_op_mode == ODP_IPSEC_OP_MODE_INLINE)
		return;

	ipsec_sa_param_fill(&param,
			    true, true, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_esp_null_sha256_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.proto = 1,
			  .pkt_out =  NULL },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_esp_ah_pkt(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	/* This test will not work properly inbound inline mode.
	 * test_in_lookup_esp_ah_pkt will be used instead. */
	if (suite_context.inbound_op_mode == ODP_IPSEC_OP_MODE_INLINE)
		return;

	ipsec_sa_param_fill(&param,
			    true, false, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_ah_sha256_1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.proto = 1,
			  .pkt_out = NULL },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_lookup_ah_esp_pkt(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, true, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_esp_null_sha256_1,
		.lookup = 1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.sa_lookup = 1,
			  .pkt_out =  NULL },
		},
	};

	ipsec_check_in_one(&test, ODP_IPSEC_SA_INVALID);

	ipsec_sa_destroy(sa);
}

static void test_in_lookup_esp_ah_pkt(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_ah_sha256_1,
		.lookup = 1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.sa_lookup = 1,
			  .pkt_out = NULL },
		},
	};

	ipsec_check_in_one(&test, ODP_IPSEC_SA_INVALID);

	ipsec_sa_destroy(sa);
}

static void test_in_ah_sha256_bad1(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, true, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_ah_sha256_1_bad1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.auth = 1,
			  .pkt_out = NULL },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_ah_sha256_bad2(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, true, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_ah_sha256_1_bad2,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.auth = 1,
			  .pkt_out = NULL },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_esp_null_sha256_bad1(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 123, NULL,
			    ODP_CIPHER_ALG_NULL, NULL,
			    ODP_AUTH_ALG_SHA256_HMAC, &key_5a_256,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_icmp_0_esp_null_sha256_1_bad1,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.auth = 1,
			  .pkt_out = NULL },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_rfc3602_5_esp(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 0x4321, NULL,
			    ODP_CIPHER_ALG_AES_CBC, &key_rfc3602,
			    ODP_AUTH_ALG_NULL, NULL,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_rfc3602_5_esp,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_rfc3602_5 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_rfc3602_6_esp(void)
{
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 0x4321, NULL,
			    ODP_CIPHER_ALG_AES_CBC, &key_rfc3602,
			    ODP_AUTH_ALG_NULL, NULL,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_rfc3602_6_esp,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_rfc3602_6 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_rfc3602_7_esp(void)
{
	odp_ipsec_tunnel_param_t tunnel = {};
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 0x8765, &tunnel,
			    ODP_CIPHER_ALG_AES_CBC, &key_rfc3602_2,
			    ODP_AUTH_ALG_NULL, NULL,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_rfc3602_7_esp,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_rfc3602_7 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_rfc3602_8_esp(void)
{
	odp_ipsec_tunnel_param_t tunnel = {};
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 0x8765, &tunnel,
			    ODP_CIPHER_ALG_AES_CBC, &key_rfc3602_2,
			    ODP_AUTH_ALG_NULL, NULL,
			    NULL);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_rfc3602_8_esp,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_rfc3602_8 },
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_mcgrew_gcm_2_esp(void)
{
	odp_ipsec_tunnel_param_t tunnel = {};
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 0xa5f8, &tunnel,
			    ODP_CIPHER_ALG_AES_GCM, &key_mcgrew_gcm_2,
			    ODP_AUTH_ALG_AES_GCM, NULL,
			    &key_mcgrew_gcm_salt_2);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_mcgrew_gcm_test_2_esp,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_mcgrew_gcm_test_2},
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_mcgrew_gcm_3_esp(void)
{
	odp_ipsec_tunnel_param_t tunnel = {};
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 0x4a2cbfe3, &tunnel,
			    ODP_CIPHER_ALG_AES_GCM, &key_mcgrew_gcm_3,
			    ODP_AUTH_ALG_AES_GCM, NULL,
			    &key_mcgrew_gcm_salt_3);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_mcgrew_gcm_test_3_esp,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_mcgrew_gcm_test_3},
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_mcgrew_gcm_4_esp(void)
{
	odp_ipsec_tunnel_param_t tunnel = {};
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 0x00000000, &tunnel,
			    ODP_CIPHER_ALG_AES_GCM, &key_mcgrew_gcm_4,
			    ODP_AUTH_ALG_AES_GCM, NULL,
			    &key_mcgrew_gcm_salt_4);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_mcgrew_gcm_test_4_esp,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_mcgrew_gcm_test_4},
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void test_in_mcgrew_gcm_12_esp(void)
{
	odp_ipsec_tunnel_param_t tunnel = {};
	odp_ipsec_sa_param_t param;
	odp_ipsec_sa_t sa;

	ipsec_sa_param_fill(&param,
			    true, false, 0x335467ae, &tunnel,
			    ODP_CIPHER_ALG_AES_GCM, &key_mcgrew_gcm_12,
			    ODP_AUTH_ALG_AES_GCM, NULL,
			    &key_mcgrew_gcm_salt_12);

	sa = odp_ipsec_sa_create(&param);

	CU_ASSERT_NOT_EQUAL_FATAL(ODP_IPSEC_SA_INVALID, sa);

	ipsec_test_part test = {
		.pkt_in = &pkt_mcgrew_gcm_test_12_esp,
		.out_pkt = 1,
		.out = {
			{ .status.warn.all = 0,
			  .status.error.all = 0,
			  .pkt_out = &pkt_mcgrew_gcm_test_12},
		},
	};

	ipsec_check_in_one(&test, sa);

	ipsec_sa_destroy(sa);
}

static void ipsec_test_capability(void)
{
	odp_ipsec_capability_t capa;

	CU_ASSERT(odp_ipsec_capability(&capa) == 0);
}

odp_testinfo_t ipsec_in_suite[] = {
	ODP_TEST_INFO(ipsec_test_capability),
	ODP_TEST_INFO_CONDITIONAL(test_in_rfc3602_5_esp,
				  ipsec_check_esp_aes_cbc_128_null),
	ODP_TEST_INFO_CONDITIONAL(test_in_rfc3602_6_esp,
				  ipsec_check_esp_aes_cbc_128_null),
	ODP_TEST_INFO_CONDITIONAL(test_in_rfc3602_7_esp,
				  ipsec_check_esp_aes_cbc_128_null),
	ODP_TEST_INFO_CONDITIONAL(test_in_rfc3602_8_esp,
				  ipsec_check_esp_aes_cbc_128_null),
	/* test 1, 5, 6, 8 -- 11 -- ESN */
	/* test 7 -- invalid, plaintext packet includes trl into IP length */
	ODP_TEST_INFO_CONDITIONAL(test_in_mcgrew_gcm_2_esp,
				  ipsec_check_esp_aes_gcm_128),
	ODP_TEST_INFO_CONDITIONAL(test_in_mcgrew_gcm_3_esp,
				  ipsec_check_esp_aes_gcm_256),
	ODP_TEST_INFO_CONDITIONAL(test_in_mcgrew_gcm_4_esp,
				  ipsec_check_esp_aes_gcm_128),
	ODP_TEST_INFO_CONDITIONAL(test_in_mcgrew_gcm_12_esp,
				  ipsec_check_esp_aes_gcm_128),
	ODP_TEST_INFO_CONDITIONAL(test_in_ah_sha256,
				  ipsec_check_ah_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_ah_sha256_tun,
				  ipsec_check_ah_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_ah_sha256_tun_notun,
				  ipsec_check_ah_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_esp_null_sha256,
				  ipsec_check_esp_null_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_esp_aes_cbc_null,
				  ipsec_check_esp_aes_cbc_128_null),
	ODP_TEST_INFO_CONDITIONAL(test_in_esp_aes_cbc_sha256,
				  ipsec_check_esp_aes_cbc_128_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_lookup_ah_sha256,
				  ipsec_check_ah_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_lookup_esp_null_sha256,
				  ipsec_check_esp_null_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_esp_null_sha256_tun,
				  ipsec_check_esp_null_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_ah_esp_pkt,
				  ipsec_check_ah_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_esp_ah_pkt,
				  ipsec_check_esp_null_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_lookup_ah_esp_pkt,
				  ipsec_check_ah_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_lookup_esp_ah_pkt,
				  ipsec_check_esp_null_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_ah_sha256_bad1,
				  ipsec_check_ah_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_ah_sha256_bad2,
				  ipsec_check_ah_sha256),
	ODP_TEST_INFO_CONDITIONAL(test_in_esp_null_sha256_bad1,
				  ipsec_check_esp_null_sha256),
	ODP_TEST_INFO_NULL,
};
