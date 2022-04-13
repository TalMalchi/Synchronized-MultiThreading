#include <iostream>
using namespace std;


//stack class implementation linked list
template <typename T> class stack {
  public:
    stack() {
      m_head = NULL;
    }
    ~stack() {
      while(m_head != NULL) {
        node *temp = m_head;
        m_head = m_head->m_next;
        delete temp;
      }
    }
    void push(const T& item) {
        cout << " inside push-stack" << endl;
      node *temp = new node;
      temp->m_data = item;
      temp->m_next = m_head;
      m_head = temp;
    }
    void pop() {
        cout << " inside pop-stack" << endl;

      if(m_head != NULL) {
        node *temp = m_head;
        m_head = m_head->m_next;
        delete temp;
      }
    }
    T top() const {
      return m_head->m_data;
    }
    bool empty() const {
      return m_head == NULL;
    }
  private:
    struct node {
      T m_data;
      node *m_next;
    };
    node *m_head;
};


