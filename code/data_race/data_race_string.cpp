#include <iostream>
#include <string>
#include <thread>

std::string x;

int main()
{
  std::thread t1([] { //
    x = "long enough string to heap allocate";
  });
  std::thread t2([] { //
    std::cout << x << "\n";
  });

  t1.join();
  t2.join();
  return 0;
}
