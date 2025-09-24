#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg){
    int thread_id = *((int*)arg);
    printf("Thread [%d] is running!\n", thread_id);
    return NULL;
}
int main(){
    pthread_t threads[2];
    int thread_args[2];
    int i;
    for(int i = 0; i < 2 ; i++){
        thread_args[i] = i + 1;
        if(pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0){
            perror("Failed to create thread\n");
        }
    }
    for (int i = 0; i < 2; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}