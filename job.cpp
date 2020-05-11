#include "job.h"
#include <iostream>
using std::cout;
using std::endl;

job::job(unsigned int runtime, unsigned int time_req, unsigned int submit_time, unsigned int processors_req) {
  this->runtime = runtime;
  this->time_req = time_req;
  this->submit_time = submit_time;
  this->processors_req = processors_req;
}

void job::print() {
  cout << "runtime = " << this->runtime << endl;
  cout << "time_req = " << this->time_req << endl;
  cout << "submit_time = " << this->submit_time << endl;
  cout << "processors_req = " << this->processors_req << endl;
}
