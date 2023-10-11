#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

// server port
int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"Usage: %s <port>\n",argv[0]);
		exit(1);
	}
	
	
	dra_net_init();
	
	dra_socket_t server;
	dra_net_set_ipvall(&server);
	dra_net_set_socket_udp(&server);
	
	dra_addr_t server_addr;
	
	int status;
	if((status = dra_net_host_addr(&server,&server_addr,argv[1],10)) != 0)
	{
		fprintf(stderr,"Error hosting: %s\n",dra_net_get_string(status));
		exit(1);
	}
	
	char msg[512];
	int msglen = 512;
	memset(msg,0,msglen);
	
	dra_address_t clientaddr;
	int bytes = dra_net_recv_from(&server,msg,msglen,&clientaddr);
	
	printf("got %d bytes\nmsg = %s\n",bytes,msg);
	
	dra_net_deinit();
	
	return 0;
}