// from https://realpython.com/python-bindings-overview/#c-or-c-source
#include <stdio.h>
#include <string.h>


#include "canvas.h"
#include "socket_server.h"
#include "bg_msg.h"

#define DELAY_MS 33

int main()
{

    // Create UN socket
    int s1, s2;
    uint8_t msgBuffer[PIX_COUNT];
    uint8_t cmdBuffer[1];

    char *symlinkpath = "test_socket2";
    // char actualpath [PATH_MAX+1];

    // init_canvas(msgBuffer, COLOR_WHITE);

    if (BG_SUCCESS != create_unix_socket(&s1))
    {
        return -1;
    }

    // Bind
    //  if(NULL == realpath(symlinkpath, actualpath)){
    //      perror("Error resolving path:");
    //      return -1;
    //  }

    if (BG_SUCCESS != bind_unix_socket(s1, symlinkpath))
    {
        return -1;
    }

    // Listen
    if (BG_SUCCESS != listen_unix_socket(s1, 1))
    {
        return -1;
    }

    struct sockaddr_un remote = {
        .sun_family = AF_UNIX,
    };

    signal(SIGPIPE, SIG_IGN);

    // Begin loop
    // from https://beej.us/guide/bgipc/html/#unixsock
    while (1)
    {
        int done;
        printf("Waiting for connection\n");
        socklen_t slen = sizeof(remote);
        if ((s2 = accept(s1, (struct sockaddr *)&remote, &slen)) == -1)
        {
            perror("accept:");
            exit(1);
        }

        printf("Connected.\n");
        init_canvas(msgBuffer, COLOR_WHITE);

        done = 0;
        do
        {
            printf("%d\n", (rand() % 2) == 0);
            for (int i = 0; i < 32; i++)
            {
                usleep(DELAY_MS * 1000);
                update_grains(msgBuffer);

                if (send(s2, msgBuffer, sizeof(msgBuffer), 0) < 0)
                {
                    printf("Opps\n]");
                    perror("send:");
                    done = 1;
                    break;
                }
            }
            add_grain(msgBuffer, 0, 15, COLOR_BLUE);

        } while (!done);
        printf("\tClosing.\n");
        close(s2);
    }
    printf("Ending\n");
    return 0;

}