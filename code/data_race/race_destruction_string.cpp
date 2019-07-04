#include <future>
#include <iostream>
#include <string>
#include <thread>

int main(int argc, char* argv[])
{
  size_t nb = 1;
  if (argc >= 2)
  {
    nb = std::stoul(argv[1]);
  }

  for (size_t i = 0; i < nb; i++)
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
}