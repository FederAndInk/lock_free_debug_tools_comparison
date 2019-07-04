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
    bool        done = false;
    std::string payload;
    std::thread t1([&done, &payload] {
      // 1: waiting for the payload to be ready
      // read of done
      while (!done) // #1
      {
      }
      // 3: use payload
      std::cout << payload << "\n";
    });
    std::thread t2([&done, &payload] {
      payload = "Really very big string that we don't see the end, even now we ";
      payload += "don't know when it'll stop...";
      // 2: the payload is ready:
      // write of done at "the same time" as #1
      done = true;
    });
    t1.join();
    t2.join();
  }
  return 0;
}
