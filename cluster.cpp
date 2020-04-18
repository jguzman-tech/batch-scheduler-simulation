#include "cluster.h"
#include <iostream>
using std::cout;
using std::endl;

cluster::cluster(unsigned int num_processors) {
  this->counter = 0;
  this->global_clock = 0;
  this->interval = 60;
  this->num_processors = num_processors;
  this->processors = new unsigned int[this->num_processors];
  for(unsigned int i = 0; i < this->num_processors; i++) {
    this->processors[i] = 0;
  }
}

void cluster::add_job(job new_job) {
  this->job_queue.push(new_job);
  if(new_job.submit_time == this->global_clock) {
    this->run_scheduler();
  }
  else {
    while(new_job.submit_time > this->global_clock) {
      this->advance();
    }
  }
}

unsigned int cluster::get_available_processors() {
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
    this->job_queue.pop();
  }
  this->counter++;
}

void cluster::advance() {
  for(unsigned int i = 0; i < this->num_processors; i++) {
    if(this->processors[i] != 0) {
      this->processors[i]--;
    }
  }
  this->global_clock++;
  if(this->global_clock % this->interval == 0) {
    this->run_scheduler();
  }
}

cluster::~cluster() {
  delete this->processors;
}
