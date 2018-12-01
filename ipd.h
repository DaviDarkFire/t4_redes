#ifndef __IPD__
#define __IPD__
#define BYTES_UNTIL_BODY 14
#define MAX_IFACES	64
#define MAX_PACKET_SIZE 65536
#define MIN_PACKET_SIZE 64
#define MAX_IFACES	64
#define MAX_IFNAME_LEN	6
#define ETH_ADDR_LEN	6
#define IP_ADDR_LEN 4
#define BUFFSIZE 1024
#define MAX_IFNAME_LEN	6

#include <pthread.h> // pthread_t

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
};

struct ip_hdr {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	unsigned char	ip_ihl:4,
								ip_v:4;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	unsigned char	ip_v:4,
								ip_ihl:4;

#endif
	unsigned char	ip_tos;		// Type of service
	unsigned short	ip_len;		// Datagram Length
	unsigned short	ip_id;		// Datagram identifier
	unsigned short	ip_offset;	// Fragment offset
	unsigned char	ip_ttl;		// Time To Live
	unsigned char	ip_proto;	// Protocol
	unsigned short	ip_csum;	// Header checksum
	unsigned int	ip_src;		// Source IP address
	unsigned int	ip_dst;		// Destination IP address
};

void print_usage();
int create_socket();
void print_error();
void bind_iface_name(int sockfd, char* iface_name);
void get_iface_info(int sockfd, char *ifname, struct iface *ifn);
void iface_pthread_create(pthread_t tid, unsigned int iface_index);
void* read_iface(void *arg);
void handle_packet(unsigned char* packet, int len);
void print_eth_address(char *s, unsigned char *eth_addr);
void print_iface_info(int sockfd, unsigned int iface_index);

#endif