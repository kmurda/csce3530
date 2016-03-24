#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   char buffer[256];
   
   if (argc < 2) {
      printf("Usage %s Port_Number \n", argv[0]);
      exit(0);
   }
	
   portno = atoi(argv[1]);
   
   /* Create a socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      printf("ERROR creating a socket");
      exit(1);
   }
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr=inet_addr("129.120.151.94"); 
   serv_addr.sin_port = htons(portno);
   
   /* Connecting to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      printf("ERROR while attempting to  connect");
      exit(1);
   }
   
   /* Request for a message from the user; this will be relayed to the server
   */
	
   printf("Please enter the message: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);
   
   /* Send message to the server */
   n = write(sockfd, buffer, strlen(buffer));
   
   if (n < 0) {
      printf("ERROR while writing to the socket");
      exit(1);
   }
   
   /* Reading the server response */
   n = read(sockfd, buffer, 255);
   
   if (n < 0) {
      printf("ERROR while reading from the socket");
      exit(1);
   }
	
   printf("%s\n",buffer);
   return 0;
}
