#include <cmath>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>

namespace {

using nnumber = std::string;

enum class Status {
   Ok = 0,
   Error
};

std::tuple<Status, nnumber, bool> ParseNNumber(std::string input) {
    static const auto match_n_regex = std::regex{"(n*)(g+!*)?"};
    auto matches = std::smatch{};

    if (std::regex_match(input, matches, match_n_regex)) {
        return { Status::Ok, matches[1], matches[2].length() > 0 };
    }

    return { Status::Error, "", false };
}

double NNumberGreatness(nnumber NNumber) {
    return std::sin(NNumber.length() / 42.0);
}

} // namespace

int main() {
    auto exited_early = true;
    auto larry = nnumber{}, curly = nnumber{}, moe = nnumber{};
    auto count = 0;

    for (auto input = std::string{};
         std::getline(std::cin, input);) {
        auto [status, nnumber, last] = ParseNNumber(input);
        if (status != Status::Ok) {
            std::cout << "That was not a valid 'n number'.  "
                         "Try again.\n";
            continue;
        }
        count++;
        if (NNumberGreatness(larry) < NNumberGreatness(nnumber)) {
            std::swap(larry, nnumber);
        }
        if (NNumberGreatness(curly) < NNumberGreatness(larry)) {
            std::swap(curly, larry);
        }
        if (NNumberGreatness(moe) < NNumberGreatness(curly)) {
            std::swap(moe, curly);
        }
        if (last) {
            exited_early = false;
            break;
        }
    }

    if (exited_early) {
        std::cout << "Tsk, tsk.  "
                     "EOF reached before last 'n number'!\n";
        return 1;
    }

    if (count < 3) {
        std::cout << "Tsk, tsk.  "
                     "I need at least three 'n number's!\n";
        return 1;
    }

    std::cout << "The three greatest 'n number's "
                 "I've seen this time:\n"
              << larry << '\n' << curly << '\n' << moe << '\n';

    return 0;
}
