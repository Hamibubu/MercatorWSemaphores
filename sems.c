#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// Number of processes
#define NPROCS 10
// Series count
#define SERIES_MEMBER_COUNT 200000

double *sums;
double x = 1.0;
double *res;

// Count the processes that are completed
int *proc_count;

// Semaphore to manage the starting of the procs
sem_t *start_all;
// Mutex for the proc count
sem_t *mutex_count;
// Semaphore to let the master process start
sem_t *done;

// Get member function
double get_member(int n, double x){
    int i;
    double numerator = 1;

    for(i=0; i<n; i++ )
        numerator = numerator*x;

    if (n % 2 == 0)
        return ( - numerator / n );
    else
        return numerator/n;
}

// Execute the procs
void proc(int proc_num){
    int i;

    //Waits for everyone
    sem_wait(start_all);
    sem_post(start_all);

    sums[proc_num] = 0;

    for(i=proc_num; i<SERIES_MEMBER_COUNT;i+=NPROCS)
        sums[proc_num] += get_member(i+1, x);

    // Mutex for the variable proc count
    sem_wait(mutex_count);
    (*proc_count)++;
    if (*proc_count == NPROCS){
        // If the final process ends running let the master begin
        sem_post(done);
    }
    sem_post(mutex_count);

    exit(0);
}

void master_proc(){
    int i;
    sleep(1);

    // Signal to start all the procs
    sem_post(start_all);

    // Wait until everyone ends
    sem_wait(done);

    *res = 0;

    for(i=0; i<NPROCS; i++)
        *res += sums[i];

    exit(0);
}

int main(){
    int *threadIdPtr;
    long long start_ts, stop_ts, elapsed_time;
    long lElapsedTime;
    struct timeval ts;
    int i, p, shmid;
    void *shmstart;

    shmid = shmget(0x1214, (NPROCS * sizeof(double) + sizeof(double) + sizeof(int)), 0666 | IPC_CREAT);
    shmstart = shmat(shmid,NULL,0);
    sums = shmstart;

    proc_count = shmstart + NPROCS * sizeof(double);
    res = shmstart + NPROCS * sizeof(double) + sizeof(int);

    // Semaphores initalization
    start_all=sem_open("/start", O_CREAT, 0644, 0);
    done=sem_open("/done", O_CREAT, 0644, 0);
    mutex_count=sem_open("/mutex", O_CREAT, 0644, 1);

    *proc_count = 0;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec; // Initial time

    for(i=0; i<NPROCS;i++){
        p = fork();
        if(p==0)
            proc(i);
    }

    p = fork();

    if(p==0)
        master_proc();

    printf("\n\033[34m------------------------------------------------------------------------\n");
    printf("| \033[37m[i] The count of ln(1 + x) members of the Mercator series is \033[31m%d \033[34m |\n", SERIES_MEMBER_COUNT); 
    printf("| \033[37m[i] The value of the argument x is \033[31m%f \033[34m                         |\n", (double)x);
    printf("------------------------------------------------------------------------\n\033[0m\n");

    for(int i=0;i<NPROCS+1;i++)
        wait(NULL);

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec; // Final time
    elapsed_time = stop_ts - start_ts;
    printf("\033[0;36m---\033[0;32m [\033[0;37m!\033[0;32m] Time: \033[0;37m%lld seconds\n\033[0m", elapsed_time);
    printf("\033[0;36m---\033[0;32m [\033[0;37m+\033[0;32m] The result: \033[0;37m%10.8f\n\033[0m", res);
    printf("\033[0;36m---\033[0;32m [\033[0;37m+\033[0;32m] Calling \033[0;37mfunction\033[0;32m ln(1 + %f) = \033[0;37m%10.8f\n\n\033[0m", x, log(1 + x));

    sem_close(start_all); // Close and unlink the semaphores
    sem_close(mutex_count);
    sem_close(done);
    sem_unlink("/start");
    sem_unlink("/done");
    sem_unlink("/mutex");

    shmdt(shmstart);
    shmctl(shmid,IPC_RMID,NULL);
}