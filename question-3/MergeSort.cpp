#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#define TASK_SIZE 100
int NUM_THREADS = 2;
using namespace std;
void setUp(int* m, int size) {
  for (int i = 0; i < size; i++)
    m[i] = rand();
}

void mergeSortAus(int* X, int n, int* tmp) {
  int i = 0;
  int j = n / 2;
  int ti = 0;

  while (i < n / 2 && j < n) {
    if (X[i] < X[j]) {
      tmp[ti] = X[i];
      ti++;
      i++;
    } else {
      tmp[ti] = X[j];
      ti++;
      j++;
    }
  }
  while (i < n / 2) {
    tmp[ti] = X[i];
    ti++;
    i++;
  }
  while (j < n) {
    tmp[ti] = X[j];
    ti++;
    j++;
  }
  memcpy(X, tmp, n * sizeof(int));
}

void mergeSort(int* X, int n, int* tmp) {
  if (n < 2)
    return;

#pragma omp task shared(X) if (n > TASK_SIZE)
  mergeSort(X, n / 2, tmp);

#pragma omp task shared(X) if (n > TASK_SIZE)
  mergeSort(X + (n / 2), n - (n / 2), tmp + n / 2);

#pragma omp taskwait
  mergeSortAus(X, n, tmp);
}
int main(int argc, char* argv[]) {
  int N = 0;
  switch (argc) {
    case 1:
      cerr << "command: ./*.out(exe) length(int) thread(int)\n";
      cerr << "length set...........todo\n";
      cerr << "thread set...........todo\n";
      return 1;
    case 2:
      cerr << "command: ./*.out(exe) length(int) thread(int)\n";
      cerr << "thread set...........todo\n";
      cout << "length set...........done\n";
      cout << "running in default mode threads=2" << endl;
      N = atoi(argv[1]);
      break;
    case 3:
      cout << "All params are set ready to take off\n"
           << "length set...........done\n";
      cout << "thread set...........done\n";
      N = atoi(argv[1]);
      NUM_THREADS = atoi(argv[2]);
      break;
  }
  int* X = static_cast<int*>(malloc(N * sizeof(int)));
  int* tmp = static_cast<int*>(malloc(N * sizeof(int)));
  omp_set_dynamic(0);
  omp_set_num_threads(NUM_THREADS);
  if (!X || !tmp) {
    if (X)
      free(X);
    if (tmp)
      free(tmp);
    return -1;
  }
  setUp(X, N);
  double begin = omp_get_wtime();
#pragma omp parallel
  {
#pragma omp single
    mergeSort(X, N, tmp);
  }
  double end = omp_get_wtime();
  printf("Computation timing = %10.4f sec\n", end - begin);
  free(X);
  free(tmp);
  return 0;
}