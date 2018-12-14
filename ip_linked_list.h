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

  extern ip_entry_t* ip_head;

  ip_entry_t* create_ip_entry(unsigned int dest_ip, unsigned int gateway, unsigned int netmask, char* iface);
  ip_entry_t* get_last_ip_entry();
  void add_ip_entry(ip_entry_t* new_entry);
  ip_entry_t* get_previous_entry_of_desired_ip_entry(unsigned int dest_ip);
  int delete_ip_entry(unsigned int dest_ip, unsigned int netmask, unsigned int gateway);
  void print_dotted_dec_address(unsigned int address, FILE* fp);
  void print_ip_entry(ip_entry_t* ip_entry, FILE* fp);
  void print_ip_table(FILE* fp);
  void initialize_ip_head();
  ip_entry_t* search_for_ip_entry_hit(unsigned int ip_address);

#endif
