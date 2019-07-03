#include <atomic>
#include <exception>
#include <iostream>
#include <memory>
#include <random>
#include <shared_mutex>
#include <stdexcept>
#include <thread>

template <class T>
class atomic_shared_ptr
{
private:
  std::shared_ptr<T> p;

public:
  atomic_shared_ptr(T* ptr) : p(ptr) {}
  atomic_shared_ptr(atomic_shared_ptr<T> const& oth) = delete;
  atomic_shared_ptr(std::shared_ptr<T> const& ptr) : p(ptr) {}
  atomic_shared_ptr(std::shared_ptr<T>&& ptr) : p(std::move(ptr)) {}

  atomic_shared_ptr&    operator=(atomic_shared_ptr<T> const& oth) = delete;
  atomic_shared_ptr<T>& operator=(std::shared_ptr<T> const& ptr)
  {
    store(ptr);
    return *this;
  }

  operator std::shared_ptr<T>() const
  {
    return load();
  }

  void store(std::shared_ptr<T> const& ptr,
             std::memory_order         ord = std::memory_order_seq_cst)
  {
    std::atomic_store_explicit(&p, ptr, ord);
  }

  std::shared_ptr<T> load(std::memory_order ord = std::memory_order_seq_cst) const
  {
    return std::atomic_load_explicit(&p, ord);
  }

  bool compare_exchange_weak(std::shared_ptr<T>&       expected,
                             std::shared_ptr<T> const& desired)
  {
    return atomic_compare_exchange_weak(&p, &expected, desired);
  }
};

template <typename T>
struct node
{
  T                        data;
  std::shared_ptr<node<T>> next;

  node(const T& data) : data(data), next(nullptr) {}
};

template <typename T>
class stack
{
  atomic_shared_ptr<node<T>> head{nullptr};

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
    std::shared_ptr<node<T>> new_node = std::make_shared<node<T>>(data);
    // std::cout << "add node(" << data << ") at: " << new_node << "\n";

    new_node->next = head.load(std::memory_order_relaxed);

    while (!head.compare_exchange_weak(new_node->next, new_node))
      ; // the body of the loop is empty
  }

  T pop()
  {
    std::shared_ptr<node<T>> old_head = head.load();
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
      ;
    if (old_head)
    {
      T tmp = std::move(old_head->data);
      // std::cout << "pop: " << tmp << " (" << tr << " try) at: " << old_head << "\n";
      return tmp;
    }
    throw std::out_of_range("stack is empty");
  }

  template <class T2>
  friend std::ostream& operator<<(std::ostream& out, stack<T2> const& s);
};

int main()
{
  std::shared_mutex mut;
  {
    std::shared_ptr<node<int>> ptr{};
    std::cout << "is atomic shared_ptr lockfree ? " << std::boolalpha
              << std::atomic_is_lock_free(&ptr) << "\n";
  }

  for (size_t i = 0; i < 10ul; i++)
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
  std::shared_ptr<node<T>> cur = s.head.load(std::memory_order_relaxed);
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
