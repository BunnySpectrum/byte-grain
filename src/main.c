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

typedef enum {
    BG_FAIL = 0,
    BG_SUCCESS,
    BG_TIMEOUT,
} BG_CODES_e;

#define SHM_NAME "bg_shm"
#define SEM_NAME "bg_sem"
#define SHM_SIZE 1024

#define SOCK_PATH "~/sockets/test"

/*
// Create shared memory object
// TODO tests
//  - create same named shm twice (EEXIST)
//  - invalid name argument (EINVAL)
//  - per-process limit on descriptions reached (EMFILE)
//  - system-wide limit on descriptors reached (ENFILE)
//  - length of name exceeds PATH_MAX (ENAMETOOLONG)
*/  

BG_CODES_e create_shared_memory(const char* name, int* fdMem){

    int fd;
    /*
    Obj w/ R/W, and only if doesn't exist
    User R/W permissions (0600) (https://man7.org/linux/man-pages/man2/open.2.html)
    https://man7.org/linux/man-pages/man3/shm_open.3.html
    */
    // fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd == -1){
        perror("Unable to create shared memory:");
        return BG_FAIL;
    }

    *fdMem = fd;

    return BG_SUCCESS;
}

BG_CODES_e unlink_shared_memory(const char* name){

    if(0 == shm_unlink(name)){
        perror("Unable to unlink shared memory.");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

/*
//  Resize shared memory. Need to do this after creating else will be size zero
//  TODO tests:
//      - length < 0 (EINVAL)
//      - length > max file size (EFBIG or EINVAL)
//      - fd not open for writing (EBADF or EINVAL)
*/
BG_CODES_e resize_shared_memory(int fdMem, off_t size){
    if(0 != ftruncate(fdMem, size)){
        perror("Unable to resize shared memory");
        return BG_FAIL;
    }

    return BG_SUCCESS;

}

BG_CODES_e mmap_shared_memory(int fdMem, size_t size, void** ptrMem){

    *ptrMem = mmap((void*)0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, 0);

    if(MAP_FAILED == ptrMem){
        perror("Unable to mmap shared memory.");
        *ptrMem = NULL;
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e munmap_shared_memory(void* ptrMem, size_t size){

    if(0 == munmap(ptrMem, size)){
        perror("Unable to m-unmap shared memory.");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}


BG_CODES_e semaphore_open(const char* name, sem_t** ptrSem){

    *ptrSem = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, 0);

    if(MAP_FAILED == *ptrSem){
        perror("Unable to open semamphore.");
        *ptrSem = NULL;
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e semaphore_close(sem_t* ptrSem){

    if(0 == sem_close(ptrSem)){
        perror("Unable to close semamphore.");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e semaphore_unlink(const char* name){

    if(0 == sem_unlink(name)){
        perror("Unable to unlink semamphore.");
        return BG_FAIL;
    }

    return BG_SUCCESS;
}



// up == V, release, signal, post, vacate
BG_CODES_e semaphore_up(sem_t* ptrSem){

    if(0 != sem_post(ptrSem)){
        perror("Unable to increase semaphore.");
        return BG_FAIL;
    }
    return BG_SUCCESS;
}

// down == P, acquire, wait, pend, procure
BG_CODES_e semaphore_down(sem_t* ptrSem){

    if(0 != sem_wait(ptrSem)){
        perror("Unable to increase semaphore.");
        return BG_FAIL;
    }
    return BG_SUCCESS;
}


/*
    Unix domain sockets
*/

BG_CODES_e create_unix_socket(int* sock){

    *sock = socket(AF_UNIX, SOCK_STREAM, 0 );
    if(*sock == -1){
        perror("Unable to create socket:");
        *sock = NULL;
        return BG_FAIL;
    }

    return BG_SUCCESS;
}

BG_CODES_e bind_unix_socket(int sock, const char* name){
    int len;

    struct sockaddr_un local = {
        .sun_family = AF_UNIX,
        // .sun_path = SOCK_PATh,
    };
    strcpy(local.sun_path, name);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);

    if(-1 == bind(sock, (struct sockaddr *)&local, len)){
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








int main(){
    sem_t* ptrSem = NULL;
    void* ptrSharedMem = NULL;
    int fdMem;
    char memBuf[SHM_SIZE];
    char val = 97; //a

/*
    // Create shared memory
    unlink_shared_memory(SHM_NAME);
    // return -1;
    if(BG_SUCCESS != create_shared_memory(SHM_NAME, &fdMem)){
        return -1;
    }
    printf("Created shared memory: %d\n", fdMem);
    // close(fdMem);
    // return -1;

    // Size memory
    if(BG_SUCCESS != resize_shared_memory(fdMem, (off_t)SHM_SIZE)){
        return -1;
    }
    printf("Resized shared memory\n");

    //Mmap
    if(BG_SUCCESS != mmap_shared_memory(fdMem, SHM_SIZE, &ptrSharedMem)){
        return -1;
    }
    printf("Mmap shared memory: %p\n", ptrSharedMem);


    // Create semaphore
    if(BG_SUCCESS != semaphore_open(SEM_NAME, &ptrSem)){
        return -1;
    }
    printf("Opened semaphore\n");


    // Write initial value to shared memory
    printf("Writing initial value\n");
    sprintf(memBuf, "%c", val);
    strcpy((char *)ptrSharedMem, memBuf);


    // Release semaphore
    if(BG_SUCCESS != semaphore_up(ptrSem)){
        printf("Error initial sem up\n");
        goto CLEANUP;
    }
    printf("Initial semaphore increment\n");
*/

    // Create UN socket
    int s1;
    if(BG_SUCCESS != create_unix_socket(&s1)){
        return -1;
    }

    //Bind
    if(BG_SUCCESS != bind_unix_socket(s1, SOCK_PATH)){
        return -1;
    }   

    //Listen
    if(BG_SUCCESS != listen_unix_socket(s1, 1)){
        return -1;
    }



    // Begin loop
    for(int idx=0; idx<10; idx++){
        printf("Loop %d.\n", idx);

        // Acquire/decrement semaphore
        if(BG_SUCCESS != semaphore_down(ptrSem)){
            printf("(%d) Error sem down\n", __LINE__);
            goto CLEANUP;
        }

        // Write to shared memory
        val++;
        sprintf(memBuf, "%c", val);
        strcpy((char *)ptrSharedMem, memBuf);


        // Release/increment semaphore
        if(BG_SUCCESS != semaphore_up(ptrSem)){
            printf("(%d) Error sem up\n", __LINE__);
            goto CLEANUP;
        }

        //Wait 5 sec
        sleep(5);
    }

CLEANUP:
    // Acquire/decrement semaphore
    if(BG_SUCCESS != semaphore_down(ptrSem)){
        printf("(%d) Cleanup error sem down\n", __LINE__);
    }


    // m-unmap memory
    if(BG_SUCCESS != munmap_shared_memory(ptrSharedMem, SHM_SIZE)){
        return -1;
    }
    printf("M-unmaped shared memory\n");


    // close file descriptor
    if (-1 == close(fdMem)) {
        perror("Error closing fd");
    }


    // unlink shared memory
    if(BG_SUCCESS != unlink_shared_memory(SHM_NAME)){
        return -1;
    }
    printf("Unlinked shared memory\n");


    // close semaphore
    if(BG_SUCCESS != semaphore_close(ptrSem)){
        return -1;
    }
    printf("Closed semaphore\n");


    // unlinke semaphore
    if(BG_SUCCESS != semaphore_unlink(SEM_NAME)){
        return -1;
    }
    printf("Unlinked semaphore\n");


    printf("Fin\n");


}