// main.c
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
// Yet another sort app using Insertion Sort algorithm
// (Distributed Client/Server implementation)
//
// Client Version
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "utils.h"
#include "print.h"
#include "network.h"
#include "debug.h"

#define MAX_ELEM                                    (500000)
#define FILENAME                                    "sorted_vector.txt"

static void
print_usage (void)
{
	printf("Usage: %s FILE SIZE\n", PROG_NAME);
	printf("\tFILE File containing vector data\n");
	printf("\tSIZE Size of vector (Maximum size: %d)\n", MAX_ELEM);
}

int
main (int argc, const char **argv)
{
	int sock;
	char *server_address = "127.0.0.1";
	char server_addrstr[INET6_ADDRSTRLEN];
	char server_port[MAXPORT_SIZE];
	struct addrinfo *addr_res;
	struct addrinfo hints;

	prgname = argv[0];
	// Test for correct number of arguments
	if (argc != 4) {
		print_usage();
		return (1);
	}

	print_debug("Client running in DEBUG mode!");

	// Construct the server address structure:
	// Zero structure, make it ambiguous (IPv4 and IPv6), TCP Stream.
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_PASSIVE;

	// Get server address info
	if (getaddrinfo(server_address, server_port, &hints, &addr_res) != 0) {
		print_error("getaddrinfo() failed");
		return (1);
	}

	get_addrinfo_ipstr(server_addrstr, addr_res);
	print_debug("Server IP address: %s", server_addrstr);

	// Create a reliable, stream socket using TCP
	sock = socket(addr_res->ai_family, addr_res->ai_socktype,
	    addr_res->ai_protocol);
	if (sock < 0) {
		print_error("socket() failed");
		return (1);
	}

	// Establish the connection to the server
	if (connect(sock, addr_res->ai_addr, addr_res->ai_addrlen) < 0) {
		print_error("connect() failed");
		return (1);
	}

	return (0);
}
