/* Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include "config.h"

#include <odp/api/event.h>
#include <odp/api/buffer.h>
#include <odp/api/crypto.h>
#include <odp/api/packet.h>
#include <odp/api/timer.h>
#include <odp/api/pool.h>
#include <odp_buffer_internal.h>
#include <odp_ipsec_internal.h>
#include <odp_buffer_inlines.h>
#include <odp_debug_internal.h>

odp_event_type_t odp_event_type(odp_event_t event)
{
	return _odp_buffer_event_type(odp_buffer_from_event(event));
}

odp_event_subtype_t odp_event_subtype(odp_event_t event)
{
	return _odp_buffer_event_subtype(odp_buffer_from_event(event));
}

odp_event_type_t odp_event_types(odp_event_t event,
				 odp_event_subtype_t *subtype)
{
	odp_buffer_t buf = odp_buffer_from_event(event);

	*subtype = _odp_buffer_event_subtype(buf);

	return _odp_buffer_event_type(buf);
}

void odp_event_free(odp_event_t event)
{
	switch (odp_event_type(event)) {
	case ODP_EVENT_BUFFER:
		odp_buffer_free(odp_buffer_from_event(event));
		break;
	case ODP_EVENT_PACKET:
		odp_packet_free(odp_packet_from_event(event));
		break;
	case ODP_EVENT_TIMEOUT:
		odp_timeout_free(odp_timeout_from_event(event));
		break;
	case ODP_EVENT_CRYPTO_COMPL:
		odp_crypto_compl_free(odp_crypto_compl_from_event(event));
		break;
	case ODP_EVENT_IPSEC_STATUS:
		_odp_ipsec_status_free(_odp_ipsec_status_from_event(event));
		break;
	default:
		ODP_ABORT("Invalid event type: %d\n", odp_event_type(event));
	}
}

uint64_t odp_event_to_u64(odp_event_t hdl)
{
	return _odp_pri(hdl);
}
