#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

#include "segment.h"


int checksum(unsigned short int cksum_arr[]){
    
  unsigned int i,sum=0, cksum;
  
  for (i=0;i<12;i++)               // Compute sum
  sum = sum + cksum_arr[i];

  cksum = sum >> 16;              // Fold once
  sum = sum & 0x0000FFFF; 
  sum = cksum + sum;

  cksum = sum >> 16;             // Fold once more
  sum = sum & 0x0000FFFF;
  cksum = cksum + sum;
 
 return cksum;
}

int main( int argc, char *argv[] ) {
  
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n, i;
   short int tcp_src;
   unsigned short int cksum_arr[12];
   
   struct tcp_hdr tcp_seg;


   /* Creating a socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      printf("ERROR while opening the socket\n");
      exit(1);
   }
 
   
   /* Initializing the socket structure */
   portno = 49157;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
  
  
   /* Binding the socket to the port*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      printf("ERROR while binding\n");
      exit(1);
   }

      
   /* Begin listening for incoming connections*/
   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   
   /* Accept connection request from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
   if (newsockfd < 0) {
      printf("ERROR on accept\n");
      exit(1);
   }

   
   /* Begin communication */
   //bzero(buffer,256);	
   n = recv(newsockfd,cksum_arr,16,0);
  //n = read(newsockfd,tcp_src,192);	
   
     for(i = 0; i < 9; i++){
	  printf("DEBUGTAG::::: %d\n", cksum_arr[i]);
  }
   //tcp_seg.des = (short int)buffer;
   
   if (n < 0) {
      printf("ERROR reading from socket\n");
      exit(1);
   }
  
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//  
 	/*declarations for tcp segment*/
	
	//populating tcp segment
	
	tcp_seg.src = portno;
	tcp_seg.des = cksum_arr[0];
	tcp_seg.seq = 2;
	tcp_seg.ack = cksum_arr[2]+1;
	tcp_seg.hdr_flags = 0x2333;
	tcp_seg.rec = 0;
	tcp_seg.cksum = checksum(cksum_arr);
	tcp_seg.ptr = 0;
	tcp_seg.opt = 0;  
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/ 

   
  /* Printing all values */
	printf("SRC Port:%d\n", cksum_arr[0]); // Printing all values
	printf("DES Port:%d\n", cksum_arr[1]);
	printf("SEQ  NUM:%d\n", cksum_arr[2]);
	printf("ACK  NUM:%d\n", cksum_arr[3]);
	printf("HDR FLAG:%d\n", cksum_arr[4]);
	printf("REC  NUM:%d\n", cksum_arr[5]);
	printf("CKSUMNUM:%d\n", cksum_arr[6]);
	printf("PTR  NUM:%d\n", cksum_arr[7]);
	printf("OPT  NUM:%d\n", cksum_arr[8]);
  //printf("Relayed  message: %s\n",buffer);

   
   
   memcpy(cksum_arr, &tcp_seg, 24);
   
   /* Write a response to the client */
   //n = write(newsockfd,"Message received\n",18);
   n = send(newsockfd, cksum_arr, 192,0);
   
   if (n < 0) {
      printf("ERROR writing to socket\n");
      exit(1);
   }
      
   return 0;
}
