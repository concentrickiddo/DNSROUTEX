#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "1031"
#define PORT2 "1032"
#define MAX_LENGTH 1000

int main(int argc, char *argv[])
{
int status;
struct addrinfo hints, *res,*p;
int root_serversocket;
struct sockaddr_storage their_addr;
socklen_t addr_size;

memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_DGRAM;

if ((status = getaddrinfo("127.0.0.3", PORT, &hints, &res)) != 0)
{
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
  exit(1);
}

for(p = res; p != NULL; p = p->ai_next) 
{
  if ((root_serversocket = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
  {
    perror("rootserver: socket");
    continue;
  }

  if (bind(root_serversocket, p->ai_addr, p->ai_addrlen) == -1) 
  {
    close(root_serversocket);
    perror("rootserver: bind");
    continue;
  }
  break;
}

//
if (p == NULL) 
{
  fprintf(stderr, "rootserver: failed to bind\n");
  exit(1);
}
freeaddrinfo(res);

char buf[100];

int numbytes;
addr_size = sizeof(their_addr);

while(1)
{
  memset(buf,0,100);
if ((numbytes = recvfrom(root_serversocket, &buf, MAX_LENGTH, 0,(struct sockaddr *)&their_addr, &addr_size)) == -1) 
{
  perror("error in receiving from local");
  exit(1);
}

buf[numbytes] = '\0'; 

printf("received from local: '%s'\n",buf);



//2nd socket to act as client for tld server
int status2;
struct addrinfo hints2, *res2;
int root_serversocket2;

memset(&hints2, 0, sizeof(hints2));
hints2.ai_family = AF_UNSPEC;
hints2.ai_socktype = SOCK_DGRAM;

char S[5];
char temp;
int i,j;
//printf("numbytes :%d\n",numbytes);
for(i = numbytes-1,j = 0; i >=0,j < 5; i--,j++)
{
  S[j] = buf[i];
  if(buf[i] == '.')
  {
    S[j] = buf[i];
    S[j+1] = '\0';
    break;
  }
}
int n = strlen(S);
for(j = 0; j < n/2; j++)
{
  temp = S[j];
  S[j] = S[n-1-j];
  S[n-1-j] = temp; 
}

if(strcmp(S,".com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.10", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else if(strcmp(S,".net")==0)
{
  if ((status2 = getaddrinfo("127.0.0.10", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else //(strcmp(S,".in")==1)
{
 if ((status2 = getaddrinfo("127.0.0.7", "1033", &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  } 
}

for(p = res2; p != NULL; p = p->ai_next) 
{
  if ((root_serversocket2 = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
  {
    perror("rootserver: socket");
    continue;
  }
  break;
}

if (p == NULL) 
{
  fprintf(stderr, "rootserver2: failed to create socket\n");
  exit(1);
}
freeaddrinfo(res2);

if((numbytes = sendto(root_serversocket2,buf,strlen(buf),0,p->ai_addr, p->ai_addrlen)) == -1)
{
  perror("error in sending to tld");
  exit(1);
}

memset(&buf,0,sizeof(buf));

if ((numbytes = recvfrom(root_serversocket2, &buf, MAX_LENGTH, 0,p->ai_addr, &p->ai_addrlen)) == -1) 
{
  perror("error in receiving from tld");
  exit(1);
}
buf[numbytes] = '\0'; 
close(root_serversocket2);


if((numbytes = sendto(root_serversocket,buf,strlen(buf),0,(struct sockaddr *)&their_addr, addr_size)) == -1)
{
  perror("error in sending to local");
  exit(1);
}
}


close(root_serversocket);

return 0;
}