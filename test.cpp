#include <queue>
using std::queue;
#include <deque>
using std::deque;
#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>
using std::min_element;

int main(int argc, char** argv) {
  deque<int> x;
  x.push(5);
  x.push(10);
  x.push(0);
  x.push(11);
  cout << min_element(x.begin(), x.end()) << endl;
  return 0;
}
