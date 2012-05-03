#ifndef CPE_DR_PBUF_INTERNAL_OPS_H
#define CPE_DR_PBUF_INTERNAL_OPS_H
#include "dr_pbuf_internal_types.h"

struct cpe_dr_pbuf_longlong {
	uint32_t low;
	uint32_t hi;
};

int cpe_dr_pbuf_encode32(uint32_t number, uint8_t buffer[10]);
int cpe_dr_pbuf_encode64(uint64_t number, uint8_t buffer[10]);
int cpe_dr_pbuf_zigzag32(int32_t number, uint8_t buffer[10]);
int cpe_dr_pbuf_zigzag64(int64_t number, uint8_t buffer[10]);

int cpe_dr_pbuf_decode(uint8_t buffer[10], struct cpe_dr_pbuf_longlong *result);
void cpe_dr_pbuf_dezigzag64(struct cpe_dr_pbuf_longlong *r);
void cpe_dr_pbuf_dezigzag32(struct cpe_dr_pbuf_longlong*r);


#endif
