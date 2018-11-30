void print_usage() {
	printf("usage: ipd <interface> [<interfaces>]\n");
	exit(1);
}

int create_socket() {
  sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
		if(sockfd < 0) {
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			exit(1);
		}
}

int main(int argc, char** argv) {
  int i, sockfd;

  if (argc < 2)
		print_usage();

    for(i = 1; i < argc; i++) {
        sockfd = create_socket();
        if(setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface_name, strlen(argv[i]) < 0) {
          perror("Server-setsockopt() error for SO_BINDTODEVICE");
			    printf("%s\n", strerror(errno));
			    close(sockfd);
			    exit(1);
        }
        // get_iface_info
    }

}
