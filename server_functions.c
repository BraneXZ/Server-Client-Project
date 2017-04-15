/* Group 8: Amorah, Cowley, Kaveti, Mathews, Shiaohongtu
 * IMPORTANT!
 *
 * Tags "TUTORIAL_BEGIN" and "TUTORIAL_END" are used to define regions of code that were either
 * copied exactly, or adapted from the tutorial at http://www.linuxhowtos.org/C_C++/socket.htm
 * All other code was written from scratch.
 */

#ifndef SERVER_FUNCTIONS_C
#define SERVER_FUNCTIONS_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

/* TUTORIAL_BEGIN */
void error(const char *message) {
  perror(message);
  exit(1);
}
/* TUTORIAL_END */

extern int tcpSockFD;
extern int udpSockFD;

void interruptHandler(int signalNo) {
  close(tcpSockFD);
  close(udpSockFD);
  exit(0);
}

void processTCPRequest(int socketFD) {
  if (socketFD < 0) {
    return;
  }

  const size_t BUFFER_SIZE = 256;
  /* calloc() sets the memory to zero, so no need to call bzero here. */
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));

  int n = (int)read(socketFD, buffer, BUFFER_SIZE - 1);
  if (n < 0) {
    error("Error reading from socket");
  }

  /* TUTORIAL_BEGIN */
  printf("Here is the message from TCP socket: %s\n", buffer);
  /* TUTORIAL_END */

  const char *confirmationMessage = "I got your message";
  n = (int)write(socketFD, confirmationMessage, strlen(confirmationMessage));

  if (n < 0) {
    error("Error writing to socket");
  }

  free(buffer);  /* Free buffer to prevent a memory leak. */
}

void processUDPRequest(int socketFD, struct sockaddr *address) {
  if (socketFD < 0) {
    return;
  }

  const size_t BUFFER_SIZE = 256;
  /* calloc() sets the memory to zero, so no need to call bzero here. */
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));

  socklen_t addressLength = sizeof(*address);
  int numBytes = (int)recvfrom(socketFD, buffer, BUFFER_SIZE - 1, 0, address, &addressLength);

  if (numBytes < 0) {
    error("Error in recvfrom");
  } else if (numBytes == 0) {
    printf("No data received from UDP socket");
  } else {
    printf("Here is the message from UDP socket: %s\n", buffer);
  }

  const char *confirmationMessage = "I got your message";
  strcpy(buffer, confirmationMessage);

  numBytes = (int)sendto(socketFD, buffer, strlen(buffer), 0, address, addressLength);
  if (numBytes < 0) {
    error("Error in sendto");
  }

  free(buffer);  /* Free buffer to prevent a memory leak. */
}

#endif
