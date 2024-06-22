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





