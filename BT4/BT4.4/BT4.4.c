#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Tài nguyên chia sẻ
int shared_data = 0;

// Khóa đọc-ghi
pthread_rwlock_t rwlock;

// Hàm Reader
void* reader(void* arg) {
    int id = *((int*)arg);
    while (1) {
        pthread_rwlock_rdlock(&rwlock);   // khóa đọc
        printf("Reader %d read value: %d\n", id, shared_data);
        pthread_rwlock_unlock(&rwlock);   // mở khóa
        usleep(200000); // ngủ 200ms mô phỏng công việc đọc
    }
    return NULL;
}

// Hàm Writer
void* writer(void* arg) {
    int id = *((int*)arg);
    while (1) {
        pthread_rwlock_wrlock(&rwlock);   // khóa ghi (độc quyền)
        shared_data++;
        printf("Writer %d wrote value: %d\n", id, shared_data);
        pthread_rwlock_unlock(&rwlock);   // mở khóa
        sleep(1); // mô phỏng công việc ghi chậm hơn
    }
    return NULL;
}

int main() {
    pthread_t readers[5], writers[2];
    int reader_ids[5], writer_ids[2];

    pthread_rwlock_init(&rwlock, NULL);

    // Tạo 5 Reader
    for (int i = 0; i < 5; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Tạo 2 Writer
    for (int i = 0; i < 2; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Chờ (trong thực tế sẽ không bao giờ kết thúc)
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
