#include "xarp.h"
#include "../global_defines.h"
#include "../misc.h"
#include "../my_socket.h"


// xarp show
unsigned char* build_xarp_show_message(){
  unsigned char* message;
  message = malloc(sizeof(char)*1);
  message[0] = XARP_SHOW;
  return message;
}

// xarp res EndereçoIP
unsigned char* build_xarp_res_message(char** args){
  unsigned char* message;
  unsigned char* ip_bytes;

  message = malloc(sizeof(unsigned char)*5); // 1 + 4

  ip_bytes = get_ip_addr_bytes_from_string(args[2]);

  message[0] = XARP_RES;
  memcpy(message+1, ip_bytes, 4); // ip address
  return message;
}

//xarp add EndereçoIP EndereçoEthernet ttl
unsigned char* build_xarp_add_message(char** args){
  unsigned char* mac_bytes;
  unsigned char* ip_bytes;
  unsigned char* ttl_bytes;
  unsigned char* message;

  message = malloc(15); // 1 + 4 + 6 + 4

  mac_bytes = get_mac_addr_bytes_from_string(args[3]);
  ip_bytes = get_ip_addr_bytes_from_string(args[2]);
  ttl_bytes = get_ttl_bytes_from_string(args[4]);

  message[0] = XARP_ADD;
  memcpy(message+1, ip_bytes, 4); // ip address
  memcpy(message+1+4, mac_bytes, 6); // ethernet address as 6 bytes
  memcpy(message+1+4+6, ttl_bytes, 4); //ttl

  free(ip_bytes);
  free(mac_bytes);
  free(ttl_bytes);

  return message;
}

// xarp del endereçoIP
unsigned char* build_xarp_del_message(char** args){
  unsigned char* message;
  unsigned char* ip_bytes;

  message = malloc(5); // 1 + 4

  ip_bytes = get_ip_addr_bytes_from_string(args[2]);
  message[0] = XARP_DEL;
  memcpy(message+1, ip_bytes, 4); // ip address

  free(ip_bytes);

  return message;
}

// xarp ttl <number>
unsigned char* build_xarp_ttl_message(char* ttl){
  unsigned char* message = malloc(5*sizeof(char));
  unsigned char* ttl_bytes = NULL;

  ttl_bytes = get_ttl_bytes_from_string(ttl);

  message[0] = XARP_TTL;
  memcpy(message+1, ttl_bytes, 4); // ttl

  free(ttl_bytes);

  return message;
}

int main(int argc, char** argv){
  char* op = argv[1];
  unsigned char* message;


  if(strcmp(op, "show") == 0){
    message = build_xarp_show_message();
  }
  else{
    if(strcmp(op, "res") == 0){
      message = build_xarp_res_message(argv);
    }
    else{
      if(strcmp(op, "add") == 0){
        message = build_xarp_add_message(argv);
      }
      else{
        if(strcmp(op, "del") == 0){
          message = build_xarp_del_message(argv);
        }
        else{
          if(strcmp(op, "ttl") == 0){
            message = build_xarp_ttl_message(argv[2]);
          }
        }
      }
    }
  }

  int sockfd, bytes_received, total_bytes_received;
	unsigned char buffer[BUFFSIZE];
	struct sockaddr_in serv_addr;

  sprintf((char*)buffer, "%s", message);

	memset((char*) &serv_addr, 0, sizeof(serv_addr));

  sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
  load_server_params(&serv_addr, DEFAULT_IP, PORT);
  my_connect(sockfd, &serv_addr);

  my_send(sockfd, (char*) buffer, sizeof(buffer));

	memset(buffer, 0, sizeof(buffer));

  total_bytes_received = 0;
  do{
      bytes_received = recv(sockfd, buffer+total_bytes_received, BUFFSIZE-total_bytes_received, 0);
      if(bytes_received < 0) {
    		fprintf(stderr, "ERROR: %s\n", strerror(errno));
    		exit(1);
    	}
			total_bytes_received += bytes_received;
	}while(bytes_received > 0);

	printf("\n%s\n", buffer);

  free(message);
  return 0;
}
