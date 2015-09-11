#include "NetApi.h"
#include <string.h>
#ifdef _MSC_VER
#include <winsock2.h>
typedef int socklen_t;
#else
	#include <fcntl.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <linux/stat.h>
	#include <linux/types.h>
	#include <unistd.h>
	#include <string.h>
	#include <time.h>
	#include <signal.h> 
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <stdarg.h> 
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#define SD_BOTH 2
#define closesocket close
typedef int SOCKET;
#define INVALID_SOCKET ((SOCKET)~0)
#endif

int WaitClient( int ssock )
{
	int sock = -1;
	socklen_t addlen;
	struct sockaddr_in caddr; 
	addlen= sizeof(caddr);
	sock = accept( ssock, (struct sockaddr *)&caddr,&addlen );
	return sock;
}

int MakeServer( int port )
{
	int error = 0;
	int listenSocket = socket (AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddress;
	if (listenSocket >= 0)
	{
		int optval;
		optval = 1;
		if ( setsockopt (listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval) ) >=0 )
		{
			serverAddress.sin_family = AF_INET;
			serverAddress.sin_addr.s_addr = htonl (INADDR_ANY);
			serverAddress.sin_port = htons (port);
			if (bind (listenSocket, (struct sockaddr *) & serverAddress, sizeof (serverAddress) ) >= 0 )
				listen (listenSocket, 5);
			else
				error = -1;
		}
		else
			error = -2;
	}
	else
		error = -3;
	if( !error )
		return listenSocket;
	else
	{
		if( listenSocket )
			close( listenSocket );
		return 0;
	}
		
}

int MakeConnection( int port )
{
	int error = 0;

#if (defined(WIN32) || defined(UNDER_CE))
	WSADATA	wsadata;
	WSAStartup(0x202, &wsadata);
#endif

	unsigned int ControlSocket = socket(AF_INET, SOCK_STREAM, 0);

	if( !error )
	{
		struct sockaddr_in serveraddress;
		memset(&serveraddress, 0, sizeof(struct sockaddr_in));
		serveraddress.sin_family = AF_INET;
		serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
		serveraddress.sin_port = htons(port);
		int ret = connect( ControlSocket, (sockaddr*)&serveraddress, sizeof(struct sockaddr_in) );
		if( ret < 0 )
			error = 2;
	}
	if( !error )
		return ControlSocket;
	else
		return 0;
}

int CloseConnection( int s )
{
	if( s )
	{
		shutdown(s, SD_BOTH);
		closesocket(s);
	}
	return 0;
}
int socket_recv(int socket, char* pbuffer, int len)
{
	int actual = 0;
	while (actual < len)
	{
		int ret = recv(socket, pbuffer+actual, len-actual, 0);
		if (ret > 0)
		{
			actual += ret;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

int ReadMessage( unsigned int* sock, int count , char* buffer , int*len , int*errorid , int timeout )
{
	int error = 0;
	fd_set set;
	FD_ZERO(&set);
	for( int i=0; i<count; i++ )
		if( sock[i] != 0 )
			FD_SET( sock[i] , &set);
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout*1000;
	int num = select( FD_SETSIZE , &set, NULL, NULL, &tv);
	if (num > 0)
	{
		for( int i=0; i<count; i++ )
		{
			if( sock[i] && FD_ISSET( sock[i] , &set ) )
			{
				int length;
				TraceMsg( "to receive %d\n" , i  );
				if( socket_recv( sock[i], (char*)&length, sizeof(length)) == 0)
				{
					if ( socket_recv( sock[i], buffer , length) == 0)
					{
						*len = length;
						return i;
					}
					else
						error = -1;
				}
				else
					error = -2;
				if( error )
					*errorid = i;
				break;
			}
		}
	}
	else if( num < 0 )
		error = -3;
	return error;
}

int ReadMessage( unsigned int sock , char* buffer , int*len , int timeout)
{
	int error = 0;
	fd_set set;
	FD_ZERO(&set);
	if( !sock )
		return -4;
	FD_SET( sock , &set);
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout*1000;
	int length = 0;
	int num = select( sock+1 , &set, NULL, NULL, &tv);
	if (num > 0)
	{
		if( FD_ISSET( sock , &set ) )
		{
			if( socket_recv( sock, (char*)&length, sizeof(length)) == 0)
			{
				if ( socket_recv( sock, buffer , length) == 0)
					*len = length;
				else
					error = -1;
			}
			else
				error = -2;
		}
	}
	else if( num < 0 )
		error = -3;
	if( !error )
		return length;
	else
		return error;
}

int socket_send(int socket, const char* pbuffer, int len)
{
	int actual = 0;
	while (actual < len)
    {
#ifdef WIN32
		int ret = send(socket, pbuffer+actual, len-actual, 0);
#else
        int ret = send(socket, pbuffer+actual, len-actual, MSG_NOSIGNAL);
#endif
        if (ret > 0)
        {
            actual += ret;                        
        }
        else 
        {
            return -1;
        }
    }
	
	return 0;
}
int SendMessage( unsigned int sock , char* buffer , int length )
{
	int error = 0;
	if (socket_send( sock , (char*)&length, sizeof(length)) == 0)
	{
		if (socket_send( sock , buffer, length) == 0)
		{
			;
		}
		else
			error = -2;
	}
	else
		error = -1;
	return error;
}


static int logfile = 0;
static int logindent = 0;
static char indenttap[256];

void OpenTrace( int argc , char* argv[] )
{
	if( argc > 1 )
	{
		char* filename = argv[1];
		unlink( filename);
		mkfifo( filename , 0777 );
		logfile = open( filename , O_WRONLY );
	}
	else
		logfile = (int)stdout;
	logindent = 0;
	memset( indenttap , '\t' , sizeof(indenttap) );
}

void TraceMsg( const char *fmt, ...) 
{ 
	va_list argptr; 
	va_start(argptr, fmt); 
	if( logfile == (int)stdout )
	{
		indenttap[logindent] = '\0';
		printf( "%s" , indenttap );
		vprintf( fmt , argptr );
		indenttap[logindent] = '\t';
	}
	else
	{
		static char temp[1024];
		memset( temp , 0 , sizeof(temp) );
		vsprintf( temp , fmt , argptr );
		write( logfile , indenttap , logindent );
		write( logfile , temp , strlen(temp)); 
	}
	va_end(argptr); 
} 

void BeginFunction( const char*funcname )
{
	TraceMsg( "<%s>\n" , funcname );
	logindent ++;
}
void EndFunction( const char*funcname )
{
	logindent --;
	TraceMsg( "</%s>\n" , funcname );
}



