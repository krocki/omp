#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "timer.h"

#define N 200000

#define randf() (rand() / (1.0 + RAND_MAX))

int main(int argc, char **argv) {

  int i, nthreads, tid;
  double k = randf();

  double A[N], B[N], C[N];

  for (i=0; i<N; i++) {
    B[i] = randf();
    C[i] = randf();
  }

  unsigned long long cs, ce;

  cs=rdtsc();
  for (i=0; i<N; i++) {
    A[i] = B[i] + k*C[i];
  }
  ce=rdtsc();
  unsigned long long seq_cycles = ce-cs;

  cs=rdtsc();
  #pragma omp parallel for
  for (i=0; i<N; i++) {
    A[i] = B[i] + k*C[i];
  }
  ce=rdtsc();
  unsigned long long par_cycles = ce-cs;

  printf("seq=%llu, par=%llu, seq/par=%f\n", seq_cycles, par_cycles, (double)seq_cycles/(double)par_cycles);

  return 0;
}
