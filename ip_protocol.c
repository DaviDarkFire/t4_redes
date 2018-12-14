#include "ip_protocol.h"

int check_ip_version(struct ip_hdr* ip_header) {
  if(ip_header->ip_v == 4) return 0; // continue
  else return -1; // discard
}

void decrement_datagram_ttl(struct ip_hdr* ip_header) {
  if (ip_header->ttl = 0) {
    // TODO: send ICMP time exceeded
    return -1; // discard
  }
  ip_header->ttl--;
  return 0;
}
