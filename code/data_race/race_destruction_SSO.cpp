#include <future>
#include <iostream>
#include <string>
#include <thread>

int main()
{
  using namespace std::chrono_literals;
  std::future<void> future;
  std::thread       t1([&future] {
    std::string s = "short"; // gcc, clang: no dynamic allocation SSO
    future = std::async(std::launch::async, [&s] {
      // s can already be destructed
      std::cout << s << "\n";
    });
    // s destructed
  });
  // task in future could continue here
  t1.join();
  future.wait();
}