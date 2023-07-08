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

#define PORT "1030"
#define PORT2 "1031"
#define MAX_LENGTH 1000

struct cache
{
  char name[20];
  char ip[20];
};

int main(int argc, char *argv[])
{ struct cache arr[2]= 
{
    "google.com",         "127.0.0.20",
    "youtube.com",        "127.0.0.21"
};
int count = 1;



int status;
struct addrinfo hints, *res, *p;
int loc_serversocket;
struct sockaddr_storage their_addr;
socklen_t addr_size;

memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_UNSPEC; 
hints.ai_socktype = SOCK_DGRAM;

if ((status = getaddrinfo("127.0.0.2", PORT, &hints, &res)) != 0)
{
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
  exit(1);
}

for(p = res; p != NULL; p = p->ai_next) 
{
  if ((loc_serversocket = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
  {
    perror("locserver: socket");
    continue;
  }

  if (bind(loc_serversocket, p->ai_addr, p->ai_addrlen) == -1) 
  {
    close(loc_serversocket);
    perror("locserver: bind");
    continue;
  }
  break;
}

freeaddrinfo(res);
if (p == NULL) 
{
  fprintf(stderr, "locserver: failed to bind\n");
  exit(1);
}

//2nd socket to act as client for root server
int status2;
struct addrinfo hints2, *res2;
int loc_serversocket2;

memset(&hints2, 0, sizeof(hints2));
hints2.ai_family = AF_UNSPEC;
hints2.ai_socktype = SOCK_DGRAM;

if ((status2 = getaddrinfo("127.0.0.3", PORT2, &hints2, &res2)) != 0)
{
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status2));
  exit(1);
}

for(p = res2; p != NULL; p = p->ai_next) 
{
  if ((loc_serversocket2 = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
  {
    perror("locserver: socket");
    continue;
  }
  break;
}

freeaddrinfo(res2);
if (p == NULL) 
{
  fprintf(stderr, "locserver: failed to bind\n");
  exit(1);
}

while(1)
{
char buf[100];
int flag = 0;
int numbytes;
addr_size = sizeof(their_addr);

  memset(buf,0,sizeof(buf));
if ((numbytes = recvfrom(loc_serversocket, &buf, MAX_LENGTH, 0,(struct sockaddr *)&their_addr, &addr_size)) == -1) 
{
  perror("error in receiving from client");
  exit(1);
}

buf[numbytes] = '\0'; 

printf("received from client: '%s'\n",buf);
count++;
for(int i = 0; i < 5; i++)
{
  if(strcmp(buf,arr[i].name) == 0)
  {
    flag = 1;
    printf("'%s'found in cache \n",buf);
    if((numbytes = sendto(loc_serversocket,arr[i].ip,sizeof(arr[i].ip),0,(struct sockaddr *)&their_addr, addr_size)) == -1)
    {
      perror("error in sending to root");
      exit(1);
    }
    
  }
}
if(count%2 == 0)
{
  strcpy(arr[1].name,buf);    //updating cache
}
else 
{
  strcpy(arr[0].name,buf); 
}
if(flag == 0)
{
if((numbytes = sendto(loc_serversocket2,buf,strlen(buf),0,p->ai_addr, p->ai_addrlen)) == -1)
{
  perror("error in sending to root");
  exit(1);
}

memset(&buf,0,sizeof(buf));

if ((numbytes = recvfrom(loc_serversocket2, &buf, MAX_LENGTH, 0,p->ai_addr, &p->ai_addrlen)) == -1) 
{
  perror("error in receiving from root");
  exit(1);
}
buf[numbytes] = '\0'; 
if(count % 2 == 0)
{
  strcpy(arr[1].ip,buf);  //updating cache
}
else
{
  strcpy(arr[0].ip,buf);
}
if((numbytes = sendto(loc_serversocket,buf,strlen(buf),0,(struct sockaddr *)&their_addr, addr_size)) == -1)
{
  perror("error in sending to client");
  exit(1);
}
}
}
close(loc_serversocket2);
close(loc_serversocket);

return 0;
}