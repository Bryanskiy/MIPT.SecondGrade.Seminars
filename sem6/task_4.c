#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int arr[3] = {0, 0, 0};
size_t iter_count = 10e7;

void* increment_02(void* tmp) {
    for(size_t i = 0; i < 10e7; ++i) {
        ++arr[0];
        ++arr[2];
    }
}

void* increment_12(void* tmp) {
    for(size_t i = 0; i < 10e7; ++i) {
        ++arr[1];
        ++arr[2];
    }
}

int main() {
    pthread_t current, second, third;
    int       second_id, third_id;

    second_id = pthread_create( &second, (pthread_attr_t *)NULL, increment_02, NULL);
    third_id = pthread_create( &third, (pthread_attr_t *)NULL, increment_12, NULL);
    if(second_id != 0){
        printf ("Error on thread create, return value = %d\n", second_id);
        exit(-1);
    } 

   if(third_id != 0){
      printf ("Error on thread create, return value = %d\n", second_id);
      exit(-1);
   } 
   
   pthread_join(second, (void **)NULL);
   pthread_join(third, (void **)NULL);

    printf("%d %d %d", arr[0], arr[1], arr[2]);
   return 0;   
}