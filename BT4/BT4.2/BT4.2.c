#include <stdio.h>
#include <pthread.h>
long long counter = 0;
pthread_mutex_t lock;
void* increase_counter(void* arg){
    pthread_mutex_lock(&lock);
    counter += 1000000;
    printf("Thread [%d] increase counter 1000000!\n", (*(int*)arg));
    pthread_mutex_unlock(&lock);
    return NULL;
}
int main(){
    pthread_t threads[3];
    int thread[3];
    for (int i = 0; i<3 ; i++){
        thread[i] = i+1;
        if(pthread_create(&threads[i], NULL, increase_counter, &thread[i]) != 0){
            perror("Failed to create thread\n");
        }
    }
    for(int i = 0; i<3;i++){
        pthread_join(threads[i], NULL);
    }
    printf("Value of counter: %lld\n", counter);
}