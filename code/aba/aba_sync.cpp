#include <atomic>
#include <exception>
#include <iostream>
#include <random>
#include <shared_mutex>
#include <stdexcept>
#include <thread>

template <typename T>
struct node
{
  T     data;
  node* next;

  node(const T& data) : data(data), next(nullptr) {}
};

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

  T pop()
  {
    node<T>* old_head = head.load();
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
      ;
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

int main(int argc, char* argv[])
{
  size_t nb = 1;
  if (argc >= 2)
  {
    // std::cout << "usage: " << argv[0] << " nb [defer=false]\n";
    nb = std::stoul(argv[1]);
  }

  std::shared_mutex mut;

  for (size_t i = 0; i < nb; i++)
  {
    // std::cout << "============================\n";
    stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    s.push(5);
    mut.lock();

    std::thread t1([&] {
      std::shared_lock lck(mut);
      s.pop();
    });
    std::thread t2([&] {
      std::shared_lock lck(mut);
      s.pop();
      s.pop();
      s.push(5);
    });
    mut.unlock();
    t1.join();
    t2.join();
    std::cout << s << "\n";
    // std::cout << "-----------------------------\n";
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
