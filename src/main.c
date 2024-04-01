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
    #define PIX_COUNT ROW_MAX*COL_MAX

typedef enum {
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

BG_CODES_e create_unix_socket(int* sock){

    *sock = socket(AF_UNIX, SOCK_STREAM, 0 );
    if(*sock == -1){
        perror("Unable to create socket:");
        sock = NULL;
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e bind_unix_socket(int sock, const char* name){
    // int len;

    struct sockaddr_un local = {
        .sun_family = AF_UNIX,
        // .sun_path = SOCK_PATh,
    };
    strncpy(local.sun_path, name, sizeof(local.sun_path) - 1);
    // printf("In bind: %s.\n", local.sun_path);
    unlink(local.sun_path);

    if(-1 == bind(sock, (struct sockaddr *)&local, sizeof(local))){
        perror("Unable to bind socket:");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e listen_unix_socket(int sock, int count){

    if(-1 == listen(sock, count)){
        perror("Error listening:");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

#define COLOR_WHITE 0
#define COLOR_BLACK 1
#define COLOR_RED 2
#define COLOR_BLUE 3

void init_canvas(char* buf, int color){
    int i;
    for(i=0; i < PIX_COUNT; i++){
        buf[i] = color;
    }
}




int main(){

    // Create UN socket
    int s1, s2;
    char msgBuffer[PIX_COUNT];

    char *symlinkpath = "test_socket2";
    char actualpath [PATH_MAX+1];

    init_canvas(msgBuffer, COLOR_WHITE);


    if(BG_SUCCESS != create_unix_socket(&s1)){
        return -1;
    }

    //Bind
    // if(NULL == realpath(symlinkpath, actualpath)){
    //     perror("Error resolving path:");
    //     return -1;
    // }

    if(BG_SUCCESS != bind_unix_socket(s1, symlinkpath)){
        return -1;
    }   

    //Listen
    if(BG_SUCCESS != listen_unix_socket(s1, 1)){
        return -1;
    }


    struct sockaddr_un remote = {
        .sun_family = AF_UNIX,
    };

    signal(SIGPIPE, SIG_IGN);

    // Begin loop
    // from https://beej.us/guide/bgipc/html/#unixsock
    while(1){
        int done, n;
        printf("Waiting for connection\n");
        socklen_t slen = sizeof(remote);
        if ((s2 = accept(s1, (struct sockaddr *)&remote, &slen)) == -1){
            perror("accept:");
            exit(1);
        }

        printf("Connected.\n");
        init_canvas(msgBuffer, COLOR_WHITE);

        done = 0;
        do{
            printf(".\n");
            for(int i = 0; i<PIX_COUNT; i++){
                usleep(33*1000);
                msgBuffer[i] = COLOR_BLACK;

                if(send(s2, msgBuffer, sizeof(msgBuffer), 0) < 0){
                    printf("Opps\n]");
                    perror("send:");
                    done = 1;
                    break;
                }
            }

        } while(!done);
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