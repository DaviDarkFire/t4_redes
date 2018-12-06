#include <stdio.h>
#include <string.h>
#include "xroute.h"
#include "../global_defines.h"
#include "../misc.h"
#include "../my_socket.h"

char* build_xroute_show_message() {
  char* message;
  char opcode[1];
  message = malloc(sizeof(char)*1);
  sprintf(opcode, "%d", XROUTE_SHOW);
  memcpy(message, opcode, sizeof(char)); // opcode
  return message;
}

char* build_xroute_add_message(char** argv) {
  char* message;
  return message;
}

char* build_xroute_del_message(char** argv) {
  char* message;
  return message;
}

int main(int argc, char** argv) {
  struct sockaddr_in serv_addr;
  char* op = argv[1];
  unsigned char* message;

  int sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
  load_socket_info(&serv_addr, DEFAULT_IP, PORT);
  my_connect(sockfd, &serv_addr);

  if(strcmp(op, "show") == 0){
    message = build_xroute_show_message();
  } else {
    if(strcmp(op, "add") == 0){
      message = build_xroute_add_message(argv);
    } else {
      if(strcmp(op, "del") == 0){
        message = build_xroute_del_message(argv);
      }
    }
  }
  char buffer[BUFFSIZE];
  sprintf(buffer, "%s", message);

  my_send(sockfd, buffer, sizeof(buffer));

  return 0;
}
