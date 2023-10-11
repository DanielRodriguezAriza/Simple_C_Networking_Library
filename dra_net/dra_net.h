#ifndef DRA_NET_H
#define DRA_NET_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* System includes */
#ifdef _WIN32
	
	//__GNUC__ -> macro for GCC
	//_MSC_VER -> macro por MSVC
	
	/*Check if the code is being compiled with gcc (MinGW) and set the windows version to at least Windows Vista*/
	#ifdef __GNUC__
	
		#if defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0600
			#undef _WIN32_WINNT
			#define WINVER 0x0600
			#define _WIN32_WINNT 0x0600
		#else
			#define WINVER 0x0600
			#define _WIN32_WINNT 0x0600
		#endif
		
	#endif
	
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h> /*This is optional*/
	#include <winsock2.h>
	#include <ws2tcpip.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <signal.h>
	#include <stdio.h>
	#include <string.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <sys/time.h>
	#include <stdlib.h>
	#include <memory.h>
	#include <ifaddrs.h>
	#include <net/if.h>
	#include <stdarg.h>
	#include <math.h>
	#include <sys/termios.h>
	#include <poll.h>
#endif


#include "dra_net_def.h"
#include "dra_net_types.h"
#include "dra_net_conversions.h"
#include "dra_net_poll.h"

#define DRA_IPV4 AF_INET
#define DRA_IPV6 AF_INET6
#define DRA_IPVALL AF_UNSPEC
#define DRA_DUAL_STACK AF_UNSPEC
#define DRA_TCP SOCK_STREAM
#define DRA_UDP SOCK_DGRAM

#ifdef __cplusplus
extern "C"{
#endif


int dra_net_init(void);
int dra_net_deinit(void);
char const *dra_net_get_string(int i);

int dra_net_get_ip(dra_address_t *addr, char *dst);

int dra_net_set_socket_blocking_mode(dra_socket_t *sock, char is_blocking);

int dra_net_close_raw_socket(DRA_SOCKET sock);
int dra_net_close_socket(dra_socket_t *sock);

int dra_net_send(dra_socket_t *sock, void *buffer, size_t buffer_size);
int dra_net_recv(dra_socket_t *sock, void *buffer, size_t buffer_size);
int dra_net_send_all(dra_socket_t *sock, void *buffer, size_t buffer_size);

int dra_net_send_to(dra_socket_t *sock, void *buffer, size_t buffer_size, dra_address_t *addr);
int dra_net_recv_from(dra_socket_t *sock, void *buffer, size_t buffer_size, dra_address_t *addr);

int dra_net_set_socket_type(dra_socket_t *sock, int socktype);
int dra_net_set_socket_tcp(dra_socket_t *sock);
int dra_net_set_socket_udp(dra_socket_t *sock);


int dra_net_connect_addr(dra_socket_t *sock, dra_address_t *addr, char const *ip, char const *port);
int dra_net_host_addr(dra_socket_t *sock, dra_address_t *addr, char const *port, int backlog);
int dra_net_accept_addr(dra_socket_t *server, dra_socket_t *client_sock, dra_address_t *client_addr);

int dra_net_connect(dra_socket_t *sock, char const *ip, char const *port);
int dra_net_host(dra_socket_t *sock, char const *port);
int dra_net_accept(dra_socket_t *server, dra_socket_t *client_sock);

void dra_net_set_ipv(dra_socket_t *sock, int ipv);
void dra_net_set_ipv4(dra_socket_t *sock);
void dra_net_set_ipv6(dra_socket_t *sock);
void dra_net_set_ipvall(dra_socket_t *sock);

/* dra_net_get_address(): currently does not do DNS resolution. Usable as an "old school" alternative to getaddrinfo for people who prefer to have addresses and sockets as separate objects in their code. */
int dra_net_get_address_ipv(char const *str);
int dra_net_get_address(dra_address_t *addr, char const *ip, char const *port);

#ifdef __cplusplus
}
#endif

#endif
