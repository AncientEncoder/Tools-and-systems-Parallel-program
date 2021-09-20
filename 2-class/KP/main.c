//1th question
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/time.h>
#include<stdlib.h>
#include<string.h>
#define Max_Matrix_size 1000
#define Max_pthread_size 100
double Matrix[Max_Matrix_size][Max_Matrix_size];
int Matrix_size;
int Thread_num;
int Per_thread_do;
double Per_thread_cal[Max_pthread_size];
double result = 0.0;
pthread_mutex_t mut;
double det_cal(double **mmatrix,int size){
	if(size == 1){
		return mmatrix[0][0];
	}
	else{
		double res = 0.0;
		double **matrix = (double **)malloc(sizeof(double*) * (size));
		for(int i = 0;i < size - 1;i++){
			matrix[i] = (double *)malloc(sizeof(double) * (size));
		}
		for(int i = 0;i < size;i++){
			for(int j = 1;j < size;j++){
				for(int k = 0;k < size;k++){
					if(k < i){
						matrix[j - 1][k] = mmatrix[j][k];
					}
					if(k > i){
						matrix[j - 1][k - 1] = mmatrix[j][k];
					}
				}
			}
			if(i % 2 == 0){
				res += mmatrix[0][i] * det_cal(matrix,size - 1);
			}
			else{
				res -= mmatrix[0][i] * det_cal(matrix,size - 1);
			}
		}
		for (int i = 0; i < size - 1; i++) {
			free(matrix[i]);
		}
		free(matrix);
		return res;
	}
}
void *t_hread(void *arg){
	int q = *(int*)arg;
	double **matrix = (double **)malloc(sizeof(double*) * (Matrix_size));     
	for(int i = 0;i < Matrix_size;i++){
		matrix[i] = (double *)malloc(sizeof(double) * (Matrix_size));
	}
	if(q != Thread_num - 1){
		for(int i = q * Per_thread_do;i < ((q + 1) * Per_thread_do);i++){
			for(int j = 1;j < Matrix_size;j++){
				for(int k = 0;k < Matrix_size;k++){
					if(k < i){
						matrix[j - 1][k] = Matrix[j][k];
					}
					if(k > i){
						matrix[j - 1][k - 1] = Matrix[j][k];
					}
				}
			}
			if(i % 2 == 0){
				Per_thread_cal[q] += Matrix[0][i] * det_cal(matrix,Matrix_size - 1);	
			}
			
			else{
				Per_thread_cal[q] -= Matrix[0][i] * det_cal(matrix,Matrix_size - 1);
			}
		}		
	} 
	if(q == Thread_num - 1){
		for(int i = q * Per_thread_do;i < Matrix_size;i++){
			for(int j = 1;j < Matrix_size;j++){
				for(int k = 0;k < Matrix_size;k++){
					if(k < i){
						matrix[j - 1][k] = Matrix[j][k];
					}
					if(k > i){
						matrix[j - 1][k - 1] = Matrix[j][k];
					}
				}
			}
			if(i % 2 == 0){
				Per_thread_cal[q] += Matrix[0][i] * det_cal(matrix,Matrix_size - 1);	
			}
			
			else{
				Per_thread_cal[q] -= Matrix[0][i] * det_cal(matrix,Matrix_size - 1);
			}
		}		
	}
	pthread_mutex_lock(&mut);
	result += Per_thread_cal[q];
	pthread_mutex_unlock(&mut);
	for(int i = 0;i < Matrix_size - 1;i++){
		free(matrix[i]);
	}
	free(matrix);
	pthread_exit(0);
}
int main(int argc,char *argv[]){
	if(argc != 3){
		printf("Input format:MATRIX_SIZE,THREAD_NUM");
		return 0;
	}	
	memset(Per_thread_cal, 0.0, sizeof(Per_thread_cal));
	Matrix_size = atoi(argv[1]);    
	Thread_num = atoi(argv[2]);		
	char *input = argv[3];			
	Per_thread_do = Matrix_size / Thread_num;		
	pthread_mutex_init(&mut,NULL);					
	for(int i = 0;i < Matrix_size;i++){
		for(int j = 0;j < Matrix_size;j++){
			scanf("%lf",&Matrix[i][j]);
		}
	}															
	pthread_t thread[Max_pthread_size];
	int *id = (int *)malloc(sizeof(int) * (Thread_num));
	struct timeval begin,end;
    gettimeofday(&begin,NULL);
	for(int i = 0;i < Thread_num;i++){
		id[i] = i;
		pthread_create(&thread[i],NULL,t_hread,(void*)&id[i]);
	}
	for(int i = 0;i < Thread_num;i++){
		pthread_join(thread[i],NULL);
	}
	free(id);
    gettimeofday(&end,NULL);
	int time = (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
    printf("time: %d s\n", time);
	printf("Определитель result is: %lf\n",result);
    printf("size is :%d * %d\nvector has :%d",Matrix_size,Matrix_size,Matrix_size);
    
	return 0;
}
/**
 * @2th question
 *
 * restrict из  стандарт c99.
 *  Его можно использовать только для ограничения и ограничения указателей и указать,
 *  указатели являются единственным и начальным способом доступа к объекту данных. 
 *  Oн сообщает компилятору, что все операции, которые изменяют содержимое Память, на которую указывает указатель, 
 *  должна быть изменена с помощью указателя, но не другими переменными или указателями; 
 * преимущество этого состоит в том, что это может помочь компилятору лучше оптимизировать код более эффективный ассемблерный код.
 * 
 */