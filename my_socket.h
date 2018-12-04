#ifndef __MY_SOCKET__
  #define __MY_SOCKET__
  void my_bind(int sockfd, struct sockaddr *address);
  void my_listen(int sockfd, unsigned int queue_max_size);
  int my_accept(int sockfd, struct sockaddr *cli_addr);
  int my_recv(int sockfd, unsigned char *buffer, unsigned int buffer_size);
  void load_socket_info(struct sockaddr_in *serv_addr, const char *ip_address, int port);
  int create_socket(int domain, int type, int protocol);
  void bind_iface_name(int sockfd, char* iface_name);
  void print_error();
#endif
