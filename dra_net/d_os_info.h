#ifndef DRA_DETECT_OS_INFO_H
#define DRA_DETECT_OS_INFO_H

	#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
		/* THIS IS UNIX */
		#define D_OS_INFO_UNIX
	#endif

	#if defined(_POSIX_VERSION)
		/* THIS IS POSIX COMPLIANT */
		#define D_OS_INFO_POSIX
	#endif

	#if defined(__APPLE__) && defined(__MACH__)
		/* THIS IS APPLE */
		#define D_OS_INFO_APPLE
	#endif

	#if defined(__unix__) && (defined(__NetBSD__) || defined(__OpenBSD__))
		/* THIS IS BSD */
		#define D_OS_INFO_BSD
	#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
		#include <sys/param.h>
		#if defined(BSD)
			/* THIS IS BSD DERIVED */
			#define D_OS_INFO_BSD
		#endif
	#endif

	#if defined(_WIN32) || defined (_WIN64) || defined(__CYGWIN__)
		/* THIS IS WINDOWS */
		#define D_OS_INFO_WINDOWS
	#endif

	#if defined(__linux) || defined(__linux__)
		/* THIS IS LINUX */
		#define D_OS_INFO_LINUX
	#endif

	#if defined(__sun) || defined(__sun__) || defined(__SunOs)
		/* THIS IS SUNOS */
		#define D_OS_INFO_SUNOS
	#endif

#endif /* END DRA_DETECT_OS_INFO_H */
