#ifndef __LINKED_LIST__
  #define __LINKED_LIST__
  #include <stdio.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdlib.h>
  #include <string.h>

  typedef struct node{
    unsigned int ip_address;
    unsigned char eth_address[6];
    unsigned int ttl;
    struct node* next;
  } node_t;

  extern node_t* g_head;

  node_t* add_node(node_t** head, unsigned int ip_address, unsigned char eth_address[6], int ttl);
  int delete_node_by_ip_address(node_t** head, unsigned int del_ip_addr);
  void print_list(node_t * head, FILE* fp);
  int list_size(node_t * head);
  void get_eth_addr_as_6_bytes_from_string(unsigned char dest_array[6], char* src_str);
  node_t* find_node_by_ip_address(node_t* head, unsigned int desired_ip_addr);
#endif
