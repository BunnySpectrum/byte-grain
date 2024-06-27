// from https://realpython.com/python-bindings-overview/#c-or-c-source
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "app/canvas.h"
#include "app/screen.h"
#include "xal/socket_server.h"
#include "xsp/socket_display.h"
#include "utils/bg_msg.h"
#include "utils/bg_colors.h"

#define FRAME_DELAY_MS 30
DisplayContext_s *pDispCtx;
ScreenContext_s *pScreenCtx;

uint8_t msgBuffer[PIX_COUNT];
ImageBuf_s imageBuf = {
    .pImgData = msgBuffer,
    .length = PIX_COUNT,
    .width = 32,
    .height = 32,
    };

uint8_t cmdBuffer[1];

int main()
{
    if(BG_SUCCESS != factory_socket_display(&pDispCtx)){
        printf("Error setting up socket_display context.\n");
        return BG_FAIL;
    }

    if(BG_SUCCESS != pDispCtx->display_register(pDispCtx->hDisplay)){
        printf("Error registering socket_display.\n");
        return BG_FAIL;
    }
    printf("Socket display registered: %d.\n", pDispCtx->hDisplay );

    // Set up screen
    if(BG_SUCCESS != factory_screen(&pScreenCtx, pDispCtx, &imageBuf)){
        printf("Error setting up screen.\n");
        return BG_FAIL;
    }




    // Begin loop
    // from https://beej.us/guide/bgipc/html/#unixsock
    while (1)
    {
        int done;
        uint8_t newGrainCounter = 32;

        init_canvas(msgBuffer, BG_COLOR_WHITE);

        printf("Waiting for connection\n");
        screen_init(pScreenCtx);
        printf("Connected.\n");

        done = 0;
        do
        {
            // Stand-in for processing commands from client
            if(--newGrainCounter == 0){
                add_grain(msgBuffer, 0, 15, BG_COLOR_BLUE);
                // add_grain(msgBuffer, 0, 15, BG_COLOR_RED);
                newGrainCounter = 15;
            }

            // Run update physics
            canvas_update(msgBuffer);

            if(BG_SUCCESS != screen_update(pScreenCtx->hScreen)){
                perror("send:");
                done = 1;
                break;
            }

            usleep(FRAME_DELAY_MS * 1000);
            
        } while (!done);
        printf("\tClosing.\n");
        screen_deinit(pScreenCtx);
    }
    printf("Ending\n");
    return 0;

}