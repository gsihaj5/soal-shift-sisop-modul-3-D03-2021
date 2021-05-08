#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int (*main_matrix)[6],
    matrix_user[4][6];
unsigned long int factorial_result[4][6];

void setupSharedMemory() {
    key_t key = 1111;
    //size of 4 * 6 -> dari perkalian matrix soal 2a
    int s_memory_id = shmget(key, sizeof(int[4][6]), 0666 | IPC_CREAT);

    //attach matrix to shared memory
    main_matrix = shmat(s_memory_id, (void *)0, 0);
}

void print_main_matrix() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            printf("%d ", main_matrix[row][col]);
        }
        printf("\n");
    }
}

void print_result_matrix() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            printf("%ld ", factorial_result[row][col]);
        }
        printf("\n");
    }
}

void get_matrix_from_user() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            scanf("%d", &matrix_user[row][col]);
        }
    }
}

unsigned long int factorial(int num) {
    int i;
    unsigned long int result = 1;
    for (i = 1; i <= num; i++) {
        result *= i;
    }
    return result;
}

void *doFactorial(void *argument) {
    unsigned long int result;
    int *arg = (int *)argument;

    int a = arg[0],
        b = arg[1];

    if (a >= b) {
        result = factorial(a) / factorial(a - b);
    } else if (a < b) {
        result = factorial(a);
    } else if (a == 0 || b == 0) {
        result = 0;
    }

    printf("calculating on %d, %d -> a = %d, b = %d result is %ld\n", arg[2], arg[3], a, b, result);

    factorial_result[arg[2]][arg[3]] = result;
    return NULL;
}

void calculate_all_pair() {
    pthread_t thread[4][6];
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            int arguments[4];
            arguments[0] = main_matrix[row][col];
            arguments[1] = matrix_user[row][col];
            arguments[2] = row;
            arguments[3] = col;

            printf("created thread on %d, %d\n", row, col);
            pthread_create(&thread[row][col], NULL, doFactorial, &arguments);
            sleep(1);
        }
    }

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            pthread_join(thread[row][col], NULL);
        }
    }

    return;
}

int main() {
    setupSharedMemory();
    print_main_matrix();
    get_matrix_from_user();
    calculate_all_pair();
    print_result_matrix();

    shmdt(main_matrix);
}

/*
1 1 1 1 1 1 
2 2 2 2 2 2
3 3 3 3 3 3
4 4 4 4 4 4

*/