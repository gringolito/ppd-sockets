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
	ssize_t i;
	struct iovec iov[2];

	if (!msg) {
		return (-1);
	}

	// Converting endianness
	for (i = 0; i < msg->size; i++) {
		msg->data[i] = htonl(msg->data[i]);
	}
	msg->size = htonl(msg->size);

	iov[0].iov_base = &msg->size;
	iov[0].iov_len = sizeof(msg->size);
	iov[1].iov_base = msg->data;
	iov[1].iov_len = msg->size;

	if (writev(msg->sock, iov, 2) < 0) {
		ret = msg->sock;
	}

	free_msg(msg);
	return (ret);
}

struct msg *
receive_msg (int sock)
{
	ssize_t i;
	ssize_t size;
	uint32_t *data = NULL;
	struct msg *msg = NULL;

	if (read(sock, &size, sizeof(size)) < 0) {
		goto error;
	}
	size = ntohl(size);

	if (size > 0) {
		data = calloc(size, sizeof(*data));
		if (!data) {
			print_error("calloc() failed");
			goto error;
		}
		if (read(sock, data, size) < 0) {
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

