#include "ip_linked_list.h"
#include <string.h>
#include <stdlib.h>
#include "global_defines.h"

ip_entry_t* create_ip_entry(unsigned int dest_ip, unsigned int gateway, unsigned int netmask, char* iface) {
  ip_entry_t* new_entry = (ip_entry_t*) malloc(sizeof(ip_entry_t));
  new_entry->dest_ip = dest_ip;
  new_entry->gateway = gateway;
  new_entry->netmask = netmask;
  strcpy(new_entry->iface, iface);
  new_entry->next = NULL;
  return new_entry;
}

ip_entry_t* get_last_ip_entry(ip_entry_t* head) {
  ip_entry_t* current = head;
  while(current->next != NULL) {
    current = current->next;
  }
  return current;
}

void add_ip_entry(ip_entry_t* head, ip_entry_t* new_entry) {
  ip_entry_t* last_entry = get_last_ip_entry(head);
  last_entry->next = new_entry;
}

ip_entry_t* get_previous_entry_of_desired_ip_entry(ip_entry_t* head, unsigned int dest_ip) {
  ip_entry_t* current = head;
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

int delete_ip_entry(ip_entry_t* head, unsigned int dest_ip) {
  ip_entry_t* previous_of_deleted_entry = get_previous_entry_of_desired_ip_entry(head, dest_ip);
  ip_entry_t* deleted_entry = previous_of_deleted_entry->next;
  if(previous_of_deleted_entry == NULL) {
    return -1;
  }

  previous_of_deleted_entry->next = NULL;
  free(deleted_entry);
  return 0;

}
