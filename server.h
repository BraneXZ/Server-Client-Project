/* Author: Fortythree Shiaohongtu
 * File: server.h
 * Description: Function declaration for server.c and server_functions.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>

void error(const char *message);

void interruptHandler(int signalNo);

void processTCPRequest(int socketFD);

void processUDPRequest(int socketFD, struct sockaddr *address);

