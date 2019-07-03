#include <atomic>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>

template <typename T>
struct node
{
  T     data;
  node* next;

  node(const T& data) : data(data), next(nullptr) {}
};

bool compare_exchange_weak(std::atomic<node<int>*>& a, node<int>*& exp, node<int>* des,
                           bool defer = false)
{
  if (defer)
  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ns);
  }
  return a.compare_exchange_weak(exp, des);
}

template <typename T>
class stack
{
  std::atomic<node<T>*> head{nullptr};

public:
  ~stack()
  {
    while (head.load(std::memory_order_relaxed))
    {
      pop();
    }
  }

  void push(const T& data)
  {
    node<T>* new_node = new node<T>(data);
    // std::cout << "add node(" << data << ") at: " << new_node << "\n";

    new_node->next = head.load(std::memory_order_relaxed);

    while (!head.compare_exchange_weak(new_node->next, new_node))
      ; // the body of the loop is empty
  }

  T pop(bool defer = false)
  {
    node<T>* old_head = head.load();
    int      tr = 1;
    while (old_head && !compare_exchange_weak(head, old_head, old_head->next, defer))
      ++tr;
    if (old_head)
    {
      T tmp = std::move(old_head->data);
      // std::cout << "pop: " << tmp << " (" << tr << " try) at: " << old_head << "\n";
      delete old_head;
      return tmp;
    }
    throw std::out_of_range("stack is empty");
  }

  template <class T2>
  friend std::ostream& operator<<(std::ostream& out, stack<T2> const& s);
};

int main(int argc, char const* argv[])
{
  using namespace std::string_literals;
  size_t nb = 1;
  if (argc >= 2)
  {
    // std::cout << "usage: " << argv[0] << " nb [defer=false]\n";
    nb = std::stoul(argv[1]);
  }

  bool defer = false;
  if (argc == 3)
  {
    defer = argv[2] == "true"s;
  }

  size_t runs = 0;
  for (size_t i = 0; i < nb; i++)
  {
    ++runs;
    // std::cout << "============================\n";
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    node<int>*  n;
    int         v;
    std::thread t1([&s, &v, defer] { v = s.pop(defer); });
    std::thread t2([&s, &n] {
      s.pop();
      s.pop();
      n = new node(666);
      s.push(5);
    });
    t1.join();
    t2.join();
    // std::cout << s << "\n";
    if (s.pop() == 666)
    {
      std::cout << "ABA' detected after: " << runs << " runs\n";
    }
    else
    {
      delete n;
    }
    //          std::cout << "-----------------------------\n";
  }
}

template <class T>
std::ostream& operator<<(std::ostream& out, stack<T> const& s)
{
  node<T>* cur = s.head.load(std::memory_order_relaxed);
  while (cur)
  {
    out << cur->data;
    if (cur->next)
    {
      out << ", ";
    }
    cur = cur->next;
  }
  return out;
}
