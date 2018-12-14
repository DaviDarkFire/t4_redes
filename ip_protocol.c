#include "ip_protocol.h"

int check_ip_version(struct ip_hdr* ip_header) {
  if(ip_header->ip_v == 4) return 0; // continue
  else return -1; // discard
}

int checksum(u_short *buf, int count){
	//lembrete dos mininino
	//no buf passa o header_ip apenas, após zerar o campo checksum
	//no campo count passa o ip_ihl multiplicada por 2
	while (count--){
		sum += *buf++;
		if(sum & 0xFFFF0000){
			//teve carry
			sum &= 0xFFFF;
			sum++;
		}
	}
	return ~(sum & 0xFFFF);
}

void decrement_datagram_ttl(struct ip_hdr* ip_header) {
  if (ip_header->ttl = 0) {
    // TODO: send ICMP time exceeded
    return -1; // discard
  }
  ip_header->ttl--;
  return 0;
}
