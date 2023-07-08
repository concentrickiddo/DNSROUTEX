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

#define PORT "1032"
#define PORT2 "1033"
#define MAX_LENGTH 1000

int main(int argc, char *argv[])
{
int status;
struct addrinfo hints, *res,*p;
int tld_serversocket;
struct sockaddr_storage their_addr;
socklen_t addr_size;

memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_DGRAM;

if ((status = getaddrinfo("127.0.0.10", PORT, &hints, &res)) != 0)
{
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
  exit(1);
}

for(p = res; p != NULL; p = p->ai_next) 
{
  if ((tld_serversocket = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
  {
    perror("tldserver: socket");
    continue;
  }

  if (bind(tld_serversocket, p->ai_addr, p->ai_addrlen) == -1) 
  {
    close(tld_serversocket);
    perror("tldserver: bind");
    continue;
  }
  break;
}


if (p == NULL) 
{
  fprintf(stderr, "tldserver: failed to create socket\n");
  exit(1);
}
freeaddrinfo(res);

char buf[100];
memset(buf,0,100);
int numbytes;
addr_size = sizeof(their_addr);

while(1)
{
  memset(buf,0,100);

if ((numbytes = recvfrom(tld_serversocket, &buf, MAX_LENGTH, 0,(struct sockaddr *)&their_addr, &addr_size)) == -1) 
{
  perror("error in receiving from root");
  exit(1);
}

buf[numbytes] = '\0'; 

printf("received from root: '%s'\n",buf);



//2nd socket to act as client for tld server
int status2;
struct addrinfo hints2, *res2;
int tld_serversocket2;

memset(&hints2, 0, sizeof(hints2));
hints2.ai_family = AF_UNSPEC;
hints2.ai_socktype = SOCK_DGRAM;


if(strcmp(buf,"google.com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else if (strcmp(buf,"youtube.com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else if (strcmp(buf,"geeksforgeeks.com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else if (strcmp(buf,"hello.com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else if (strcmp(buf,"uday.com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else if (strcmp(buf,"advaith.com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else if (strcmp(buf,"rahul.com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else if (strcmp(buf,"gnana.com")==0)
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}
else 
{
  if ((status2 = getaddrinfo("127.0.0.11", PORT2, &hints2, &res2)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
    exit(1);
  }
}


for(p = res2; p != NULL; p = p->ai_next) 
{
  if ((tld_serversocket2 = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
  {
    perror("tldserver: socket");
    continue;
  }

  break;
}


if (p == NULL) 
{
  fprintf(stderr, "tldserver: failed to bind\n");
  exit(1);
}
freeaddrinfo(res2);

if((numbytes = sendto(tld_serversocket2,buf,strlen(buf),0,p->ai_addr, p->ai_addrlen)) == -1)
{
  perror("error in sending to auth");
  exit(1);
}

memset(&buf,0,sizeof(buf));

if ((numbytes = recvfrom(tld_serversocket2, &buf, MAX_LENGTH, 0,p->ai_addr, &p->ai_addrlen)) == -1) 
{
  perror("error in receiving from auth");
  exit(1);
}
buf[numbytes] = '\0'; 

close(tld_serversocket2);


if((numbytes = sendto(tld_serversocket,buf,strlen(buf),0,(struct sockaddr *)&their_addr, addr_size)) == -1)
{
  perror("error in sending to root");
  exit(1);
}

}

close(tld_serversocket);

return 0;
}