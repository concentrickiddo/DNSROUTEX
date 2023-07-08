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

#define PORT "1033"
#define MAX_LENGTH 1000

struct auth
{
  const char* name;
  const char* ip;
};

int main()
{
  struct auth arr[10]= {
    "google.com",         "127.0.0.20",
    "youtube.com",        "127.0.0.21",
    "geeksforgeeks.com",  "127.0.0.22",
    "hello.com",          "127.0.0.23",
    "uday.com",           "127.0.0.24",
    "advaith.com",        "127.0.0.25",
    "rahul.com",          "127.0.0.26",
    "rohith.com",         "127.0.0.27",
    "gnana.com",          "127.0.0.28",
    "isaac.com",          "127.0.0.29"
  };

int status;
struct addrinfo hints, *res,*p;
int auth_serversocket;
struct sockaddr_storage their_addr;
socklen_t addr_size;

memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_DGRAM;

if ((status = getaddrinfo("127.0.0.11", PORT, &hints, &res)) != 0)
{
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
  exit(1);
}
for(p = res; p != NULL; p = p->ai_next) 
{
  if ((auth_serversocket = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
  {
    perror("authserver: socket");
    continue;
  }

  if (bind(auth_serversocket, p->ai_addr, p->ai_addrlen) == -1) 
  {
    close(auth_serversocket);
    perror("authserver: bind");
    continue;
  }
  break;
}

freeaddrinfo(res);
if (p == NULL) 
{
  fprintf(stderr, "authserver: failed to bind\n");
  exit(1);
}

char buf[100];
int numbytes;
addr_size = sizeof(their_addr);

while(1)
{
if ((numbytes = recvfrom(auth_serversocket, &buf, MAX_LENGTH, 0,(struct sockaddr *)&their_addr, &addr_size)) == -1) 
{
  perror("error in receiving from tld");
  exit(1);
}

buf[numbytes] = '\0'; 

printf("received from tld: '%s'\n",buf);

int flag = 0;

for(int i = 0; i < 10; i++)
{
  if(strcmp(buf,arr[i].name) == 0)
  {
    printf("%s",arr[i].ip);
    if((numbytes = sendto(auth_serversocket,arr[i].ip,strlen(arr[i].ip),0,(struct sockaddr *)&their_addr, addr_size)) == -1)
    {
      perror("error in sending to tld");
      exit(1);
    }
    flag = 1;
    break;
  }
}

char ret[20] = "ip not found!";
if(flag == 0)
{
  if((numbytes = sendto(auth_serversocket,ret,strlen(ret),0,(struct sockaddr *)&their_addr, addr_size)) == -1)
    {
      perror("error in sending to tld");
      exit(1);
    } 
}
}
close(auth_serversocket);

return 0;
}