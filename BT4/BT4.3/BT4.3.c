#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


int data = 0;
int data_ready = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

// Hàm producer
void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        // Tạo số ngẫu nhiên
        int value = rand() % 100;

        pthread_mutex_lock(&lock);

        // Ghi dữ liệu và bật cờ
        data = value;
        data_ready = 1;

        printf("Producer produced: %d\n", data);

        // Báo hiệu cho Consumer
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&lock);

        sleep(1); // mô phỏng thời gian sản xuất
    }
    return NULL;
}

// Hàm consumer
void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&lock);

        // Chờ cho đến khi có dữ liệu
        while (data_ready == 0) {
            pthread_cond_wait(&cond, &lock);
        }

        // Đọc dữ liệu và tắt cờ
        printf("Consumer consumed: %d\n", data);
        data_ready = 0;

        pthread_mutex_unlock(&lock);

        sleep(2); // mô phỏng thời gian tiêu thụ
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    srand(time(NULL));

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    // Tạo luồng
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Chờ các luồng kết thúc
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
