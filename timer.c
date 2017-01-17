/* The MIT License (MIT)

Copyright (c) 2013 Haruhiko Matsuo <halm.matsuo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */


#include "timer.h"

inline void timer_mpi_start(MPI_Comm comm, Timer* t, bool brr) {
  if(brr) MPI_Barrier(comm);
  t->start = MPI_Wtime();
}

inline void timer_mpi_stop(MPI_Comm comm, Timer* t) {
  t->elaps += MPI_Wtime() - t->start;
  ++(t->count);
}

inline void timer_mpi_clear(MPI_Comm comm, Timer* t) {
  t->start = 0;
  t->elaps = 0;
  t->count = 0;
}

void pack(Timer* t, int num_timer, double* elaps, double* count) {
  for (int i = 0; i < num_timer; ++i) {
    elaps[i] = (double) t[i].elaps;
    count[i] = (double) t[i].count;
  }
}

void timer_summarize(MPI_Comm comm, Timer* t, int num_timer) {
  int np;
  int rank;

  MPI_Comm_size(comm, &np);
  MPI_Comm_rank(comm, &rank);

  double elaps [num_timer];
  double elaps_ave [num_timer];
  double elaps_min [num_timer];
  double elaps_max [num_timer];

  double count [num_timer];
  double count_ave [num_timer];
  double count_min [num_timer];
  double count_max [num_timer];

  pack(t, num_timer, elaps, count);

  MPI_Reduce(&elaps[0], &elaps_ave[0], num_timer, MPI_DOUBLE, MPI_SUM, 0, comm);
  MPI_Reduce(&elaps[0], &elaps_min[0], num_timer, MPI_DOUBLE, MPI_MIN, 0, comm);
  MPI_Reduce(&elaps[0], &elaps_max[0], num_timer, MPI_DOUBLE, MPI_MAX, 0, comm);

  MPI_Reduce(&count[0], &count_ave[0], num_timer, MPI_DOUBLE, MPI_SUM, 0, comm);
  MPI_Reduce(&count[0], &count_min[0], num_timer, MPI_DOUBLE, MPI_MIN, 0, comm);
  MPI_Reduce(&count[0], &count_max[0], num_timer, MPI_DOUBLE, MPI_MAX, 0, comm);

  for (int i = 0; i < num_timer; ++i) {
    elaps_ave[i] /= np;
    count_ave[i] /= np;
  }

  if (rank == 0) {

    for(int i = 0; i < num_timer; ++i) {
      printf("timer: %5d %-55s"
  	     " %12.3lf %12.3lf %12.3lf %12.3lf"
  	     " %12.1f %12.1f %12.1f %12.1f\n",
  	     i, t[i].label,
  	     elaps[i], elaps_ave[i], elaps_min[i], elaps_max[i],
  	     count[i], count_ave[i], count_min[i], count_max[i]);
    }
  }
}


void  timer_detaile(MPI_Comm comm, const Timer *t, const char *mode) {
  int np;
  int rank;

  MPI_Comm_size(comm, &np);
  MPI_Comm_rank(comm, &rank);

  char output_file[128];
  const char *pref = "timer";
  const char *suf  = ".dat";

  snprintf(output_file, 128, "%s%05d%s", pref, rank, suf);
  FILE* fd = fopen(output_file, mode);

  if (fd == NULL) {
    fprintf(stderr, "Failed to open file %s\n", output_file);
    exit(EXIT_FAILURE);
  }

  fprintf(fd, "timer: " "%5d %-55s"
	  " %12.10lf %12d\n", rank, t->label, t->elaps, t->count);
}

