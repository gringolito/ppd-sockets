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
	size_t size;
	uint32_t *data;
};

// Functions prototypes

/**
 * @brief 
 *
 * @param msg
 */
void free_msg (struct msg *msg);

/**
 * @brief 
 *
 * @param msg
 *
 * @return 
 */
int send_msg (struct msg *msg);

/**
 * @brief 
 *
 * @param sock
 *
 * @return 
 */
struct msg *receive_msg (int sock);

#endif // __FUTZIG_MSG_H
