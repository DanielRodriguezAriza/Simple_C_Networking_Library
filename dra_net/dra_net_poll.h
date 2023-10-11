#ifndef DRA_NET_POLL_H
#define DRA_NET_POLL_H

/* System poll function alias */
#ifndef _WIN32
	#define dra_poll(fds,nfds,timeout) poll(fds,nfds,timeout)
	#define dra_pollfd struct pollfd
#else
	#define dra_poll(fds,nfds,timeout) WSAPoll(fds,nfds,timeout)
	#define dra_pollfd WSAPOLLFD
#endif

#endif
