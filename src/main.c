/* 
**  Copyright 2017-2018
**  Author(s): Bart Gysens <gysens.bart@gmx.com>
**
**  This file is part of Knock.
**
**  Knock is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Knock is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Knock. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>


static int _syntax( int error )
{
    printf( "syntax: knock OPTIONS hostname|ip-address port-1 .. port-n\n"
            "\n"
            "        hostname          host dns name\n"
            "        ip-address        host ip address\n"
            "        port-1 .. port-n  ports to knock\n"
            "\n"
            "        OPTIONS\n"
            "        -d x              x specifies the delay, in ms, between knocks (default 250)\n" );

    return error;
}


static int _error( int error, const char* format, ... )
{
    va_list args; 
    va_start( args, format );
    printf( "error %d: ", error );
    vprintf( format, args );
    printf( "\n" );
    va_end( args );
    return error;
}


static int _hostnameToIp( const char* hostname, char* ip )
{
    struct hostent* he;

    if ( ( he = gethostbyname( hostname ) ) == NULL )
    {
        return _error( -101, hstrerror( h_errno ) );
    }

    if ( he->h_addr_list == NULL || *he->h_addr_list == NULL )
    {
        return _error( -102, "failed to lookup host \'%s\'", hostname );
    }

    strcpy( ip, inet_ntoa( *(struct in_addr*) (he->h_addr_list[0]) ) );
    return 0;
}


static int _knock( const char* ip, unsigned short port, int delay )
{
    int s;
    long on = 1L;
    struct sockaddr_in server;

    memset( &server, 0, sizeof( server ) );
    server.sin_addr.s_addr = inet_addr( ip );
    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    if ( ( s = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        return _error( -201, "failed to create socket");
    }

    if ( ioctl( s, FIONBIO, &on ) != 0 )
    {
        return _error( -202, "failed to set non-blocking socket i/o" ); 
    }

    connect( s, (struct sockaddr*) &server, sizeof( server ) );
    usleep( delay * 1000L );
    close( s );
    return 0;
}


int main( int argc, char *argv[] ) 
{
    int error;
    int delay = 250;
    int index;
    char* hostname;
    char ip[64];

    if (   ( argc < 3 )
        || ( *argv[1] == '-' && argc < 5 ) )
    {
        return _syntax( -1 );
    }

    if ( strcmp( argv[1], "-d" ) == 0 )
    {
        hostname = argv[3];
        index = 4;

        if ( ( delay = atoi( argv[2] ) ) == 0 )
        {
            return _error( -2, "invalid delay value specified \'%s\'", argv[2] );
        }
    }
    else
    {
        hostname = argv[1];
        index = 2;
    }

    if ( ( error = _hostnameToIp( hostname, ip ) ) != 0 )
    {
        return error;
    }

    if ( strcmp( hostname, ip ) == 0 )
    {
        printf( "%s: knocking on port", hostname );
    }
    else
    {
        printf( "%s [%s]: knocking on port", hostname, ip );
    }

    for ( ; index < argc; ++index )
    {
        unsigned short port = (unsigned short) atoi( argv[index] );

        if ( port == 0 )
        {
            printf( "\n" );
            return _error( -3, "invalid port value specified \'%s\'", argv[index] );
        }

        if ( ( error = _knock( ip, port, delay ) ) != 0 )
        {
            return error;
        }

        printf( " %d", port );
    }

    printf( "\n" );
    return 0;
}

