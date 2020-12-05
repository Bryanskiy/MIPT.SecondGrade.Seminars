#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;

void *thread_create(void *dummy)
{
   pthread_t mythid;
   mythid = pthread_self();
   printf("Thread %lu, was generated\n", mythid);
   return NULL;
}

int main()
{
   pthread_t current, second, third; 
   int       second_id, third_id;

   second_id = pthread_create( &second, (pthread_attr_t *)NULL, thread_create, NULL);
   third_id = pthread_create( &third, (pthread_attr_t *)NULL, thread_create, NULL);
   if(second_id != 0){
      printf ("Error on thread create, return value = %d\n", second_id);
      exit(-1);
   } 

   if(third_id != 0){
      printf ("Error on thread create, return value = %d\n", second_id);
      exit(-1);
   } 
   
   current = pthread_self();
   
   printf("Main thread %lu\n", current);
 
   pthread_join(second, (void **)NULL);
   pthread_join(third, (void **)NULL);
   return 0;
}