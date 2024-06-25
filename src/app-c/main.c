// from https://realpython.com/python-bindings-overview/#c-or-c-source
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "app/canvas.h"
#include "xal/socket_server.h"
#include "xsp/socket_display.h"
#include "utils/bg_msg.h"
#include "utils/bg_colors.h"

#define FRAME_DELAY_MS 30
DisplayContext_s *pDispCtx;

int main()
{
    if(BG_SUCCESS != factory_socket_display(&pDispCtx)){
        printf("Error setting up socket_display context.\n");
        return BG_FAIL;
    }

    if(BG_SUCCESS != pDispCtx->display_register(&pDispCtx->hDisplay)){
        printf("Error registering socket_display.\n");
        return BG_FAIL;
    }else{
        printf("Socket display registered: %d.\n", pDispCtx->hDisplay );
    }



    uint8_t msgBuffer[PIX_COUNT];
    uint8_t cmdBuffer[1];


    // Begin loop
    // from https://beej.us/guide/bgipc/html/#unixsock
    while (1)
    {
        int done;
        uint8_t newGrainCounter = 32;

        printf("Waiting for connection\n");
        // socket_disp_init(&dispCtx);
        pDispCtx->display_init(pDispCtx);
        printf("Connected.\n");

        init_canvas(msgBuffer, BG_COLOR_WHITE);

        done = 0;
        do
        {
            for (;;)
            {
                update_grains(msgBuffer);

                if(BG_SUCCESS != pDispCtx->display_draw_buffer(pDispCtx, 0, 0, 32, 32, (BG_COLOR_e*)msgBuffer) ){
                    printf("Opps\n]");
                    perror("send:");
                    done = 1;
                    break;
                }

                usleep(FRAME_DELAY_MS * 1000);
                
                if(--newGrainCounter == 0){
                    // add_grain(msgBuffer, 0, 15, BG_COLOR_BLUE);
                    add_grain(msgBuffer, 0, 15, BG_COLOR_RED);
                    newGrainCounter = 15;
                }
            }

        } while (!done);
        printf("\tClosing.\n");
        pDispCtx->display_deinit(pDispCtx);
    }
    printf("Ending\n");
    return 0;

}