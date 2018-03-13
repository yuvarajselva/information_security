#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
  
#define PORT 4444  
#define BUF_SIZE 2000  
#define CLADDR_LEN 100  
  
void main() {  
 struct sockaddr_in addr, cl_addr;  
 int sockfd, len, ret, newsockfd;  
 char buffer[BUF_SIZE];  
 pid_t childpid;  
 char clientAddr[CLADDR_LEN];  
   
 sockfd = socket(AF_INET, SOCK_STREAM, 0);  
 if (sockfd < 0) {  
  printf("Error creating socket!\n");  
  exit(1);  
 }  
 printf("Socket created...\n");  
   
 memset(&addr, 0, sizeof(addr));  
 addr.sin_family = AF_INET;  
 addr.sin_addr.s_addr = INADDR_ANY;  
 addr.sin_port = PORT;  
   
 ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
 if (ret < 0) {  
  printf("Error binding!\n");  
  exit(1);  
 }  
 printf("Binding done...\n");  
  
 printf("Waiting for a connection...\n");  
 listen(sockfd, 5);  
  
 for (;;) { //infinite loop  
  len = sizeof(cl_addr);  
  newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);  
  if (newsockfd < 0) {  
   printf("Error accepting connection!\n");  
   exit(1);  
  }  
  printf("Connection accepted...\n");  
  
  inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);  
  if ((childpid = fork()) == 0) { //creating a child process  
  
   close(sockfd);   
//stop listening for new connections by the main process.   
//the child will continue to listen.   
//the main process now handles the connected client.  
  
   for (;;) {  
    memset(buffer, 0, BUF_SIZE);  
    ret = recvfrom(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, &len);  
    if(ret < 0) {  
     printf("Error receiving data!\n");    
     exit(1);  
    }  
    printf("Received data from %d: %s\n", getpid(), buffer);   
    memset(buffer, 0, BUF_SIZE);  
    fgets(buffer,BUF_SIZE,stdin);
    ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);     
    if (ret < 0) {    
     printf("Error sending data!\n");    
     exit(1);    
    }    
    printf("Sent data to %d: %s\n", getpid(), buffer);  
   }  
  }  
  close(newsockfd);  
 }  
}  
