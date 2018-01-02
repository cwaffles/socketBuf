#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pb_encode.h"
#include "amessage.pb.h"
#include "incl/general.h"

//char* socket_path = "./socket";
char socket_path[] = "\0hidden";

char* arr[] = {"hi", "hello", "guten tag", "zdrastvyte"};
size_t sizes[] = {3, 6, 10, 11};

void initSend(int* fd, struct sockaddr_un* addr) {
	if ((*fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
		perror("socket error");
		exit(-1);
	}

	addr->sun_family = AF_UNIX;
	if (socket_path[0] != '\0') {
		unlink(socket_path);
	}

	addr->sun_family = AF_UNIX;
	memcpy(addr->sun_path, socket_path, sizeof(socket_path));

	//use connect to avoid passing more params with sendto vs write
	if (connect(*fd, (struct sockaddr*) addr, sizeof(struct sockaddr_un)) == -1) {
		perror("connect error");
		exit(-1);
	}
}


//generates protobuf message targetBuffer
//expects data_count to be set as it only transmits however many elements you specify
status_t pbPackage(pb_byte_t* targetBuffer, size_t targetBufferSize, AMessage* data) {
	pb_ostream_t stream = pb_ostream_from_buffer(targetBuffer, targetBufferSize);
	bool status = pb_encode(&stream, AMessage_fields, data);
	if (!status) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return STATUS_ERROR;
	}

	return STATUS_OK;
}

int main(int argc, char* argv[]) {
	//socket setup stuff
	int fd;
	struct sockaddr_un addr = {0};//need to zero out memory because abstract socket compares all 108 chars
	initSend(&fd, &addr);

	//protobuf setup stuff
	AMessage msg = AMessage_init_default; // AMessage
	pb_byte_t buffer[AMessage_size]; // Buffer to store serialized data

	for (int i = 0; i < 4; i++) {
		//generate protobuf message
		msg.timestamp = i + 1;
		msg.data[0] = i + 2;
		msg.data[1] = i + 2;
		msg.data_count = 2; //only transmits however many elements you specify

		pbPackage(buffer, AMessage_size, &msg);

		printf("\nTimestamp: %ld, data[0]:%lf, data[1]:%lf || ", msg.timestamp, msg.data[0], msg.data[1]);

		pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
		bool status = pb_encode(&stream, AMessage_fields, &msg);
		if (!status) {
			printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
			return 1;
		}

		printf("sending %lu bytes: ", stream.bytes_written);
		fwrite(buffer, stream.bytes_written, 1, stdout); // Write to stdout raw bytes(printf stops on null byte)

		ssize_t bytesSent = send(fd, buffer, stream.bytes_written, 0);
		if (bytesSent != stream.bytes_written)
//		if(sendto(fd, arr[i], sizes[i], 0, (const struct sockaddr*) &addr, sizeof(addr)) != sizes[i])
		{
			if (bytesSent > 0) {
				perror("partial write\n");
			} else {
				perror("write error");
				exit(-1);
			}
		}
	}

}