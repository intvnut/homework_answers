#include <algorithm> 
#include <iostream> 
#include <stack> 
#include <string> 
 
int main() { 
  auto stack = std::stack<std::string>{}; 
 
  stack.emplace("Hello World!"); 
  std::cout 
    << "Before: " << stack.top() << '\n'; 
 
  std::reverse( 
    stack.top().begin(), 
    stack.top().end() 
  ); 
 
  std::cout 
    << "After:  " << stack.top() << '\n'; 
} 
