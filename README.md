This program shows how a producer and a consumer can share data using shared memory and semaphores in C++. 
The producer creates random integer values and places them into a shared buffer. 
The consumer then removes these values from the buffer. 
Semaphores are used to ensure mutual exclusion so that they do not access the shared buffer at the same time. 
The program also creates and removes shared memory and semaphores to prevent leaks.

To compile the programs, open a terminal and open the producer_consumer folder. Then use these commands:
g++ producer.cpp -pthread -o producer
g++ consumer.cpp -pthread -o consumer
After compilation, run both programs at the same time so they can communicate through shared memory using 
this command:
./producer & ./consumer &

Shared memory is used so that both the producer and consumer can use the same buffer even though they are separate processes. 
The shm_open() function creates or opens a shared memory object, while mmap() connects it to the program so both can read and write to it. 
Semaphores are used to control access to this shared memory. 
The mutex semaphore makes sure only one process can modify the buffer at a time. 
The empty and full semaphores keep track the number of empty and filled slots in the buffer, so that the producer does not add to a full buffer and the consumer does not read from an empty one.
