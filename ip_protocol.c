#include "ip_protocol.h"


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