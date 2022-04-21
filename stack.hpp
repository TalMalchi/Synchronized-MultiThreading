#include <iostream>
#include <cstdlib>
#include <pthread.h>
//#include <mutex>
//#include "mymalloc.hpp"
using namespace std;




//stack class implementation linked list
template <typename T> class stack {
  // private:
    struct node {
      T data;
      size_t size;
      bool is_free;
      node *next;
      
      
    };
    node *head;
    node *start;
    node *end1;
    pthread_mutex_t lock;
  
  public:

    stack() {
      // unsigned long size = sizeof(node);
      // cout << "size of node is " << size << endl;
      //node *check= (node *)my_malloc(sizeof(node)); 
      node *check= (node*)my_malloc(sizeof(node)); 
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
        //delete temp;
      free(temp);
      
      }
    }
    void push(const T& new_data) {
      
      //node *check= (node*)my_malloc(sizeof(node));
      node *check= (node*)my_malloc(sizeof(node));
      if (check==NULL)
      {
       cout << "Null pointer has been returned" << endl;
      }
      
      cout << " inside push-stack" << endl;
      node *temp = new node;
      temp->data = new_data;
      temp->next = head;
      temp->is_free = false;
      temp->size = sizeof(node);
      // update the other variables!!!!!!!!!
      head = temp;
    }
    void pop() {
      
        cout << " inside pop-stack" << endl;
      if(head != NULL) {
        node *temp = head;
        head = head->next;
        //delete temp;
        my_free((void*)temp);
      }}
  
    
    T top() const {
      if (head != NULL)
      {
        return head->data;
      }
      return NULL; 
    }

    
    bool empty() const {
      return head == NULL;
    }

  

    node *look_for_memory(size_t size){
      node *temp = start;
      while(temp != NULL) {
        if(temp->is_free && temp->size >= size)
          return temp;
        temp = temp->next;
      }
        return (NULL);
    }
    node *update_node(node *nnode, size_t size, void *memory_block){
      nnode = (node*)memory_block;
      nnode->size = size;
      nnode->is_free = false;
      nnode->next = NULL;
      if(!start)
        start = nnode;
      if(end1)
        end1->next = nnode;
      end1 = nnode;
      pthread_mutex_unlock(&lock);
      return (nnode);
    }

      void * my_malloc(unsigned long size){
        
        void *memory_block;
        node *header;
        pthread_mutex_lock(&lock);
        if(!size)
          return (NULL);
        header = look_for_memory(size);
        if(header != NULL) {
          header->is_free = false;
          pthread_mutex_unlock(&lock);
          return (void*)(header + 1);
        }
        memory_block = sbrk((sizeof(node) + size));
        if(memory_block == (void*) - 1) {
          pthread_mutex_unlock(&lock);
          return (NULL);
        }
        header = update_node(header, size, memory_block);
        return (void*)(header + 1);
      }
        
    

 

void my_free(void *ptr)

{
    node *new_node;
    void *new_brk;

 

    if (!ptr)

        return;

    pthread_mutex_lock(&lock);

    new_node = (node*)ptr - 1;

    new_brk = sbrk(0);

    if ((char*)ptr + new_node->size == new_brk) {

        if (start == end1) {

            start = NULL;

            end1 = NULL;

        } else {

            delete_pointers();

        }

        sbrk((0 - sizeof(node) - new_node->size));

    } else {

        new_node->is_free = true;

    }

    pthread_mutex_unlock(&lock);

}   
    

void delete_pointers(void)

{

    node *buff = start;

 

    while (buff) {

        if (buff->next == end1) {

            buff->next = NULL;

            end1 = buff;

        }

        buff = buff->next;

    }
}




 
};


