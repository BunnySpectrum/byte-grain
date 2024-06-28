#include "socket_server.h"



BG_CODES_e create_unix_socket(int *sock)
{

    *sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (*sock == -1)
    {
        perror("Unable to create socket:");
        sock = NULL;
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e bind_unix_socket(int sock, const char *name)
{
    // int len;

    struct sockaddr_un local = {
        .sun_family = AF_UNIX,
        // .sun_path = SOCK_PATh,
    };
    strncpy(local.sun_path, name, sizeof(local.sun_path) - 1);
    // printf("In bind: %s.\n", local.sun_path);
    unlink(local.sun_path);

    if (-1 == bind(sock, (struct sockaddr *)&local, sizeof(local)))
    {
        perror("Unable to bind socket:");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e listen_unix_socket(int sock, int count)
{

    if (-1 == listen(sock, count))
    {
        perror("Error listening:");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}


BG_CODES_e setup_socket(char* symLinkPath, sockServerConn_s *conn){
    // Create UN socket

    if (BG_SUCCESS != create_unix_socket( &(conn->s1)) )
    {
        return BG_FAIL;
    }

    // Bind
    //  if(NULL == realpath(symlinkpath, actualpath)){
    //      perror("Error resolving path:");
    //      return -1;
    //  }

    if (BG_SUCCESS != bind_unix_socket(conn->s1, symLinkPath))
    {
        return BG_FAIL;
    }

    // Listen
    if (BG_SUCCESS != listen_unix_socket(conn->s1, 1))
    {
        return BG_FAIL;
    }


    signal(SIGPIPE, SIG_IGN);
    return BG_SUCCESS;
}


BG_CODES_e accept_conn(sockServerConn_s *conn){
    socklen_t slen = sizeof(remote);
    if ((conn->s2 = accept(conn->s1, (struct sockaddr *)&remote, &slen)) == -1)
    {
        return BG_FAIL;
    }
    return BG_SUCCESS;
}

BG_CODES_e close_conn(sockServerConn_s *conn){
    close(conn->s2);
    return BG_SUCCESS;
}

BG_CODES_e send_buffer(sockServerConn_s *conn, void* buffer, size_t length){
    if (send(conn->s2, buffer, length, 0) < 0){
        return BG_FAIL;
    }
    return BG_SUCCESS;
}

BG_CODES_e read_buffer(sockServerConn_s *conn, void* buffer, size_t length){
    if (recv(conn->s2, buffer, length, 0) < 0){
        return BG_FAIL;
    }
    return BG_SUCCESS;
}