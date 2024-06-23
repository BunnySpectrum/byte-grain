#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <semaphore.h>

// POSIX shared memory
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/syslimits.h>

#define SOCK_PATH "test_socket"

typedef enum
{
    BG_FAIL = 0,
    BG_SUCCESS,
    BG_TIMEOUT,
} BG_CODES_e;
    
static struct sockaddr_un remote = {
    .sun_family = AF_UNIX,
};

typedef struct sockServerConn{
    int s1;
    int s2;
}sockServerConn_s;

BG_CODES_e create_unix_socket(int *sock);
BG_CODES_e bind_unix_socket(int sock, const char *name);
BG_CODES_e listen_unix_socket(int sock, int count);
BG_CODES_e setup_socket(char* symLinkPath, sockServerConn_s *conn);

BG_CODES_e accept_conn(sockServerConn_s *conn);
BG_CODES_e close_conn(sockServerConn_s *conn);
BG_CODES_e send_buffer(sockServerConn_s *conn, void* buffer, size_t length);

#endif