#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* socket_path = "./socket";
//char *socket_path = "\0hidden";

char* arr[] = {"hi", "hello", "guten tag", "zdrastvyte"};
size_t sizes[] = {3, 6, 10, 11};

//char* arr[] = {"hi\n", "hello\n", "guten tag\n", "zdrastvyte\n"};
//size_t sizes[] = {4, 7, 11, 12};

int main(int argc, char* argv[])
{
	struct sockaddr_un addr;
	int fd, rc;
	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	if(*socket_path == '\0')
	{
		*addr.sun_path = '\0';
		strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
	}
	else
	{
		strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
	}

	if(connect(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1)
	{
		perror("connect error");
		exit(-1);
	}


	for(int i = 0; i < 4; i++)
	{
		printf("sending %lu bytes: %s\n", sizes[i], arr[i]);
		if(send(fd, arr[i], sizes[i]-1, 0) != sizes[i]-1)
		{
			if(sizes[i] > 0)
			{
				fprintf(stderr, "partial write\n");
			}
			else
			{
				perror("write error");
				exit(-1);
			}
		}
	}

//	send(fd, "hi", 2, 0);
//	send(fd, "hello", 5, 0);
//	send(fd, "guten tag", 9, 0);
//	send(fd, "zdrastvyte", 10, 0);

	return 0;
}