#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>

void *producer(void*param);
void *consumer(void*param);
int *myBuffer;
int itemCounter=0;
int bufferSize;
sem_t empty,full, binary;

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
  myBuffer= (int*)calloc(bufferSize,sizeof(int));
  sem_init(&empty,0,bufferSize);
  sem_init(&full,0,0);
  sem_init(&binary,0,1);
  // thread identifier
  pthread_t tid[2];
  //thread attributes
  pthread_attr_t attr;
  // default attributes
  pthread_attr_init(&attr);
  //create pthreadh
  pthread_create(&tid[0], &attr,producer, NULL);
  pthread_create(&tid[1], &attr,consumer, NULL);
  pthread_join(tid[0],NULL);
  pthread_join(tid[1],NULL);

  return 0;
}
int produceItem(){
  return (rand()%50)+1;
}
void addItem(int item, int bufferSize){
  int x;
  for(x=0; x< bufferSize; x++){
    if(myBuffer[x]==0){
      printf("Produced %d to position %d\n",item,x );
      myBuffer[x]=item;
      break;
    }
  }
}
void consumeItem(){
  int x;
  for(x=0; x< bufferSize; x++){
    if(myBuffer[x]!=0){
      printf("Consumed Item %d in position %d\n",myBuffer[x],x );
      myBuffer[x]=0;
      break;
    }
  }
}
void *producer(void* param){
  do {
    int item= produceItem();
    sem_wait(&empty);
    sem_wait(&binary);
    addItem(item,bufferSize);
    sem_post(&binary);
    sem_post(&full);
  } while(1);
}

void *consumer(void* param){
  do {
    sem_wait(&full);
    sem_wait(&binary);
    consumeItem();
    sem_post(&binary);
    sem_post(&empty);
  } while(1);
}
