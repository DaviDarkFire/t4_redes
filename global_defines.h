#ifndef __GLOBAL_DEFINES__
  #define __GLOBAL_DEFINES__
  #define BYTES_UNTIL_BODY 14
  #define MAX_IFACES	64
  #define MAX_PACKET_SIZE 65536
  #define MIN_PACKET_SIZE 20
  #define MAX_IFACES	64
  #define MAX_IFNAME_LEN	6
  #define ETH_ADDR_LEN	6
  #define IP_ADDR_LEN 4
  #define BUFFSIZE 1024
  #define MAX_IFNAME_LEN	6
  #define LISTEN_ENQ 5
  #define PORT 5050
  #define DEFAULT_IP "127.0.0.1"
  #define UP 1
  #define DOWN 0

  #define XARP_SHOW 0
  #define XARP_RES 1 // TODO: fazer opcode ser bytes e não
  #define XARP_ADD 2
  #define XARP_DEL 3
  #define XARP_TTL 4
  #define XIFCONFIG_INFO 5
  #define XIFCONFIG_IP 6
  #define XIFCONFIG_MTU 7
  #define XROUTE_SHOW 8
  #define XROUTE_ADD 9
  #define XROUTE_DEL 10 //
#endif
