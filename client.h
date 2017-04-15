/* File: Client.h 
 * Author: Fortythree Shiaohongtu
 * Description: header file for client.c and client_functions.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(const char *msg);

void processTCPConnection(int socketFD, struct sockaddr *serverAddress,
				char *buffer, int bufferSize);

void processUDPConnection(int socketFD, struct sockaddr *serverAddress, 
				char *buffer, int bufferSize);


