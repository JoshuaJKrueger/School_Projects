#include <winsock.h>

#define RECVBUFFSIZE 2

void DisplayFatalErr(char* errMsg);

DWORD WINAPI ProcessClient(LPVOID _clientSock)
{
	SOCKET clientSock = (SOCKET)_clientSock;
	char buffer[RECVBUFFSIZE];
	int recvMsgLen;

	if ((recvMsgLen = recv(clientSock, buffer, RECVBUFFSIZE, 0)) < 0)
		DisplayFatalErr("recv() failed");

	while (recvMsgLen > 0)
	{
		if (send(clientSock, buffer, recvMsgLen, 0) != recvMsgLen)
			DisplayFatalErr("send() failed");

		if ((recvMsgLen = recv(clientSock, buffer, RECVBUFFSIZE, 0)) < 0)
			DisplayFatalErr("recv() failed");
	}

	closesocket(clientSock);
}
