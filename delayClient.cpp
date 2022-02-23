/***
 * Created by Kaylor on 22/2/23.
 */
// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "sys/time.h"

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in	 servaddr;

  // Creating socket file descriptor
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 500;
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof (timeout)) == -1){
    perror("setsockopt failed");
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  unsigned n, len;
  struct timeval first_time, last_time;
  int max_gap=0, min_gap=0, avg_gap=0;
  int count = 10000;
  char data[4096];
  for (int i = 0; i < count; ++i) {
    gettimeofday(&first_time, NULL);
    sendto(sockfd, (const char *)&data[0], sizeof (data),
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *) &servaddr,
                 &len);
    gettimeofday(&last_time, NULL);
    int tmp = last_time.tv_sec*1000000+last_time.tv_usec - first_time.tv_sec*1000000-first_time.tv_usec;
    if (i == 0 ){
      min_gap = tmp;
      max_gap = tmp;
    }
    if (tmp < min_gap) min_gap = tmp;
    if (tmp > max_gap) max_gap = tmp;
    avg_gap += tmp;
    printf("********************%d*****************\n",i);
    printf("send sec = %ld, usec =%ld\n", first_time.tv_sec, first_time.tv_usec);
    printf("receive sec = %ld, usec =%ld\n", last_time.tv_sec, last_time.tv_usec);
    printf("delta time = %ldus\n", last_time.tv_sec*1000000+last_time.tv_usec - first_time.tv_sec*1000000-first_time.tv_usec);
    printf("********************end*****************\n",i);
  }
  avg_gap /= count;
  printf("max = %dus, min =%dus, average = %dus\n", max_gap, min_gap, avg_gap);

  close(sockfd);
  return 0;
}

