#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 256

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char message[BUFFER_SIZE];
int message_ready = 0;

void* server_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // 메시지가 준비될 때까지 대기
        while (!message_ready) {
            pthread_cond_wait(&cond, &mutex);
        }

        // 메시지를 모든 클라이언트에게 방송
        printf("Server broadcasting: %s\n", message);

        // 메시지 처리 완료 표시
        message_ready = 0;

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* client_thread(void* arg) {
    int client_id = *((int*)arg);
    char local_message[BUFFER_SIZE];

    while (1) {
        // 클라이언트에서 메시지를 생성
        sleep(rand() % 5 + 1);  // 랜덤 대기 시간
        snprintf(local_message, BUFFER_SIZE, "Message from client %d", client_id);

        pthread_mutex_lock(&mutex);

        // 메시지를 공유 버퍼에 복사
        strncpy(message, local_message, BUFFER_SIZE);
        message_ready = 1;

        printf("Client %d sent: %s\n", client_id, local_message);

        // 서버에 신호 전송
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t server;
    pthread_t clients[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];

    // 서버 쓰레드 생성
    pthread_create(&server, NULL, server_thread, NULL);

    // 클라이언트 쓰레드 생성
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_ids[i] = i + 1;
        pthread_create(&clients[i], NULL, client_thread, &client_ids[i]);
    }

    // 클라이언트 쓰레드 대기
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(clients[i], NULL);
    }

    // 서버 쓰레드 대기
    pthread_join(server, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
