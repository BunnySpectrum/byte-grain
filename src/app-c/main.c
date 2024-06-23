// from https://realpython.com/python-bindings-overview/#c-or-c-source
#include <stdio.h>
#include <string.h>

#include "canvas.h"
#include "socket_server.h"
#include "bg_msg.h"

#define DELAY_MS 33

int main()
{
    uint8_t msgBuffer[PIX_COUNT];
    uint8_t cmdBuffer[1];
    sockServerConn_s conn;

    setup_socket("test_socket2", &conn);

    // Begin loop
    // from https://beej.us/guide/bgipc/html/#unixsock
    while (1)
    {
        int done;
        printf("Waiting for connection\n");
        if(BG_SUCCESS != accept_conn(&conn)){
            perror("accept:");
            exit(1);
        }
        printf("Connected.\n");

        init_canvas(msgBuffer, COLOR_WHITE);

        done = 0;
        do
        {
            // printf("%d\n", (rand() % 2) == 0);
            for (int i = 0; i < 32; i++)
            {
                usleep(DELAY_MS * 1000);
                update_grains(msgBuffer);

                if(BG_SUCCESS != send_buffer(&conn, msgBuffer, sizeof(msgBuffer))){
                    printf("Opps\n]");
                    perror("send:");
                    done = 1;
                    break;
                }
            }
            add_grain(msgBuffer, 0, 15, COLOR_RED);

        } while (!done);
        printf("\tClosing.\n");
        close_conn(&conn);
    }
    printf("Ending\n");
    return 0;

}