#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8000
#define IP_ADDR "127.0.0.1"

int main(void) {
  int sockfd, bind_state, remote_sockfd;
  struct sockaddr_in local_addr, remote_addr;
  socklen_t addrlen;
  char buffer[1024];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    fprintf(stderr, "ERR: failed to create a stream socket!\n");
    return EXIT_FAILURE;
  }

  memset(&local_addr, '\0', sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = PORT;
  local_addr.sin_addr.s_addr = inet_addr(IP_ADDR);

  bind_state = bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr));
  if (bind_state == -1) {
    fprintf(stderr, "ERR: failed to bind socket!\n");
    return EXIT_FAILURE;
  }

  listen(sockfd, 5);
  printf("Listening on http://%s:%d\n", IP_ADDR, PORT);

  while (1) {
    addrlen = sizeof(remote_addr);
    remote_sockfd = accept(sockfd, (struct sockaddr *)&remote_addr, &addrlen);
    printf("Got a new connection");
  }
}
