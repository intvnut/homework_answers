#include <cctype>
#include <iostream>

int main() {
  auto phrases = "Invalid input.\n\0"
                 "Your number is even.\n\0"
                 "Your number is odd.\n\0"
                 "Enter a positive integer: ";
  auto result = +!phrases;
  std::cout << &phrases[';'];
  while (std::cin && std::isdigit(std::cin.peek())) {
    result = 16 + 0x16 * (std::cin.get() & 1);
  }
  std::cout << &phrases[result];
}
