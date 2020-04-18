#include "job.h"

job::job(unsigned int runtime, unsigned int time_req, unsigned int submit_time, unsigned int processors_req) {
  this->runtime = runtime;
  this->time_req = time_req;
  this->submit_time = submit_time;
  this->processors_req = processors_req;
}
