#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dra_net.h"

int main(int argc, char **argv)
{
	
	//server <ip version> <port>
	
	if(argc != 4)
	{
		fprintf(stderr,"Usage: %s <server ip version> <port> <client ip version>\nnote: ip versions are: {\"4\",\"6\",\"all\"}",argv[0]);
		exit(1);
	}
	
	
	dra_net_init();
	
	dra_socket_t server,client;
	
	if(!strcmp(argv[1],"4"))
	{
		dra_set_ipv4(&server);
	}
	else
	if(!strcmp(argv[1],"6"))
	{
		dra_set_ipv6(&server);
	}
	else
	if(!strcmp(argv[1],"all"))
	{
		dra_set_ipvall(&server);
	}
	else
	{
		fprintf(stderr,"Unknown server ip version.\n");
		exit(1);
	}
	
	if(!strcmp(argv[3],"4"))
	{
		dra_set_ipv4(&client);
	}
	else
	if(!strcmp(argv[3],"6"))
	{
		dra_set_ipv6(&client);
	}
	else
	if(!strcmp(argv[3],"all"))
	{
		dra_set_ipvall(&client);
	}
	else
	{
		fprintf(stderr,"Unknown client ip version.\n");
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
	char recvmsg[1024];
	
	dra_set_socket_blocking_mode(&server,1);
	dra_set_socket_blocking_mode(&client,1);
	
	printf("Server started: listening on port %s...\n",argv[2]);
	while(1)
	{
		if(dra_accept(&server,&client) != -1)
		{
			char ipstr[INET6_ADDRSTRLEN];
			dra_get_ip(&client,ipstr);
			
			printf("accept: success! [connection from %s]\n",ipstr);
			dra_send(&client,msg,strlen(msg));
			dra_close_socket(&client);
		}
	}
	
	dra_close_socket(&server);
	dra_net_deinit();
	return 0;
}