#include <array>
#include <cctype>
#include <cmath>
#include <iostream>
#include <string>

void decode_nucleotide(const char c) {
  const double uc = std::toupper(c);
  std::array<char, 5> dec = {
    char(2579984.7105263127 + uc*(-145861.7543859648
                + uc*(3082.5043859649
                + uc*(-28.8508771930
                + uc*(0.1008771930))))),
    char(3272576.9643962830 + uc*(-184959.6863340477
                + uc*(3907.9459494324
                + uc*(-36.5730134159
                + uc*(0.1278776693))))),
    char(182127.6826625386 + uc*(-10289.0994879731
                + uc*(217.4167956656
                + uc*(-2.0348297214
                + uc*(0.0071147892))))),
    char(21634849.0294117630 + uc*(-1222918.4558823530
                + uc*(25840.3308823529
                + uc*(-241.8382352941
                + uc*(0.8455882353))))),
    0
  };

  throw std::string{char(uc)} + &dec[0] + "ine";
}

int main() {
  std::string s;

  do {
    std::cout << "Input 5 nucleotide characters w/out spaces." << std::endl;
    std::cin >> s;
  } while (std::cin.good() && s.length() != 5);

  for (const auto c : s) {
    try {
      decode_nucleotide(c);
    } catch (std::string n) {
      std::cout << n << '\n';
    }
  }
}
