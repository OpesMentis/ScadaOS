default: server client

client: client.c
	gcc -Wall -Wfatal-errors client.c -o client

server: server.c
	gcc -Wall -Wfatal-errors server.c -o server
