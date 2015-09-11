#ifndef NETAPI_H
#define NETAPI_H

int WaitClient( int ssock );
int MakeServer( int port );
int MakeConnection( int port );
int CloseConnection( int s );
int ReadMessage( unsigned int* sock, int count , char* buffer , int*len , int*errorid , int timeout );
int ReadMessage( unsigned int sock , char* buffer , int*len , int timeout );
int SendMessage( unsigned int sock , char* buffer , int length );

void OpenTrace( int argc , char* argv[] );
void TraceMsg( const char *fmt, ...) ;
void BeginFunction( const char*funcname );
void EndFunction( const char*funcname );
#define BEGINFUNC BeginFunction( __FUNCTION__ );
#define ENDFUNC EndFunction( __FUNCTION__ );

#endif

