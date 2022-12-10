#include <winsock2.h>
#include <stdio.h>
#include <ws2tcpip.h>

#define BUFFLEN 50
#define MAXQUEUED 5
#define OSRESERVED 1024

void DisplayFatalErr(char* errMsg);
DWORD WINAPI ProcessClient(LPVOID _clientSock);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	struct sockaddr_in6 serverInfo;
	struct sockaddr_in6 clientInfo;

	if (argc != 2)
		DisplayFatalErr("Usage: WSEchoServerv6 <server_port>");

	unsigned short serverPort = (unsigned short)atoi(argv[1]);

	if (serverPort <= OSRESERVED)
		DisplayFatalErr("Usage: <server_port> can't be in os reserved space\n");

	WORD wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);

	if (err != S_OK)
		DisplayFatalErr("Couldn't find usable Winsock DLL\n");

	SOCKET serverSock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

	if (serverSock == INVALID_SOCKET)
		DisplayFatalErr("socket() failed");

	// Fill sockaddr_in with server info
	memset(&serverInfo, 0, sizeof(serverInfo));
	serverInfo.sin6_family = AF_INET6;
	serverInfo.sin6_port = htons(serverPort);
	serverInfo.sin6_addr = in6addr_any;

	// Bind server socket
	if (bind(serverSock, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) < 0)
		DisplayFatalErr("bind() failed");

	// Listen for connections
	if (listen(serverSock, MAXQUEUED) < 0)
		DisplayFatalErr("listen() failed");

	printf("JJK's IPv6 echo server is ready for client connections...\n");

	int clientInfoLen = sizeof(clientInfo);
	SOCKET clientSock;

	HANDLE hThread;
	LPDWORD dwThreadId;

	// Accept connections and process them
	for (;;)
	{
		char clientAddr[BUFFLEN];

		if ((clientSock = accept(serverSock, (struct sockaddr*)&clientInfo, &clientInfoLen)) == INVALID_SOCKET)
			DisplayFatalErr("accept() failed");

		if (inet_ntop(AF_INET6, (const void*)&clientInfo.sin6_addr, clientAddr, BUFFLEN - 1) == NULL)
			DisplayFatalErr("Couldn't convert incoming ip address to string");

		printf("Processing client on %s, client port %d, server port %d\n", clientAddr, ntohs(clientInfo.sin6_port), ntohs(serverInfo.sin6_port));

		hThread = CreateThread(NULL, 0, ProcessClient, clientSock, 0, &dwThreadId);
	}
	// Don't need to call WSACleanup, closesocket(), exit() since loop never finishes
}
