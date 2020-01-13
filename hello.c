#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

  int n = argc > 1 ? strtol(argv[1], NULL, 10) : 2;
  int nthreads, tid;

  omp_set_num_threads(n);

  #pragma omp parallel private(nthreads, tid)
  {
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    printf("thread %d/%d is alive\n",
      tid, nthreads);

  }

  return 0;
}
