#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "malloc.hpp"
using namespace std;



//stack class implementation linked list
template <typename T> class stack {
  public:
    stack() {
      //node *check= (node *)malloc(sizeof(node)); 
      node *check = (node*) my_malloc(sizeof(node)); 
      if (check==NULL)
      {
       cout << "Null pointer has been returned" << endl;
      }
      
      head = NULL;
    }
    ~stack() {
      while(head != NULL) {
        node *temp = head;
        head = head->next;
        delete temp;
      }
    }
    void push(const T& new_data) {
      //node *check= (node*)malloc(sizeof(node));
      node *check = (node *)my_malloc(sizeof(node));
      
      if (check==NULL)
      {
       cout << "Null pointer has been returned" << endl;
      }
      
      cout << " inside push-stack" << endl;
      node *temp = new node;
      temp->data = new_data;
      temp->next = head;
      head = temp;
    }
    void pop() {
        cout << " inside pop-stack" << endl;
      if(head != NULL) {
        node *temp = head;
        head = head->next;
        //delete temp;
        free(temp);
      }
    }
    T top() const {
      return head->data;
    }
    bool empty() const {
      return head == NULL;
    }
  private:
    struct node {
      T data;
      node *next;
    };
    node *head;
};


