#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

#define NUM 640000
#define NUM_CLIENTS 4

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"Usage: %s <server port>\n",argv[0]);
		exit(1);
	}
	
	dra_net_init();
	
	dra_socket_t server_socket,client_socket;
	
	dra_set_socket_tcp(&server_socket);
	dra_set_socket_tcp(&client_socket);
	
	dra_host_on_port(&server_socket,argv[1]);
	
	dra_accept(&server_socket,&client_socket);
	
	char msg[512];
	dra_recv(&client_socket,msg,sizeof(float));
	
	float number;
	dra_network_to_host(&number,&msg,sizeof(float));
	
	printf("I have received this : %s\n",msg);
	printf("which translates to  : %f\n",number);
	
	dra_close_socket(&server_socket);
	dra_close_socket(&client_socket);
	
	dra_net_deinit();
	return 0;
}
