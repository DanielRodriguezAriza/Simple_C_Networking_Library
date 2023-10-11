#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s <server port>\n",argv[0]);
		exit(1);
	}
	
	dra_net_init();
	
	dra_socket_t server_socket, client_socket;
	dra_set_socket_tcp(&server_socket);
	dra_set_socket_tcp(&client_socket);
	
	dra_host_on_port(&server_socket,argv[1]);
	
	dra_accept(&server_socket,&client_socket);
	
	char *received_message[512];
	memset(received_message,0,512);
	dra_recv(&client_socket,received_message,511);
	
	printf("The client has sent this information: %s\n",received_message);
	
	dra_net_deinit();
	return 0;
}