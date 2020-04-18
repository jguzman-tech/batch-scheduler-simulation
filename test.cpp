#include <queue>
using std::queue;
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char** argv) {
  queue<int> my_queue;
  my_queue.push(1);
  my_queue.push(2);
  cout << "front: " << my_queue.front() << endl;
  cout << "front: " << my_queue.front() << endl;
  return 0;
}
