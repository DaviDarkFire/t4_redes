#ifndef __IPD__
#define __IPD__

#include <pthread.h> // pthread_t
#include <stdio.h>
#include "arp_linked_list.h"
#include "global_defines.h"

struct ether_hdr {
	unsigned char	ether_dhost[6];	// Destination address
	unsigned char	ether_shost[6];	// Source address
	unsigned short	ether_type;	// Type of the payload
};

struct iface {
	int		sockfd;
	int		ttl;
	int		mtu;
	char		ifname[MAX_IFNAME_LEN];
	unsigned char	mac_addr[6];
	unsigned int	ip_addr;
	unsigned int bcast_addr;
	unsigned int netmask;
	unsigned int	rx_pkts;
	unsigned int	rx_bytes;
	unsigned int	tx_pkts;
	unsigned int	tx_bytes;
	char up_or_down;
};
void print_usage();
void print_error();
void get_iface_info(int sockfd, char *ifname, struct iface *ifn);
void iface_pthread_create(pthread_t tid, unsigned int iface_index);
void* read_iface(void *arg);
void handle_packet(unsigned char* packet, int len);
void print_eth_address(char *s, unsigned char *eth_addr);
void print_iface_info(int sockfd, FILE* fp, unsigned int iface_index);
unsigned int get_iface_index(char* iface_name);
void daemon_handle_request(unsigned char* request, int sockfd, unsigned int qt_ifaces);
char* get_iface_by_gateway(unsigned int gateway, unsigned int qt_ifaces);
void xifconfig_up_or_down(unsigned char* request, unsigned int qt_ifaces);


#endif
