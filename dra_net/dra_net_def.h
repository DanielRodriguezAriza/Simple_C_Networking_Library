#ifndef DRA_NET_DEF_H
#define DRA_NET_DEF_H

/*Error codes*/
#define DRA_NET_INIT_OK 0
#define DRA_NET_INIT_WSA_STARTUP_FAILED 1
#define DRA_NET_INIT_WSA_VERSION_FAILED 2
#define DRA_NET_INIT_ERR 3

/*Error messages*/
#define DRA_NET_MSG_STR "DRA Net: "

/*Internal Socket Type*/
#ifndef _WIN32
	#define DRA_SOCKET int
#else
	#define DRA_SOCKET SOCKET
#endif

#endif