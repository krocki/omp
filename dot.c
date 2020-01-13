#define N (1 << 24)

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdint.h>

typedef uint64_t u64;

#define N_THREADS 2
#define RANDF() rand() / (RAND_MAX + 1.0)

int main(int argc, char **argv) {

  u64 i, j, tid;
  clock_t t;

  double *a = calloc(N, sizeof(double));
  double *b = calloc(N, sizeof(double));

  double ref = .0f;
  double sum = .0f;

  omp_set_num_threads(N_THREADS);

  for (u64 i=0; i<N; i++) {
    a[i] = RANDF();
    b[i] = RANDF();
  }

  t = clock();
  for (u64 i=0; i<N; i++) {
    ref += a[i] * b[i];
  }

  t = clock() - t;
  double t0 = ((double)t)/CLOCKS_PER_SEC;

  t = clock();

  double psum[N_THREADS];
  u64 chunk = N/N_THREADS;

  #pragma omp parallel private(psum, i, j, tid, chunk) \
    shared(a,b)
  {
    tid = omp_get_thread_num();
    psum[tid] = 0.0;
    #pragma omp for
    for (i=0; i<N/8; i++) {
      double p =
        a[8*i  ] * b[8*i  ] +
        a[8*i+1] * b[8*i+1] +
        a[8*i+2] * b[8*i+2] +
        a[8*i+3] * b[8*i+3] +
        a[8*i+4] * b[8*i+4] +
        a[8*i+5] * b[8*i+5] +
        a[8*i+6] * b[8*i+6] +
        a[8*i+7] * b[8*i+7];
      psum[tid] += p;
    }
    printf("tid=%zu, psum[%zu] = %f\n",
      tid, tid, psum[tid]);
    sum += psum[tid];
  }

  t = clock() - t;
  double t1 = ((double)t)/CLOCKS_PER_SEC;

  printf("sum=%f, ref=%f, t0=%f, t1=%f\n",
    sum, ref, t0, t1);

  free(a);
  free(b);

  return 0;

}
