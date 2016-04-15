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
      printf("ERROR creating a socket\n");
      exit(1);
   }
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr=inet_addr("129.120.151.94"); 
   serv_addr.sin_port = htons(portno);
   
   /* Connecting to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      printf("ERROR while attempting to connect\n");
      exit(1);
   }
   
   /* Request for a message from the user; this will be relayed to the server*/
	
   //printf("Please enter the message: ");
   //bzero(buffer,256);
   //fgets(buffer,255,stdin);
   
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  struct tcp_hdr tcp_seg;
  unsigned short int cksum_arr[12];
  unsigned int i,sum=0, cksum;

  tcp_seg.src = 49200;
  tcp_seg.des = portno;
  tcp_seg.seq = 1;
  tcp_seg.ack = 0;
  tcp_seg.hdr_flags = 0x6002;
  tcp_seg.rec = 0;
  tcp_seg.cksum = 0;
  tcp_seg.ptr = 0;
  tcp_seg.opt = 0;

   
  memcpy(cksum_arr, &tcp_seg, 24);
  tcp_seg.cksum = checksum(cksum_arr);
   
  memcpy(buffer, tcp_seg.src, 16);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//   
  
  
   /* Send message to the server */
   
   n = write(sockfd, buffer, strlen(buffer));
   //n = write(sockfd, tcp_seg, 192);
   
   if (n < 0) {
      printf("ERROR while writing to the socket\n");
      exit(1);
   }
   
   /* Reading the server response */
   n = read(sockfd, buffer, 255);
   //n = read(sockfd, cksum_arr, 24);
   
   //memcpy(tcp_seg, &cksum_arr, 24);
   
   if (n < 0) {
      printf("ERROR while reading from the socket\n");
      exit(1);
   }
	
   //printf("%s\n",buffer);
	printf("0x%04X\n", tcp_seg.src); // Printing all values
	printf("0x%04X\n", tcp_seg.des);
	printf("0x%08X\n", tcp_seg.seq);
	printf("0x%08X\n", tcp_seg.ack);
	printf("0x%04X\n", tcp_seg.hdr_flags);
	printf("0x%04X\n", tcp_seg.rec);
	printf("0x%04X\n", tcp_seg.cksum);
	printf("0x%04X\n", tcp_seg.ptr);
	printf("0x%08X\n", tcp_seg.opt);
   
   
   return 0;
}

