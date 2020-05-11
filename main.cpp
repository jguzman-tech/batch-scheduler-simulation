#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::istringstream;
#include <stdexcept>
using std::runtime_error;
#include "job.h"
#include <string>
using std::string;
#include "cluster.h"


int main(int argc, char** argv) {
  if(argc != 4) {
    cout << "Usage: ./a.out <data-file> <number-of-jobs> <algorithm>" << endl;
    cout << "\t<data-file>: the path of the CEA-Curie-2011-2.swf dataset" << endl;
    cout << "\t<number-of-jobs>: the number of jobs to read from the SWF file" << endl;
    cout << "\t<algorithm>: three algorithms are supported {FCFS, SJF, and LJF}" << endl;
    cout << "\tFCFS --> First Come First Serve, SJF --> Shortest Job First, ";
    cout << "LJF --> Longest Job First" << endl;
    return 1;
  }
  
  char line[120];
  ifstream file(argv[1]);
  unsigned int limit = atoi(argv[2]);
  string algorithm = string(argv[3]);
  
  unsigned int num_jobs = 0;
  unsigned int num_valid_jobs = 0;

  cluster* system;

  if(algorithm.compare("FCFS") == 0) {
    system = new cluster(cluster::algorithm::FCFS, 480);
  }
  else if(algorithm.compare("SJF") == 0) {
    system = new cluster(cluster::algorithm::SJF, 480);
  }
  else if(algorithm.compare("LJF") == 0) {
    system = new cluster(cluster::algorithm::LJF, 480);
  }
  else {
    cout << "unknown algorithm selection" << endl;
    exit(1);
  }
  unsigned int count = 0;
  
  if(file.is_open()) {
    while(file.getline(line, sizeof(line))) {
      if(line[0] == ';') {
	// line is a comment, do nothing
      }
      else {
	if(count > limit) {
	  break;
	}
	// this may only work for the CIEMAT logs
	istringstream iss(line);
	int values[18];
	for(int i = 0; i < 18; i++) {
	  iss >> values[i];
	}
	num_jobs++;
	int job_number, submit_time, wait_time, runtime, processors_used;
	int avg_cpu_time, avg_mem_used, processors_req, time_req, mem_req, status;
	int uid, gid, executable, queue_n, partition, prior_job, think_time;
	
	job_number = values[0];
	submit_time = values[1];
	wait_time = values[2];
	runtime = values[3];
        processors_used = values[4];
	avg_cpu_time = values[5];
	avg_mem_used = values[6];
	processors_req = values[7];
	time_req = values[8];
	mem_req = values[9];
	status = values[10];
	uid = values[11];
	gid = values[12];
	executable = values[13];
	queue_n = values[14];
	partition = values[15];
	prior_job = values[16];
	think_time = values[17];

	// print all job info
	// cout << "job_number = " << job_number << ", submit_time = " << submit_time;
	// cout << ", wait_time = " << wait_time << ", runtime = " << runtime;
	// cout << ", processors_used = " << processors_used << ", avg_cpu_time = ";
	// cout << avg_cpu_time << ", avg_mem_used = " << avg_mem_used;
	// cout << ", processors_req = " << processors_req << ", time_req = " << time_req << ", mem_req = ";
	// cout << mem_req << ", status = " << status << ", uid = " << uid;
	// cout << ", gid = " << gid << ", executable = " << executable;
	// cout << ", queue_n = " << queue_n << ", partition = " << partition;
	// cout << ", prior_job = " << prior_job << ", think_time = ";
	// cout << think_time << endl;

	if(runtime >= 0 && time_req >= 0 && submit_time >= 0 &&
	   processors_req >= 0 && processors_req <= 480) {
	  job new_job(runtime, time_req, submit_time, processors_req);	  
	  system->add_job(new_job);
	  num_valid_jobs++;
	}
      }
      count++;
    }
    file.close();
  }
  else {
    throw runtime_error("unable to open file");
  }
  system->drain();
  cout << "after draining: global_clock = " << system->global_clock << endl;
  cout << "num_jobs = " << num_jobs << endl;
  cout << "num_valid_jobs = " << num_valid_jobs << endl;
  cout << "sceduled_jobs = " << system->get_jobs_scheduled() << endl;
  cout << "avg_cpu_util = " << system->get_avg_cpu_util() << endl;
  cout << "avg_latency = " << system->get_avg_latency() << endl;
  return 0;
}
