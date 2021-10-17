//1th question
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>


#define num 200
int num_t;
long sum=0;
pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;
int arr_num[num];

void iniz(){
	for(int i=0;i<num;i++){
		arr_num[i]=i;
	}
}

void *arrSum(void *arg){
	int id=*(int*)arg;
	long tmp_sum=0;

	for(int i=id;i<num;i+=num_t){
		tmp_sum+=arr_num[i];
	}
	pthread_mutex_lock(&mut);
	sum+=tmp_sum;
	pthread_mutex_unlock(&mut);
	return NULL;
}


int vectorSum(int *v,int size,int nthreads){
	pthread_t thread[nthreads];
	int index[nthreads];
	for(int i=0;i<nthreads;i++)
	{
		index[i]=i;
		int error=pthread_create(&thread[i],NULL,arrSum,&index[i]);
		if(error!=0)
		{
			printf("cant cread thread:%s/n",strerror(error));
			return -1;
		}
	}

	for(int i=0;i<nthreads;i++){
		pthread_join(thread[i],NULL);
	}

	return 0;
}

int main(int argc,char* argv[])
{
	num_t=atoi(argv[1]);
	iniz();
	vectorSum(arr_num,num,num_t);
	printf("sum=%ld\n",sum);
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