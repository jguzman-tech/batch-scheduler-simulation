#pragma once
#include <deque>
using std::deque;
#include <vector>
using std::vector;
#include "job.h"

class cluster {
public:
  enum algorithm { FCFS = 0, SJF = 1, LJF = 2 };
  algorithm scheduler;
  unsigned int interval;
  unsigned int num_processors;
  unsigned int* processors;
  unsigned int global_clock;
  unsigned int jobs_scheduled;
  bool no_running_jobs;
  vector<double> cpu_util; // will get the CPU Util for each second
  // avg this to get overall utilization
  vector<unsigned int> latency;
  deque<job> job_queue;
  deque<bool>* reservations;
  cluster(algorithm, unsigned int);
  void add_job(job);
  void run_scheduler();
  void advance();
  void drain(); // let all remaining jobs complete
  unsigned int get_available_processors();
  unsigned int get_jobs_scheduled();
  double get_avg_cpu_util();
  double get_avg_latency();
  ~cluster();
};
