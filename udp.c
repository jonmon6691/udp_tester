#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main() {
	int fd;
	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	perror("socket");

#if 0 // Bind to device block
	setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, "eth0", 4);
	perror("Bind to eth0");
#endif	

	struct sockaddr_in sock_ptr;
	memset(&sock_ptr, 0, sizeof(struct sockaddr_in));

	sock_ptr.sin_family = AF_INET;
	sock_ptr.sin_port = htons(3000);
#if 0 // Bind to specific local IP
	inet_aton("127.0.0.1", &sock_ptr.sin_addr);
#else // Listen on any
	sock_ptr.sin_addr.s_addr = INADDR_ANY;
#endif

	bind(fd, (struct sockaddr *)(&sock_ptr), sizeof(struct sockaddr));
	perror("bind");

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);


	struct sockaddr source;
	#define MAX_DATA 2048
	char data[MAX_DATA];

	while (1) {
		select(fd + 1, &fds, NULL, NULL, NULL);
		perror("select");
		
		socklen_t socklen = sizeof(source);
		int bytes = recvfrom(fd, &data, MAX_DATA, 0, &source, &socklen);
		perror("recvfrom");
		
		struct sockaddr_in *sin;
		sin = (void *)&source;
		
		printf("FROM: %s\n", inet_ntoa(sin->sin_addr));

		printf("DATA:\n%.*s\n\n", bytes, data);
	}
	return 0;
}
