// #include <iostream>
// #include <stack>
// #include <mutex>
// #include <pthread.h>

// using namespace std;





// template <typename T> class stack_mutex {
//   public:
//     void push(const T& item) {
//       //pthread_mutex_lock(&mutex);
//       lock_guard<mutex> lock(m_mutex);
//       cout << " inside push" << endl;
//       m_stack.push(item);
//       lock_guard<mutex> unlock(m_mutex);
//       //pthread_mutex_unlock(&mutex);

//     }
//     void pop() {
//       lock_guard<mutex> lock(m_mutex);
//       //pthread_mutex_lock(&mutex);
//       cout << " inside pop" << endl;
//       m_stack.pop();
//       lock_guard<mutex> unlock(m_mutex);

//       //pthread_mutex_unlock(&mutex);
      
//     }
//     T top() const { // note that we shouldn't return a reference,
//                     // because another thread might pop() this
//                     // object in the meanwhile
      
//       lock_guard<mutex> lock(m_mutex);
//       return m_stack.top();
//         lock_guard<mutex> unlock(m_mutex);

//       //pthread_mutex_unlock(&m_mutex);

//     }

//   private:
//     //pthread_mutex_t mutex;
//     mutable std::mutex m_mutex;

//     std::stack<T> m_stack;
// };
