#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void ReadMatrix(char *filename, int64_t local_A[], int N, int local_N, int rank, MPI_Comm comm){
    int64_t *A = NULL;
    int tmp = 0;

    if(rank == 0){
        FILE *f = fopen(filename, "rb");
        fread(&tmp, sizeof(int), 1, f);
        fread(A, sizeof(int64_t), N * N, f);
        fclose(f);

        if(MPI_Scatter(A, local_N * N, MPI_INT64_T,
                       local_A, local_N * N, MPI_INT64_T, 0, comm) != MPI_SUCCESS)
            exit(-1);
    } else{
        if(MPI_Scatter(A, local_N * N, MPI_INT64_T,
                       local_A, local_N * N, MPI_INT64_T, 0, comm) != MPI_SUCCESS)
            exit(-1);
    }
}

void ReadVector(char *filename, int64_t local_vec[], int N, int local_N, int rank, MPI_Comm comm){
    int64_t *vec = NULL;

    if(rank == 0){
        vec = (int64_t *) malloc(N * sizeof(int64_t));
        FILE *f = fopen(filename, "rb");
        fread(vec, sizeof(int64_t), N, f);
        fclose(f);

        if(MPI_Scatter(vec, local_N, MPI_INT64_T,
                       local_vec, local_N, MPI_INT64_T, 0, comm) != MPI_SUCCESS)
            exit(-1);
    } else{
        if(MPI_Scatter(vec, local_N, MPI_INT64_T,
                       local_vec, local_N, MPI_INT64_T, 0, comm) != MPI_SUCCESS)
            exit(-1);
    }
}

void PrintMatrix(char str[], int64_t local_A[], int N, int local_N, int rank, MPI_Comm comm){
    int64_t *A = NULL;
    int i;

    if(rank == 0){
        A = (int64_t *) malloc(N * N * sizeof(int64_t));
        if(MPI_Gather(local_A, local_N * N, MPI_INT64_T,
                      A, local_N * N, MPI_INT64_T, 0, comm) != MPI_SUCCESS)
            exit(-1);
        printf("Matrix %s\n", str);
        for(i = 0; i < N * N; i++){
            printf("%ld ", A[i]);
            if(i % N == N - 1)
                printf("\n");
        }

        printf("\n");
        free(A);
    } else{
        if(MPI_Gather(local_A, local_N * N, MPI_INT64_T,
                      A, local_N * N, MPI_INT64_T, 0, comm) != MPI_SUCCESS)
            exit(-1);
    }
}

void PrintVector(char str[], int64_t local_vec[], int N, int local_N, int rank, MPI_Comm comm){
    int64_t *vec = NULL;
    int i;

    if(rank == 0){
        vec = (int64_t *) malloc(N * sizeof(int64_t));
        if(MPI_Gather(local_vec, local_N, MPI_INT64_T,
                      vec, local_N, MPI_INT64_T, 0, comm) != MPI_SUCCESS)
            exit(-1);

        printf("Vector %s\n", str);
        for(i = 0; i < N; i++)
            printf("%ld ", vec[i]);
        printf("\n\n");
        free(vec);
    } else{
        if(MPI_Gather(local_vec, local_N, MPI_INT64_T,
                      vec, local_N, MPI_INT64_T, 0, comm) != MPI_SUCCESS)
            exit(-1);
    }
}

void ParallelExec(const int64_t local_A[], int64_t local_b[], int64_t local_c[],
                                 int local_m, int N, int local_n, MPI_Comm comm){
    int64_t *x;
    int i, j;

    x = (int64_t *) malloc(N * sizeof(int64_t));
    if(MPI_Allgather(local_b, local_n, MPI_INT64_T,
                     x, local_n, MPI_INT64_T, comm) != MPI_SUCCESS)
        exit(-1);

    for(i = 0; i < local_m; i++){
        local_c[i] = 0;
        for(j = 0; j < N; j++)
            local_c[i] += local_A[i * N + j] * x[j];
    }

    free(x);
}

int main(int argc, char **argv){
    if(argc > 3){
        int64_t *A;
        int64_t *b;
        int64_t *c;
        int N, local_N;
        int rank, comm_size;
        double start, end;
        MPI_Comm comm = MPI_COMM_WORLD;

        MPI_Init(NULL, NULL);
        MPI_Comm_size(comm, &comm_size);
        MPI_Comm_rank(comm, &rank);

        FILE *f = fopen(argv[1], "rb");
        fread(&N, sizeof(int), 1, f);
        fclose(f);

        if(rank == 0){
            if(N % comm_size != 0){
                return -2;
            }
        }

        MPI_Bcast(&N, 1, MPI_INT, 0, comm);
        local_N = N / comm_size;

        A = (int64_t *) malloc(local_N * N * sizeof(int64_t));
        if(A == NULL){
            return 1 ;
        }
        b = (int64_t *) malloc(local_N * sizeof(int64_t));
        if(b == NULL){
            free(A);
            return -1;
        }
        c = (int64_t *) malloc(local_N * sizeof(int64_t));
        if(c == NULL){
            free(A);
            free(b);
            return -1;
        }

        ReadMatrix(argv[1], A, N, local_N, rank, comm);
        ReadVector(argv[2], b, N, local_N, rank, comm);
        start = MPI_Wtime();
        ParallelExec(A, b, c, local_N, N, local_N, comm);
        end = MPI_Wtime();

        if(rank == 0){
            f = fopen(argv[3], "wb");
            fwrite(c, sizeof(int64_t), N, f);
            fclose(f);
        }

        if(rank == 0){
            printf("\nProcesses: %d\n"
                   "Used Time: %f s.\n", comm_size, end - start);
        }


        free(A);
        free(b);
        free(c);
        MPI_Finalize();
    } else{
        printf("Command line format: ./<binary> <matrix_A> <vector_b> <vector_c>\n");
    }
    return 0;
}
