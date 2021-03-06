#ifndef __IP_PROTOCOL__
  #define __IP_PROTOCOL__
	#include "global_defines.h"
	#include "misc.h"
	#include "ipd.h"
	#include "arp_protocol.h"

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

  int check_ip_version(struct ip_hdr* ip_header);
  void decrement_datagram_ttl(struct ip_hdr* ip_header);
  int checksum(u_short *buf, int count);
  unsigned char* get_eth_adress_based_on_ip(unsigned int ip_address);
#endif
