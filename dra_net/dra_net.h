#ifndef DRA_NET_H
#define DRA_NET_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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
#endif

#include "dra_net_def.h"
#include "dra_net_types.h"
#include "dra_net_conversions.h"

#ifdef __cplusplus
extern "C"{
#endif


int dra_net_init(void);
void dra_net_deinit(void);
char *dra_net_get_init_str(int i);


int dra_create_socket(dra_socket_t *sock, int sock_type, char const *ip, char const *port);
int dra_create_tcp_socket(dra_socket_t *sock, char const *ip, char const *port);
int dra_create_udp_socket(dra_socket_t *sock, char const *ip, char const *port);

int dra_set_socket_blocking_mode(dra_socket_t *sock, char is_blocking);

int dra_close_socket(dra_socket_t *sock);
int dra_bind(dra_socket_t *sock);
int dra_listen(dra_socket_t *sock);
int dra_host(dra_socket_t *sock);

int dra_accept(dra_socket_t *server, dra_socket_t *client);

int dra_connect(dra_socket_t *sock);

int dra_send(dra_socket_t *sock, void *buffer, size_t buffer_size);
int dra_recv(dra_socket_t *sock, void *buffer, size_t buffer_size);
int dra_send_all(dra_socket_t *sock, void *buffer, size_t buffer_size);

int dra_set_socket_blocking_mode(dra_socket_t *sock, char is_blocking);

int dra_get_ip(dra_socket_t *sock, char *dest);


int dra_set_socket_type(dra_socket_t *sock, int socktype);
int dra_set_socket_tcp(dra_socket_t *sock);
int dra_set_socket_udp(dra_socket_t *sock);

int dra_connect_to_address(dra_socket_t *sock, char const *ip, char const *port);
int dra_host_on_port(dra_socket_t *sock, char const *port);

#ifdef __cplusplus
}
#endif

#endif
