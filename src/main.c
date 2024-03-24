// from https://realpython.com/python-bindings-overview/#c-or-c-source
#include <stdio.h>

#include <sys/types.h>

#include <semaphore.h>

// POSIX shared memory
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

typedef enum {
    BG_FAIL = 0,
    BG_SUCCESS = 1
} BG_CODES_e;

/*
// Create shared memory object
// TODO tests
//  - create same named shm twice (EEXIST)
//  - invalid name argument (EINVAL)
//  - per-process limit on descriptions reached (EMFILE)
//  - system-wide limit on descriptors reached (ENFILE)
//  - length of name exceeds PATH_MAX (ENAMETOOLONG)
*/  

BG_CODES_e create_shared_memory(const char* name, int* fdSharedMem){

    int fd;
    /*
    Obj w/ R/W, and only if doesn't exist
    User R/W permissions (0600) (https://man7.org/linux/man-pages/man2/open.2.html)
    https://man7.org/linux/man-pages/man3/shm_open.3.html
    */
    fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if(fd == -1){
        perror("Unable to create shared memory.");
        return BG_FAIL;
    }

    *fdSharedMem = fd;

    return BG_SUCCESS;
}

/*
//  Resize shared memory. Need to do this after creating else will be size zero
//  TODO tests:
//      - length < 0 (EINVAL)
//      - length > max file size (EFBIG or EINVAL)
//      - fd not open for writing (EBADF or EINVAL)
*/
BG_CODES_e resize_shared_memory(int fdSharedMem, off_t size){
    if(0 != ftruncate(fdSharedMem, size)){
        perror("Unable to resize shared memory.");
        return BG_FAIL;
    }

    return BG_SUCCESS;

}


int main(){
    sem_t* semaphore = NULL;
    void* sharedMem = NULL;

    int fdMem;

    if(BG_SUCCESS != create_shared_memory("test", &fdMem)){
        return -1;
    }

    printf("Created shared memory\n");

    if(BG_SUCCESS != resize_shared_memory(fdMem, 1024)){
        return -1;
    }
    printf("Resized shared memory\n");


}