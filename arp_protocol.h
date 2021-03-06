#ifndef __ARP_PROTOCOL__
  #define __ARP_PROTOCOL__
  // Define a struct for ARP header
  typedef struct _arp_hdr arp_hdr;
  struct _arp_hdr {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t opcode;
    uint8_t sender_mac[6];
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
    uint8_t target_ip[4];
  };

  // Define some constants.
  #define ETH_HDRLEN 14      // Ethernet header length
  #define IP4_HDRLEN 20      // IPv4 header length
  #define ARP_HDRLEN 28      // ARP header length
  #define ARPOP_REQUEST 1    // Taken from <linux/if_arp.h>

  void send_arp_request(char* ifname, char* requested_ip);
#endif
