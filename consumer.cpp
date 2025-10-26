//Taylor Dubois 
//10/20/25

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

struct SharedData{
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
};

int main(){
    int shm_fd = shm_open("/table", O_RDWR, 0666);
    void* ptr = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    SharedData* table = static_cast<SharedData*>(ptr);

    sem_t* empty = sem_open("/empty", 0);
    sem_t* full  = sem_open("/full", 0);
    sem_t* mutex = sem_open("/mutex", 0);

    for(int i = 0; i < 10; i++){
        sem_wait(full);
        sem_wait(mutex);

        int item = table->buffer[table->out];
        std::cout << "Consumed: " << item << "\n";
        table->out = (table->out + 1) % BUFFER_SIZE;
        table->count--;

        sem_post(mutex);
        sem_post(empty);
        sleep(2);
    }

    munmap(ptr, sizeof(SharedData));
    close(shm_fd);
    return 0;
}

