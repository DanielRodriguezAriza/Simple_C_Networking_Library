#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dra_net.h"

#define NUM 640000

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s <server address> <server port>",argv[0]);
		return 1;
	}
	
	dra_net_init();
	
	dra_socket_t client_socket;
	dra_set_socket_tcp(&client_socket);
	
	dra_connect_to_address(&client_socket,argv[1],argv[2]);
	
	printf("Write an input floating point number: ");
	float number = 0;
	scanf("%f",&number);
	char message[512];
	dra_host_to_network(message,&number,sizeof(number));
	dra_send(&client_socket,message,sizeof(number));
	
	float expected;
	dra_network_to_host(&expected,&message,sizeof(number));
	
	printf("I originally had this : %f\n",number);
	printf("I have sent this      : %s\n",message);
	printf("Server expects this   : %f\n",expected);
	
	dra_close_socket(&client_socket);
	
	dra_net_deinit();
	return 0;
}
