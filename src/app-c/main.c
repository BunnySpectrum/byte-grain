// from https://realpython.com/python-bindings-overview/#c-or-c-source
#include <stdio.h>
#include <string.h>

#include "app/canvas.h"
#include "xal/socket_server.h"
#include "xsp/socket_display.h"
#include "utils/bg_msg.h"
#include "utils/bg_colors.h"

#define DELAY_MS 33
DisplayContext_s dispCtx;

int main()
{
    if(BG_SUCCESS != factory_socket_display(&dispCtx)){
        printf("Error setting up socket_display context.\n");
        return BG_FAIL;
    }

    if(BG_SUCCESS != socket_disp_register(&dispCtx.hDisplay)){
        printf("Error registering socket_display.\n");
        return BG_FAIL;
    }else{
        printf("Socket display registered: %d.\n", dispCtx.hDisplay );
    }



    uint8_t msgBuffer[PIX_COUNT];
    uint8_t cmdBuffer[1];


    // Begin loop
    // from https://beej.us/guide/bgipc/html/#unixsock
    while (1)
    {
        int done;

        printf("Waiting for connection\n");
        socket_disp_init(&dispCtx);
        printf("Connected.\n");

        init_canvas(msgBuffer, BG_COLOR_WHITE);

        done = 0;
        do
        {
            for (int i = 0; i < 32; i++)
            {
                usleep(DELAY_MS * 1000);
                update_grains(msgBuffer);

                if(BG_SUCCESS != socket_disp_draw_buffer(&dispCtx, 0, 0, 32, 32, (BG_COLOR_e*)msgBuffer) ){
                    printf("Opps\n]");
                    perror("send:");
                    done = 1;
                    break;
                }
            }
            add_grain(msgBuffer, 0, 15, BG_COLOR_RED);

        } while (!done);
        printf("\tClosing.\n");
        socket_disp_deinit(&dispCtx);
    }
    printf("Ending\n");
    return 0;

}