// utils.h
//
// "THE BEER-WARE LICENSE" (Revision 42):
// <filipeutzig@gmail.com> wrote this file. As long as you retain this
// notice you can do whatever you want with this stuff. If we meet some
// day, and you think this stuff is worth it, you can buy me a beer in
// return.
//
// Initial version by Filipe Utzig <filipeutzig@gmail.com> on 3/20/15.
//
// The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
// "SHOULD", "SHOULD NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in
// this document are to be interpreted as described in RFC 2119.
//
// Personal misc macros
//
//    Changelog:
//    03/31/15 - License revision
//    04/03/15 - Added result saving macros
//

#ifndef __FUTZIG_UTILS_H
#define __FUTZIG_UTILS_H

#include <debug.h>
#include <print.h>

#ifndef PROG_NAME
#define PROG_NAME                      (prgname)
#endif
static const char *prgname;

#ifndef EVER
#define EVER                           (;;)
#endif

#ifndef MIN
#define MIN(a,b)                       ((a < b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)                       ((a > b) ? (a) : (b))
#endif

#define RESULTS_WRITE                  (0)
#define RESULTS_APPEND                 (1)
#define SAVE_RESULTS(a, b, c)          do {                             \
		FILE *_fd;                                              \
		_fd = fopen(FILENAME, (a == RESULTS_WRITE) ? "w" : "a+");\
		if (!_fd) {                                             \
			print_errno("fopen() failed!");                 \
			exit(1);                                        \
		}                                                       \
		if (fprinti_vector(_fd, b, c)) {                        \
			print_error("fprinti_vector() failed!");        \
		}                                                       \
		fflush(_fd);                                            \
		fclose(_fd);                                            \
	} while(0)

#endif //__FUTZIG_UTILS_H

