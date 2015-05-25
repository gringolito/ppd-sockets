// print.h
//
// "THE BEER-WARE LICENSE" (Revision 42):
// <filipeutzig@gmail.com> wrote this file. As long as you retain this
// notice you can do whatever you want with this stuff. If we meet some
// day, and you think this stuff is worth it, you can buy me a beer in
// return.
//
// Initial version by Filipe Utzig <filipeutzig@gmail.com> on 3/19/15.
//
// The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
// "SHOULD", "SHOULD NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in
// this document are to be interpreted as described in RFC 2119.
//
// Some useful print functions
//
//    Changelog:
//    03/31/15 - License revision
//             - Added fprint_intvector function
//

#ifndef __FUTZIG_PRINT_H
#define __FUTZIG_PRINT_H

#include <sys/time.h>
#include <stdio.h>

/**
 * @brief Calculate and write on stdout the difference betwen given
 *        times. The output is formatted like:
 *        E.g. Elasped Time: 999s:999ms:999us
 *
 * @param init Initial time
 * @param end Final time
 */
void print_time (struct timeval init, struct timeval end);

/**
 * @brief Write the given int buffer in a file stream, one value per
 *        line.
 *
 * @param stream file stream
 * @param buf buffer to be printed
 * @param size number of elements of buf
 *
 * @retval 0, on success
 * @retval a negative number on fail
 */
int fprinti_vector (FILE *stream, const int *buf, size_t size);

#endif // __FUTZIG_PRINT_H

