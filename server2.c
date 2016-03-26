#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   char parser[32];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   
   /* Creating a socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      printf("ERROR while opening the socket");
      exit(1);
   }
   
   /* Initializing the socket structure */

   portno = 8888;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Binding the socket to the port*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      printf("ERROR while binding");
      exit(1);
   }
      
   /* Begin listening for incoming connections
   */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   /* Accept connection request from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
   if (newsockfd < 0) {
      printf("ERROR on accept");
      exit(1);
   }
   
   /* Begin communication */
   bzero(buffer,256);
   n = read( newsockfd,buffer,255 );
   
   if (n < 0) {
      printf("ERROR reading from socket");
      exit(1);
   }
   
   printf("\nRelayed  message:\n%s\n\n",buffer);
   


/*Check if there is a GET request, return the website URL requested*/
  
  strncpy(parser, buffer,32);
  //printf("parserarray: \n%s\n\n",parser);
 
  int i;
  for(i=0;i<32;i++){
    if(parser[i]=='G' && parser[i+1]=='E' && parser[i+2]=='T'){
      printf("\nGET request\n");
      char isget[3] = {'G','E','T'};
      if (isget != 0){
        int j;
        char website[16];
        for(j=0; parser[j+4] != ' '; j++){
          website[j] = parser[j+5];
        }
      printf("\n\nrequested file: %s \n\n",website);
      }
    }
  }
    
   
   
   
   /* Write a response to the client */
   n = write(newsockfd,"Message received",18);
   
   if (n < 0) {
      printf("ERROR writing to socket");
      exit(1);
   }
      
   return 0;
}
