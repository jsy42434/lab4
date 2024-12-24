
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5 // 생성할 쓰레드의 수

// 쓰레드에서 실행할 함수
void* print_message(void* thread_id) {
    long tid = (long)thread_id; // 전달받은 thread_id
    printf("쓰레드 %ld: 작업 시작\n", tid);
    
    // 각 쓰레드가 특정 작업 수행 (여기서는 간단히 sleep 사용)
    sleep(1 + tid % 3); // 1~3초 대기
    printf("쓰레드 %ld: 작업 완료\n", tid);

    pthread_exit(NULL); // 쓰레드 종료
}

int main() {
    pthread_t threads[NUM_THREADS]; // 쓰레드 ID를 저장할 배열
    int result;
    long t;

    // 쓰레드 생성
    for (t = 0; t < NUM_THREADS; t++) {
        printf("메인: 쓰레드 %ld 생성 요청\n", t);
        result = pthread_create(&threads[t], NULL, print_message, (void*)t);

        if (result) {
            fprintf(stderr, "쓰레드 생성 실패: %d\n", result);
            exit(-1);
        }
    }

    // 모든 쓰레드가 종료될 때까지 대기
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
        printf("메인: 쓰레드 %ld 종료 확인\n", t);
    }

    printf("모든 쓰레드가 완료되었습니다.\n");
    pthread_exit(NULL);
}

