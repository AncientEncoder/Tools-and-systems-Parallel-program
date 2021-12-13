//formate to execute:
//./run  part_to_devide thread_to_use
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
double result = 0.0;  //Glb vals for rst
int intervalsDef=1;  //intervals Define default 1
int threadsDef=1;   //define threads default 1
double fullSegPart=1;
void *Generator(void *arg){
    double tempRst;
    double  segement;
    double rstByPart = 0;
    int  threadID = *((int *)arg);
    segement = fullSegPart / intervalsDef;
    for (int i = threadID; i < intervalsDef; i += threadsDef){
        tempRst = ((double)i + 0.5) * segement;
        rstByPart += 4.0 / (1 + tempRst * tempRst);
        
    }

    rstByPart *= segement;
    result += rstByPart;
    return NULL;
}

int main(int argc, char *argv[]){
    if (argc == 3){
        intervalsDef = atoi(argv[1]);
        threadsDef = atoi(argv[2]);
    }else{
        printf("->Warning running in default mod no args<-\n");
    }   
    struct timeval start;
    struct timeval end;
    gettimeofday(&start,NULL);
    pthread_t threads[threadsDef];
    int threadID[threadsDef];
    //Generate threads
    for (int i = 0; i < threadsDef; i++){
        threadID[i] = i;
        pthread_create(&threads[i], NULL, Generator, threadID + i);
    }
    //Execute threads
    for (int i = 0; i < threadsDef; i++){
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end,NULL);
    double elapsed_time = (end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;//redefine time from clock_t to real time 
    printf("%f \n", result);
    printf("Completed by using time: %f s\n", elapsed_time);
    return 0;
}
