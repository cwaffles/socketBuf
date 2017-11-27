#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pb_decode.h>
#include <amessage.pb.h>

//char* socket_path = "./socket";
char socket_path[] = "\0hidden";


void initRecv(int* fd, struct sockaddr_un* addr)
{
	if((*fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket() error");
		exit(-1);
	}

	addr->sun_family = AF_UNIX;
	if(socket_path[0] != '\0')
	{
		unlink(socket_path);
	}

	memcpy(addr->sun_path, socket_path, sizeof(socket_path));
	if(bind(*fd, (struct sockaddr*) addr, sizeof(struct sockaddr_un)) == -1)
	{
		perror("bind() error");
		exit(-1);
	}
}

int main(int argc, char* argv[])
{
	int fd; //file descriptor for socket
	struct sockaddr_un addr = {0}; //init to 0 so we don't have issues with the abstract socket domain
	initRecv(&fd, &addr);

	pb_byte_t buf[AMessage_size];

	while(true) //keep listening until we quit
	{
		ssize_t rc; //return code
		while((rc = recv(fd, buf, sizeof(buf), 0)) > 0)
		{
			printf("read %lu bytes:\t", rc);
			fwrite(buf, sizeof(pb_byte_t), (size_t) rc, stdout);

			/* Allocate space for the decoded message. */
			AMessage message = AMessage_init_zero;

			/* Create a stream that reads from the buffer. */
			pb_istream_t stream = pb_istream_from_buffer(buf, (size_t) rc);

			/* Now we are ready to decode the message. */
			bool status = pb_decode(&stream, AMessage_fields, &message);

			/* Check for errors... */
			if(!status)
			{
				printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
				return -2;
			}

			// display the message's fields.
//			int len = sizeof(AMessage.data)/sizeof(AMessage.data[0]);
			printf("\tReceived: timestamp=%ld, data= ", message.timestamp);  // required field
			for(int i = 0; i < message.data_count; i++) //print out all the fields
			{
				printf("%lf ", message.data[i]);  // required field
			}
			printf("\n");
		}
		if(rc == -1)
		{
			perror("recv() fail");
			exit(-1);
		}
	}

}
