#include <future>
#include <iostream>
#include <string>
#include <thread>

struct T
{
  int i;
};

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
      T t{1}; // t is on the stack
      future = std::async(std::launch::async, [&t] {
        // use of t but we are out of the t1 scope ?
        std::cout << t.i << "\n";
      });
      // t is destructed here
    });
    // task in future could continue here
    t1.join();
    future.wait();
  }
}