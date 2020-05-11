#include "cluster.h"
#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>
using std::sort;

cluster::cluster(algorithm scheduler, unsigned int num_processors) {
  this->scheduler = scheduler;
  this->global_clock = 0;
  this->interval = 60;
  this->num_processors = num_processors;
  this->processors = new unsigned int[this->num_processors];
  for(unsigned int i = 0; i < this->num_processors; i++) {
    this->processors[i] = 0;
  }
  this->jobs_scheduled = 0;
  this->no_running_jobs = false;
}

void cluster::add_job(job new_job) {
  while(new_job.submit_time > this->global_clock) {
    this->advance();
  }
  this->job_queue.push_back(new_job);
}

unsigned int cluster::get_available_processors() {
  if(this->no_running_jobs == true) {
    return this->num_processors;
  }
  unsigned int available_processors = 0;
  for(unsigned int i = 0; i < this->num_processors; i++) {
    if(this->processors[i] == 0) {
      available_processors++;
    }
  }
  return available_processors;
}

// I'm only doing FIFO for right now
void cluster::run_scheduler() {
  if(this->scheduler == this->algorithm::FCFS) {
    while(this->job_queue.size() > 0 &&
	  this->get_available_processors() >= this->job_queue.front().processors_req) {
      unsigned int processors_req = this->job_queue.front().processors_req;
      for(unsigned int i = 0; i < this->num_processors; i++) {
	if(this->processors[i] == 0) {
	  this->processors[i] = this->job_queue.front().runtime;
	  processors_req--;
	  if(processors_req == 0) {
	    break;
	  }
	}
      }
      this->latency.push_back(this->global_clock -
      			      this->job_queue.front().submit_time);
      this->job_queue.pop_front();
      this->jobs_scheduled++;
      this->no_running_jobs = false;
    }
  }
  else if(this->scheduler == cluster::algorithm::SJF) {
    while(this->job_queue.size() > 0) {
      unsigned int processors_req = this->job_queue.front().processors_req;
      for(unsigned int i = 0; i < this->num_processors; i++) {
  	if(this->processors[i] == 0) {
  	  this->processors[i] = this->job_queue.front().runtime;
  	  processors_req--;
  	  if(processors_req == 0) {
  	    break;
  	  }
  	}
      }
      this->latency.push_back(this->global_clock -
  			      this->job_queue.front().submit_time);
      this->job_queue.pop_back();
      this->jobs_scheduled++;
      this->no_running_jobs = false;
    }
  }
  else if(this->scheduler == cluster::algorithm::LJF) {
    sort(this->job_queue.begin(), this->job_queue.end(), 
  	 [](job & a, job & b) -> bool
  	 { 
  	   return a.time_req < b.time_req;
  	 });
    while(this->job_queue.size() > 0 &&
  	  this->get_available_processors() >= this->job_queue.front().processors_req) {
      unsigned int processors_req = this->job_queue.front().processors_req;
      for(unsigned int i = 0; i < this->num_processors; i++) {
  	if(this->processors[i] == 0) {
  	  this->processors[i] = this->job_queue.front().runtime;
  	  processors_req--;
  	  if(processors_req == 0) {
  	    break;
  	  }
  	}
      }
      this->latency.push_back(this->global_clock -
  			      this->job_queue.front().submit_time);
      this->job_queue.pop_back();
      this->jobs_scheduled++;
      this->no_running_jobs = false;
    }    
  }
}

void cluster::advance() {
  unsigned int count = 0;
  if(this->no_running_jobs == false) {
    for(unsigned int i = 0; i < this->num_processors; i++) {
      if(this->processors[i] != 0) {
	this->processors[i]--;
	count++;
      }
    }
  }
  if(count == 0) {
    this->no_running_jobs = true;
  }
  this->cpu_util.push_back(((double) count ) / this->num_processors);
  if(this->global_clock % this->interval == 0 &&
     this->job_queue.front().submit_time <= this->global_clock) {
    this->run_scheduler();
  }
  this->global_clock++;
}

cluster::~cluster() {
  delete this->processors;
}

unsigned int cluster::get_jobs_scheduled() {
  return this->jobs_scheduled;
}

// utilization is a proportion not percentage
double cluster::get_avg_cpu_util() {
  double avg = 0.0;
  for(unsigned int i = 0; i < this->cpu_util.size(); i++) {
    avg += this->cpu_util.at(i);
  }
  avg /= (double) this->cpu_util.size(); // avg num of cpu's
  return avg;
}

double cluster::get_avg_latency() {
  unsigned int sum = 0;
  for(unsigned int i = 0; i < this->latency.size(); i++) {
    sum += this->latency.at(i);
  }
  return ((double) sum) / this->latency.size();
}

void cluster::drain() {
  while(this->job_queue.size()) {
    this->advance();
  }
  while (this->get_available_processors() != this->num_processors) {
    this->advance();
  }
}
