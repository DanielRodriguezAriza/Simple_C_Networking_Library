#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

int main(int argc, char **argv)
{
	dra_net_init();
	
	dra_socket_t client;
	dra_set_ipv4(&client);
	dra_set_socket_udp(&client);
	dra_connect_to_address(&client,"localhost","27015");
	
	char msg[512] = "hello this is a test.\n";
	dra_send(&client,msg,512);
	
	printf("message sent.\n");
	
	dra_net_deinit();
	return 0;
}