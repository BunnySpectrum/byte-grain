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
    BG_SUCCESS,
    BG_TIMEOUT,
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
BG_CODES_e resize_shared_memory(int fdSharedMem, off_t size){
    if(0 != ftruncate(fdSharedMem, size)){
        perror("Unable to resize shared memory.");
        return BG_FAIL;
    }

    return BG_SUCCESS;

}

BG_CODES_e mmap_shared_memory(int fdMem, size_t size, void* ptrMem){

    ptrMem = mmap((void*)0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fdMem, 0);

    if(MAP_FAILED == ptrMem){
        perror("Unable to mmap shared memory.");
        ptrMem = NULL;
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


BG_CODES_e semaphore_open(const char* name, sem_t* ptrSem){

    ptrSem = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, 0);

    if(MAP_FAILED == ptrSem){
        perror("Unable to open semamphore.");
        ptrSem = NULL;
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