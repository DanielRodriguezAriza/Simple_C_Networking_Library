#ifndef DRA_NET_CONVERSIONS_H
#define DRA_NET_CONVERSIONS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

#define dra_host_to_network(dst,src,size) dra_change_byte_order(dst,src,size)
#define dra_network_to_host(dst,src,size) dra_change_byte_order(dst,src,size)

int dra_is_little_endian();

void dra_swap_byte_order(void *dst, void *src, size_t size);
void dra_keep_byte_order(void *dst, void *src, size_t size);
void dra_change_byte_order(void *dst, void *src, size_t size);

#ifdef __cplusplus
}
#endif

#endif
