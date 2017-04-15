/* Group 8: Amorah, Cowley, Kaveti, Mathews, Shiaohongtu
 * IMPORTANT!
 *
 * Tags "TUTORIAL_BEGIN" and "TUTORIAL_END" are used to define regions of code that were either
 * copied exactly, or adapted from the tutorial at http://www.linuxhowtos.org/C_C++/socket.htm
 * All other code was written from scratch.
 */

#include "server.h"

/* TCP socket file descriptor for the server process. */
int tcpSockFD;

/* UDP socket file descriptor for the server process. */
int udpSockFD;

int main(int argc, char *argv[]) {
  /* TUTORIAL_BEGIN */
  int newtcpSockFD, portno;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
  fd_set fdSet;  /* File descriptor set to store file descriptors for TCP and UDP sockets */

  if (argc < 2) {
    fprintf(stderr, "Error, no port provided\n");
    exit(1);
  }

  tcpSockFD = socket(AF_INET, SOCK_STREAM, 0);
  if (tcpSockFD < 0) {
    error("Error opening TCP socket");
  }
  udpSockFD = socket(AF_INET, SOCK_DGRAM, 0);
  if (udpSockFD < 0) {
    error("Error opening UDP socket");
  }
  /* TUTORIAL_END */

  printf("Server is running...\n");

  /* Enable reusing of addresses to help avoid "Address already in use" errors. */
  const int enable = 1;
  setsockopt(tcpSockFD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
  setsockopt(udpSockFD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

  /* Install an interrupt handler to catch when user presses Ctrl+C (interrupt)
   to terminate the program. */
  if (signal(SIGINT, interruptHandler) == SIG_ERR) {
    fprintf(stderr, "Could not set interrupt handler in server process");
  }

  /* These two should fail, but we try anyway. */
  signal(SIGSTOP, interruptHandler);
  signal(SIGKILL, interruptHandler);

  /* TUTORIAL_BEGIN */
  /* Set the server address */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(tcpSockFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr,
            "Error on binding TCP socket. This usually happens when the server process is killed "
            "(e.g. Ctrl+Z). To fix this, please clean-up all remaining processes named \"server\".\n");
    error(NULL);
  }
  if (bind(udpSockFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr,
            "Error on binding UDP socket. This usually happens when the server process is killed "
            "(e.g. Ctrl+Z). To fix this, please clean-up all remaining processes named \"server\".\n");
    error(NULL);
  }
  /* TUTORIAL_END */

  //this loop will go on forever, attempting to create child processes when activated - edited by Nate Cowley
  while (1) 
  {
    // TUTORIAL_BEGIN
    listen(tcpSockFD, 5);
    listen(udpSockFD, 5);
    // TUTORIAL_END

    FD_ZERO(&fdSet);
    FD_SET(tcpSockFD, &fdSet);
    FD_SET(udpSockFD, &fdSet);
    int maxFD = (tcpSockFD > udpSockFD) ? tcpSockFD : udpSockFD;
    int numReady = select(maxFD + 1, &fdSet, NULL, NULL, NULL);
    
	if (numReady < 0) 
	{
      error("Error in select function");
    }

    if (FD_ISSET(tcpSockFD, &fdSet)) 
	{
      // Handle TCP connection

      // TUTORIAL_BEGIN
      clilen = sizeof(cli_addr);
      newtcpSockFD = accept(tcpSockFD, (struct sockaddr *)&cli_addr, &clilen);
     
	  if (newtcpSockFD < 0) 
	  {
        error("Error on accept");
      }
      // TUTORIAL_END

      pid_t pid = fork();
      if (pid == 0) 
	  {
        /* This is the child process. Read socket and print message from client here. 
		We don't need this (parent's) socket here, so it is safe to close it. */
        close(tcpSockFD);

        processTCPRequest(newtcpSockFD);
        // Child should exit immediately after sending the client confirmation. 
        return 0;
      }
	  else if (pid < 0) 
	  {
        // Something went wrong and the fork operation failed.
        error("Could not fork a child process");
      } 
	  else 
	  {
        /* This is the parent process. This process should keep listening for new connections. 
        We don't need this (child's) socket here, so it is safe to close it. */
        close(newtcpSockFD);
      }
    }

    if (FD_ISSET(udpSockFD, &fdSet)) 
	{
      // Handle UDP connection 
      pid_t pid = fork();

      if (pid == 0) {
        // This is the child process. Process request and print message from client here.

        processUDPRequest(udpSockFD, (struct sockaddr *)&cli_addr);
        // Child should exit immediately after sending the client confirmation.
        return 0;
      } 
	  else if (pid < 0) 
	  {
        // Something went wrong and the fork operation failed. 
        error("Could not fork a child process");
      } 
	  else
	  {
        /* This is the parent process. This process should keep listening for new connections. 
		Nothing else needs to be done here. */
      }
    }
  }
  
  return 0;
}
