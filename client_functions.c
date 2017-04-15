/* Group 8: Amorah, Cowley, Kaveti, Mathews, Shiaohongtu
 * IMPORTANT!
 *
 * Tags "TUTORIAL_BEGIN" and "TUTORIAL_END" are used to define regions of code that were either
 * copied exactly, or adapted from the tutorial at http://www.linuxhowtos.org/C_C++/socket.htm
 * All other code was written from scratch.
 */

#ifndef CLIENT_FUNCTIONS_C
#define CLIENT_FUNCTIONS_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

/* TUTORIAL_BEGIN */
void error(const char *msg) {
  perror(msg);
  exit(0);
}
/* TUTORIAL_END */

void processTCPConnection(int socketFD, struct sockaddr *serverAddress,
                          char *buffer, int bufferSize) {
  if (connect(socketFD, (struct sockaddr *)serverAddress, sizeof(*serverAddress)) < 0) {
    error("Client count not connect to server");
  }

  int numBytes = (int)write(socketFD, buffer, strlen(buffer));
  if (numBytes < 0) {
    error("Client could not write to TCP socket");
  }

  bzero(buffer, bufferSize);
  numBytes = (int)read(socketFD, buffer, bufferSize - 1);
  if (numBytes < 0) {
    error("Client could not read from TCP socket");
  }

  /* Print server's response */
  printf("%s\n", buffer);

  close(socketFD);
  free(buffer);  /* Free buffer to prevent a memory leak. */
}

void processUDPConnection(int socketFD, struct sockaddr *serverAddress,
                          char *buffer, int bufferSize) {
  socklen_t serverAddressLength = sizeof(*serverAddress);
  int numBytes = sendto(socketFD, buffer, strlen(buffer), 0, serverAddress, serverAddressLength);
  if (numBytes < 0) {
    error("Client could not write to UDP socket");
  }

  numBytes = recvfrom(socketFD, buffer, bufferSize, 0, serverAddress, &serverAddressLength);
  if (numBytes < 0) {
    error("Client could not read from UDP socket");
  }

  /* Print server's response */
  printf("%s\n", buffer);

  free(buffer);  /* Free buffer to prevent a memory leak. */
}

#endif
