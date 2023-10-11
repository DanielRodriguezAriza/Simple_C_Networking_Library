#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

// client ip port
int main(int argc, char **argv)
{
	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s <ip> <port>\n",argv[0]);
		exit(1);
	}
	
	
	dra_net_init();
	
	dra_socket_t client;
	dra_net_set_ipvall(&client);
	dra_net_set_socket_udp(&client);
	dra_addr_t addr;
	
	int status;
	if((status = dra_net_connect_addr(&client,&addr,argv[1],argv[2])) != 0)
	{
		fprintf(stderr,"Error connecting: %s\n",dra_net_get_string(status));
		exit(1);
	}
	
	char msg[512] = "una prueba";
	int msglen = 512;
	
	dra_net_send_to(&client,msg,msglen,&addr);
	
	
	dra_net_deinit();
	
	return 0;
}