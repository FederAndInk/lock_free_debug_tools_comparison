#include <iostream>

int main()
{
  int* i = new int();
  std::cout << i << "\n";
  delete i;
  int* i2 = new int();
  std::cout << i2 << "\n";
  delete i2;
  if (i == i2)
  {
    std::cout << "same\n";
  }

  return 0;
}
