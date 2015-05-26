// msg.c
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

#include "msg.h"
#include "network.h"
#include "debug.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <arpa/inet.h>

void
free_msg (struct msg *msg)
{
	free(msg->data);
	free(msg);
}

int
send_msg (struct msg *msg)
{
	int ret = 0;
	int i;

	if (!msg) {
		return (-1);
	}

	// Converting endianness
	for (i = 0; i < msg->size; i++) {
		msg->data[i] = htonl(msg->data[i]);
	}

	i = htonl(msg->size);
	if (send(msg->sock, &i, sizeof(i), 0) < 0) {
		print_error("send() failed");
		ret = msg->sock;
	}

	if (send(msg->sock, msg->data, msg->size * sizeof(*msg->data), 0) < 0) {
		print_error("send() failed");
		ret = msg->sock;
	}

	free_msg(msg);
	return (ret);
}

struct msg *
receive_msg (int sock)
{
	int i;
	int size;
	int *data = NULL;
	struct msg *msg = NULL;

	if (recv(sock, &size, sizeof(size), 0) < 0) {
		print_error("recv() failed");
		goto error;
	}
	size = ntohl(size);

	if (size > 0) {
		data = calloc(size, sizeof(*data));
		if (data == NULL) {
			print_errno("calloc() failed");
			goto error;
		}
		if (recv(sock, data, size * sizeof(*data), 0) < 0) {
			print_error("recv() failed");
			free(data);
			goto error;
		}

		// Converting endianness
		for (i = 0; i < size; i++) {
			data[i] = ntohl(data[i]);
		}

	}

	msg = calloc(1, sizeof(*msg));
	if (!msg) {
		print_error("calloc() failed");
		free(data);
		goto error;
	}

	msg->sock = sock;
	msg->size = size;
	msg->data = data;

error:
	return (msg);
}

