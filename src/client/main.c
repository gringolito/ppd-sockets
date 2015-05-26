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
#include "debug.h"
#include "network.h"
#include "msg.h"

#define MAX_ELEM                                    (500000)
#define MAX_SERVERS                                 (10)
#define MAXNAME_SIZE                                (64)
#define FILENAME                                    "sorted_vector.txt"
#define SERVERLIST                                  "servers.txt"

static void
print_usage (void)
{
	printf("Usage: %s SERVERS JOBS FILE\n", PROG_NAME);
	printf("\tSERVERS Number of servers to use\n");
	printf("\tJOBS    Number of jobs to send\n");
	printf("\tFILE    File containing vector data\n");
}

int
main (int argc, const char **argv)
{
	int i;
	int ret;
	int sock;
	int slices;
	int servers;
	int servcnt = 0;
	int fdmax = 0;
	int socks[MAX_SERVERS];
	int readv[MAX_ELEM];
	char server_addr[INET6_ADDRSTRLEN];
	char server_addrstr[INET6_ADDRSTRLEN];
	char server_port[MAXPORT_SIZE];
	char filename[MAXNAME_SIZE];
	struct addrinfo *addr;
	struct addrinfo hints;
	struct timeval timeout;
	struct timeval to;
	FILE *fd;
	fd_set sel;
	fd_set writers;

	prgname = argv[0];
	// Test for correct number of arguments
	if (argc != 4) {
		print_usage();
		return (1);
	}

	servers = atoi(argv[1]);
	if (servers <= 0 || servers > MAX_SERVERS) {
		print_error("Invalid number of servers %d (Max: %d)",
		    servers, MAX_SERVERS);
		return (1);
	}

	slices = atoi(argv[2]);
	if (slices <= 0) {
		print_error("Invalid number of jobs %d", slices);
		return (1);
	}
	strncpy(filename, argv[3], sizeof(filename));

	print_debug("Client running in DEBUG mode!");

	// Read data vector
	fd = fopen(filename, "r");
	if (!fd) {
		print_errno("fopen() failed");
		return (1);
	}
	for (i = 0; (size_t) i < MAX_ELEM; i++) {
		ret = fscanf(fd, "%d", &readv[i]);
		if (ret < 0) {
			print_errno("fscanf() failed!");
			return (ret);
		}
	}

	fclose(fd);

	// Initializing descriptors
	FD_ZERO(&writers);

	// Setting timeout parameters to 125ms
	timeout.tv_sec  = 0;
	timeout.tv_usec = 125000;

	fd = fopen(SERVERLIST, "r");
	if (!fd) {
		print_errno("fopen() failed");
		return (1);
	}

	// Connect to servers
	for (i = 0; i < servers; i++) {
		if (fscanf(fd, "%128[^:]:%5s", server_addr, server_port) != 2) {
			continue;
		}

		// Construct the server address structure:
		// Zero structure, make it ambiguous (IPv4/IPv6), TCP Stream.
		memset(&hints, 0, sizeof(hints));
		hints.ai_family   = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags    = AI_PASSIVE;

		// Get server address info
		if (getaddrinfo(server_addr, server_port, &hints, &addr)) {
			print_errno("getaddrinfo() failed");
			continue;
		}

		// Create a reliable, stream socket using TCP
		sock = socket(addr->ai_family, addr->ai_socktype,
		    addr->ai_protocol);
		if (sock < 0) {
			print_errno("socket() failed");
			continue;
		}

		// Establish the connection to the server
		if (connect(sock, addr->ai_addr, addr->ai_addrlen) < 0) {
			print_errno("connect() failed");

			continue;
		}

		socks[servcnt++] = sock;
		get_addrinfo_ipstr(server_addrstr, addr);
		print_debug("Server %02d IP address: %s", servcnt,
		    server_addrstr);

		FD_SET(sock, &writers);
		fdmax = MAX(fdmax, sock);
	}

	fclose(fd);

	for (i = 0; i < slices; i++) {

	}

	SAVE_RESULTS(RESULTS_WRITE, readv, MAX_ELEM);

	printf("The result can be found at file '%s'\n", FILENAME);

	return (0);
}
