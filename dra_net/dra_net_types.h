#ifndef DRA_NET_TYPES_H
#define DRA_NET_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dra_socket_t
{
	DRA_SOCKET socket_descriptor;
	struct sockaddr_storage socket_address;
	socklen_t socket_address_len;
	int socket_type;
} dra_socket_t;


#ifdef __cplusplus
}
#endif

#endif
