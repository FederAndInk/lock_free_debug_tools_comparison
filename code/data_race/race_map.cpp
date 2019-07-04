#include <map>
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
    std::map<int, int> m;

    std::thread t1([&m] {
      // 1: modify the map
      m[123] = 1;
    });
    std::thread t2([&m] {
      // 2: modify the map at "the same time" as #1
      m[345] = 0;
    });
    // std::map isn't safe for concurrency
    t1.join();
    t2.join();
  }
  return 0;
}
