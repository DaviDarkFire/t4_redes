#include "ipd.h"
#include <errno.h>
#include <stdio.h>
#include <pthread.h> // pthread_t
#include <sys/ioctl.h> // ioctl()
#include <stdlib.h> //exit
#include <sys/socket.h>//sockets
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <net/if.h>


struct iface my_ifaces[MAX_IFACES];

void print_usage() {
	printf("usage: ipd <interface> [<interfaces>]\n");
	exit(1);
}

int create_socket() {
  int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(sockfd < 0) {
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			exit(1);
		}
  return sockfd;
}

void print_error(){
	fprintf(stderr, "%s\n", strerror(errno));
}

void bind_iface_name(int sockfd, char* iface_name) {

	if(setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface_name, strlen(iface_name) < 0)) {
        perror("Server-setsockopt() error for SO_BINDTODEVICE");
		print_error();
		close(sockfd);
		exit(1);
    }
}

void get_iface_info(int sockfd, char *ifname, struct iface *ifn) {
	struct ifreq s;

	strcpy(s.ifr_name, ifname);
	// Getting HW address
	if(0 == ioctl(sockfd, SIOCGIFHWADDR, &s)){
		memcpy(ifn->mac_addr, s.ifr_addr.sa_data, ETH_ADDR_LEN);
		ifn->sockfd = sockfd;
		strcpy(ifn->ifname, ifname);
	} else {
		perror("Error getting MAC address\n");
		exit(1);
	}

	// Getting MTU value
	if (0 ==ioctl(sockfd, SIOCGIFMTU, &s)) {
		ifn->mtu = s.ifr_mtu;
	} else {
		perror("Error getting MTU value\n");
		exit(1);
	}
}

void iface_pthread_create(pthread_t tid, unsigned int iface_index) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, read_iface, &my_ifaces[iface_index]);
}

void* read_iface(void *arg)
{
	struct iface *ifn = (struct iface*) arg;

	socklen_t	saddr_len;
	struct sockaddr	saddr;
	unsigned char	*packet_buffer;
	int		n;

	saddr_len = sizeof(saddr);
	packet_buffer = malloc(MAX_PACKET_SIZE);
	if (!packet_buffer) {
		printf("\nCould not allocate a packet buffer\n");
		exit(1);
	}

	while(1) {
		n = recvfrom(ifn->sockfd, packet_buffer, MAX_PACKET_SIZE, 0, &saddr, &saddr_len);
		if(n < 0) {
			print_error();
			exit(1);
		}
		ifn->rx_pkts++;
		ifn->rx_bytes += n;
		handle_packet(packet_buffer, n);
		// free(arg);
	}
}

void handle_packet(unsigned char* packet, int len) {
	if(!len || len < MIN_PACKET_SIZE)
		return;

	struct ether_hdr* eth = (struct ether_hdr*) packet;

	if(htons(0x0800) == eth->ether_type) { // IP
		struct ip_hdr* ip_header = (struct ip_hdr*) (packet+BYTES_UNTIL_BODY);

	}
	// Ignore if it is not an ARP packet
}

void print_eth_address(char *s, unsigned char *eth_addr)
{
	printf("%s %02X:%02X:%02X:%02X:%02X:%02X", s,
	       eth_addr[0], eth_addr[1], eth_addr[2],
	       eth_addr[3], eth_addr[4], eth_addr[5]);
}

void print_iface_info(int sockfd, unsigned int iface_index){
	struct sockaddr_in sa_ip, sa_bcast, sa_mask;

	sa_ip.sin_addr.s_addr = (unsigned long) my_ifaces[iface_index].ip_addr;
	sa_bcast.sin_addr.s_addr = (unsigned long) my_ifaces[iface_index].bcast_addr;
	sa_mask.sin_addr.s_addr = (unsigned long) my_ifaces[iface_index].netmask;

	printf("%s Link encap: Endereço de HW %02X:%02X:%02X:%02X:%02X:%02X\n",
	my_ifaces[iface_index].ifname,
	my_ifaces[iface_index].mac_addr[0],
	my_ifaces[iface_index].mac_addr[1],
	my_ifaces[iface_index].mac_addr[2],
	my_ifaces[iface_index].mac_addr[3],
	my_ifaces[iface_index].mac_addr[4],
	my_ifaces[iface_index].mac_addr[5]);

	printf("\tinet end.:%s Bcast:%s Masc: %s\n",
	inet_ntoa(sa_ip.sin_addr), inet_ntoa(sa_bcast.sin_addr),
	inet_ntoa(sa_mask.sin_addr));
	// ip_address, bcast_address, netmask);

	printf("\tUP MTU: %d\n", my_ifaces[iface_index].mtu);

	printf("\tRX packets:%u TX packets:%u\n",
	my_ifaces[iface_index].rx_pkts,
	my_ifaces[iface_index].tx_pkts);

	printf("\tRX bytes:%u TX bytes:%u\n",
	my_ifaces[iface_index].rx_bytes,
	my_ifaces[iface_index].tx_bytes);
}

int main(int argc, char** argv) {
  int i, sockfd;

  if (argc < 2)
		print_usage();

    for(i = 1; i < argc; i++) {
        sockfd = create_socket();
        bind_iface_name(sockfd, argv[i]);
        get_iface_info(sockfd, argv[i], &my_ifaces[i-1]);
        // get_iface_info
    }

    pthread_t tid[argc-1];
	for (i = 0; i < argc-1; i++) {
		struct iface *arg = malloc(sizeof(*arg));
		print_eth_address(my_ifaces[i].ifname, my_ifaces[i].mac_addr);
		printf("\n");

		iface_pthread_create(tid[i], i);
		print_iface_info(sockfd, i);
		// Create one thread for each interface. Each thread should run the function read_iface.
	}

	for(i = 0; i < argc-1; i++){
		pthread_join(tid[i], NULL);
	}

	return 0;
}