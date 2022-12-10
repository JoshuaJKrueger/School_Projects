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
	if (argc != 4)
	{
		fprintf(stderr, "Usage: WSEchoClientUDPv6 <server IPv6 addr> <server port> <\"Msg to echo\">\n");
		exit(1);
	}

	char* ip_addr = argv[1];
	unsigned short svr_port = (unsigned short)atoi(argv[2]);
	char* msg = argv[3];

	if (svr_port < WELLKNOWNPORTS)
	{
		fprintf(stderr, "Port must be above %d\n", WELLKNOWNPORTS);
		exit(1);
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != S_OK)
	{
		fprintf(stderr, "Coundn't find useable Winsock DLL\n");
		exit(1);
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
	int inetStatusCode = inet_pton(AF_INET6, ip_addr, &serverInfo.sin6_addr);
	if (inetStatusCode == 0)
		DisplayFatalErr("inet_pton: Invalid address text string\n");
	else if (inetStatusCode < 0)
		DisplayFatalErr("inet_ption failed\n");
	else
		printf("inet_pton succeeded\n");

	for (;;)
	{
		if (sendto(sock, msg, (int)strlen(msg), 0, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) != strlen(msg))
			DisplayFatalErr("Error sending msg\n");

		struct sockaddr_in6 fromAddr;
		char recvBuf[BUFLEN];
		int fromSize = sizeof(fromAddr);
		int res = recvfrom(sock, recvBuf, BUFLEN, 0, (struct sockaddr*)&fromAddr, &fromSize);

		if (res != strlen(msg) || res == SOCKET_ERROR, memcmp(&fromAddr.sin6_addr, &serverInfo.sin6_addr, sizeof(fromAddr.sin6_addr)) != 0)
			DisplayFatalErr("Failed recieving message\n");

		printf("Client recieved: %s\n", recvBuf);
	}

	// Normally call closesocket(), WSACleanup(), exit(0);
}
