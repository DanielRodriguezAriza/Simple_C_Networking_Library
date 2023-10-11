#include "dra_net.h"

int dra_net_init(void)
{
	#ifdef _WIN32
	WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return DRA_NET_INIT_WSA_STARTUP_FAILED;
    }
	
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return DRA_NET_INIT_WSA_VERSION_FAILED;
	}
	#endif
	
	return DRA_NET_OK;
}

int dra_net_deinit(void)
{
	#ifdef _WIN32
	WSACleanup();
	#endif
	return DRA_NET_OK;
}

char const *dra_net_get_string(int i)
{
	if (i < 0 || i >= DRA_NET_NUM_ERROR_CODES)
	{
		return "Unknown status or error code.";
	}
	else
	if (i == DRA_NET_GETADDRINFO_ERROR)
	{
		return gai_strerror(i);
	}
	else
	{
		return DRA_NET_STATUS_STRING[i];
	}
}

int dra_net_get_ip(dra_address_t *addr, char *dst)
{
	struct sockaddr_in *ipv4 = (struct sockaddr_in*)(&addr->address);
	struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*)(&addr->address);
	
	if(addr->address.ss_family == AF_INET)
	{
		inet_ntop(addr->address.ss_family,&(ipv4->sin_addr),dst,INET6_ADDRSTRLEN);
	}
	else
	{
		inet_ntop(addr->address.ss_family,&(ipv6->sin6_addr),dst,INET6_ADDRSTRLEN);
	}
	
	return DRA_NET_OK;
}



int dra_net_set_socket_blocking_mode(dra_socket_t *sock, char is_blocking)
{
#ifdef _WIN32
	unsigned long mode = is_blocking? 0 : 1;
	return (ioctlsocket(sock->socket_descriptor, FIONBIO, &mode) == 0) ? 0 : -1;
#else
	int flags = fcntl(sock->socket_descriptor, F_GETFL, 0);
	if (flags == -1) { return -1; }
	flags = is_blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	return (fcntl(sock->socket_descriptor, F_SETFL, flags) == 0) ? 0 : -1;
#endif
}

int dra_net_close_raw_socket(DRA_SOCKET sock)
{
	#ifdef _WIN32
		return closesocket(sock);
	#else
		return close(sock);
	#endif
}

int dra_net_close_socket(dra_socket_t *sock)
{
	dra_net_close_raw_socket(sock->socket_descriptor);
}

int dra_net_send(dra_socket_t *sock, void *buffer, size_t buffer_size)
{
	return send(sock->socket_descriptor,buffer,buffer_size,0);
}

int dra_net_recv(dra_socket_t *sock, void *buffer, size_t buffer_size)
{
	return recv(sock->socket_descriptor,buffer,buffer_size,0);
}

int dra_net_send_all(dra_socket_t *sock, void *buffer, size_t buffer_size)
{
	int total_bytes_sent = 0;
	while(total_bytes_sent < buffer_size)
	{
		int bytes_sent = dra_net_send(sock,buffer,buffer_size);
		if(total_bytes_sent < 0)
		{
			return total_bytes_sent;
		}
		total_bytes_sent += bytes_sent;
	}
	return total_bytes_sent;
}

int dra_net_send_to(dra_socket_t *sock, void *buffer, size_t buffer_size, dra_address_t *addr)
{
	return sendto(sock->socket_descriptor,buffer,buffer_size,0,(struct sockaddr*)&(addr->address),addr->address_len);
}

int dra_net_recv_from(dra_socket_t *sock, void *buffer, size_t buffer_size, dra_address_t *addr)
{
	return recvfrom(sock->socket_descriptor,buffer,buffer_size,0,(struct sockaddr*)&(addr->address),&addr->address_len);
}

int dra_net_set_socket_type(dra_socket_t *sock, int socktype)
{
	sock->socket_type = socktype;
}

int dra_net_set_socket_tcp(dra_socket_t *sock)
{
	dra_net_set_socket_type(sock,SOCK_STREAM);
}

int dra_net_set_socket_udp(dra_socket_t *sock)
{
	dra_net_set_socket_type(sock,SOCK_DGRAM);
}

int dra_net_connect_addr(dra_socket_t *sock, dra_address_t *addr, char const *ip, char const *port)
{
	int status;
	struct addrinfo hints, *serverinfo, *ptr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = sock->ip_version;
	hints.ai_socktype = sock->socket_type;
	/*hints.ai_flags = AI_PASSIVE;*/
	
	if(getaddrinfo(ip,port,&hints,&serverinfo) != 0)
	{
		/*printf("getaddrinfo failed\n");*/
		return DRA_NET_GETADDRINFO_ERROR;
	}
	
	for(ptr = serverinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if((sock->socket_descriptor = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)) == -1)
		{
			/*fprintf(stderr,"client: socket\n");*/
			continue;
		}
		
		DRA_SOCKOPT_TYPE yes = DRA_SOCKOPT_TRUE;
		if (setsockopt(sock->socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (DRA_SOCKOPT_PTR)&yes, sizeof(yes)) != 0)
		{
			return DRA_NET_SOCKOPT_ERROR;
		}
		
		DRA_SOCKOPT_TYPE mode = DRA_SOCKOPT_FALSE;
		if (setsockopt(sock->socket_descriptor,IPPROTO_IPV6,IPV6_V6ONLY,(DRA_SOCKOPT_PTR)&mode,sizeof(mode)) != 0)
		{
			return DRA_NET_SOCKOPT_ERROR;
		}
		
		if(sock->socket_type == SOCK_DGRAM)
		{
			break;
		}
		if(connect(sock->socket_descriptor,ptr->ai_addr,ptr->ai_addrlen) == -1)
		{
			dra_net_close_socket(sock);
			/*fprintf(stderr,"client: connect\n");*/
			continue;
		}
		break;
	}
	
	if(ptr == NULL)
	{
		/*fprintf(stderr,"client: failed to connect\n");*/
		return DRA_NET_CONNECT_ERROR;
	}
	
	if(addr != NULL)
	{
		addr->address = *((struct sockaddr_storage*)(ptr->ai_addr));
		addr->address_len = sizeof(addr->address);
	}
	
	freeaddrinfo(serverinfo);
	
	return DRA_NET_OK;
}

int dra_net_host_addr(dra_socket_t *sock, dra_address_t *addr, char const *port, int backlog)
{
	int status;
	struct addrinfo hints, *serverinfo, *ptr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = sock->ip_version;
	hints.ai_socktype = sock->socket_type;
	hints.ai_flags = AI_PASSIVE;
	
	if(getaddrinfo(NULL,port,&hints,&serverinfo) != 0)
	{
		return DRA_NET_GETADDRINFO_ERROR;
	}
	
	for(ptr = serverinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if((sock->socket_descriptor = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)) == -1)
		{
			/*fprintf(stderr,"server: socket\n");*/
			continue;
		}
		
		DRA_SOCKOPT_TYPE yes = DRA_SOCKOPT_TRUE;
		if (setsockopt(sock->socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (DRA_SOCKOPT_PTR)&yes, sizeof(yes)) != 0)
		{
			return DRA_NET_SOCKOPT_ERROR;
		}
		
		DRA_SOCKOPT_TYPE mode = DRA_SOCKOPT_FALSE;
		if (setsockopt(sock->socket_descriptor,IPPROTO_IPV6,IPV6_V6ONLY,(DRA_SOCKOPT_PTR)&mode,sizeof(mode)) != 0)
		{
			return DRA_NET_SOCKOPT_ERROR;
		}
		
		if(bind(sock->socket_descriptor,ptr->ai_addr,ptr->ai_addrlen) == -1)
		{
			dra_net_close_socket(sock);
			/*fprintf(stderr,"server: bind\n");*/
			continue;
		}
		break;
	}
	
	
	if(ptr == NULL)
	{
		/*fprintf(stderr,"server: failed to bind\n");*/
		return DRA_NET_BIND_ERROR;
	}
	
	if(addr != NULL)
	{
		addr->address = *((struct sockaddr_storage*)(ptr->ai_addr));
		addr->address_len = sizeof(addr->address);
	}
	
	freeaddrinfo(serverinfo);
	
	if(sock->socket_type != SOCK_DGRAM)
	{
		if(listen(sock->socket_descriptor,backlog) < 0)
		{
			/*fprintf(stderr,"could not listen...\n");*/
			return DRA_NET_LISTEN_ERROR;
		}
	}
	
	return DRA_NET_OK;
}

int dra_net_accept_addr(dra_socket_t *server, dra_socket_t *client, dra_address_t *addr)
{
	client->socket_descriptor = accept(server->socket_descriptor,(struct sockaddr*)&(addr->address),&(addr->address_len));
	return client->socket_descriptor;
}

int dra_net_connect(dra_socket_t *sock, char const *ip, char const *port)
{
	return dra_net_connect_addr(sock,NULL,ip,port);
}

int dra_net_host(dra_socket_t *sock, char const *port)
{
	return dra_net_host_addr(sock,NULL,port,10);
}

int dra_net_accept(dra_socket_t *server, dra_socket_t *client_sock)
{
	return dra_net_accept_addr(server,client_sock,NULL);
}

void dra_net_set_ipv(dra_socket_t *sock, int ipv)
{
	sock->ip_version = ipv;
}

void dra_net_set_ipv4(dra_socket_t *sock)
{
	dra_net_set_ipv(sock,AF_INET);
}

void dra_net_set_ipv6(dra_socket_t *sock)
{
	dra_net_set_ipv(sock,AF_INET6);
}

void dra_net_set_ipvall(dra_socket_t *sock)
{
	dra_net_set_ipv(sock,AF_UNSPEC);
}


int dra_net_get_address_ipv(char const *str)
{
	for(int i = 0; str[i]; ++i)
	{
		if(str[i] == ':')
		{
			return AF_INET6;
		}
	}
	return AF_INET;
}

int dra_net_get_address(dra_address_t *addr, char const *ip, char const *port)
{
	if(addr == NULL || ip == NULL || port == NULL)
	{
		return DRA_NET_NULL_PTR_ERROR;
	}
	
	addr->address_len = sizeof(addr->address);
	
	int address_ipv = dra_net_get_address_ipv(ip);
	
	if(address_ipv == AF_INET)
	{
		((struct sockaddr_in*)&(addr->address))->sin_family = AF_INET;
		((struct sockaddr_in*)&(addr->address))->sin_port = htons(atoi(port));
		inet_pton(address_ipv,ip,&(((struct sockaddr_in*)&(addr->address))->sin_addr));
	}
	else
	{
		((struct sockaddr_in6*)&(addr->address))->sin6_family = AF_INET6;
		((struct sockaddr_in6*)&(addr->address))->sin6_port = htons(atoi(port));
		inet_pton(address_ipv,ip,&(((struct sockaddr_in6*)&(addr->address))->sin6_addr));
	}
	
	return DRA_NET_OK;
}

