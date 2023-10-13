#ifndef DRA_NET_DEF_H
#define DRA_NET_DEF_H

enum DRA_NET_STATUS
{
	DRA_NET_OK = 0,
	DRA_NET_INIT_WSA_STARTUP_FAILED,
	DRA_NET_INIT_WSA_VERSION_FAILED,
	DRA_NET_INIT_ERROR,
	DRA_NET_UNKNOWN_ERROR,
	DRA_NET_SOCKET_ERROR,
	DRA_NET_BIND_ERROR,
	DRA_NET_CONNECT_ERROR,
	DRA_NET_SOCKOPT_ERROR,
	DRA_NET_GETADDRINFO_ERROR,
	DRA_NET_INVALID_INPUT_ERROR,
	DRA_NET_LISTEN_ERROR,
	DRA_NET_NULL_PTR_ERROR,
	DRA_NET_ACCEPT_ERROR,
	
	
	
	DRA_NET_NUM_ERROR_CODES
};

static char const * const DRA_NET_STATUS_STRING[] = {
	"Operation completed successfully.",
	"WSA Startup error.",
	"WSA Version error.",
	"Error during network initialization.",
	"Unknown error.",
	"Error creating a socket.",
	"Error binding a socket.",
	"Error connecting to a socket.",
	"Error setting sockopt.",
	"gai error",
	"Function received invalid input.",
	"Error listening to socket.",
	"Function received a null pointer.",
	"Error accepting a socket connection."
	
};

#endif