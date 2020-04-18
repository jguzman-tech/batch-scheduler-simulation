#pragma once
#include <queue>
using std::queue;
#include "job.h"

class cluster {
public:
  unsigned int interval;
  unsigned int num_processors;
  unsigned int* processors;
  unsigned int global_clock;
  unsigned int counter;
  queue<job> job_queue;
  cluster(unsigned int);
  void add_job(job);
  void run_scheduler();
  void advance();
  unsigned int get_available_processors();
  ~cluster();
};
