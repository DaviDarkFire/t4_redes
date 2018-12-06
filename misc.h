#ifndef __MISC__
#define __MISC__
	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/ioctl.h>
	#include <net/if.h>
	#include <sys/socket.h>
	#include <errno.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include "arp_linked_list.h"

	#define PORT 5050
	#define DEFAULT_IP "127.0.0.1" 

	unsigned char* get_ip_addr_bytes_from_string(char* dotted_dec_ip);
	char* get_ip_address_as_dotted_dec(char* ifname);
	char* get_bcast_address_as_dotted_dec(char* ifname);
	char* get_netmask_as_dotted_dec(char* ifname);
	void printBits(size_t const size, void const * const ptr);
	unsigned char* get_mac_addr_bytes_from_string(char* colon_format_mac);
	unsigned char* get_ttl_bytes_from_string(char* str_ttl);
#endif
