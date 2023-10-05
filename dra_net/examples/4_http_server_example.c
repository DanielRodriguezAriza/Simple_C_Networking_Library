#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

int main(int argc, char **argv)
{
	
	//server <ip version> <port>
	
	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s <ip version (4,6,all)> <port>\n",argv[0]);
		exit(1);
	}
	
	
	dra_net_init();
	
	dra_socket_t server,client;
	
	if(!strcmp(argv[1],"4"))
	{
		dra_set_ipv4(&server);
		dra_set_ipv4(&client);
	}
	else
	if(!strcmp(argv[1],"6"))
	{
		dra_set_ipv6(&server);
		dra_set_ipv6(&client);
	}
	else
	if(!strcmp(argv[1],"all"))
	{
		dra_set_ipvall(&server);
		dra_set_ipvall(&client);
	}
	else
	{
		fprintf(stderr,"Unknown ip version.\n");
		exit(1);
	}
	
	
	dra_set_socket_tcp(&server);	
	dra_set_socket_tcp(&client);
	
	dra_host_on_port(&server,argv[2]);
	
	char msg[] =
	R"(HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 512
Server: Poteitos

<!DOCTYPE html>
<html>
<head>
    <title>Example Page</title>
</head>
<body>
    <h1>Hello, World!</h1>
    <p>This is an example web page.</p>
</body>
</html>
)";
	
	char request[512];
	
	while(1)
	{
		memset(request,0,512);
		if(dra_accept(&server,&client) == -1)
		{
			printf("accept: failure!\n");
		}
		else
		{
			printf("accept: success!\n");
			
			dra_send(&client,msg,strlen(msg));
			dra_close_socket(&client);
		}
	}
	
	dra_close_socket(&server);
	dra_net_deinit();
	return 0;
}