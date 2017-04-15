/* Group 8: Amorah, Cowley, Kaveti, Mathews, Shiaohongtu
 * IMPORTANT!
 *
 * Tags "TUTORIAL_BEGIN" and "TUTORIAL_END" are used to define regions of code that were either
 * copied exactly, or adapted from the tutorial at http://www.linuxhowtos.org/C_C++/socket.htm
 * All other code was written from scratch.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* Prints an error message to stderr and exits. */
void error(const char *);

/* Tries to create a TCP connection to the server. If successful, it then sends a message,
 * and displays the server's response
 */
void processTCPConnection(int socketFD, struct sockaddr *serverAddress,
                          char *buffer, int bufferSize);

/* Tries to create a UDP connection to the server. If successful, it then sends a message,
 * and displays the server's response
 */
void processUDPConnection(int socketFD, struct sockaddr *serverAddress,
                          char *buffer, int bufferSize);

int main(int argc, char *argv[])
{
  /* TUTORIAL_BEGIN */
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  if (argc < 3)
  {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  portno = atoi(argv[2]);
  server = gethostbyname(argv[1]);
    
  if (server == NULL)
  {
    fprintf(stderr, "error, no such host\n");
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);
  /* TUTORIAL_END */

  char input[100];
  char socketType;
    
  while (1)
  {
    printf("Please choose socket type ('U' for UDP, 'T' for TCP): ");
    scanf("%s", input);
    socketType = input[0];

    if (socketType == 'u' || socketType == 'U' || socketType == 't' || socketType == 'T')
    {
      break;
    }
  }

  /* This is to clear any redundant characters (including newlines) in the standard input stream */
//  char c;
//  while ((c = getchar()) != '\n' && c != EOF)
//  {
//  }

  const size_t BUFFER_SIZE = 256;
  /* calloc() sets the memory to zero, so no need to call bzero here. */
  char *buffer = calloc(BUFFER_SIZE, sizeof(char));

  /* TUTORIAL_BEGIN */
  /* Read message from stdin */
  printf("Please enter the message: ");
  fgets(buffer, BUFFER_SIZE - 1, stdin);
  /* TUTORIAL_END */

  if (socketType == 'T' || socketType == 't')
  {
    /* We will communicate with server using TCP */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
      
    if (sockfd < 0)
    {
      error("error opening socket");
    }

    processTCPConnection(sockfd, (struct sockaddr *)&serv_addr, buffer, BUFFER_SIZE);
  }
  
  else
  {
    /* We will communicate with server using UDP */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      
    if (sockfd < 0)
    {
      error("error opening socket");
    }

    processUDPConnection(sockfd, (struct sockaddr *)&serv_addr, buffer, BUFFER_SIZE);
  }
  
  return 0;
}
