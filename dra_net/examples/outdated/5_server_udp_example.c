#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

int main(int argc, char **argv)
{
	dra_net_init();
	
	dra_socket_t server;
	dra_set_ipv4(&server);
	dra_set_socket_udp(&server);
	dra_host_on_port(&server,"27015");
	
	char msg[512];
	msg[511] = 0;
	dra_recv(&server,msg,511);
	
	printf("the message is: %s\n", msg);
	
	dra_net_deinit();
	return 0;
}