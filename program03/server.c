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
   int  n;
   
   struct tcp_hdr tcp_seg;


   /* Creating a socket */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      printf("ERROR while opening the socket\n");
      exit(1);
   }
 
   
   /* Initializing the socket structure */
   portno = 49155;
   
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
   bzero(buffer,256);	
    n = read(newsockfd,buffer,16);
  //n = read(newsockfd,tcp_seg,192);
   tcp_seg.src = buffer;
   
   if (n < 0) {
      printf("ERROR reading from socket\n");
      exit(1);
   }
  
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//  
 	/*declarations for tcp segment/
	struct tcp_hdr tcp_seg;
	unsigned int i,sum=0, cksum;
	//memcpy(cksum_arr, newsockfd,24);
	
	/*populating tcp segment/
	tcp_seg.src = portno;
	tcp_seg.des = 2200;
	tcp_seg.seq = 1;
	tcp_seg.ack = 2;
	tcp_seg.hdr_flags = 0x2333;
	tcp_seg.rec = 0;
	tcp_seg.cksum = 0;
	tcp_seg.ptr = 0;
	tcp_seg.opt = 0;  
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/*/ 

   
   
  printf("0x%04X\n", tcp_seg.src); // Printing all values
  printf("0x%04X\n", tcp_seg.des);
  printf("0x%08X\n", tcp_seg.seq);
  printf("0x%08X\n", tcp_seg.ack);
  printf("0x%04X\n", tcp_seg.hdr_flags);
  printf("0x%04X\n", tcp_seg.rec);
  printf("0x%04X\n", tcp_seg.cksum);
  printf("0x%04X\n", tcp_seg.ptr);
  printf("0x%08X\n", tcp_seg.opt);
   //printf("Relayed  message: %s\n",buffer);

   
   /* Write a response to the client */
   n = write(newsockfd,"Message received\n",18);
   
   if (n < 0) {
      printf("ERROR writing to socket\n");
      exit(1);
   }
      
   return 0;
}
