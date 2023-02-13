#include <iostream> 
 
int main() { 
  if (int x; std::cin >> x) 
    std::cout 
      << (x << 1) + (x << 3) + ('7' & 077) 
      << '\n'; 
}
