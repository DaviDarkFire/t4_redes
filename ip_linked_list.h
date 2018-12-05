#ifndef __IP_LINKED_LIST__
  #define __IP_LINKED_LIST__
  #include "global_defines.h"
  #include <stdio.h>

  typedef struct ip_entry {
      unsigned int dest_ip;
      unsigned int gateway;
      unsigned int netmask;
      char iface[MAX_IFNAME_LEN];
      int ttl;
      struct ip_entry * next;
  } ip_entry_t;

  ip_entry_t* create_ip_entry(unsigned int dest_ip, unsigned int gateway, unsigned int netmask, char* iface);
  ip_entry_t* get_last_ip_entry(ip_entry_t* head);
  void add_ip_entry(ip_entry_t* head, ip_entry_t* new_entry);
  ip_entry_t* get_previous_entry_of_desired_ip_entry(ip_entry_t* head, unsigned int dest_ip);
  int delete_ip_entry(ip_entry_t* head, unsigned int dest_ip);
  void print_dotted_dec_address(unsigned int address, FILE* fp);
  void print_ip_entry(ip_entry_t* ip_entry, FILE* fp);
  void print_ip_table(ip_entry_t* ip_head, FILE* fp);

#endif
