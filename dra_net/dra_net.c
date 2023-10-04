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
	if(status < 0)
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

int dra_create_udp_socket(dra_socket_t *sock, char const *ip, char const *port)
{
	return dra_create_socket(sock,SOCK_DGRAM,ip,port);
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
		return recvfrom(sock->socket_descriptor,buffer,buffer_size,0,(struct sockaddr*)&(sock->socket_address),&sock->socket_address_len);
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

/*Currently has a problem in windows if you define DRA_NET_USE_WINDOWS_HACK when using DNS resolution where addresses such as "localhost", where it first tries to connect through IPv6*/
int dra_connect_to_address(dra_socket_t *sock, char const *ip, char const *port)
{
	/*Planned for future implementation: test if the address is an IPv4, then test if it's an IPv6, then test if it can be translated from IPv4 to IPv6 and then attempt DNS resolution. This will prevent the need for the hack in dra_host_on_port for windows so that IPv6 sockets can be compatible with IPv4 connections without having to give priority to generating an IPv4 socket in windows This could slow down address resolution, so this idea might be scrapped in the future.*/
	/*
	struct in_addr ipv4_addr;
	if(inet_pton(AF_INET,ip,&ipv4_addr) == -1)
	{
		fprintf(stderr,"not an ipv4 address\n");
	}
	
	struct in6_addr ipv6_addr;
	if(inet_pton(AF_INET,ip,&ipv6_addr) == -1)
	{
		fprintf(stderr,"not an ipv6 address\n");
	}
	*/
	
	/*planned for future implementation: remove redundant code in dra_connect_to_address and dra_host_on_port by using a modified version of dra_create_socket function and some kind of dra_addrinfo_t struct to simplify storing address information.*/
	int status;
	struct addrinfo hints, *serverinfo, *ptr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
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
	hints.ai_family = AF_UNSPEC;
	//hints.ai_family = AF_INET;
	hints.ai_socktype = sock->socket_type;
	hints.ai_flags = AI_PASSIVE;
	
	status = getaddrinfo(NULL,port,&hints,&serverinfo);
	if(status != 0)
	{
		printf("getaddrinfo failed\n");
		return status;
	}
	
	
	#ifdef DRA_NET_USE_WINDOWS_HACK
	/*
		If you are on a windows system, give priority to IPV4 addresses so that machines that support ipv6 but have an ipv4 address don't find themselves stuck to only being able to communicate through localhost or ::1. The wonders of technology. Oh btw, if you do translate a public ivp4 to ipv6 form, it refuses connection INSTANTLY, which means that this is a windows specific thing.
		
		In the future, add a way to manually select if you want to give priority to IPv4 or IPv6 for the current machine, and make IPv4 the default, as that is what most people will use. All of this could be prevented if AF_UNSPEC worked the way it was meant to across all platforms, but this is not a perfect world.
	*/
	int socket_has_been_created = 0;
	for(ptr = serverinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if(ptr->ai_family == AF_INET)
		{
			if((sock->socket_descriptor = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)) != -1)
			{
				if(bind(sock->socket_descriptor,ptr->ai_addr,ptr->ai_addrlen) != -1)
				{
					socket_has_been_created = 1;
					break;
				}
				else
				{
					dra_close_socket(sock);
					fprintf(stderr,"server: bind\n");
				}
			}
			else
			{
				fprintf(stderr, "server: socket\n");
			}
		}
	}
	
	if(socket_has_been_created == 0)
	{
		for(ptr = serverinfo; ptr != NULL; ptr = ptr->ai_next)
		{
			if(ptr->ai_family == AF_INET6)
			{
				if((sock->socket_descriptor = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)) != -1)
				{
					if(bind(sock->socket_descriptor,ptr->ai_addr,ptr->ai_addrlen) != -1)
					{
						break;
					}
					else
					{
						dra_close_socket(sock);
						fprintf(stderr,"server: bind\n");
					}
				}
				else
				{
					fprintf(stderr, "server: socket\n");
				}
			}
		}
	}
	#else
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
	#endif
	
	
	if(ptr == NULL)
	{
		fprintf(stderr,"server: failed to bind\n");
		return 1;
	}
	
	sock->socket_address = *((struct sockaddr_storage*)(ptr->ai_addr));
	sock->socket_address_len = sizeof(sock->socket_address);
	
	freeaddrinfo(serverinfo);
	
	if(listen(sock->socket_descriptor,10) < 0)
	{
		fprintf(stderr,"could not listen...\n");
		return 1;
	}
	
	return 0;
}
