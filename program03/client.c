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
   
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  struct tcp_hdr tcp_seg;
  unsigned short int cksum_arr[12];
  unsigned int i,sum=0, cksum;
  
  //the TCP segment
  tcp_seg.src = 49200;
  tcp_seg.des = portno;
  tcp_seg.seq = 7;
  tcp_seg.ack = 0;
  tcp_seg.hdr_flags = 0x6002;
  tcp_seg.rec = 0;
  tcp_seg.cksum = 0;
  tcp_seg.ptr = 0;
  tcp_seg.opt = 0;
  
  tcp_seg.cksum = checksum(cksum_arr);   
  
  memcpy(cksum_arr, &tcp_seg, 24);
  
  printf("The initial TCP segment to send looks like:\n");
  printf("SRC Port:%d\n", cksum_arr[1]);
  printf("DES Port:%d\n", cksum_arr[0]);
  printf("SEQ  NUM:%d\n", cksum_arr[2]);
  printf("ACK  NUM:%d\n", cksum_arr[3]);
  printf("HDR FLAG:%d\n", cksum_arr[4]);
  printf("REC  NUM:%d\n", cksum_arr[5]);
  printf("CKSUMNUM:%d\n", cksum_arr[6]);
  printf("PTR  NUM:%d\n", cksum_arr[7]);
  printf("OPT  NUM:%d\n\n\n", cksum_arr[8]);

//~~~~~SEND MESSAGE TO THE SERVER~~~~~~~~~//   

   n = send(sockfd, cksum_arr, 192,0);
   
   if (n < 0) {
      printf("ERROR while writing to the socket\n");
      exit(1);
   }
//~~~~~~READING THE SERVER RESPONSE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   
   n = recv(sockfd,cksum_arr,16,0);
   memcpy(cksum_arr, &cksum_arr, 24);
   
//~~~~~~GENERATING TCP RESPONSE MESSAGE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   
	tcp_seg.src = portno;
	tcp_seg.des = cksum_arr[0];
	tcp_seg.seq = 2;
	tcp_seg.ack = cksum_arr[2]+1;
	tcp_seg.hdr_flags = 0x6012;
	tcp_seg.rec = 0;
	tcp_seg.cksum = checksum(cksum_arr);
	tcp_seg.ptr = 0;
	tcp_seg.opt = 0;  

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//	
	
	memcpy(cksum_arr, &tcp_seg, 24);
	n = send(sockfd, cksum_arr, 192,0);
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//   
   
   if (n < 0) {
      printf("ERROR while reading from the socket\n");
      exit(1);
   }
	
   //printf("%s\n",buffer);
	printf("SRC Port:%d\n", cksum_arr[1]);
	printf("DES Port:%d\n", cksum_arr[0]);
	printf("SEQ  NUM:%d\n", cksum_arr[2]);
	printf("ACK  NUM:%d\n", cksum_arr[3]);
	printf("HDR FLAG:%d\n", cksum_arr[4]);
	printf("REC  NUM:%d\n", cksum_arr[5]);
	printf("CKSUMNUM:%d\n", cksum_arr[6]);
	printf("PTR  NUM:%d\n", cksum_arr[7]);
	printf("OPT  NUM:%d\n", cksum_arr[8]);
   
   
   return 0;
}

