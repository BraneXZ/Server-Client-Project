all: server client

server: server.c server_functions.c
	gcc server.c server_functions.c -o server

client: client.c client_functions.c
	gcc client.c client_functions.c -o client

clean:
	rm -f server client
