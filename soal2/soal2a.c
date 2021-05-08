#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
1 1 1
2 2 2
1 1 1
3 3 3
1 1 1 1 1 1
2 2 2 2 2 2
3 3 3 3 3 3
*/

#include <unistd.h>

int matrix_a[4][3],
    matrix_b[3][6],
    (*main_matrix)[6];

void input_matrix_a() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 3; col++) {
            scanf("%d", &matrix_a[row][col]);
        }
    }
}

void input_matrix_b() {
    int row, col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 6; col++) {
            scanf("%d", &matrix_b[row][col]);
        }
    }
}

int rainbow_multiplication(int row, int col) {
    int result = 0,
        i;
    for (i = 0; i < 3; i++) {
        result += matrix_a[row][i] * matrix_b[i][col];
    }

    return result;
}

void cross_mxa_with_mxb() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            int rainbow_result = rainbow_multiplication(row, col);
            main_matrix[row][col] = rainbow_result;
        }
    }
}

void setupSharedMemory() {
    key_t key = 1111;
    //size of 4 * 6 -> hasil dari perkalian matrix
    int s_memory_id = shmget(key, sizeof(int[4][6]), 0666 | IPC_CREAT);

    //attach matrix to shared memory
    main_matrix = shmat(s_memory_id, (void *)0, 0);
}

void print_mxa() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 3; col++) {
            printf("%d ", matrix_a[row][col]);
        }
        printf("\n");
    }
}

void print_mxb() {
    int row, col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 6; col++) {
            printf("%d ", matrix_b[row][col]);
        }
        printf("\n");
    }
}

void print_result() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            printf("%d ", main_matrix[row][col]);
        }
        printf("\n");
    }
}

int main() {
    setupSharedMemory();
    input_matrix_a();
    input_matrix_b();
    cross_mxa_with_mxb();
    print_result();

    shmdt(main_matrix);
}