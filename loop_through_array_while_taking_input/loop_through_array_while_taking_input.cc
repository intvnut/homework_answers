#include <array>
#include <iostream>
#include <future>
#include <string>
#include <thread>

int loop_through_int_array(const std::array<int, 10>& array) {
  int sum = 0;
  for (auto elem : array) {
    sum += elem;
  }
  return sum;
}

bool function_that_takes_user_input() {
  std::string s;

  while (true) {
    std::cout << "Are we there yet? [y/n]  ";
    std::cin >> s;
    if (s == "y" || s == "n") break;
    std::cout << "Please enter y or n.\n";
  }
  return s == "y";
}

int main() {
  auto array = std::array<int, 10>{ 0,1,2,3,4,5,6,7,8,9 };
  auto future = std::future<int>{
    std::async(std::launch::async, [&array]{
      return loop_through_int_array(array);
    })};

  while (!function_that_takes_user_input()) {
    // do nothing
  }

  future.wait();

  std::cout << "loop_through_int_array returned " << future.get()
            << '\n';
}
