#include <stdio.h>
#include <stdlib.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "amessage.pb.h"
#define MAX_MSG_SIZE (AMessage_size + 1000)

static size_t
read_buffer (unsigned max_length, uint8_t *out)
{
  size_t cur_len = 0;
  size_t nread;
  while ((nread=fread(out + cur_len, 1, max_length - cur_len, stdin)) != 0)
  {
    cur_len += nread;
    if (cur_len == max_length)
    {
      fprintf(stderr, "max message length exceeded\n");
      exit(1);
    }
  }
  return cur_len;
}


int main (int argc, const char * argv[])
{
  bool status;

  // Read packed message from standard-input.
  uint8_t buffer[MAX_MSG_SIZE];
  size_t message_length = read_buffer (MAX_MSG_SIZE, buffer);


  /* Allocate space for the decoded message. */
AMessage message = AMessage_init_zero;

  /* Create a stream that reads from the buffer. */
  pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

  /* Now we are ready to decode the message. */
  status = pb_decode(&stream, AMessage_fields, &message);

  /* Check for errors... */
  if (!status)
  {
      printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
      return 1;
  }


  // display the message's fields.
  printf("Received: a=%d",message.a);  // required field
  printf("  b=%d",message.b);
  printf("\n");


  return 0;
}
