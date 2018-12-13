#include "ip_linked_list.h"
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_defines.h"

ip_entry_t* ip_head;

void initialize_ip_head(){
  ip_head = (ip_entry_t*) malloc(sizeof(ip_entry_t));
}

ip_entry_t* create_ip_entry(unsigned int dest_ip, unsigned int gateway, unsigned int netmask, char* iface) {
  ip_entry_t* new_entry = (ip_entry_t*) malloc(sizeof(ip_entry_t));
  new_entry->dest_ip = dest_ip;
  new_entry->gateway = gateway;
  new_entry->netmask = netmask;
  strcpy(new_entry->iface, iface);
  new_entry->next = NULL;
  return new_entry;
}

ip_entry_t* get_last_ip_entry() {
  ip_entry_t* current = ip_head;
  while(current->next != NULL) {
    current = current->next;
  }
  return current;
}

void add_ip_entry(ip_entry_t* new_entry) {
  ip_entry_t* previous = ip_head;
  ip_entry_t* current = ip_head->next;
  while(current != NULL) {
    if((new_entry->dest_ip & new_entry->netmask) > (current->dest_ip & current->netmask)) {
      new_entry->next = current;
      break;
    }
    previous = current;
    current = current->next;
  }
  previous->next = new_entry;
}

ip_entry_t* get_previous_entry_of_desired_ip_entry(unsigned int dest_ip) {
  ip_entry_t* current = ip_head;
  ip_entry_t* previous;

  while(current->next != NULL) {
    previous = current;
    current = current->next;
    if(current->dest_ip == dest_ip) {
      return previous;
    }
  }
  return NULL;
}

int delete_ip_entry(unsigned int dest_ip) {
  ip_entry_t* previous_of_deleted_entry = get_previous_entry_of_desired_ip_entry(dest_ip);
  ip_entry_t* deleted_entry = previous_of_deleted_entry->next;
  if(previous_of_deleted_entry == NULL) {
    return -1;
  }

  previous_of_deleted_entry->next = NULL;
  free(deleted_entry);
  return 0;
}

void print_ip_entry(ip_entry_t* ip_entry, FILE* fp) {
  print_dotted_dec_address(ip_entry->dest_ip, fp);
  print_dotted_dec_address(ip_entry->gateway, fp);
  print_dotted_dec_address(ip_entry->netmask, fp);
  fprintf(fp, "  %s  |", ip_entry->iface);
  fprintf(fp, " %3d \n", ip_entry ->ttl);
}

void print_dotted_dec_address(unsigned int address, FILE* fp) {
  fprintf(fp, "%3u.%3u.%3u.%3u | ", (address & 0xFF000000) >> 24 , (address & 0x00FF0000) >> 16,
                            (address & 0x0000FF00) >> 8 , address & 0x000000FF);
}

void print_ip_table(FILE* fp) {
  fprintf(fp, "    Destino     |     Gateway     |     Máscara     | Interface | TTL\n");
  ip_entry_t* current = ip_head->next;
  while(current != NULL) {
    print_ip_entry(current, fp);
    current = current->next;
  }
  fprintf(fp, "\n");
}
