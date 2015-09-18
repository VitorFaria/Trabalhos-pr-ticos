#define SRV_IP "150.164.32.214"//"192.168.0.13"//"127.0.0.1"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define BUFLEN 100 //Tamanho em bytes da mensagem
#define NPACK 100000 //Número de repetições (pacotes enviados)
#define PORT 9930
/*
Cliente
*/

void diep(char *s)
{
    perror(s);
    exit(1);
}

long long current_timestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    //printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}


int main(void)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];

    strcpy (buf,"0");

    for(i = 0; i < (BUFLEN-1); i++)
    {
        strcat(buf,"0");
    }

    long long latenciaFinal = 0;

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        diep("socket");

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    if (inet_aton(SRV_IP, &si_other.sin_addr)==0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    for (i=0; i<NPACK; i++)
    {
        printf("Sending packet %d\n", i);
        //sprintf(buf, "This is packet %d\n", i);

        long long tempoUm = current_timestamp();

        if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
            diep("sendto()");
        //Mods
        if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1)
            diep("recvfrom()");

        long long tempoDois = current_timestamp();
        long long diferenca = tempoDois - tempoUm;
        latenciaFinal += diferenca;
        printf("Latencia Parcial: %lld ms\n", diferenca);

        //printf("Received packet from %s:%d\nData: %s\n",
        //       inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
        printf("\n\n");
        //Modsends
    }

    close(s);

    double latMedia = (double)latenciaFinal/(double)NPACK;
    double thr = 0.0;
    if(latMedia != 0) thr = (double)BUFLEN*8.0/latMedia;

    printf("Latencia Total: %lld ms\n", latenciaFinal);
    printf("Latencia Media: %f ms\n", latMedia);
    printf("Throughput: %f bps\n", thr);
    printf("Bytes: %d \n", BUFLEN);

    return 0;
}
