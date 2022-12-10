// CS 2690 Program 1 
// Simple Windows Sockets Echo Client (IPv6)
// Last update: 2/12/19
// Joshua Krueger 001 10/22/22
// Windows 11 VS Community 2022 v17.3.6
//
// Usage: WSEchoClientv6 <server IPv6 address> <server port> <"message to echo">
// Companion server is WSEchoServerv6
// Server usage: WSEchoServerv6 <server port>
//
// This program is coded in conventional C programming style, with the 
// exception of the C++ style comments.
//
// I declare that the following source code was written by me or provided
// by the instructor. I understand that copying source code from any other 
// source or posting solutions to programming assignments (code) on public
// Internet sites constitutes cheating, and that I will receive a zero 
// on this project if I violate this policy.
// ----------------------------------------------------------------------------

// Minimum required header files for C Winsock program
#include <stdio.h>       // for print functions
#include <stdlib.h>      // for exit() 
#include <winsock2.h>	 // for Winsock2 functions
#include <ws2tcpip.h>    // adds support for getaddrinfo & getnameinfo for v4+6 name resolution
#include <Ws2ipdef.h>    // optional - needed for MS IP Helper

// #define ALL required constants HERE, not inline 
// #define is a macro, don't terminate with ';'  For example...
#define RCVBUFSIZ 50
#define OSRESERVED 1024

// declare any functions located in other .c files here
void DisplayFatalErr(int exitCode, char *errMsg); // writes error message before abnormal termination

void main(int argc, char* argv[])   // argc is # of strings following command, argv[] is array of ptrs to the strings
{
	// Declare ALL variables and structures for main() HERE, NOT INLINE (including the following...)
	WSADATA wsaData;                // contains details about WinSock DLL implementation
	struct sockaddr_in6 serverInfo = { 0 };	// standard IPv6 structure that holds server socket info
	int exitCode = 1;

	// Verify correct number of command line arguments, else do the following:
	if (argc != 4)
	{
		fprintf(stderr, "Usage: WSEchoClientv6 <server_IPv6_address> <server_port> <\"message to echo\">\n");
		exit(exitCode);	  // ...and terminate with abnormal termination code
	}
	else
	{
		exitCode++;
	}

	// Retrieve the command line arguments. (Sanity checks not required, but port and IP addr will need
	// to be converted from char to int.  See slides 11-15 & 12-3 for details.)
	char* ip_addr = argv[1];
	int svr_port = atoi(argv[2]);
	char* msg = argv[3];
	int msgLen = strlen(msg);

	if (svr_port <= OSRESERVED)
	{
		fprintf(stderr, "Usage: <server_port> <= %d\n", OSRESERVED);
		exit(exitCode);	  // ...and terminate with abnormal termination code
	}
	else
	{
		exitCode++;
	}

	if (msgLen < 1)
	{
		fprintf(stderr, "Usage: <\"message to echo\"> < 1\n");
		exit(exitCode);	  // ...and terminate with abnormal termination code
	}
	else
	{
		exitCode++;
	}

	// Initialize Winsock 2.0 DLL. Returns 0 if ok. If this fails, fprint error message to stderr as above & exit(1).
	WORD wVersionRequested;
	int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != S_OK)
	{
		fprintf(stderr, "Could not find a usable Winsock DLL\n");
		exit(exitCode);	  // ...and terminate with abnormal termination code
	}
	else
	{
		exitCode++;
	}

	// Confirm that the WinSock DLL supports 2.2.
	// Note that if the DLL supports versions greater
	// than 2.2 in addition to 2.2, it will still return
	// 2.2 in wVersion since that is the version we requested.                                       
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		fprintf(stderr, "Could not find a usable Winsock DLL\n");
		WSACleanup();
		exit(exitCode);	  // ...and terminate with abnormal termination code
	}
	else
	{
		exitCode++;
	}

	// Create an IPv6 TCP stream socket.  Now that Winsock DLL is loaded, we can signal any errors as shown on next line:
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		DisplayFatalErr(exitCode, "socket() function failed.");
	}
	else
	{
		exitCode++;

		// Display helpful confirmation messages after key socket calls like this:
		printf("Socket %d created successfully.  Press any key to continue...\n", (int)sock);
		getchar();     // needed to hold console screen open
	}
   	
	// Zero out the sockaddr_in6 structure and load server info into it.  See slide 11-15.
	// Don't forget any necessary format conversions.
	// memset(&serverInfo, 0x00, sizeof(serverInfo)); // alternative to = { 0 };
	u_short bigEndianPortNum = htons(svr_port);
	serverInfo.sin6_port = bigEndianPortNum;
	serverInfo.sin6_family = AF_INET6;
	int statusCode = inet_pton(AF_INET6/*&serverInfo.sin6_family*/, ip_addr, &serverInfo.sin6_addr);

	if (statusCode == 0)
	{
		DisplayFatalErr(exitCode, "inet_pton: Invalid address text string.");
	}
	else if (statusCode < 0)
	{
		DisplayFatalErr(exitCode, "inet_pton() failed.");
	}
	else
	{
		exitCode++;

		// Display helpful confirmation messages after key socket calls like this:
		printf("inet_pton() succeeded.\n");
	}

	// Attempt connection to the server.  If it fails, call DisplayFatalErr() with appropriate message,
	// otherwise printf() confirmation message
	if (connect(sock, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) < 0)
	{
		DisplayFatalErr(statusCode, "connect() function failed.");
	}
	else
	{
		exitCode++;

		printf("connect() function succeeded.\n");
	}

	// Send message to server (without '\0' null terminator). Check for null msg (length=0) & verify all bytes were sent...
	// ...else call DisplayFatalErr() with appropriate message as before
	int totalBytesSent = 0;

	while (totalBytesSent < msgLen)
	{
		int bytesSent = send(sock, &msg[totalBytesSent], msgLen - totalBytesSent, 0);

		if (bytesSent == -1)
		{
			DisplayFatalErr(statusCode, "send() function failed.");
		}
		else
		{
			totalBytesSent += bytesSent;
		}
	}
	exitCode++;

 	// Retrieve the message returned by server.  Be sure you've read the whole thing (could be multiple segments). 
	// Manage receive buffer to prevent overflow with a big message.
 	// Call DisplayFatalErr() if this fails.  (Lots can go wrong here, see slides.)
	int totalBytesReceived = 0;
	char responce[RCVBUFSIZ] = { 0 };

	while (totalBytesReceived < msgLen)
	{
		int bytesReceived = recv(sock, &responce[totalBytesReceived], RCVBUFSIZ - totalBytesReceived, 0);

		if (bytesReceived == -1)
		{
			DisplayFatalErr(statusCode, "recv() function failed.");
		}
		else if (bytesReceived > 0)
		{
			totalBytesReceived += bytesReceived;
		}
		else
		{
			break;
		}
	}
	exitCode++;
 	
 	// Display ALL of the received message, in printable C string format.
	printf("Recieved msg (or len %d): %s\n", totalBytesReceived, responce);

 	// Close the TCP connection (send a FIN) & print appropriate message.
	closesocket(sock);

	// Release the Winsock DLL
	WSACleanup();

	exit(0);
}
