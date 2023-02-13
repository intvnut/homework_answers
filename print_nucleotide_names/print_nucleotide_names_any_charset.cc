#include <array>
#include <cctype>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>

void decode_nucleotide(const char c) {
  static const char *alpha = "abcdefghijklmnopqrstuvwxyz";
  const double dc = std::strchr(alpha, std::tolower(c)) - alpha;
  std::array<char, 5> dec = {
    alpha[unsigned(3.5
             + dc*(-7.45614035088
             + dc*(13.8201754386
             + dc*(-2.62280701754
             + dc*0.100877192982)))) % 27],
    alpha[unsigned(4.5
             + dc*(-16.0382432325
             + dc*(17.907249742
             + dc*(-3.32481940144
             + dc*0.127877669286)))) % 27],
    alpha[unsigned(13.5
             + dc*(-0.786794474875
             + dc*(0.984907120743
             + dc*(-0.184984520124
             + dc*0.00711478923553)))) % 27],
    alpha[unsigned(26.5
             + dc*(6.70588235294
             + dc*(-8.17647058824
             + dc*(1.52941176471
             + dc*-0.0588235294118)))) % 27],
    0
  };

  throw std::string{c} + &dec[0] + "ine";
}

int main() {
  std::string s;

  do {
    std::cout << "Input 5 nucleotide characters w/out spaces." << std::endl;
    std::cin >> s;
  } while (std::cin.good() && s.length() != 5);

  for (const auto c : s) {
    if (std::isalpha(c)) {
      try {
          decode_nucleotide(c);
      } catch (std::string n) {
          std::cout << n << '\n';
      }
    }
  }
}
