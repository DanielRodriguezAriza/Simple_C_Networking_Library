#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s <server address> <server port>\n",argv[0]);
		exit(1);
	}
	
	dra_net_init();
	
	dra_sock_t client_socket;
	dra_addr_t server_address; 
	dra_net_set_ipvall(&client_socket);
	dra_net_set_socket_tcp(&client_socket);
	
	
	
	int status;
	if((status = dra_net_connect_addr(&client_socket,&server_address,argv[1],argv[2])) != DRA_NET_OK)
	{
		fprintf(stderr,"Failed to connect: %s\n",dra_net_get_string(status));
		exit(1);
	}
	printf("Connection successful! (%s)\nAwaiting input: ",dra_net_get_string(status));
	char *message[512];
	memset(message,0,512);
	
	scanf(" %[^\n]",&message);
	
	dra_net_send(&client_socket,message,512);
	
	dra_net_deinit();
	return 0;
}