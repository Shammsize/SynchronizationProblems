#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>

void *reader(void*param);
void *writer(void*param);
int *myBuffer;
int readCounter=0;
int bufferSize,count=0, count2;
sem_t writerSem, binary;

int askForNumber(){
    int i;
    printf( "Enter a number:");
    scanf("%d", &i);
    return i;
}
using namespace std;
int main(int argc, char const *argv[]) {
  cout<<"Size of buffer\n";
  bufferSize= askForNumber();
  count2= bufferSize/2;
  myBuffer= (int*)calloc(bufferSize,sizeof(int));
  sem_init(&binary,0,1);
  sem_init(&writerSem,0,1);
  // thread identifier
  pthread_t tid[2];
  //thread attributes
  pthread_attr_t attr;
  // default attributes
  pthread_attr_init(&attr);
  //create pthreadh
  pthread_create(&tid[0], &attr,reader, NULL);
  pthread_create(&tid[1], &attr,writer, NULL);
  pthread_join(tid[0],NULL);
  pthread_join(tid[1],NULL);

  return 0;
}

void *reader(void* param){
  do {
    sem_wait(&binary);
    readCounter++;
    if(readCounter==1){
      sem_wait(&writerSem);
    }
    sem_post(&binary);
    printf("Reading %d from %d\n",myBuffer[count2],count2);
    count2++;
    if(count2==bufferSize){
      count2=0;
    }
    sem_wait(&binary);
    readCounter--;
    if(readCounter==0){
      sem_post(&writerSem);
    }
    sem_post(&binary);
  } while(1);
}

void *writer(void* param){
  do {
    sem_wait(&writerSem);
    int i= rand()%50+1;
    printf("Writting %d to position %d\n",i,count);
    myBuffer[count]= i;
    count++;
    if(count==bufferSize){
      count=0;
    }
    sem_post(&writerSem);
  } while(1);
}
