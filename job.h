#pragma once

class job {
public:
  unsigned int runtime, time_req, submit_time, processors_req;
  job(unsigned int, unsigned int, unsigned int, unsigned int);
  void print();
};
