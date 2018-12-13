#ifndef __XIFCONFIG__
  #define __XIFCONFIG__
  #include <stdio.h>
  #include <sys/ioctl.h>
  #include <arpa/inet.h>
  #include <net/if.h>
  #include <string.h>
  #include <stdlib.h>
  #include <stddef.h>             /* offsetof */
  #include <linux/sockios.h>
  #include <netinet/in.h>
  #if __GLIBC__ >=2 && __GLIBC_MINOR >= 1
  #include <netpacket/packet.h>
  #include <net/ethernet.h>
  #else
  #include <asm/types.h>
  #include <linux/if_ether.h>
  #endif
  #include "../misc.h"

  #define ifreq_offsetof(x)  offsetof(struct ifreq, x)

  #define DEFAULT_MODE 0
  #define CONFIG_IP_MODE 1
  #define SET_MTU_MODE 2
  #define UP_OR_DOWN_MODE 3

  #define XIFCONFIG_INFO 5
  #define XIFCONFIG_IP 6
  #define XIFCONFIG_MTU 7

  #define MAX_IFNAME_LEN 6
  #define BUFFSIZE 1024

  unsigned int decide_mode(int argc, char** argv);
  void set_mtu_mode(char* iface, char* new_mtu);
  void configure_ip_mode(char* iface, char* ip_addr, char* netmask);
  unsigned char* build_xifconfig_info_message();
  unsigned char* build_xifconfig_ip_message(char** args);
  unsigned char* build_xifconfig_mtu_message(char** args);
  unsigned char* build_xifconfig_up_or_down_message(char** args);
#endif
