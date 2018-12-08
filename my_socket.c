#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "my_socket.h"

void my_bind(int sockfd, struct sockaddr *address) {
  if(bind(sockfd, address, sizeof(*address)) < 0) {
    print_error();
		exit(1);
  }
}

void my_listen(int sockfd, unsigned int queue_max_size) {
	if(listen(sockfd, queue_max_size) < 0) {
		print_error();
		exit(1);
	}
}

int my_accept(int sockfd, struct sockaddr *cli_addr) {
	int cli_len = sizeof(cli_addr);
	int connfd = accept(sockfd, cli_addr, (unsigned int*) &cli_len);
	if(connfd < 0) {
		print_error();
		exit(1);
	}
	return connfd;
}

int my_recv(int sockfd, unsigned char *buffer, unsigned int buffer_size) {
  memset(buffer, 0, buffer_size);
  int bytes_received = recv(sockfd, buffer, buffer_size, 0);
  if(bytes_received < 0){
    print_error();
		exit(1);
  }
  return bytes_received;
}

void load_server_params(struct sockaddr_in *serv_addr, const char *ip_address, int port)
{
  memset((char*) serv_addr, 0, sizeof(*serv_addr));
  serv_addr->sin_family = AF_INET;
  serv_addr->sin_addr.s_addr = inet_addr(ip_address);
  serv_addr->sin_port = port;
}

int create_socket(int domain, int type, int protocol) {
  int sockfd = socket(domain, type, protocol);
  if(sockfd < 0) {
    print_error();
    exit(1);
  }
  return sockfd;
}

void bind_iface_name(int sockfd, char* iface_name) {
	if(setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface_name, strlen(iface_name) < 0)) {
        perror("Server-setsockopt() error for SO_BINDTODEVICE");
		print_error();
		close(sockfd);
		exit(1);
    }
}

void my_send(int sockfd, char *message, unsigned int message_len) {
  int bytes_sent = 0;
  do {
      bytes_sent += send(sockfd, message+bytes_sent, message_len - bytes_sent, 0);
      if (bytes_sent < 0) {
        print_error();
        exit(1);
      }
  } while (bytes_sent < message_len);
}

void my_connect(int sockfd, struct sockaddr_in* serv_addr) {
  if(connect(sockfd, (struct sockaddr*) serv_addr, sizeof(*serv_addr)) < 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(1);
	}
}

void print_error(){
	fprintf(stderr, "%s\n", strerror(errno));
}
