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
#define MYPORT "1030"
#define MAX_LENGTH 1000

int main(int argc,char *argv[])
{
int client_socket;
struct addrinfo hints, *res,*p;
int status;
int numbytes;

memset(&hints, 0, sizeof (hints));
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_DGRAM;

if((status = getaddrinfo("127.0.0.2",MYPORT , &hints, &res)) != 0)
{
  fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
  exit(1);
}

for(p = res; p != NULL; p = p->ai_next) 
{
  if ((client_socket = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
  {
    perror("client: socket");
    continue;
  }
  break;
}

if (p == NULL) 
{
  fprintf(stderr, "client: failed to create socket\n");
  exit(1);
}
freeaddrinfo(res);

while(1)
{
char string[100];
memset(string,0,100);
scanf("%s",string);
//gets(string,100,stdin);

if((numbytes = sendto(client_socket,string,strlen(string),0,res->ai_addr, res->ai_addrlen)) == -1)
{
  perror("error in sending to locserver");
  exit(1);
}

char response[100];
if((numbytes = recvfrom(client_socket, &response, MAX_LENGTH, 0,res->ai_addr, &res->ai_addrlen)) == -1)
{
    perror("error in receiving from locserver");
  exit(1);
}

printf("%s\n",response);
}

/*int arr[10];
int n=10;
for(int i= 0;i< n;i++)
{
  scanf("%d",&arr[i]);
}

sendto(client_socket,arr,sizeof(arr),0,res->ai_addr, res->ai_addrlen);
int response[n];
recvfrom(client_socket, &response, MAX_LENGTH, 0,res->ai_addr, &res->ai_addrlen);

for(int i=0;i<n;i++)
{
printf("%d\n",response[i]);
}*/

close(client_socket);

return 0;
}