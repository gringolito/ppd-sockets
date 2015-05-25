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
#include <stdlib.h>

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
	size_t i;

	if (!msg) {
		return (-1);
	}

	// Converting endianness
	for (i = 0; i < msg->size; i++) {
		msg->data[i] = htonl(msg->data[i]);
	}
	msg->size = htonl(msg->size);

	if (tcp_send(msg->sock, &msg->size, sizeof(msg->size), 0) < 0) {
		ret = msg->sock;
		goto error;
	}

	if (tcp_send(msg->sock, msg->data, msg->size, 1) < 0) {
		ret = msg->sock;
	}

error:
	free_msg(msg);
	return (ret);
}

struct msg *
receive_msg (int sock)
{
	size_t i;
	size_t size;
	uint32_t *data;
	struct msg *msg = NULL;

	if (tcp_recv(sock, &size, sizeof(size)) < 0) {
		goto error;
	}
	size = ntohl(size);

	data = calloc(size, sizeof(*data));
	if (!data) {
		print_error("calloc() failed");
		goto error;
	}
	if (tcp_recv(sock, data, size) < 0) {
		free(data);
		goto error;
	}

	// Converting endianness
	for (i = 0; i < size; i++) {
		data[i] = ntohl(data[i]);
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

