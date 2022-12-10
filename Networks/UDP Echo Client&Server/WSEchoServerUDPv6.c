/*
	Program 3
	Joshua J. Krueger
	CS 2690-001
	Nov 12, 2022
	Windows 11
	VS 2022 v17.3.6
	No cheating took place in writing this assignment.
*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>

#define WELLKNOWNPORTS 1024
#define BUFLEN 1024

void DisplayFatalErr(char* errMsg);

void main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: WSEchoServerUDPv6 <server port>\n");
		exit(0);
	}

	unsigned short svr_port = (unsigned short)atoi(argv[1]);

	if (svr_port < WELLKNOWNPORTS)
	{
		fprintf(stderr, "Port must be above %d\n", WELLKNOWNPORTS);
		exit(1);
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != S_OK)
	{
		fprintf(stderr, "Coundn't find useable Winsock DLL\n");
		exit(0);
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		DisplayFatalErr("Coundn't find useable Winsock DLL\n");

	SOCKET sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)
		DisplayFatalErr("Socket creation failed\n");

	printf("Socket %d created successfully\n", (int)sock);

	struct sockaddr_in6 serverInfo = { 0 };
	serverInfo.sin6_port = htons(svr_port);
	serverInfo.sin6_family = AF_INET6;
	serverInfo.sin6_addr = in6addr_any;

	if (bind(sock, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) < 0)
		DisplayFatalErr("Failed to bind\n");

	printf("JK's IPv6 echo server is ready for UDP client...\n");

	for (;;)
	{
		struct sockaddr_in6 fromAddr;
		char recvBuf[BUFLEN];
		int fromSize = sizeof(fromAddr);
		int res = recvfrom(sock, recvBuf, BUFLEN, 0, (struct sockaddr*)&fromAddr, &fromSize);

		if (res == SOCKET_ERROR)
			DisplayFatalErr("Failed recieving message\n");

		char clientAddr[BUFLEN];
		if (inet_ntop(AF_INET6, (const void*)&fromAddr.sin6_addr, clientAddr, BUFLEN - 1) == NULL)
			DisplayFatalErr("Couldn't convert incoming ip to string");

		printf("Processing client on %s, client port %d, server port %d\n", clientAddr, ntohs(fromAddr.sin6_port), ntohs(serverInfo.sin6_port));

		if (sendto(sock, recvBuf, res, 0, (struct sockaddr*)&fromAddr, fromSize) == SOCKET_ERROR)
			DisplayFatalErr("Error sending msg\n");

		printf("Server recieved: %.*s\n", res, recvBuf);
	}

	// Normally call closesocket(), WSACleanup(), and exit(0)
}

	



	

	

	