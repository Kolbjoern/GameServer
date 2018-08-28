#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORTNUM 9966
#define CLIENT_CHUNK 10
#define MAX_BUFFER_LEN 500

struct sockaddr_in get_server_address_info()
{
	struct sockaddr_in server_info;
	memset(&server_info, 0, sizeof(server_info)); //zero struct before filling
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = htonl(INADDR_ANY);
	server_info.sin_port = htons(PORTNUM);
	return server_info;
}

int main(int argc, char* argv[])
{
	printf("SERVER INIT::");
	
	struct sockaddr_in server_info = get_server_address_info();

	// socket for incoming connections
	// create upd socket, with IP version 4
	int server_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_sock == -1) {
		printf("Could not create socket\n");
	}

	// bind server information to socket
	int bind_response = bind(server_sock, (struct sockaddr*) &server_info, sizeof(struct sockaddr));
	if (bind_response < 0){
		printf("Could not bind socket\n");
	}

	// client next
	struct sockaddr_in client_info;
	int client_addr_len = sizeof(client_info);

	// listen for incoming messages
	int message_size;
	char buffer[MAX_BUFFER_LEN + 1]; // +1 for null terminator
	while (1) {
		// receive
		message_size = recvfrom(server_sock, buffer, MAX_BUFFER_LEN, 0, (struct sockaddr*) &client_info, &client_addr_len);
		if (message_size > 0){
			buffer[message_size] = '\0';
			printf("Received from: %s (p %d)\n", inet_ntoa(client_info.sin_addr), client_info.sin_port);
			printf("Data: %s (%d bytes)\n", buffer, message_size);
		}
		
		// send back
		if (sendto(server_sock, buffer, message_size, 0, (struct sockaddr*) &client_info, client_addr_len) == -1) {
			printf("Could not send data back to client\n");
		}
	}
	
	printf("EVERETHEN FINE\n");

	close(server_sock);

	return 0;
}
