#include <iostream>
#include <stack>
#include <mutex>

using namespace std;



template <typename T> class MyThreadSafeStack {
  public:
    void push(const T& item) {
      lock_guard<mutex> lock(m_mutex);
      m_stack.push(item);
    }
    void pop() {
      lock_guard<mutex> lock(m_mutex);
      m_stack.pop();
    }
    T top() const { // note that we shouldn't return a reference,
                    // because another thread might pop() this
                    // object in the meanwhile
      lock_guard<mutex> lock(m_mutex);
      return m_stack.top();
    }

  private:
    mutable std::mutex m_mutex;
    std::stack<T> m_stack;
};