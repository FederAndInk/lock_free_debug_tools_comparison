#include <future>
#include <iostream>
#include <string>
#include <thread>

int main()
{
  using namespace std::chrono_literals;
  std::future<void> future;
  std::thread       t1([&future] {
    std::string s = "long enough to provoc dynamic allocation";
    future = std::async(std::launch::async, [&s] { std::cout << s << "\n"; });
  });
  t1.join();
  future.wait();
}