#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define MSGLEN 150
#define NPACK 10
#define PORT 9988
#define NUM_GLOBES 50
//#define SRV_IP "127.0.0.1"


void diep(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char** argv)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char* ip_addr;
    uint8_t r, g, b;
    int j, k;

    // Read in the IP address of the target
    if (argc < 5) {
        printf("Usage: sender ip-address-of-receiver rr gg bb\n");
        exit(1);
    } else {
        ip_addr = argv[1];
        r = atoi(argv[2]);
        g = atoi(argv[3]);
        b = atoi(argv[4]);
    }
    printf("Red %d Green %d Blue %d\n", r, g, b);

    // Fill up the first 150 bytes of buf with the colours
    for (j=0, k=0; j < NUM_GLOBES; j++) {
        buf[k++] = r;
        buf[k++] = g;
        buf[k++] = b;
    }

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
      diep("socket");

    memset((void *) &si_other, 0, (size_t) sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    if (inet_aton(ip_addr, &si_other.sin_addr)==0) {
      fprintf(stderr, "inet_aton() failed\n");
      exit(1);
    }

    for (i=0; i<NPACK; i++) {
      printf("Sending packet %d\n", i);
      //sprintf(buf, "This is packet %d\n", i);
      if (sendto(s, buf, MSGLEN, 0, (struct sockaddr*) &si_other, slen)==-1)
        diep("sendto()");
    }

    close(s);
    return 0;
}