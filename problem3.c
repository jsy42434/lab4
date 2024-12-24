#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5 // 버퍼의 크기
#define PRODUCERS 2   // 생산자 수
#define CONSUMERS 2   // 소비자 수

// 버퍼와 관련된 전역 변수
int buffer[BUFFER_SIZE];
int count = 0; // 버퍼 내 아이템 수
int in = 0;    // 다음에 추가할 위치
int out = 0;   // 다음에 제거할 위치

// 동기화 도구
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

// 생산자 함수
void* producer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        int item = rand() % 100; // 임의의 아이템 생성
        pthread_mutex_lock(&mutex);

        // 버퍼가 가득 찬 경우 대기
        while (count == BUFFER_SIZE) {
            printf("생산자 %d: 버퍼가 가득 찼습니다. 대기 중...\n", id);
            pthread_cond_wait(&not_full, &mutex);
        }

        // 아이템을 버퍼에 추가
        buffer[in] = item;
        printf("생산자 %d: 아이템 %d를 버퍼에 추가 (버퍼 위치 %d)\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        // 소비자를 깨움
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3); // 랜덤 대기 시간
    }

    pthread_exit(NULL);
}

// 소비자 함수
void* consumer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        // 버퍼가 비어 있는 경우 대기
        while (count == 0) {
            printf("소비자 %d: 버퍼가 비어 있습니다. 대기 중...\n", id);
            pthread_cond_wait(&not_empty, &mutex);
        }

        // 버퍼에서 아이템 제거
        int item = buffer[out];
        printf("소비자 %d: 아이템 %d를 버퍼에서 제거 (버퍼 위치 %d)\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        // 생산자를 깨움
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3); // 랜덤 대기 시간
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producers[PRODUCERS], consumers[CONSUMERS];
    int producer_ids[PRODUCERS], consumer_ids[CONSUMERS];

    // 생산자 쓰레드 생성
    for (int i = 0; i < PRODUCERS; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // 소비자 쓰레드 생성
    for (int i = 0; i < CONSUMERS; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // 쓰레드 종료 대기 (무한 루프라서 도달하지 않음)
    for (int i = 0; i < PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    return 0;
}
