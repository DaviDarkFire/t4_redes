#include "misc.h"

unsigned char* get_ip_addr_bytes_from_string(char* dotted_dec_ip){
  int values[4];
  int i;
  unsigned char* ip_bytes;
  ip_bytes = malloc(sizeof(char)*4);

  sscanf(dotted_dec_ip, "%d.%d.%d.%d",
  &values[3], &values[2], &values[1], &values[0]);

  for(i = 0; i < 4; i++){
    ip_bytes[i] = (unsigned char) values[i];
  }
  return ip_bytes;
}

char* get_ip_address_as_dotted_dec(char* ifname){
	int fd;
 	struct ifreq ifr;
	char * ip_address = malloc(sizeof(char)*(16));
 	fd = socket(AF_INET, SOCK_DGRAM, 0);

 	ifr.ifr_addr.sa_family = AF_INET;
 	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
 	ioctl(fd, SIOCGIFADDR, &ifr);

 	close(fd);

	ip_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

 	return ip_address;
}

char* get_bcast_address_as_dotted_dec(char* ifname){
	int fd;
 	struct ifreq ifr;
	char * ip_address = malloc(sizeof(char)*(15));
 	fd = socket(AF_INET, SOCK_DGRAM, 0);

 	ifr.ifr_addr.sa_family = AF_INET;
 	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
 	ioctl(fd, SIOCGIFBRDADDR, &ifr);

 	close(fd);

	ip_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_broadaddr)->sin_addr);

 	return ip_address;
}

char* get_netmask_as_dotted_dec(char* ifname){
	int fd;
 	struct ifreq ifr;
	char * ip_address = malloc(sizeof(char)*(15));
 	fd = socket(AF_INET, SOCK_DGRAM, 0);

 	ifr.ifr_addr.sa_family = AF_INET;
 	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
 	ioctl(fd, SIOCGIFNETMASK, &ifr);

 	close(fd);

	ip_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr);

 	return ip_address;
}

unsigned char* get_mac_addr_bytes_from_string(char* colon_format_mac){
  unsigned char* mac_bytes;
  int i;
  unsigned int i_aux[6];
  mac_bytes = malloc(sizeof(unsigned char) * 6);
  sscanf(colon_format_mac, "%X:%X:%X:%X:%X:%X",
	&i_aux[0], &i_aux[1], &i_aux[2],
	&i_aux[3], &i_aux[4], &i_aux[5]);

  for(i = 0; i < 6; i++){
    mac_bytes[i] = (char) i_aux[i];
  }

  return mac_bytes;
}

unsigned char* get_ttl_bytes_from_string(char* str_ttl){
  unsigned char* ttl_bytes;
  int i_ttl;
  i_ttl = atoi(str_ttl);
  ttl_bytes = malloc(sizeof(unsigned char)*4);

  ttl_bytes[3] = i_ttl >> 24;
  ttl_bytes[2] = i_ttl >> 16;
  ttl_bytes[1] = i_ttl >> 8;
  ttl_bytes[0] = i_ttl >> 0;

  return ttl_bytes;
}

void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}
