#include "xifconfig.h"
#include "../global_defines.h"
#include "../misc.h"
#include "../my_socket.h"

unsigned int decide_mode(int argc, char** argv){
  if (argc == 1)
    return DEFAULT_MODE;
  if(strcmp(argv[2], "mtu") == 0)
    return SET_MTU_MODE;
  if(strcmp(argv[2], "up") == 0 || strcmp(argv[2], "down") == 0)
    return UP_OR_DOWN_MODE;
  return CONFIG_IP_MODE;
}

void set_mtu_mode(char* iface, char* new_mtu){
  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  struct ifreq ifr;

  strcpy(ifr.ifr_name, iface);

  ifr.ifr_mtu = atoi(new_mtu);
  if(!ioctl(sockfd, SIOCSIFMTU, &ifr)) {
    printf("MTU changed successfully.\n");
  }
}

void configure_ip_mode(char* iface, char* ip_addr, char* netmask){
    struct ifreq ifr;
    int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strncpy(ifr.ifr_name, iface, IFNAMSIZ); //IFNAMSIZ is the iface's max name length
    ifr.ifr_addr.sa_family = AF_INET;
    inet_pton(AF_INET, ip_addr, ifr.ifr_addr.sa_data + 2);
    ioctl(sockfd, SIOCSIFADDR, &ifr);

    inet_pton(AF_INET, netmask, ifr.ifr_addr.sa_data + 2);
    ioctl(sockfd, SIOCSIFNETMASK, &ifr);

    ioctl(sockfd, SIOCGIFFLAGS, &ifr);
    strncpy(ifr.ifr_name, iface, IFNAMSIZ);
    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);

    ioctl(sockfd, SIOCSIFFLAGS, &ifr);
}

unsigned char* build_xifconfig_info_message(){
  unsigned char* message;

  message = malloc(sizeof(unsigned char)*1); //
  message[0] = XIFCONFIG_INFO;
  return message;
}

// xifconfig <interface> <IP address> <IP Netmask>
unsigned char* build_xifconfig_ip_message(char** args){
  unsigned char* message;

  int ifname_len = (int) strlen(args[1]);
  message = malloc(sizeof(unsigned char)*(1+ifname_len+4+4));
  message[0] = XIFCONFIG_IP;
  memcpy(message+1, args[1], (size_t) ifname_len); // ifname
  return message;
}

unsigned char* build_xifconfig_mtu_message(char** args){
  int ifname_len = (int) strlen(args[1]);
  unsigned char* message = malloc(sizeof(unsigned char)*(1+ifname_len));
  message[0] = XIFCONFIG_MTU;
  memcpy(message+1, args[1], (size_t) ifname_len);

  return message;
}

unsigned char* build_xifconfig_up_or_down_message(char** args){
  int ifname_len = (int) strlen(args[1]);
  unsigned char* message = malloc(sizeof(unsigned char)*(2+ifname_len));
  message[0] = XIFCONFIG_UP_OR_DOWN;
  if(strcmp(args[2],"up") == 0)
    message[1] = UP;
  else
    message[1] = DOWN;

  memcpy(message+2, args[1], (size_t) ifname_len);
  printf("Mensagem: %s\n", message);//DEBUG
  return message;

}
int main(int argc, char** argv){
  unsigned int mode = decide_mode(argc, argv);
  unsigned char* message;
  switch(mode){
    case DEFAULT_MODE:{
      message = build_xifconfig_info_message();
      // printf("You chose default mode.\n"); //DEBUG
      break;
    }

    case CONFIG_IP_MODE:
      message = build_xifconfig_ip_message(argv);
      // printf("You chose ip configuration mode.\n"); // DEBUG
      configure_ip_mode(argv[1], argv[2], argv[3]);
    break;

    case SET_MTU_MODE:
      message = build_xifconfig_mtu_message(argv);
      printf("You chose MTU mode.\n"); //DEBUG
      set_mtu_mode(argv[1], argv[3]);
    break;

    case UP_OR_DOWN_MODE:
      message = build_xifconfig_up_or_down_message(argv);
      break;

    default:
      printf("Couldn't understand desired operation.\n");
    break;
  }

  int sockfd, bytes_received, total_bytes_received;
	unsigned char buffer[BUFFSIZE];
	struct sockaddr_in serv_addr;

  sprintf((char*)buffer, "%s", message);

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

	printf("\n%s\n", buffer); //DEBUG

  free(message);

  return 0;
}
