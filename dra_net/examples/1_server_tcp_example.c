#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"Usage: %s <server port>\n",argv[0]);
		exit(1);
	}
	
	dra_net_init();
	
	dra_sock_t server_socket, client_socket;
	dra_addr_t server_addr, client_addr;
	dra_net_set_socket_type(&server_socket,DRA_TCP);
	dra_net_set_socket_type(&client_socket,DRA_TCP);
	
	
	
	dra_net_set_ipv(&server_socket,DRA_IPVALL);
	dra_net_set_ipv(&client_socket,DRA_IPVALL);
	
	dra_net_host_addr(&server_socket,&server_addr,argv[1],10);
	
	
	
	dra_net_accept_addr(&server_socket,&client_socket,&client_addr);
	
	char *received_message[512];
	memset(received_message,0,512);
	dra_net_recv(&client_socket,received_message,511);
	
	char ip[INET6_ADDRSTRLEN];
	dra_net_get_ip(&client_addr,ip);
	printf("Connection received from: %s\n",ip);
	printf("The client has sent this information: %s\n",received_message);
	
	dra_net_deinit();
	return 0;
}