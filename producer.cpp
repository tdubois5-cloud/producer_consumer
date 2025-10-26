//Taylor Dubois
//10/20/25

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstdlib>
#include <ctime>

#define BUFFER_SIZE 5

struct SharedData{
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
};

int main(){
    std::srand(std::time(nullptr));
    int shm_fd = shm_open("/table", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedData));
    void* ptr = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    SharedData* table = static_cast<SharedData*>(ptr);
    table->in = table->out = table->count = 0;

    sem_t* empty = sem_open("/empty", O_CREAT, 0666, BUFFER_SIZE);
    sem_t* full  = sem_open("/full",  O_CREAT, 0666, 0);
    sem_t* mutex = sem_open("/mutex", O_CREAT, 0666, 1);

    for(int i = 0; i < 10; i++){
        int item = rand() % 100;
        sem_wait(empty);
        sem_wait(mutex);

        table->buffer[table->in] = item;
        std::cout << "Produced: " << item << "\n";
        table->in = (table->in + 1) % BUFFER_SIZE;
        table->count++;

        sem_post(mutex);
        sem_post(full);  
        sleep(1);
    }

//Cleanup
    shm_unlink("/table");
    sem_unlink("/empty");
    sem_unlink("/full");
    sem_unlink("/mutex");

    munmap(ptr, sizeof(SharedData));
    close(shm_fd);
    return 0;
}

