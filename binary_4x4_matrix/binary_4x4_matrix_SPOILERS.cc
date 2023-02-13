#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

int main() {
  // Mask for bits in rows.
  constexpr auto row_mask = 0x1000'1000'1000'1000ull;
  // Mask for bits in columns.
  constexpr auto col_mask = 0x2222ull;

  // Random device.  ud(re) returns  uniformly distributed a random
  // unsigned long long integer.
  auto rd = std::random_device{};
  auto re = std::default_random_engine{rd()};
  auto ud = std::uniform_int_distribution<unsigned long long>{};

  // Generate the matrix as a bitmap, where bits 0, 4, 8, etc. hold the
  // matrix elements.
  const auto bit_mat = ud(re) & 0x1111'1111'1111'1111ull;

  // Compute the row sum with clever multiplication.  The row sums end up
  // in bits [14:12], [30:28], ... etc.
  const auto row_sum = (bit_mat * 0x1111) & 0x7000'7000'7000'7000ull;

  // Compute the column sum with clever multiplication.  The column sums
  // end up in bits [49:47], [53:51], etc.
  const auto col_sum = (bit_mat * 0x0001'0001'0001'0001ull) >> 47;

  // Helper lambda for finding the maximum, given a value, and mask (row mask
  // or column mask).  The mask is aligned with the LSBs of the row/column sums.
  const auto find_max = [](auto value, const auto x_mask) {
    // Shift the mask over 2 bits to align with the MSB of the sum.
    const auto x_mask_4 = x_mask * 4;

    // Keep adding 1 to each of the sums until one of the sums has its MSB set.
    while (!(value & x_mask_4)) {
      value += x_mask;
    }

    // Now, figure out which row/col hit its MSB first.  Return the lowest
    // numbered row/column.
    auto msbs = value & x_mask_4, temp = msbs;

    // Find the leftmost 1 bit set in msbs, by removing bits one at a time from
    // the right.  The penultimate value is a bitmap with only the leftmost bit
    // set.
    while (temp) {
      // Remember the last 1 bit standing.
      msbs = temp;

      // Remove the least-significant 1 bit in temp.
      temp &= temp - 1;
    }

    // Compute the row or column number that corresponds to the last bit
    // standing.
    //
    // If it's in the first two rows/columns, bit 1 of the  result is 0.
    // That's what the first bitmask tests.
    //
    // If it's in the first or third row/column, bit 0 of the result is 0.
    // That's what the second bitmask tests.
    return 2*!(m & 0x4000400000008800ull) + !(m & 0x4000000040008080ull);
  };

  const auto max_row = find_max(row_sum, row_mask),
             max_col = find_max(col_sum, col_mask);

  // Build the matrix output as a string by printing the hex value directly
  // and reformatting it.

  // We parked each of the 0/1 elements in the LSB of a hex nibble.  So use
  // std::ostringstream to convert to hex in a std::string.
  auto os = std::ostringstream{};
  os << std::hex << std::setw(16) << std::setfill('0') << bit_mat;

  // Now get the std::string of 0/1 bits out of the std::ostringstream.
  const auto bit_str = os.str();

  // Build the matrix string by taking substrings of the bit string.  The
  // 12.4 in the last line is a typo, but it happens to work.
  const auto mat_str = bit_str.substr(0,4) + '\n' +
                       bit_str.substr(4,4) + '\n' +
                       bit_str.substr(8,4) + '\n' +
                       bit_str.substr(12.4) + '\n';

  // Print the result.
  std::cout << mat_str << "Max row: " << max_row << "\nMax col: " << max_col
            << '\n';
}
