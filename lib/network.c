// network.c
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

#include "network.h"
#include <arpa/inet.h>

void
get_addrinfo_ipstr (char *dest, struct addrinfo *ptr)
{
	void *addr;

	/* ÌPv4 */
	if (ptr->ai_family == AF_INET) {
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
		addr = &(ipv4->sin_addr);
	/* ÌPv6 */
	} else {
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ptr->ai_addr;
		addr = &(ipv6->sin6_addr);
	}
	/* Convert the IP to a string */
	inet_ntop(ptr->ai_family, addr, dest, sizeof(INET6_ADDRSTRLEN));
}

void
get_sockaddr_ipstr (char *dest, struct sockaddr *src)
{
	void *addr;

	/* ÌPv4 */
	if (src->sa_family == AF_INET) {
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)src;
		addr = &(ipv4->sin_addr);
	/* ÌPv6 */
	} else {
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)src;
		addr = &(ipv6->sin6_addr);
	}
	/* Convert the IP to a string */
	inet_ntop(src->sa_family, addr, dest, sizeof(INET6_ADDRSTRLEN));
}

