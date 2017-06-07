#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
  int position;
  int count;
  sem_t *chopsticks;
  sem_t *lock;
} params_t;

void initialize_semaphores(sem_t *lock, sem_t *chopsticks, int num_chopsticks);
void run_all_threads(pthread_t *threads, sem_t *chopsticks, sem_t *lock, int num_philosophers);

void *philosopher(void *params);
void think(int position);
void eat(int position);
int askForNumber(){
    int i;
    printf( "Enter a number:");
    scanf("%d", &i);
    return i;
}
int main()
{
  int num_philosophers = askForNumber();

  sem_t lock;
  sem_t chopsticks[num_philosophers];
  pthread_t philosophers[num_philosophers];

  int i;
  for(i = 0; i < num_philosophers; i++) {
    sem_init(&chopsticks[i], 0, 1);
  }
  sem_init(&lock, 0, num_philosophers - 1);

  //run_all_threads
  for(i = 0; i < num_philosophers; i++) {
    params_t *arg = (params_t*)malloc(sizeof(params_t));
    arg->position = i;
    arg->count = num_philosophers;
    arg->lock = &lock;
    arg->chopsticks = chopsticks;

    pthread_create(&philosophers[i], NULL, philosopher, (void *)arg);
  }
  pthread_exit(NULL);
}

void *philosopher(void *params)
{
  int i;
  params_t me = *(params_t *)params;

  for(i=0;i<100;i++){
    think(me.position);

    sem_wait(me.lock);
    sem_wait(&me.chopsticks[me.position]);
    sem_wait(&me.chopsticks[(me.position + 1) % me.count]);
    eat(me.position);
    sem_post(&me.chopsticks[me.position]);
    sem_post(&me.chopsticks[(me.position + 1) % me.count]);
    sem_post(me.lock);
  }

  think(me.position);
  pthread_exit(NULL);
}

void think(int position)
{
  printf("I am philosopher %d thinking.\n", position+1);
}

void eat(int position)
{
  printf("I am philosopher %d eating.\n", position+1);
}
