// debug.h
//
// "THE BEER-WARE LICENSE" (Revision 42):
// <filipeutzig@gmail.com> wrote this file. As long as you retain this
// notice you can do whatever you want with this stuff. If we meet some
// day, and you think this stuff is worth it, you can buy me a beer in
// return.
//
// Initial version by Filipe Utzig <filipeutzig@gmail.com> on 8/31/13.
//
// The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
// "SHOULD", "SHOULD NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in
// this document are to be interpreted as described in RFC 2119.
//
// Personal Debug macros
//
//    Changelog:
//    03/31/15 - License revision
//

#ifndef __FUTZIG_DEBUG_H
#define __FUTZIG_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>

#ifdef DEBUG
#define print_debug(...)   do {                                     \
		printf("[ DEBUG ] %s():%d - ", __func__, __LINE__); \
		printf(__VA_ARGS__);                                \
		printf("\n");                                       \
	} while (0)
#else
	#define print_debug(...)
#endif

#define print_error(...)   do {                                              \
		fprintf(stderr, "[ ERROR ] %s():%d - ", __func__, __LINE__); \
		fprintf(stderr, __VA_ARGS__);                                \
		fprintf(stderr, "\n");                                       \
	} while (0)

#define print_errno(...)   do {                                              \
		int __err = errno;                                           \
		fprintf(stderr, "[ ERROR ] %s():%d - ", __func__, __LINE__); \
		fprintf(stderr, __VA_ARGS__);                                \
		fprintf(stderr, ": %s\n", strerror(__err));                  \
	} while (0)

#endif // __FUTZIG_DEBUG_H

