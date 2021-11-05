#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
int NUM_THREADS = 2;
using namespace std;
int Partition(int* data, int p, int r) {
  int x, i, j, tmp;
  x = data[r];
  i = p - 1;
  for (j = p; j < r; j++)
    if (data[j] <= x) {
      i++;
      tmp = data[i];
      data[i] = data[j];
      data[j] = tmp;
    }
  tmp = data[i + 1];
  data[i + 1] = data[r];
  data[r] = tmp;

  return i + 1;
}
int* QuickSort_parallel(int* Data, int p, int r) {
  int q;
  if (p < r) {
    q = Partition(Data, p, r);
#pragma omp parallel
    {
#pragma omp sections nowait
      {
#pragma omp section
        QuickSort_parallel(Data, p, q - 1);
#pragma omp section
        QuickSort_parallel(Data, q + 1, r);
      }
    }
  }
  return Data;
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
    default:
      N = atoi(argv[1]);
      NUM_THREADS = atoi(argv[2]);
      cerr << "Warring Entered No Used Args";
  }
  int* Data;
  Data = new int[N];
  for (int i = 0; i < N; i++) {
    Data[i] = rand();
  }
  clock_t timep = clock();
  omp_set_num_threads(NUM_THREADS);
  QuickSort_parallel(Data, 0, N - 1);
  clock_t timeFinish = clock();
  cout << "Time usage " << timeFinish - timep << "ms in " << NUM_THREADS
       << " thread(s)" << endl;
  timep = clock();
  omp_set_num_threads(1);
  QuickSort_parallel(Data, 0, N - 1);
  timeFinish = clock();
  cout << "Time usage " << timeFinish - timep << "ms in 1 thread" << endl;
  return 0;
}
//алгоритм:
/**
 * QUICKSORT(A,p,r)
 *if p<r
 *   then q=PARTITION(A,p,r)
 *        QUICKSORT(A,p,q-1)
 *        QUICKSORT(A,q+1,r)
 */
/**
 * PARTITION(A,p,r)
 *x=A[r]
 *i=p-1
 *for j=p to r-1
 *    do if A[j]<=x
 *        then i=i+1
 *            exchange A[i] and A[j]
 *exchange A[i+1] and A[r]
 *return i+1
 */