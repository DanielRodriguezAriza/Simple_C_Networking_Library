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
	
	dra_socket_t client_socket;
	dra_set_socket_tcp(&client_socket);
	
	dra_connect_to_address(&client_socket,argv[1],argv[2]);
	
	char *message[512];
	memset(message,0,512);
	
	scanf(" %[^\n]",&message);
	
	dra_send(&client_socket,message,512);
	
	dra_net_deinit();
	return 0;
}