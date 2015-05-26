// network.h
//
// "THE BEER-WARE LICENSE" (Revision 42):
// <filipeutzig@gmail.com> wrote this file. As long as you retain this
// notice you can do whatever you want with this stuff. If we meet some
// day, and you think this stuff is worth it, you can buy me a beer in
// return.
//
// Initial version by Filipe Utzig <filipeutzig@gmail.com> on 5/24/15.
//
// The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
// "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in
// this document are to be interpreted as described in RFC 2119.
//
// Networking misc library, with some usefull functions
//

#ifndef __FUTZIG_NETWORK_H
#define __FUTZIG_NETWORK_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXPORT_SIZE                   (6)
#define MAX_PENDING                    (100)

/**
 * @brief Get a human readable IP address from a given struct addrinfo
 *
 * @param dest Human reabable IPv4/IPv6 address, must be alloced before
 *     (Size: INET6_ADDRSTRLEN) (out)
 * @param ptr Filled structure where to get information (in)
 */
void get_addrinfo_ipstr (char *dest, struct addrinfo *ptr);

/**
 * @brief Get a human readable IP address from a given struct sockaddr
 *
 * @param dest Human reabable IPv4/IPv6 address, must be alloced before
 *     (Size: INET6_ADDRSTRLEN) (out)
 * @param src Filled structure where to get information (in)
 */
void get_sockaddr_ipstr (char *dest, struct sockaddr *src);

/**
 * @brief Update data for select and close connection on socket
 *
 * @param fds I/O descriptors
 * @param fdmax Number of descriptors
 * @param fd File descriptor to close
 *
 * @return Updated number of descriptors
 */
int close_socket (fd_set *fds, int fdmax, int fd);

#endif // __FUTZIG_NETWORK_H

