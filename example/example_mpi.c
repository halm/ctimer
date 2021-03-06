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

// #include <unistd.h>
#include <stdio.h>
#include <mpi.h>

#include "timer.h"

static Timer t []  = { {0, 0, "label_1", 0}, {0, 0, "label_2", 0} };
int num_timer = sizeof(t) / sizeof(t[0]);

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  for (int i = 0; i < 3; ++i) {
    timer_mpi_start(MPI_COMM_WORLD, &t[0], 1);
    sleep(1);
    timer_mpi_stop(MPI_COMM_WORLD, &t[0]);
  }

  printf("%s elapsed time: %f [sec]\n", t[0].label, t[0].elaps);
  printf("%s elapsed time: %f [sec]\n", t[1].label, t[1].elaps);

  timer_summarize(MPI_COMM_WORLD, t, num_timer);
  timer_detail(MPI_COMM_WORLD, &t[0], "a");

  MPI_Finalize();
  return 0;
}
