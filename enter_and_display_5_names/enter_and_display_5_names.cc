#include <iostream>
#include <string>

int main() {
    std::string s;

    while (true) {
        std::cout << "Enter 5 names: ";
        std::getline(std::cin, s);
        if (s == "5 names") {
            break;
        }
        std::cout << "That is not 5 names.  Please try again.\n";
    }

    std::cout << "Congratulations!  Now displaying: " << s << '\n';
    return 0;
}
