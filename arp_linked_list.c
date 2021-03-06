#include "arp_linked_list.h"

node_t* arp_head;

void initialize_arp_head(){
  arp_head = (node_t*) malloc(sizeof(node_t));
}

// adds node to the end of the list
node_t* add_node(unsigned int ip_address, unsigned char eth_address[6], int ttl){
    node_t* new_node;
    new_node = (node_t*) malloc(sizeof(node_t));
    new_node->ip_address = ip_address;
    memcpy(new_node->eth_address, eth_address, 6);
    new_node->ttl = ttl;
    new_node->next = NULL;

    if (arp_head->next == NULL){
      arp_head->next = new_node;
      return new_node;
    } // creating the arp_head

    node_t* current = arp_head->next;
    while(current->next != NULL){
      current = current->next;
    }

    current->next = new_node;
    return new_node;
}

int delete_node_by_ip_address(unsigned int del_ip_addr){
  // int retval = -1;
  node_t* current = arp_head->next;
  node_t* previous = NULL;

  // no nodes in the list
  if(arp_head->next == NULL)
    return -1;

  // deleting arp_head
  if(del_ip_addr == (arp_head->next)->ip_address){
    node_t* aux = arp_head->next;
    arp_head->next = (arp_head->next)->next;
    free(aux);
    return 1;
  }

  while(1){
    if(current->ip_address == del_ip_addr) // current is the node we'll delete
      break;
    if(current->next == NULL)
      return -1;
    previous = current;
    current = current->next;
  }
  if(previous != NULL)
    previous->next = current->next;
  free(current);
  return 1;
}

void print_list(FILE* fp){
  node_t * current = arp_head->next;
  struct in_addr ip_addr;
  int i;
  fprintf(fp ,"Entrada\t\tEndereço IP\t\tEndereço Ethernet\t\tTTL\n");

  for(i = 0; current != NULL; i++){
    ip_addr.s_addr = (uint32_t) ntohl(current->ip_address);

    fprintf(fp ,"%d\t\t%s\t\t%02X:%02X:%02X:%02X:%02X:%02X\t\t%d\n",
    i, inet_ntoa(ip_addr), current->eth_address[0], current->eth_address[1],
    current->eth_address[2], current->eth_address[3], current->eth_address[4],
    current->eth_address[5], current->ttl);
    current = current->next;
  }
}

int list_size(){
    node_t * current = arp_head->next;
    int i = 0;
    for(i = 0; current != NULL; i++){current = current->next;}
    return  i*(sizeof(node_t)-sizeof(node_t*));
}

// modifies dest_array with 6 separate bytes on each array element given a string with format XX:XX:XX:XX:XX:XX
void get_eth_addr_as_6_bytes_from_string(unsigned char dest_array[6], char* src_str){
    unsigned int aux[6];

    sscanf(src_str, "%x:%x:%x:%x:%x:%x",
    &aux[0], &aux[1], &aux[2],
    &aux[3], &aux[4], &aux[5]);

    dest_array[0] = (unsigned char) aux[0];
    dest_array[1] = (unsigned char) aux[1];
    dest_array[2] = (unsigned char) aux[2];
    dest_array[3] = (unsigned char) aux[3];
    dest_array[4] = (unsigned char) aux[4];
    dest_array[5] = (unsigned char) aux[5];
}

// returns desired node if it exists in the list, otherwise, returns NULL (be careful!)
node_t* find_node_by_ip_address(unsigned int desired_ip_addr){
    node_t* current = arp_head->next;
    while(current != NULL){
      if(current->ip_address == desired_ip_addr)
        return current;
      current = current->next;
    }
    return NULL;
}
