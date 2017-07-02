#define   __LIBRARY__
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define   Total        500
#define   BUFFERSIZE   10

int main()
{
    int shmid;
    int get_pos = 0, i;
    int *ShareAddress;
    sem_t *empty, *full, *mutex;

    empty = (sem_t *)sem_open("empty", O_CREAT, 0777, 10);
    full  = (sem_t *)sem_open("full", O_CREAT, 0777, 0);
    mutex = (sem_t *)sem_open("mutex",O_CREAT, 0777, 1);

    shmid = shmget( 555204, BUFFERSIZE*sizeof(int), IPC_CREAT|0666 );  /*返回共享内存的标识符*/
  /*  if(!shmid)
    {    
         printf("shmget failed!");
         fflush(stdout);
         exit(0);
    }
*/
    ShareAddress = (int*)shmat(shmid, NULL, 0);
 /*   
    if(!ShareAddress)
    {
         printf("shmat failed!");
         fflush(stdout);
         exit(0);
    }
*/
    for(i = 0; i < Total; i++)
    {
        sem_wait(full);
        sem_wait(mutex);
        
        
        printf("%d\n", ShareAddress[get_pos]);
        fflush(stdout);
        get_pos = ( get_pos + 1 ) % BUFFERSIZE;
        
        sem_post(mutex);
        sem_post(empty);
    }

    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("mutex");
    
    return 0;
}