#include <stdio.h>
#include <string.h>
#include "xroute.h"
#include "../global_defines.h"
#include "../misc.h"
#include "../my_socket.h"

unsigned char* build_xroute_show_message(unsigned int* message_size) {
  unsigned char* message;
  *message_size = 1;
  message = malloc(sizeof(char)*1);
  message[0] = XROUTE_SHOW;
  return message;
}

// xroute add target netmask gateway
unsigned char* build_xroute_add_message(char** argv, unsigned int* message_size) {
  unsigned char* message;
  unsigned char* target_bytes;
  unsigned char* netmask_bytes;
  unsigned char* gateway_bytes;

  message = malloc(sizeof(char)*13); // 1 + 4 + 4 + 4
  *message_size = 13;
  target_bytes = get_ip_addr_bytes_from_string(argv[2]);
  netmask_bytes = get_ip_addr_bytes_from_string(argv[3]);
  gateway_bytes = get_ip_addr_bytes_from_string(argv[4]);

  message[0] = XROUTE_ADD;
  memcpy(message+1, target_bytes, 4);
  memcpy(message+1+4, netmask_bytes, 4);
  memcpy(message+1+4+4, gateway_bytes, 4);

  free(target_bytes);
  free(netmask_bytes);
  free(gateway_bytes);

  return message;
}

unsigned char* build_xroute_del_message(char** argv, unsigned int* message_size) {
  unsigned char* message;
  unsigned char* target_bytes;
  unsigned char* netmask_bytes;
  unsigned char* gateway_bytes;

  message = malloc(sizeof(char)*13); // 1 + 4 + 4 + 4
  *message_size = 13;
  target_bytes = get_ip_addr_bytes_from_string(argv[2]);
  netmask_bytes = get_ip_addr_bytes_from_string(argv[3]);
  gateway_bytes = get_ip_addr_bytes_from_string(argv[4]);

  message[0] = XROUTE_DEL;
  memcpy(message+1, target_bytes, 4);
  memcpy(message+1+4, netmask_bytes, 4);
  memcpy(message+1+4+4, gateway_bytes, 4);

  free(target_bytes);
  free(netmask_bytes);
  free(gateway_bytes);

  return message;
}
int get_opcode(char* command) {
  if(strcmp(command, "show") == 0)
    return XROUTE_SHOW;
  if(strcmp(command, "add") == 0)
    return XROUTE_ADD;
  if(strcmp(command, "del") == 0)
    return XROUTE_DEL;
  return -1;
}

void print_xroute_usage() {
  printf("Usage: xroute show\nxroute add target netmask gateway\nxroute del target netmask gateway\n");
  exit(1);
}

int main(int argc, char** argv) {

  if(argc < 2 || argc > 5) {
    print_xroute_usage();
  }

  struct sockaddr_in serv_addr;
  int opcode = get_opcode(argv[1]);
  unsigned char* message;
  unsigned int message_size;

  int sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
  load_server_params(&serv_addr, DEFAULT_IP, PORT);
  my_connect(sockfd, &serv_addr);


  switch(opcode) {
    case XROUTE_SHOW:
      message = build_xroute_show_message(&message_size);
      break;

    case XROUTE_ADD:
      message = build_xroute_add_message(argv, &message_size);
      break;

    case XROUTE_DEL:
      message = build_xroute_del_message(argv, &message_size);
      break;
  }

  char buffer[BUFFSIZE];
  // sprintf(buffer, "%s", message);

  printf("%s\n", message); // DEBUG

  my_send(sockfd, (char*)message, message_size);

  int bytes_received;
  int total_bytes_received = 0;
  memset(buffer, 0, BUFFSIZE);
  do {
      bytes_received = recv(sockfd, buffer+total_bytes_received, BUFFSIZE-total_bytes_received, 0);
      printf("Receiving %d bytes\n", bytes_received); // DEBUG
      if(bytes_received < 0) {
    		fprintf(stderr, "ERROR: %s\n", strerror(errno));
    		exit(1);
    	}
			total_bytes_received += bytes_received;
	} while(bytes_received > 0);

  printf("%s", buffer);

  return 0;
}
