// from https://realpython.com/python-bindings-overview/#c-or-c-source
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

#define ROW_MAX 32
#define COL_MAX 32
#define PIX_COUNT ROW_MAX *COL_MAX

typedef enum
{
    BG_FAIL = 0,
    BG_SUCCESS,
    BG_TIMEOUT,
} BG_CODES_e;

#define SHM_NAME "bg_shm"
#define SEM_NAME "bg_sem"
#define SHM_SIZE 1024

#define SOCK_PATH "test_socket"

/*
    Unix domain sockets
*/

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

#define COLOR_WHITE 0
#define COLOR_BLACK 1
#define COLOR_RED 2
#define COLOR_BLUE 3

void init_canvas(uint8_t *buf, int color)
{
    int i;
    for (i = 0; i < PIX_COUNT; i++)
    {
        buf[i] = color;
    }
}

void add_grain(uint8_t *buf, int row, int col, int color)
{
    buf[row * ROW_MAX + col] = color;
}

#define GRAIN_2D_TO_1D(row, col) ((row) * (ROW_MAX) + (col))
#define GET_GRAIN_VALID(grain) (((grain) >> 7) & 0x1)
#define GET_GRAIN_ACTIVE(grain) (((grain) >> 6) & 0x1)

#define SET_GRAIN_VALID(grain) ((grain) |= (1 << 7))
#define CLEAR_GRAIN_VALID(grain) ((grain) &= ~(1 << 7))

#define SET_GRAIN_ACTIVE(grain) ((grain) |= (1 << 6))
#define CLEAR_GRAIN_ACTIVE(grain) ((grain) &= ~(1 << 6))

#define SET_GRAIN_COLOR(grain, color) ((grain) = ((grain) & ~0x3F) | ((color) & 0x3F))
#define GET_GRAIN_COLOR(grain) ((grain) & 0x3F)

#define CLEAR_GRAIN(grain) ((grain) = 0x0)
#define IS_EMPTY_GRAIN(grain) (COLOR_WHITE == GET_GRAIN_COLOR(grain))

void update_grains(uint8_t *buf)
{
    int row, col, idx;
    int below, current, belowLeft, belowRight;
    for (row = 0; row < ROW_MAX; row++)
    {
        for (col = 0; col < COL_MAX; col++)
        {
            if (row + 1 == ROW_MAX)
            {
                continue;
            }
            current = GRAIN_2D_TO_1D(row, col);
            below = GRAIN_2D_TO_1D(row + 1, col);
            belowLeft = GRAIN_2D_TO_1D(row + 1, col - 1);
            belowRight = GRAIN_2D_TO_1D(row + 1, col + 1);

            if (1 == GET_GRAIN_VALID(buf[current]))
            {
                continue;
            }

            if (IS_EMPTY_GRAIN(buf[current]))
            {
                continue;
            }

            if (IS_EMPTY_GRAIN(buf[below]))
            {
                // printf("Current %#x\n", buf[current]);
                // There is an empty space below us
                buf[below] = buf[current];
                CLEAR_GRAIN(buf[current]);

                SET_GRAIN_VALID(buf[below]);
                SET_GRAIN_VALID(buf[current]);

                // Mark the current grain as 'active' (in motion)
                // printf("Below %#x\n", buf[below]);
                SET_GRAIN_ACTIVE(buf[below]);
                // printf("\t%#x\n", buf[below]);
            }
            else if (1 == GET_GRAIN_ACTIVE(buf[current]))
            {
                if ((rand() % 2) == 0){
                    // look left
                    if (IS_EMPTY_GRAIN(buf[belowLeft])){
                        buf[belowLeft] = buf[current];
                        CLEAR_GRAIN(buf[current]);

                        SET_GRAIN_VALID(buf[belowLeft]);
                        SET_GRAIN_VALID(buf[current]);

                        // CLEAR_GRAIN_ACTIVE(buf[belowLeft]);
                        continue;
                    }
                }else{
                    // look right
                    if (IS_EMPTY_GRAIN(buf[belowRight])){
                        buf[belowRight] = buf[current];
                        CLEAR_GRAIN(buf[current]);

                        SET_GRAIN_VALID(buf[belowRight]);
                        SET_GRAIN_VALID(buf[current]);

                        // CLEAR_GRAIN_ACTIVE(buf[belowRight]);
                        continue;
                    }
                }


                CLEAR_GRAIN_ACTIVE(buf[current]);
                SET_GRAIN_VALID(buf[current]);
                
            }
            else
            {
                SET_GRAIN_VALID(buf[current]);
            }
        }
    }
    for (idx = 0; idx < PIX_COUNT; idx++)
    {
        CLEAR_GRAIN_VALID(buf[idx]);
    }
}

int main()
{

    // Create UN socket
    int s1, s2;
    uint8_t msgBuffer[PIX_COUNT];

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
        // add_grain(msgBuffer, 0, 16, COLOR_BLACK);
        add_grain(msgBuffer, 0, 15, COLOR_RED);

        add_grain(msgBuffer, 31, 15, COLOR_RED);
        add_grain(msgBuffer, 30, 15, COLOR_RED);
        add_grain(msgBuffer, 29, 15, COLOR_RED);
        add_grain(msgBuffer, 28, 15, COLOR_RED);
        add_grain(msgBuffer, 27, 15, COLOR_RED);
        add_grain(msgBuffer, 26, 15, COLOR_RED);
        add_grain(msgBuffer, 25, 15, COLOR_RED);
        add_grain(msgBuffer, 24, 15, COLOR_RED);
        add_grain(msgBuffer, 23, 15, COLOR_RED);
        add_grain(msgBuffer, 22, 15, COLOR_RED);
        add_grain(msgBuffer, 21, 15, COLOR_RED);

        done = 0;
        do
        {
            printf("%d\n", (rand() % 2) == 0);
            for (int i = 0; i < 32; i++)
            {
                usleep(33 * 1000);
                update_grains(msgBuffer);

                if (send(s2, msgBuffer, sizeof(msgBuffer), 0) < 0)
                {
                    printf("Opps\n]");
                    perror("send:");
                    done = 1;
                    break;
                }
            }
            add_grain(msgBuffer, 0, 15, COLOR_RED);

        } while (!done);
        printf("\tClosing.\n");
        close(s2);
    }
    printf("Ending\n");
    return 0;

    // Write to shared memory
    // val++;
    // sprintf(memBuf, "%c", val);
    // strcpy((char *)ptrSharedMem, memBuf);
}