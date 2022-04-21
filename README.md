# Synchronized-MultiThreading

* Written by Guy Azoulay - 207709734 
             Tal Malchi -  208278556
             
             
In this assigment we asked to build our own stack using MUTEX and multiThreading, we wrote this assigment in the C++ language.

## Main goal of this project
The main goal of this project is deep understanding about the multiThreading world and communication
between number of clients and one server.
In addition, in this project we were facing with some issues of how to let one client to ask from the server
one action from the action that standart stack have ( PUSH , POP , TOP) without other client will interupt to his action,
this is where the Mutex takes place, every time that the one of our clients is asking from the server to do one action,
we lock this function using Mutex and in that way we ensure that no other client will interupt to his action.

We were implemnted commends such as:
* PUSH - insert to our stack new string
* POP - remove the upper string 
* TOP - introduce to the client the upper string

In addition we were implemented the Malloc, Calloc and free commends which we will explain about later.

## Explanation about the Server class:

In our server class we use most of our code from Ex1 and made the necessary changes in aim to 
adjust our code to this assigment.
First of all, in this class we created our server socket using all the known commends (create socket ,bind , listen and accept)
and when the client ask from the server an action, the server do it job depands on the key words which stack have ( PUSH, POP, TOP).
Each one of the commends are surrounded by pthread mutex lock at the beggining and at the end of the function we unlock it, in aim to prevent a
case which more than one client trying to reach to the same function.

## Explanation about the Client class:

In our client class we use most of our code from Ex1 and made the necessary changes in aim to 
adjust our code to this assigment.
First of all, in this class we created our client socket using all the known commends (create socket ,connect and send)
and when the client want from the server an action, the client send one of the following messages ( PUSH, POP, TOP) to the server, 
If the commend is PUSH so the client also send the string he wants to add.


## Explanation about the Stack class:

In our stack stuct the are 4 fields:
* T data - which store the data inside the node
* size_t size - which store the size of our node
* bool is_free - a boolean variable that check if the place in memory we allocate to it was free or not.
* node * next - pointer to the next node in our stack.
* 

In the stack class there are the main three function of stack:
* PUSH
* POP
* TOP

And as we asked for, we were implemented 3 function which related to memory allocation:
* Malloc - function that get a size of variable and return a pointer to the new place in memory which 
           it now allocated to, when we are creating new stack we are using this function.
           
* Free - free the memory space which was given by the malloc or calloc functions, using in the "POP" method.

* Calloc - function that gets the number of arguments and the size of them and allocating a new place in the heap memory,
           when we want to use the "PUSH" commend we are using this function.
           
           
           
           
## The My_malloc function:

In aim to achive the main goal of the malloc function, we write two auxiliary functions:

* look_for_memory function - this function duty is to locate new place in our memory.
* update_node function - this function update all the stack fields.

In the my_malloc function we used the sbrk which used to change the space allocated for the calling process,
and again we lock our function using mutex in aim that we will allocate only the necessary place And more than one client can not 
access the allocation of new space in memory. 


## The My_calloc funtion:

In this function we used our my_malloc function, the only change in it is that we send also the numbers
of variables we want to allocate memory.

## The my_free function:

In this function we release the space which we have allocated to the specific variable, 
we use this function when we perform the "POP" operation.












             
