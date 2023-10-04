#include <stdio.h>
#include <string.h>
#include "dra_net.h"

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s <address> <port>\n", argv[0]);
		exit(1);
	}
	
	dra_net_init();
	
	dra_socket_t sock;
	dra_set_socket_tcp(&sock);
	
	dra_connect_to_address(&sock,argv[1],argv[2]);
	
	char *send_msg = "GET /\r\n";
	char recv_msg[512] = {0};
	
	dra_send(&sock,send_msg,strlen(send_msg));
	
	while(dra_recv(&sock,recv_msg,512))
	{
		printf("%s\n",recv_msg);
		memset(recv_msg,0,512);
	}
	
	dra_net_deinit();
	return 0;
}
