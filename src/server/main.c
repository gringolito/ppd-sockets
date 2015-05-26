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
// Yet another sort app using Insertion Sort algorithm (Server)
// (Distributed Client/Server implementation)
//
// Server Version
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
#include "fifo.h"

static void
print_usage (void)
{
	printf("Usage: %s <PORT>\n", PROG_NAME);
	printf("\tPORT\tTCP listen port\n");
}

int
fn_compare (const void *e1, const void *e2)
{
	int ret = 0;
	uint32_t a = *((uint32_t *)e1);
	uint32_t b = *((uint32_t *)e2);

	if (a > b) {
		ret = 1;
	} else if (a < b) {
		ret = -1;
	}

	return (ret);
}

static int
handle_messages (fifo_t *recv_buffer, fifo_t *send_buffer)
{
	int empty;
	int ret_count = 0;
	struct msg *msg;

	empty = fifo_empty(recv_buffer);
	while (!empty) {
		msg = fifo_remove(recv_buffer);
		if (!msg) {
			print_error("fifo_remove() failed");
			return (-1);
		}
		// Sort vector
		qsort(msg->data, msg->size, sizeof(*msg->data), fn_compare);
		fifo_add(send_buffer, msg);
		empty = fifo_empty(recv_buffer);
		ret_count++;
	}

	return (ret_count);
}

int
main (int argc, const char **argv)
{
	int i;
	int yes;
	int empty;
	int fdmax;
	int fdret;
	int addr_info;
	int server_sock;
	int client_sock;
	char server_port[MAXPORT_SIZE];
	char server_addrstr[INET6_ADDRSTRLEN];
	char client_addrstr[INET6_ADDRSTRLEN];
	struct addrinfo *addr_res;
	struct addrinfo hints;
	struct sockaddr_storage client_addr;
	struct timeval timeout;
	struct timeval to;
	struct msg *msg;
	socklen_t client_addrsize;
	fd_set sel;
	fd_set readers;
	fifo_t *receive_buffer;
	fifo_t *send_buffer;

	prgname = argv[0];
	if (argc > 2) {
		print_usage();
		return (1);
	}

	// Use given port, if any
	if (argc == 2) {
		strncpy(server_port, argv[1], sizeof(server_port));
	} else {
		// 33333 is the default port for server
		strncpy(server_port, "33333", sizeof(server_port));
	}

	print_debug("Server running in DEBUG mode!");

	// Setting timeout parameters to 200ms
	timeout.tv_sec  = 0;
	timeout.tv_usec = 200000;

	// Construct the server address structure:
	// Zero structure, make it ambiguous (IPv4 and IPv6), TCP Stream.
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_PASSIVE;

	// Get server address info
	addr_info = getaddrinfo("localhost", server_port, &hints, &addr_res);
	if (addr_info) {
		print_error("getaddrinfo() failed: %s",
		    gai_strerror(addr_info));
		return (1);
	}

	// Create socket for incoming connections stream socket using TCP
	server_sock = socket(addr_res->ai_family, addr_res->ai_socktype,
	    addr_res->ai_protocol);
	if (server_sock < 0) {
		print_errno("socket() failed");
		return (1);
	}
	print_debug("Socket descriptor received %d", server_sock);

	// Lose the pesky "Address already in use" error message
	if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes,
	    sizeof(yes)) < 0) {
		print_errno("setsockopt() failed");
		return (1);
	}

	// Bind to the local address
	if (bind(server_sock, addr_res->ai_addr, addr_res->ai_addrlen) < 0) {
		close(server_sock);
		print_errno("bind() failed");
		return (1);
	}

	// Mark the socket so it will listen for incoming connections
	if (listen(server_sock, MAX_PENDING) < 0) {
		print_errno("listen() failed");
		return (1);
	}

	get_addrinfo_ipstr(server_addrstr, addr_res);
	print_debug("Server IP address: %s", server_addrstr);
	print_debug("Server port: %s", server_port);

	// Initializing descriptors and FIFOs
	FD_ZERO(&readers);
	FD_SET(server_sock, &readers);
	fdmax = server_sock;
	receive_buffer = fifo_new();
	send_buffer = fifo_new();

	client_addrsize = (socklen_t) sizeof(client_addr);

	for EVER {
		to = timeout;
		sel = readers;
		if (select(fdmax + 1, &sel, NULL, NULL, &to) < 0) {
			print_error("select() failed!");
			return (1);
		}

		// Handle/Wait for a client connection
		if (FD_ISSET(server_sock, &sel)) {
			client_sock = accept(server_sock,
			    (struct sockaddr *) &client_addr, &client_addrsize);
			if (client_sock < 0) {
				print_errno("accept() failed");
				continue;
			}

			// Getting IP address
			get_sockaddr_ipstr(client_addrstr,
			    (struct sockaddr *) &client_addr);
			printf("Handling client %s on socket %d\n",
			    client_addrstr, client_sock);

			// Setting readers descriptors
			FD_SET(client_sock, &readers);
			fdmax = MAX(fdmax, client_sock);

			continue;
		}

		// Receive messages
		for (i = 0; i <= fdmax; i++) {
			if (!FD_ISSET(i, &sel) || i == server_sock) {
				continue;
			}
			print_debug("Trying receive_msg() in socket %d ...", i);
			msg = receive_msg(i);
			if (!msg) {
				print_error("receice_msg() failed, closing socket"
				    "%d", i);
				fdmax = close_socket(&readers, fdmax, i);
				continue;
			}

			fifo_add(receive_buffer, msg);
		}

		// Handle received messages
		if (handle_messages(receive_buffer, send_buffer) < 0) {
			print_error("handle_messages() failed");
		}

		// If are messages, send it to clients
		empty = fifo_empty(send_buffer);
		while (!empty) {
			print_debug("Trying send_msg() ...");
			msg = fifo_remove(send_buffer);
			fdret = send_msg(msg);
			if (fdret > 0) {
				print_error("send_msg() failed, closing socket"
				    "%d", fdret);
				fdmax = close_socket(&readers, fdmax, fdret);
			}
			empty = fifo_empty(send_buffer);
		}
	}

	return (0);
}

