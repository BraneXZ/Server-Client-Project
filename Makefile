#Two step process of compiling and making executable files for server and client
#Author: Fortythree Shiaohongtu

all: server client


server: server.o server_functions.o server.h
	g++ -o server server.o server_functions.o server.h
server.o: server.c 
	g++ -c server.c
server_functions.o: server_functions.c
	g++ -c server_functions.c
client: client.o client_functions.o client.h
	g++ client.c client_functions.c -o client
client.o: client.c
	g++ -c client.c
client_functions.o: client_functions.c
	g++ -c client_functions.c
clean:
	rm -rf server client server.o server_functions.o client.o client_functions.o
