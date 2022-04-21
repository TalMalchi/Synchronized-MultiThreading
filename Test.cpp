#include <iostream>
#include <string>
#include "stack.hpp"
#include "doctest.h"
#include <cstdlib>
#include <pthread.h>
using namespace std;
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


TEST_CASE("Test Stack") {
    stack<string> stack;
    stack.push("Hello");
    stack.push("World");
    stack.push("!");
    CHECK(stack.top() == "!");
    stack.pop(); 
    CHECK(stack.top() == "World");
    stack.pop();
    CHECK(stack.top() == "Hello");
    stack.pop();
    CHECK(stack.empty());
    CHECK(stack.top() == "Error- Stack is empty!");
    //check
}