// msg.h
//
// "THE BEER-WARE LICENSE" (Revision 42):
// <filipeutzig@gmail.com> wrote this file. As long as you retain this
// notice you can do whatever you want with this stuff. If we meet some
// day, and you think this stuff is worth it, you can buy me a beer in
// return.
//
// Initial version by Filipe Utzig <filipeutzig@gmail.com> on 5/25/15.
//
// The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
// "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in
// this document are to be interpreted as described in RFC 2119.
//
// Communication messages library
//

#ifndef __FUTZIG_MSG_H
#define __FUTZIG_MSG_H

#include <stdint.h>
#include <sys/types.h>

// Data structures

struct msg {
	int sock;
	int size;
	int *data;
};

// Functions prototypes

/**
 * @brief Free a structure message
 *
 * @param msg Message to be released
 */
void free_msg (struct msg *msg);

/**
 * @brief Unpack and send a structure message over a TCP socket
 *
 * @param msg Message to be sent (this message will be released after send)
 *
 * @retval 0, on success
 * @retval Socket fd otherwise
 */
int send_msg (struct msg *msg);

/**
 * @brief Alloc and pack a structure message received on a TCP socket
 *
 * @param sock Socket fd to receive message
 *
 * @return a pointer to message, on success
 * @return NULL, otherwise
 */
struct msg *receive_msg (int sock);

#endif // __FUTZIG_MSG_H
