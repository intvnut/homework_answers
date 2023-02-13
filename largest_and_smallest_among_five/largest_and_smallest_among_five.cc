#include <algorithm> 
#include <array> 
#include <iostream> 
 
int main() { 
  auto a = std::array{ 
    47, 19, 6, 84, 23 
  }; 
 
  while ( 
    std::next_permutation( 
      a.begin(), a.end() 
    ) 
  ); 
 
  std::cout 
    << "Minimum: " << a.front() << '\n' 
    << "Maximum: " << a.back() << '\n'; 
} 
