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
	int size;
	struct iovec iov[2];

	if (!msg) {
		return (-1);
	}

	// Converting endianness
	for (i = 0; i < msg->size; i++) {
		msg->data[i] = htonl(msg->data[i]);
	}

	i = htonl(msg->size);
	size = msg->size * sizeof(*msg->data);

	iov[0].iov_base = &i;
	iov[0].iov_len = sizeof(i);
	iov[1].iov_base = msg->data;
	iov[1].iov_len = size;

	size += sizeof(i);
	if (writev(msg->sock, iov, 2) != size) {
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
	int ret;
	int rec_size;
	int *data = NULL;
	struct msg *msg = NULL;

	if (recv(sock, &size, sizeof(size), 0) < 0) {
		goto error;
	}
	size = ntohl(size);

	if (size > 0) {
		data = calloc(size, sizeof(*data));
		if (data == NULL) {
			print_errno("calloc() failed");
			goto error;
		}

		rec_size = size * sizeof(*data);
		ret = recv(sock, data, rec_size, 0);
		if (ret < 0) {
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

