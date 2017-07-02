#define   __LIBRARY__
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define ALLNUM 550
#define CUSTOMERNUM 5
#define BUFFERSIZE 10
void Producters(pid_t pid,FILE *fp);
void Customer(pid_t pid,FILE *fp);
sem_t *empty,*full,*mutex;
FILE *fp=NULL;
int Inpos=0;
int Outpos=0;


int main()
{
    int i,j,k;
    pid_t producter;
    pid_t customer;
    empty=(sem_t *)sem_open("empty",O_CREAT,0064,10);
    full=(sem_t *)sem_open("full",O_CREAT,0064,0);
    mutex=(sem_t*)sem_open("mutex",O_CREAT,0064,1);
    fp=fopen("products.txt","wb+");
    fseek(fp,10*sizeof(int),SEEK_SET);
    fwrite(&Outpos,sizeof(int),1,fp);
    fflush(fp);
    producter=fork();
    if(producter==0)
    {
        Producters(producter,fp);
    }
    for (i=0;i<CUSTOMERNUM;i++)
    {
        customer=fork();
        if(customer==0)
        {
            Customer(customer,fp);
        }
    }
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    //开了6个进程
    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("mutex");
    fclose(fp);
    return 0;
}
void Producters(pid_t pid,FILE *fp)
{
    int i=0;
    for (i=0;i<ALLNUM;i++)
    {
        sem_wait(empty);
        sem_wait(mutex);
        fseek( fp, Inpos * sizeof(int), SEEK_SET );
        fwrite(&i,sizeof(int),1,fp);
        fflush(fp);
        Inpos=(Inpos +1) % BUFFERSIZE;
        sem_post(mutex);
        sem_post(full);
    }
     return;
}

void Customer(pid_t pid,FILE *fp)
{
    int j,productid;
    for (j=0;j<ALLNUM/CUSTOMERNUM;j++)
    {
        sem_wait(full);
        sem_wait(mutex);
        fflush(stdout);
        fseek(fp,10*sizeof(int),SEEK_SET);
        fread(&Outpos,sizeof(int),1,fp);
        fseek(fp,Outpos*sizeof(int),SEEK_SET);
        fread(&productid,sizeof(int),1,fp);
        printf("%d:   %d\n",getpid(),productid);
        fflush(stdout);
        Outpos=(Outpos+1)% BUFFERSIZE;
        fseek(fp,10*sizeof(int),SEEK_SET);
        fwrite(&Outpos,sizeof(int),1,fp);
        fflush(fp);
        sem_post(mutex);
        sem_post(empty);
    }
    return;
}


