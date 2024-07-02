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
#include "utils/bg_log.h"

#define FRAME_DELAY_MS 30
DisplayContext_s *pDispCtx;
ScreenContext_s *pScreenCtx;
SocketDisplayCtx_s socketDisplayCtx;

uint8_t msgBuffer[PIX_COUNT];
ImageBuf_s imageBuf = {
    .pImgData = msgBuffer,
    .length = PIX_COUNT,
    .width = 32,
    .height = 32,
    };

uint8_t cmdBuffer[4];
BG_BOOL_e initSuccess = BG_True;

int main()
{

    printf("Built at %s .\n", __TIME__);

    initSuccess &= log_if_err(
        factory_socket_display(&pDispCtx, &socketDisplayCtx), "Error setting up socket_display context.");
    ASSERT_TRUE(initSuccess);

    initSuccess &= log_if_err(
        pDispCtx->display_register(pDispCtx->hDisplay), "Error registering socket_display.");
    ASSERT_TRUE(initSuccess);

    initSuccess &= log_if_err(
        factory_screen(&pScreenCtx, pDispCtx, &imageBuf), "Error setting up screen.");
    ASSERT_TRUE(initSuccess);


    // Begin loop
    // c.f. https://beej.us/guide/bgipc/html/#unixsock
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
            if(BG_SUCCESS != read_buffer(&socketDisplayCtx.conn, cmdBuffer, sizeof(cmdBuffer))){
                printf("Recv error.\n");
                perror("recv:\n");
            }

            if(cmdBuffer[0] == 1){
                // add grain
                add_grain(msgBuffer, cmdBuffer[1], cmdBuffer[2], cmdBuffer[3]);
            }

            // if(--newGrainCounter == 0){
            //     // add_grain(msgBuffer, 0, 15, BG_COLOR_BLUE);
            //     add_grain(msgBuffer, 0, 15, BG_COLOR_YELLOW);
            //     newGrainCounter = 15;
            // }

            // Run update physics
            canvas_update(msgBuffer);

            if(!log_if_err(screen_update(pScreenCtx->hScreen), "Error updating screen.")){
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