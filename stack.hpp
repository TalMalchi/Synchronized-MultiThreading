#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

// stack class implementation
template <typename T>
class stack
{
  // private:
  struct node
  {
    T data;       // variable which holds the data
    size_t size;  // variable which holds the size of the data
    bool is_free; // variable which holds the status of the node
    node *next;   // variable which holds the address of the next node
  };
  node *head;           // variable which holds the address of the head node
  node *start;          // variable which holds the address of the start node
  node *end1;           // variable which holds the address of the end node
  pthread_mutex_t lock; // variable which holds the lock

public:
  // constructor of the stack class
  stack()
  {
    node *check = (node *)my_malloc(sizeof(node)); // allocate memory for the head node using my_malloc function
    if (check == NULL)                             // if the allocation fails
    {
      cout << "Memory allocation failed" << endl; // print the error message
    }

    head = NULL;
  }
  // destructor of the stack class
  ~stack()
  {
    while (head != NULL)
    { // while the head node is not NULL
      node *temp = head;
      head = head->next;
      // free(temp);
      my_free(temp);
    }
  }

  // this function pushes the data to the stack
  void push(const T &new_data)
  {

    // node *check= (node*)calloc(sizeof(node));
    node *check = (node *)my_calloc(1, sizeof(node)); // allocate memory for the new node using my_calloc function
    if (check == NULL)
    {
      cout << "Null pointer has been returned" << endl;
    }

    cout << "inside push-stack" << endl; // self-checking
    node *temp = new node;                // create a new node
    temp->data = new_data;                // assign the data to the new node
    temp->next = head;                    // assign the next node to the head node
    temp->is_free = false;                // assign the status of the node to false
    temp->size = sizeof(node);            // assign the size of the node to the size of the node
    head = temp;                          // assign the head node to the new node
  }

  // this function pops the data from the stack
  void pop()
  {

    cout << "inside pop-stack" << endl; // self-checking
    if (head != NULL)
    {
      node *temp = head; // create a new node
      head = head->next; // assign the next node to the head node
      // free(temp)
      my_free((void *)temp); // free the memory of the node using my_free function
    }
  }

  // this function returns the top element of the stack
  T top() const
  {
    if (head != NULL)
    {
      return head->data;
    }
    return NULL;
  }

  // this function returns if the stack is empty or not
  bool empty() const
  {
    return head == NULL;
  }

  // this function looks for new memory by giving the size of the memory we need to allocate
  node *look_for_memory(size_t size)
  {
    node *temp = start;
    // while the start node is not NULL
    while (temp != NULL)
    {
      // checking the status of the node (if it is free) and the size of the node (if it is greater than or equal to the size we need)
      if (temp->is_free && temp->size >= size)
        return temp;
      temp = temp->next;
    }
    return (NULL);
  }

  // this function updates all node is variables
  node *update_node(node *new_node, size_t size, void *memory_block)
  {
    new_node = (node *)memory_block;
    new_node->size = size;
    new_node->is_free = false; // assign the status of the node to false
    new_node->next = NULL;
    if (!start) // if the start node is NULL
      start = new_node;
    if (end1) // if the end node is not NULL
      end1->next = new_node;
    end1 = new_node;
    pthread_mutex_unlock(&lock); // unlock the mutex
    return (new_node);
  }

  // this function allocates memory for the new node using the previous functions
  void *my_malloc(unsigned long size)
  {

    void *memory_block;
    node *new_node;
    pthread_mutex_lock(&lock);
    if (!size)
      return (NULL);
    new_node = look_for_memory(size); // look for the memory using the look_for_memory function
    if (new_node != NULL)
    {
      new_node->is_free = false;     // assign the status of the node to false
      pthread_mutex_unlock(&lock);   // unlock the mutex
      return (void *)(new_node + 1); // return pointer to the node
    }
    memory_block = sbrk(size);
    //memory_block = sbrk((sizeof(node) + size)); // allocate memory using sbrk function
    if (memory_block == (void *)-1)
    { // if the allocation fails
      pthread_mutex_unlock(&lock);
      return (NULL);
    }
    new_node = update_node(new_node, size, memory_block);
    return (void *)(new_node + 1);
  }

  // this function allocate memory for number of nodes using my_malloc function
  void *my_calloc(int number, unsigned long size)
  {
    return (my_malloc(number * size));
  }

  // this function frees the memory
  void my_free(void *ptr)

  {
    node *new_node;
    void *new_brk;
    if (!ptr)
      return;
    pthread_mutex_lock(&lock); // lock the mutex
    new_node = (node *)ptr - 1;
    new_brk = sbrk(0); // get the current sbrk val
                       // if the node is the last node
    if ((char *)ptr + new_node->size == new_brk)
    {
      if (start == end1)
      {
        start = NULL;
        end1 = NULL;
      }
      else
      {
        delete_pointers(); // delete the pointers using the delete_pointers function
      }

      sbrk((0 - sizeof(node) - new_node->size)); // free the memory using sbrk function
    }
    else
    {
      new_node->is_free = true; // assign the status of the node to true
    }
    pthread_mutex_unlock(&lock);
  }

  // this function deletes the pointers
  void delete_pointers(void)

  {

    node *buff = start;

    while (buff)
    {

      if (buff->next == end1)
      {

        buff->next = NULL;

        end1 = buff;
      }

      buff = buff->next;
    }
  }
};
