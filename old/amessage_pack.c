#include <stdio.h>
#include <stdlib.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "amessage.pb.h"

int main(int argc, const char* argv[])
{
	AMessage msg = AMessage_init_default; // AMessage
	uint8_t buffer[AMessage_size];        // Buffer to store serialized data
	size_t len;

	if(argc != 2 && argc != 3)
	{   // Allow one or two integers
		fprintf(stderr, "usage: amessage a [b]\n");
		return 1;
	}

	msg.a = atoi(argv[1]);
	msg.b = atoi(argv[2]);

	pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	bool status = pb_encode(&stream, AMessage_fields, &msg);
	len = stream.bytes_written;

	if(!status)
	{
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		return 1;
	}

	fprintf(stderr, "Writing %d serialized bytes\n", len); // See the length of message
	fwrite(buffer, len, 1, stdout); // Write to stdout to allow direct command line piping

	return 0;
}
