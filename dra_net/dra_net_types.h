#ifndef DRA_NET_TYPES_H
#define DRA_NET_TYPES_H

#include "d_os_info.h"

/* Internal Socket Type */
#ifdef DRA_USE_ULLI_SOCKETS
	#define DRA_SOCKET unsigned long long int
#elif defined(_WIN32)
	#define DRA_SOCKET SOCKET
#else
	#define DRA_SOCKET int
#endif

/* Internal sockopt type */
#ifdef D_OS_INFO_SUNOS
	#define DRA_SOCKOPT_TYPE char
	#define DRA_SOCKOPT_TRUE '1'
	#define DRA_SOCKOPT_FALSE '0'
#else
	#define DRA_SOCKOPT_TYPE int
	#define DRA_SOCKOPT_TRUE 1
	#define DRA_SOCKOPT_FALSE 0
#endif

#ifdef _WIN32
	#define DRA_SOCKOPT_PTR char*
#else
	#define DRA_SOCKOPT_PTR void*
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	struct addrinfo hints, *serverinfo, *ptr;
} dra_address_info_t;

typedef struct {
	struct sockaddr_storage address;
	socklen_t address_len;
} dra_address_t;

/*
typedef struct {
	DRA_SOCKET socket_descriptor;
	int socket_type;
	int ip_version;
} dra_socket_t;
*/

/*
typedef struct {
	DRA_SOCKET socket_descriptor;
	dra_address_t address;
	int socket_type, ip_version;
} dra_socket_t;
*/

typedef struct {
	DRA_SOCKET socket_descriptor;
	int socket_type, ip_version;
} dra_socket_t;


#define dra_sock_t dra_socket_t
#define dra_addr_t dra_address_t


#ifdef __cplusplus
}
#endif

#endif
