#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 100 //Tamanho em bytes da mensagem
#define NPACK 100000 //Número de repetições (pacotes enviados)
#define PORT 59930

void diep(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    //int messageLen = BUFLEN -1
    struct sockaddr_in si_me, si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];

   // int latenciaTotal = 0;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        diep("socket");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, &si_me, sizeof(si_me))==-1)
        diep("bind");

    for (i=0; i<NPACK; i++)
    {
        if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1)
            diep("recvfrom()");
        //printf("Received packet from %s:%d\nData: %s",
        //       inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
        //Mods
        printf("Resending packet %d\n", i);
        //sprintf(buf, "This is packet %d\n", i);
        if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
            diep("sendto()");
        printf("\n\n");
        //Modsends
    }

    close(s);
    return 0;
}
