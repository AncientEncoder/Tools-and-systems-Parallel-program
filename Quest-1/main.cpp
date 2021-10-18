#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
void mode0_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int i, j, k;
    int32_t sum;
    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++){
            sum = 0;
            for(k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
}

void mode1_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int i, k, j;
    int32_t tmp;
    for(i = 0; i < N; i++){
        for(k = 0; k < N; k++){
            tmp = A[i][k];
            for(j = 0; j < N; j++){
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}


void mode2_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int k, i, j;
    int32_t tmp;
    for(k = 0; k < N; k++){
        for(i = 0; i < N; i++){
            tmp = A[i][k];
            for(j = 0; j < N; j++){
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}

void mode3_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int j, i, k;
    int32_t sum;
    for(j = 0; j < N; j++){
        for(i = 0; i < N; i++){
            sum = 0;
            for(k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void mode4_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int j, k, i;
    int32_t tmp;
    for(j = 0; j < N; j++){
        for(k = 0; k < N; k++){
            tmp = B[k][j];
            for(i = 0; i < N; i++){
                C[i][j] += A[i][k] * tmp;
            }
        }
    }
}

void mode0_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int i, j, k;
    int64_t sum;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            sum = 0;
            for(k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void mode1_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int i, k, j;
    int64_t tmp;
    for(i = 0; i < N; i++){
        for(k = 0; k < N; k++){
            tmp = A[i][k];
            for(j = 0; j < N; j++){
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}

void mode2_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int k, i, j;
    int64_t tmp;
    for(k = 0; k < N; k++){
        for(i = 0; i < N; i++){
            tmp = A[i][k];
            for(j = 0; j < N; j++){
                C[i][j] += tmp * B[k][j];
            }
        }
    }
}
void mode3_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int j, i, k;
    int64_t sum;
    for(j = 0; j < N; j++){
        for(i = 0; i < N; i++){
            sum = 0;
            for(k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}
void mode4_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int j, k, i;
    int64_t tmp;
    for(j = 0; j < N; j++){
        for(k = 0; k < N; k++){
            tmp = B[k][j];
            for(i = 0; i < N; i++){
                C[i][j] += A[i][k] * tmp;
            }
        }
    }
}
void mode5_32(int32_t N, int32_t **A, int32_t **B, int32_t **C){
    int k, j, i;
    int32_t tmp;
    for(k = 0; k < N; k++){
        for(j = 0; j < N; j++){
            tmp = B[k][j];
            for(i = 0; i < N; i++){
                C[i][j] += A[i][k] * tmp;
            }
        }
    }
}
void mode5_64(int32_t N, int64_t **A, int64_t **B, int64_t **C){
    int k, j, i;
    int64_t tmp;
    for(k = 0; k < N; k++){
        for(j = 0; j < N; j++){
            tmp = B[k][j];
            for(i = 0; i < N; i++){
                C[i][j] += A[i][k] * tmp;
            }
        }
    }
}
int main(int argc, char **argv){
    if(argc != 5){
        printf("arg err");
        return 1;
    }
        char T;
        int32_t N;
        int mode = atoi(argv[4]);
        FILE *bin_a, *bin_b, *bin_c;
        clock_t begin, end;
        double used_time = 0;
        int32_t **A_32 = NULL;
        int64_t **A_64 = NULL;
        int32_t **B_32 = NULL;
        int64_t **B_64 = NULL;
        int32_t **C_32 = NULL;
        int64_t **C_64 = NULL;
        bin_a = fopen("a", "rb");
        fread(&T, sizeof(char), 1, bin_a);
        fread(&N, sizeof(int32_t), 1, bin_a);
        if(T == 'd'){
            A_32 = (int32_t **) malloc(N * sizeof(int32_t *));
            for(int i = 0; i < N; i++){
                *(A_32 + i) = (int32_t *) malloc(N * sizeof(int32_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(A_32 + i), sizeof(int32_t), N, bin_a);
            }
        } else if(T == 'l'){
            A_64 = (int64_t **) malloc(N * sizeof(int64_t *));
            for(int i = 0; i < N; i++){
                *(A_64 + i) = (int64_t *) malloc(N * sizeof(int64_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(A_64 + i), sizeof(int64_t), N, bin_a);
            }
        }

        fclose(bin_a);
        bin_b = fopen("b", "rb");
        fread(&T, sizeof(char), 1, bin_b);
        fread(&N, sizeof(int32_t), 1, bin_b);
        if(T == 'd'){
            B_32 = (int32_t **) malloc(N * sizeof(int32_t *));
            for(int i = 0; i < N; i++){
                *(B_32 + i) = (int32_t *) malloc(N * sizeof(int32_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(B_32 + i), sizeof(int32_t), N, bin_b);
            }
        } else if(T == 'l'){
            B_64 = (int64_t **) malloc(N * sizeof(int64_t *));
            for(int i = 0; i < N; i++){
                *(B_64 + i) = (int64_t *) malloc(N * sizeof(int64_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(B_64 + i), sizeof(int64_t), N, bin_b);
            }
        }
        fclose(bin_b);

        bin_c = fopen("c", "rb");
        fread(&T, sizeof(char), 1, bin_c);
        fread(&N, sizeof(int32_t), 1, bin_c);
        if(T == 'd'){
            C_32 = (int32_t **) malloc(N * sizeof(int32_t *));

            for(int i = 0; i < N; i++){
                *(C_32 + i) = (int32_t *) malloc(N * sizeof(int32_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(C_32 + i), sizeof(int32_t), N, bin_c);
            }
        } else if(T == 'l'){
            C_64 = (int64_t **) malloc(N * sizeof(int64_t *));
            for(int i = 0; i < N; i++){
                *(C_64 + i) = (int64_t *) malloc(N * sizeof(int64_t));
            }
            for(int i = 0; i < N; i++){
                fread(*(C_64 + i), sizeof(int64_t), N, bin_c);
            }
        }
        fclose(bin_c);

        if(T == 'd'){
            if (mode==0){
                begin = clock();
                 mode0_32(N, A_32, B_32, C_32);
            }else if (mode==1){
                begin = clock();
                 mode1_32(N, A_32, B_32, C_32);
            }else if (mode==2){
                begin = clock();
                 mode2_32(N, A_32, B_32, C_32);
            }else if (mode==3){
                begin = clock();
                mode3_32(N, A_32, B_32, C_32);
            }else if (mode==4){
                begin = clock();
                mode4_32(N, A_32, B_32, C_32);
            }else if (mode==5){
                begin = clock();
                mode5_32(N, A_32, B_32, C_32);
            }else{
                return 1;
            }
            end = clock();
           used_time = (double) (end - begin) / CLOCKS_PER_SEC;
            printf("In Mode: %d used time: %lf s.\n", mode,used_time);
        } else if(T == 'l'){
            if (mode==0){
                begin = clock();
                  mode0_64(N, A_64, B_64, C_64);
            }else if (mode==1){
                begin = clock();
                 mode1_64(N, A_64, B_64, C_64);
            }else if (mode==2){
                begin = clock();
                 mode2_64(N, A_64, B_64, C_64);
            }else if (mode==3){
                begin = clock();
                mode3_64(N, A_64, B_64, C_64);
            }else if (mode==4){
                begin = clock();
                mode4_64(N, A_64, B_64, C_64);
            }else if (mode==5){
                begin = clock();
                 mode5_64(N, A_64, B_64, C_64);
            }else{
                return 1;
            }
            end = clock();
           used_time = (double) (end - begin) / CLOCKS_PER_SEC;
            printf("In Mode: %d used time: %lf s.\n", mode,used_time);
        }

        if(T == 'd'){
            for(int i = 0; i < N; i++){
                free(A_32[i]);
                free(B_32[i]);
                free(C_32[i]);
            }
            free(A_32);
            free(B_32);
            free(C_32);
        } else if(T == 'l'){
            for(int i = 0; i < N; i++){
                free(A_64[i]);
                free(B_64[i]);
                free(C_64[i]);
            }
            free(A_64);
            free(B_64);
            free(C_64);
        }
    return 0;
}

