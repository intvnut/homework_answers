#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>

int main() {
    std::cout << "Input 10 numbers: ";

    try {
        std::accumulate(
            std::istream_iterator<int>{std::cin},
            std::istream_iterator<int>{}, std::pair{0,0},
            [](auto sum, const auto elem) {
                static int i = 0;
                sum.first += elem;
                sum.second += elem * (elem & 1);
                i = i == 9 ? throw sum : i + 1;
                return sum;
            }
        );
    } catch(std::pair<int,int>& sum) {
        std::cout << "Sum of evens: " << sum.first - sum.second << '\n';
        std::cout << "Sum of odds:  " << sum.second << '\n';
        std::cout << "Sum of all:   " << sum.first << '\n';
    }
    return 0;
}
