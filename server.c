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
   
   char file[80];
   
   /* Creating a socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      printf("ERROR while opening the socket");
      exit(1);
   }
   
   /* Initializing the socket structure */

   portno = 5050;
   
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
   
   printf("Current buffer %s\n", buffer);
   
   if(newsockfd > 0){
           printf("The Client is connected...\n");         
   }
   
/********************************************************************************/

  strncpy(parser, buffer,32);
  printf("parserarray: \n%s\n\n",parser);
  int i;

  for(i=0;i<32;i++){
    if(parser[i]=='G' && parser[i+1]=='E' && parser[i+2]=='T'){
      printf("\n\nGET request\n");
      char isget[3] = {'G','E','T'};
      if (isget != 0){
        int j;
                char website[16];
        for(j=0; parser[j+4] != ' '; j++){
          website[j] = parser[j+5];
                  file[j] = parser[j+5];
        }
      printf("This is the website: %s \n\n",website);
      }
    }
  }
  
/********************************************************************************/
   
        FILE *in_file,
                 *out_file;
        char ch;

        in_file = fopen("index.html", "r");
        out_file = fopen("home.html", "w");

        for(ch = getc(in_file); ch != EOF; ch = getc(in_file)){
                putc(ch, out_file);
        }

/*********************************************************************************/

   
      //recv(newsockfd, buffer, strlen(buffer), 0);    
      printf("The buffer has .... %s\n", buffer);    
      send(newsockfd, "HTTP/1.1 200 OK\n", 16, 0);
      send(newsockfd, "Content-length: 46\n", 19, 0);
      send(newsockfd, "Content-Type: text/html\n\n", 25, 0);
      send(newsockfd, "<html><body><H1>Hello world</H1></body></html>", 46, 0);
      //send(newsockfd, out_file, 1024, 0);
          //send(newsockfd, fpw, strlen(fpw), 0);
          //send(newsockfd, fpw, strlen(fpw), 0);
      close(newsockfd);    
          
   close(sockfd);  
        fclose(in_file);
        fclose(out_file);   

   return 0;
}
