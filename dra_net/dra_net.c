#include "dra_net.h"

int dra_net_init(void)
{
	#ifdef _WIN32
	WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 1;
    }
	
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return 2;
	}
	#endif
	
	return 0;
}

void dra_net_deinit(void)
{
	#ifdef _WIN32
	WSACleanup();
	#endif
}

char *dra_net_get_init_str(int i)
{
	switch(i)
	{
		case 0:
			return DRA_NET_MSG_STR "Networking Initialized.";
			break;
		#ifdef _WIN32
		case 1:
			return DRA_NET_MSG_STR "WSAStartup failed.";
			break;
		case 2:
			return DRA_NET_MSG_STR "Version 2.2 of Winsock is not supported.";
			break;
		#endif
		default:
			return DRA_NET_MSG_STR "Error during network initialization.";
			break;
	}
}

int dra_create_socket(dra_socket_t *sock, int sock_type, char const *ip, char const *port)
{
	int status;
	struct addrinfo hints, *res;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = sock_type;
	if(ip == NULL)
	{
		hints.ai_flags = AI_PASSIVE;
	}
	
	status = getaddrinfo(ip,port,&hints,&res);
	if(status != 0)
	{
		printf("getaddrinfo failed\n");
		return status;
	}
	
	sock->socket_descriptor = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(sock->socket_descriptor < 0)
	{
		printf("Creating socket failed.\n");
		return -1;
	}
	
	sock->socket_address = *((struct sockaddr_storage*)(res->ai_addr));
	sock->socket_address_len = sizeof(sock->socket_address);
	sock->socket_type = sock_type;
	
	freeaddrinfo(res);
	return 0;
}

int dra_create_tcp_socket(dra_socket_t *sock, char const *ip, char const *port)
{
	return dra_create_socket(sock,SOCK_STREAM,ip,port);
}

int dra_create_udp_socket(dra_socket_t *sock, char const *ip, char const *port)
{
	return dra_create_socket(sock,SOCK_DGRAM,ip,port);
}

int dra_set_socket_blocking_mode(dra_socket_t *sock, char is_blocking)
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

int dra_close_socket(dra_socket_t *sock)
{
	#ifdef _WIN32
		return closesocket(sock->socket_descriptor);
	#else
		return close(sock->socket_descriptor);
	#endif
}

int dra_bind(dra_socket_t *sock)
{
	return bind(sock->socket_descriptor,(struct sockaddr*)&(sock->socket_address),sock->socket_address_len);
}

int dra_listen(dra_socket_t *sock)
{
	return listen(sock->socket_descriptor,10);
}

int dra_host(dra_socket_t *sock)
{
	int s1 = dra_bind(sock);
	int s2 = dra_listen(sock);
	return (s1 >= 0 && s2 >= 0) ? 0 : -1;
}

int dra_accept(dra_socket_t *server, dra_socket_t *client)
{
	client->socket_descriptor = accept(server->socket_descriptor,(struct sockaddr*)&(client->socket_address),&(client->socket_address_len));
	return client->socket_descriptor;
}

int dra_connect(dra_socket_t *sock)
{
	return connect(sock->socket_descriptor,(struct sockaddr*)&(sock->socket_address),sock->socket_address_len);
}

int dra_send(dra_socket_t *sock, void *buffer, size_t buffer_size)
{
	if(sock->socket_type == SOCK_STREAM)
	{
		return send(sock->socket_descriptor,buffer,buffer_size,0);
	}
	else
	{
		return sendto(sock->socket_descriptor,buffer,buffer_size,0,(struct sockaddr*)&(sock->socket_address),sock->socket_address_len);
	}
}

int dra_recv(dra_socket_t *sock, void *buffer, size_t buffer_size)
{
	if(sock->socket_type == SOCK_STREAM)
	{
		return recv(sock->socket_descriptor,buffer,buffer_size,0);
	}
	else
	{
		//return recvfrom(sock->socket_descriptor,buffer,buffer_size,0,(struct sockaddr*)&(sock->socket_address),&sock->socket_address_len);
		return recvfrom(sock->socket_descriptor,buffer,buffer_size,0,NULL,NULL);
	}
}

int dra_send_all(dra_socket_t *sock, void *buffer, size_t buffer_size)
{
	int total_bytes_sent = 0;
	while(total_bytes_sent < buffer_size)
	{
		int bytes_sent = dra_send(sock,buffer,buffer_size);
		if(total_bytes_sent < 0)
		{
			return total_bytes_sent;
		}
		total_bytes_sent += bytes_sent;
	}
	return total_bytes_sent;
}

int dra_get_ip(dra_socket_t *sock, char *dest)
{
	struct sockaddr_in *ipv4 = (struct sockaddr_in*)(&sock->socket_address);
	struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*)(&sock->socket_address);
	
	if(sock->socket_address.ss_family == AF_INET)
	{
		inet_ntop(sock->socket_address.ss_family,&(ipv4->sin_addr),dest,INET6_ADDRSTRLEN);
	}
	else
	{
		inet_ntop(sock->socket_address.ss_family,&(ipv6->sin6_addr),dest,INET6_ADDRSTRLEN);
	}
	
	return 0;
}


int dra_set_socket_type(dra_socket_t *sock, int socktype)
{
	sock->socket_type = socktype;
}

int dra_set_socket_tcp(dra_socket_t *sock)
{
	dra_set_socket_type(sock,SOCK_STREAM);
}

int dra_set_socket_udp(dra_socket_t *sock)
{
	dra_set_socket_type(sock,SOCK_DGRAM);
}

int dra_connect_to_address(dra_socket_t *sock, char const *ip, char const *port)
{
	int status;
	struct addrinfo hints, *serverinfo, *ptr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = sock->ip_version;
	hints.ai_socktype = sock->socket_type;
	hints.ai_flags = AI_PASSIVE;
	
	status = getaddrinfo(ip,port,&hints,&serverinfo);
	if(status != 0)
	{
		printf("getaddrinfo failed\n");
		return status;
	}
	
	for(ptr = serverinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if((sock->socket_descriptor = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)) == -1)
		{
			fprintf(stderr,"client: socket\n");
			continue;
		}
		if(sock->socket_type == SOCK_DGRAM)
		{
			break;
		}
		if(connect(sock->socket_descriptor,ptr->ai_addr,ptr->ai_addrlen) == -1)
		{
			dra_close_socket(sock);
			fprintf(stderr,"client: connect\n");
			continue;
		}
		break;
	}
	
	if(ptr == NULL)
	{
		fprintf(stderr,"client: failed to connect\n");
		return 1;
	}
	
	sock->socket_address = *((struct sockaddr_storage*)(ptr->ai_addr));
	sock->socket_address_len = sizeof(sock->socket_address);
	
	freeaddrinfo(serverinfo);
	return 0;
}

int dra_host_on_port(dra_socket_t *sock, char const *port)
{
	int status;
	struct addrinfo hints, *serverinfo, *ptr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = sock->ip_version;
	hints.ai_socktype = sock->socket_type;
	hints.ai_flags = AI_PASSIVE;
	
	status = getaddrinfo(NULL,port,&hints,&serverinfo);
	if(status != 0)
	{
		printf("getaddrinfo failed\n");
		return status;
	}
	
	for(ptr = serverinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if((sock->socket_descriptor = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)) == -1)
		{
			fprintf(stderr,"server: socket\n");
			continue;
		}
		
		if(bind(sock->socket_descriptor,ptr->ai_addr,ptr->ai_addrlen) == -1)
		{
			dra_close_socket(sock);
			fprintf(stderr,"server: bind\n");
			continue;
		}
		break;
	}
	
	
	if(ptr == NULL)
	{
		fprintf(stderr,"server: failed to bind\n");
		return 1;
	}
	
	sock->socket_address = *((struct sockaddr_storage*)(ptr->ai_addr));
	sock->socket_address_len = sizeof(sock->socket_address);
	
	freeaddrinfo(serverinfo);
	
	if(sock->socket_type != SOCK_DGRAM)
	{
		if(listen(sock->socket_descriptor,10) < 0)
		{
			fprintf(stderr,"could not listen...\n");
			return 1;
		}
	}
	
	return 0;
}


void dra_set_ipv(dra_socket_t *sock, int ipv)
{
	sock->ip_version = ipv;
}

void dra_set_ipv4(dra_socket_t *sock)
{
	dra_set_ipv(sock,AF_INET);
}

void dra_set_ipv6(dra_socket_t *sock)
{
	dra_set_ipv(sock,AF_INET6);
}

void dra_set_ipvall(dra_socket_t *sock)
{
	dra_set_ipv(sock,AF_UNSPEC);
}