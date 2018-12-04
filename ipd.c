#include "ipd.h"
#include <errno.h>
#include <stdio.h>
#include <pthread.h> // pthread_t
#include <sys/ioctl.h> // ioctl()
#include <stdlib.h> //exit
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
#include <semaphore.h>
#include "my_socket.h"
#include "arp_protocol.h"
#include "linked_list.h"
#include "misc.h"


struct iface my_ifaces[MAX_IFACES];

int global_ttl = 60;

sem_t sem;

void update_mtu(char* ifname){
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	struct ifreq ifr;
	unsigned int iface_index = get_iface_index(ifname);

	strcpy(ifr.ifr_name, ifname);
	if(!ioctl(sock, SIOCGIFMTU, &ifr)) {
  	my_ifaces[iface_index].mtu = ifr.ifr_mtu;
	}
}

unsigned int get_iface_index(char* iface_name){
	unsigned int i;
	for(i = 0; i < MAX_IFACES; i++){
		if(strcmp(my_ifaces[i].ifname, iface_name) == 0)
			return i;
	}
	return -1;
}

void print_usage() {
	printf("usage: ipd <interface> [<interfaces>]\n");
	exit(1);
}

void get_iface_info(int sockfd, char *ifname, struct iface *ifn){
	struct ifreq s;
	// unsigned int iface_index = get_iface_index(ifname);

	strcpy(s.ifr_name, ifname);
	if (0 == ioctl(sockfd, SIOCGIFHWADDR, &s)) {
		memcpy(ifn->mac_addr, s.ifr_addr.sa_data, ETH_ADDR_LEN);
		ifn->sockfd = sockfd;
		strcpy(ifn->ifname, ifname);
	} else {
		perror("Error getting MAC address");
		exit(1);
	}

	update_mtu(ifname);
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

	if(htons(0x0806) == eth->ether_type) { // ARP
		int i;
		arp_hdr *arphdr = (arp_hdr*) (eth + 6 + 6 + 2); // 6B for MAC dst/src, 2B for eth type
		unsigned int ip_address;
		unsigned char mac_address[6];

  	for (i=0; i<6; i++) {
			mac_address[i] = (unsigned char) arphdr->sender_mac[i];
  	}

		node_t* new_node = add_node(&g_head, ip_address, mac_address, global_ttl);
		printf("(%d.%d.%d.%d, %2x:%2x:%2x:%2x:%2x:%2x, %d)",
		arphdr->sender_ip[0], arphdr->sender_ip[1],
		arphdr->sender_ip[2], arphdr->sender_ip[3],
	 	mac_address[0], mac_address[1], mac_address[2],
		mac_address[3], mac_address[4], mac_address[5],
		new_node->ttl);

		sem_post(&sem);
	} else {
		if(htons(0x0800) == eth->ether_type) { // IP
			struct ip_hdr* ip_header = (struct ip_hdr*) (packet+BYTES_UNTIL_BODY);

		}
	}
}

void print_eth_address(char *s, unsigned char *eth_addr)
{
	printf("%s %02X:%02X:%02X:%02X:%02X:%02X", s,
	       eth_addr[0], eth_addr[1], eth_addr[2],
	       eth_addr[3], eth_addr[4], eth_addr[5]);
}

void print_iface_info(int sockfd, FILE* fp, unsigned int iface_index){
	// TODO: como conseguir o Link encap?
	fprintf(fp, "%s Link encap: Endereço de HW %02X:%02X:%02X:%02X:%02X:%02X\n",
	my_ifaces[iface_index].ifname,
	my_ifaces[iface_index].mac_addr[0],
	my_ifaces[iface_index].mac_addr[1],
	my_ifaces[iface_index].mac_addr[2],
	my_ifaces[iface_index].mac_addr[3],
	my_ifaces[iface_index].mac_addr[4],
	my_ifaces[iface_index].mac_addr[5]);

	fprintf(fp, "\tUP MTU: %d\n", my_ifaces[iface_index].mtu);

	char* ip_address = get_ip_address_as_dotted_dec(my_ifaces[iface_index].ifname);
	fprintf(fp, "\tinet end.:%s ", ip_address);
	char* bcast_address = get_bcast_address_as_dotted_dec(my_ifaces[iface_index].ifname);
	fprintf(fp, "Bcast:%s ", bcast_address);
	char* netmask = get_netmask_as_dotted_dec(my_ifaces[iface_index].ifname);
	fprintf(fp, "Masc:%s\n", netmask);

	fprintf(fp, "\tRX packets:%u TX packets:%u\n",
	my_ifaces[iface_index].rx_pkts,
	my_ifaces[iface_index].tx_pkts);

	fprintf(fp, "\tRX bytes:%u TX bytes:%u\n",
	my_ifaces[iface_index].rx_bytes,
	my_ifaces[iface_index].tx_bytes);
}

void * decrease_ttl_every_sec(void* arg){
	node_t* current = g_head;
	while(1){
		while(current != NULL){
			if(current->ttl != -1)
				current->ttl -= 1;
			if(current->ttl == 0)
				delete_node_by_ip_address(&g_head, current->ip_address);
			current = current->next;
		}
		current = g_head;
		sleep(1);
	}
}

void xarp_show(FILE* fp, node_t** head) {
	print_list(*head, fp);
}

void xarp_res(FILE* fp, unsigned char* request) {
	unsigned int ip_address = (request[4] << 24) | (request[3] << 16) | (request[2] << 8) | (request[1]);
	node_t* found_node = find_node_by_ip_address(*head, ip_address);

	if(found_node != NULL){
		fprintf(fp, "(%d.%d.%d.%d, %02x:%02x:%02x:%02x:%02x:%02x, %u)",
		request[4], request[3], request[2], request[1],
		found_node->eth_address[0], found_node->eth_address[1],
		found_node->eth_address[2], found_node->eth_address[3],
		found_node->eth_address[4], found_node->eth_address[5],
		found_node->ttl);
	} else {
		unsigned char* dd_ip = malloc(sizeof(char)*16);
		sprintf(dd_ip, "%d.%d.%d.%d", request[4], request[3], request[2], request[1]);
		send_arp_request(my_ifaces[0].ifname, dd_ip);

		struct timespec ts;
		if (clock_gettime(CLOCK_REALTIME, &ts) == -1){
			printf("Error at clock_gettime.\n");
		}
		ts.tv_sec += 3;
		int res = sem_timedwait(&sem, &ts);
		if (res == -1){
			if (errno == ETIMEDOUT)
				fprintf(fp, "Endereço IP desconhecido.\n");
			else
				perror("sem_timedwait unexpected error.\n");
		}
	}

}

void xarp_add(FILE* fp, node_t** head, unsigned char* request) {
	unsigned int ip_address = (request[4] << 24) | (request[3] << 16) | (request[2] << 8) | (request[1]);
	unsigned char eth_address[6];
	memcpy(eth_address, request+1+4, 6); // 1B for opcode, 4B for ip address, 6B for eth_address
	int ttl = (request[14] << 24) | (request[13] << 16) | (request[12] << 8) | (request[11]);
	node_t* found_node = find_node_by_ip_address(*head, ip_address);

	if(found_node == NULL){
		fprintf(fp, "Node not found, adding new node\n");
		add_node(head, ip_address, eth_address, ttl);
	} else {
		fprintf(fp, "Node found, modifying node\n");
		found_node->ip_address = ip_address;
		memcpy(found_node->eth_address, eth_address, 6);
		found_node->ttl = (ttl == -1 ? global_ttl : ttl);
	}
	fprintf(fp, "Successfull add.\n");
}

void xarp_del(FILE* fp, node_t** head, unsigned char* request) {
	unsigned int ip_address = (request[4] << 24) | (request[3] << 16) | (request[2] << 8) | (request[1]);
	if(delete_node_by_ip_address(head, ip_address) == 1)
		fprintf(fp, "Node deleted succesfully.\n");
	else
		fprintf(fp, "Couldn't delete node.\n");
}

void xarp_ttl(FILE* fp, unsigned char* request) {
	int ttl = (request[4] << 24) | (request[3] << 16) | (request[2] << 8) | (request[1]);
	global_ttl = ttl;
	fprintf(fp, "New default TTL is: %d\n", global_ttl);
}

void xifconfig_info(FILE* fp, int sockfd, unsigned int qt_ifaces) {
	unsigned int i;
	for(i = 0; i < qt_ifaces; i++){
		print_iface_info(sockfd, fp, i);
	}
}

void xifconfig_mtu(unsigned char* request) {
	char* ifname = request+1;
	update_mtu(ifname);
}

void daemon_handle_request(unsigned char* request, int sockfd, node_t** head, unsigned int qt_ifaces){
	int opcode = request[0] - '0';
	FILE * fp = fdopen(sockfd, "w");

	switch(opcode){

		case XARP_SHOW: //DONE
			xarp_show(fp, head);
			break;

		case XARP_RES:
			xarp_res(fp, request);
			break;

		case XARP_ADD: //DONE
			xarp_add(fp, head, request);
			break;

		case XARP_DEL: //DONE
			xarp_del(fp, head, request);
			break;

		case XARP_TTL: //DONE
			xarp_ttl(fp, request);
			break;

		case XIFCONFIG_INFO: // DONE
			xifconfig_info(fp, sockfd, qt_ifaces);
			break;

		case XIFCONFIG_IP: // Unnecessary?
			break;

		case XIFCONFIG_MTU: //DONE
			xifconfig_mtu(request);
			break;

		default:
			fprintf(fp, "Daemon couldn't recognize this request.\n");
	}
	fclose(fp);
}

int main(int argc, char** argv) {
  int i, sockfd;

  if (argc < 2)
		print_usage();

    for(i = 1; i < argc; i++) {
        sockfd = create_socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
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
		// print_iface_info(sockfd,  i); // passar o file pointer se quiser usar
	}

	node_t* head = NULL;
	g_head = head;

	// aux program listener
	int connfd;
	unsigned char buffer[BUFFSIZE];
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	int listen_sockfd = create_socket(AF_INET, SOCK_STREAM, 0);

	load_socket_info(&serv_addr, LOOPBACK_IP, PORT);
	my_bind(listen_sockfd, (struct sockaddr*) &serv_addr);
	my_listen(listen_sockfd, LISTEN_ENQ);

	while(1) {
		connfd = my_accept(listen_sockfd, (struct sockaddr*) &cli_addr);
		my_recv(connfd, buffer, sizeof(buffer));
		daemon_handle_request(buffer, connfd, &head, argc-1);
		g_head = head;
	}

	for(i = 0; i < argc-1; i++){
		pthread_join(tid[i], NULL);
	}

	return 0;
}
