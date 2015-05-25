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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_ELEM                                    (500000)
#define MPI_TAG                                     (0)
#define MPI_TERMINATE                               (-1)
#define FILENAME                                    "sorted_vector.txt"

static void
print_usage (void)
{
	printf("Usage: %s FILE SIZE\n", PROG_NAME);
	printf("\tFILE File containing vector data\n");
	printf("\tSIZE Size of vector (Maximum size: %d)\n", MAX_ELEM);
}

void
get_addrinfo_ipstr (char *dest, struct addrinfo *ptr)
{
	void *addr;

	/* ÌPv4 */
	if (ptr->ai_family == AF_INET) {
		struct sockaddr_in *ipv4 = (struct sockaddr_in *) ptr->ai_addr;
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

void
send_to_client (int sock, unsigned char type, uint16_t size, void *payload)
{
	bpmsg_t * send_msg;
	print_debug("entrei funcao send_to_client");

	/* Monta o pacote que ira para o servidor */
	send_msg = malloc(sizeof(*send_msg));
	send_msg->type = type;
	send_msg->size = size;
	send_msg->sock = sock;
	send_msg->payload = malloc(size);

	/*memcpy(send_msg->payload, payload, size);*/

	print_debug("Montei pacote");

	fifo_add(send_buffer, send_msg);
	print_debug("add pacote na fifo");
}

void
remove_client (int sock)
{
	/* Unsetting file descriptors for select */
	FD_CLR(sock, &readers);

	if (sock == fdmax) {
		fdmax--;
	}

	/* Closing socket connection */
	close(sock);

	/* Releasing and cleaning memory */
	clients[sock].uid = 0;
	clients[sock].sock = 0;
	clients[sock].status = 0;
	free(clients[sock].username);
	free(clients[sock].ip);

	/* Updating counters */
	connected_clients--;
}

int
handle_messages (void)
{
	int empty;
	int return_status = 0;

	empty = fifo_empty(receive_buffer);
	while (!empty) {
		msg = fifo_remove(receive_buffer);

		client = &clients[msg->sock];

		if (!client) {
			print_error("[Handler] client not found in list");
			return_status++;
			free(msg);
			empty = fifo_empty(receive_buffer);
			continue;
		}

		// Handle messages
		switch (msg->type) {
		default :
			print_error("[Handler] message type error");
			break;
		}

		free(msg);
		empty = fifo_empty(receive_buffer);
	}

	return (return_status);
}

int
main (int argc, const char **argv)
{
	int i;
	int yes;
	int empty;
	int fdmax;
	int sock_error;
	int select_ret;
	int addr_infost;
	int server_sock;
	int client_sock;
	char *client_addrstr;
	char server_port[MAXPORT_SIZE];
	char server_addrstr[INET6_ADDRSTRLEN];
	struct addrinfo *addr_res;
	struct addrinfo hints;
	struct sockaddr_storage client_addr;
	struct timeval timeout;
	struct timeval sel_to;
	socklen_t client_addrsize;
	fd_set sel_read;
	fd_set readers;
	fd_set writers;

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

	// Setting timeout parameters to 125ms
	timeout.tv_sec = 0;
	timeout.tv_usec = 125000;

	// Construct the server address structure:
	// Zero structure, make it ambiguous (IPv4 and IPv6), TCP Stream.
	memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_PASSIVE;

	// Get server address info
	addr_infost = getaddrinfo("localhost", server_port, &hints, &addr_res);
	if (addr_infost) {
		print_error("getaddrinfo() failed: %s",
		    gai_strerror(addr_infost));
		return (1);
	}

	get_addrinfo_ipstr(server_addrstr, addr_res);
	print_debug("Server IP address: %s", server_addrstr);
	print_debug("Server port: %s", server_port);

	// Create socket for incoming connections stream socket using TCP
	server_sock = socket(addr_res->ai_family, addr_res->ai_socktype,
	    addr_res->ai_protocol);
	if (server_sock < 0) {
		print_error("socket() failed");
		return (1);
	}
	print_debug("Socket descriptor received %d", server_sock);

	// Lose the pesky "Address already in use" error message
	if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes,
	    sizeof(yes)) < 0) {
		print_error("setsockopt() failed");
		return (1);
	}

	// Bind to the local address
	if (bind(server_sock, addr_res->ai_addr, addr_res->ai_addrlen) < 0) {
		close(server_sock);
		print_error("bind() failed");
		return (1);
	}

	// Mark the socket so it will listen for incoming connections
	if (listen(server_sock, BS_MAX_PENDING) < 0) {
		print_error("listen() failed");
		return (1);
	}

	// Initializing descriptors
	FD_ZERO(&readers);
	FD_SET(server_sock, &readers);
	fdmax = server_sock;

	client_addrsize = (socklen_t) sizeof(client_addr);

	for (EVER) {
		sel_to = timeout;
		sel_read = readers;
		select_ret = select(fdmax + 1, &sel_read, NULL, NULL, &sel_to);
		if (select_ret < 0) {
			print_error("select() failed!");
			return (1);
		}

		// Handle/Wait for a client connection
		if (FD_ISSET(server_sock, &sel_read)) {
			client_sock = accept(server_sock,
			    (struct sockaddr *) &client_addr, &client_addrsize);
			if (client_sock < 0) {
				print_error("accept() failed");
			}

			// Getting IP address
			client_addrstr = malloc(sizeof(INET6_ADDRSTRLEN));
			get_sockaddr_ipstr(client_addrstr,
			    (struct sockaddr *) &client_addr);
			printf("Handling client %s on socket %d\n",
			    client_addrstr, client_sock);

			// Setting readers and writers descriptors
			FD_SET(client_sock, &readers);
			FD_SET(client_sock, &writers);
			fdmax = MAX(fdmax, client_sock);

			continue;
		}

		// Send / receive messages
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &sel_read) && i != server_sock) {
				print_debug("Trying receive_bp() in "
				    "socket %d ...", i);
				recv_msg = receive_bp(i);
				if (!recv_msg) {
					print_error("receice_bp() failed, "
					    "closing socket");
					FD_CLR(i, &readers);
					if (i == fdmax) {
						fdmax--;
					}
					close(i);
				} else {
					fifo_add(receive_buffer, recv_msg);
				}
			}
		}

		// Handle received messages
		if (handle_messages() < 0) {
			print_error("handle_messages() failed");
		}

		// If are messages, send it to clients
		empty = fifo_empty(send_buffer);
		while (!empty) {
			print_debug("Trying send_bp() ...");
			send_msg = fifo_remove(send_buffer);
			sock_error = send_bp(send_msg);
			if (sock_error > 0) {
				print_error("send_bp() failed, closing socket");
				FD_CLR(sock_error, &readers);
				if (sock_error == fdmax) {
					fdmax--;
				}
				close(sock_error);
			}
			empty = fifo_empty(send_buffer);
		}
	}

	return (0);
}

