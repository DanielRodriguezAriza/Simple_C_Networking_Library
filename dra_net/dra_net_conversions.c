#include "dra_net_conversions.h"

int dra_is_little_endian()
{
	int i = 1;
	return *((char*)(&i)) == 1;
}

void dra_swap_byte_order(void *dst, void *src, size_t size)
{
	unsigned char *dst_ptr = (unsigned char *)(dst);
	unsigned char *src_ptr = (unsigned char *)(src);
	
	for(size_t i = 0; i < size; ++i)
	{
		dst_ptr[i] = src_ptr[size-i-1];
	}
}

void dra_keep_byte_order(void *dst, void *src, size_t size)
{
	unsigned char *dst_ptr = (unsigned char *)(dst);
	unsigned char *src_ptr = (unsigned char *)(src);
	
	for(size_t i = 0; i < size; ++i)
	{
		dst_ptr[i] = src_ptr[i];
	}
}

void dra_change_byte_order(void *dst, void *src, size_t size)
{
	if(dra_is_little_endian())
	{
		dra_swap_byte_order(dst,src,size);
	}
	else
	{
		dra_keep_byte_order(dst,src,size);
	}
}