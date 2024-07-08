#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define THREAD_POOL_SIZE 10

void handle_request(int remote_sockfd) {
  int nread, nwrite;
  char buffer[BUFFER_SIZE],
      response[BUFFER_SIZE] =
          "http/1.0 200 OK\r\nServer: HTTP-Server-C\r\nContent-type: "
          "text/html\r\n\r\n<html><body><h1>%.*s</h1></body></html>\r\n";

  char method[100], resource[100], version[100];
  // read data from the remote socket file
  nread = read(remote_sockfd, (void *)buffer, BUFFER_SIZE);
  if (nread == -1) {
    perror("(read)");
    exit(EXIT_FAILURE);
  }
  sscanf(buffer, "%s %s %s", method, resource, version);
  if (nread == -1) {
    perror("(sscanf)");
    exit(EXIT_FAILURE);
  }

  printf("method: %s\nresource: %s\nversion: %s\n", method, resource, version);

  // prepare the HTTP response
  char resp_message[BUFFER_SIZE];
  sprintf(resp_message, "%.*s", nread, buffer);
  nwrite = sprintf(buffer, response, nread, resp_message);
  if (nwrite == -1) {
    perror("(sprintf)");
    exit(EXIT_FAILURE);
  }

  // write a response to the remote socket file
  nwrite = write(remote_sockfd, buffer, nwrite);
  if (nwrite == -1) {
    perror("(write)");
    exit(EXIT_FAILURE);
  }

  // close remote socket
  close(remote_sockfd);
}

int main(void) {
  // declare variables
  int local_sockfd, sockopt_state, bind_state, listen_state, remote_sockfd;
  struct sockaddr_in local_addr, remote_addr;
  socklen_t addrlen = sizeof(local_addr);

  // create a stream socket
  local_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (local_sockfd == -1) {
    perror("(socket)");
    return EXIT_FAILURE;
  }

  // define local address
  memset(&local_addr, '\0', addrlen);
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(PORT);
  local_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  // set socket option to allow socket reuse (for faster reruns)
  sockopt_state = setsockopt(local_sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1},
                             sizeof(int));
  if (sockopt_state == -1) {
    perror("(setsockopt)");
    return EXIT_FAILURE;
  }

  // bind to port
  bind_state = bind(local_sockfd, (struct sockaddr *)&local_addr, addrlen);
  if (bind_state == -1) {
    perror("(bind)");
    return EXIT_FAILURE;
  }

  // change socket state to listening state
  listen_state = listen(local_sockfd, 5);
  if (listen_state == -1) {
    perror("(listen)");
    return EXIT_FAILURE;
  }
  printf("Listening on http://%s:%d\n", inet_ntoa(local_addr.sin_addr), PORT);

  // accept new requests
  while (1) {
    // create a socket for remote host (i.e. client)
    remote_sockfd =
        accept(local_sockfd, (struct sockaddr *)&remote_addr, &addrlen);
    if (remote_sockfd == -1) {
      perror("(accept)");
      return EXIT_FAILURE;
    }

    handle_request(remote_sockfd);
  }
}
