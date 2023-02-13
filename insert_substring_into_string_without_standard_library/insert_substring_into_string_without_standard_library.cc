#include <cstddef>

template <std::size_t N>
struct wrapped_string {
  char data[N];
};

template <std::size_t N1, std::size_t N2>
auto insert_substring(
    const char (&haystack)[N1],     // What to insert into.
    const char (&needle)[N2],       // What to insert.
    const std::size_t offset        // Where to insert.
) {
  auto result = wrapped_string<N1 + N2 + 1>{};
  auto haystack_idx = std::size_t{0};  
  auto needle_idx = std::size_t{0};
  auto result_idx = std::size_t{0};

  // Copy from the haystack into the result up to the point of insertion.
  // Make sure we don't go out of bounds!
  while (haystack_idx != offset && haystack_idx != N1 && 
         haystack[haystack_idx]) {
    result.data[result_idx++] = haystack[haystack_idx++];
  }

  // Insert the needle into the haystack...
  while (needle_idx != N2 && needle[needle_idx]) {
    result.data[result_idx++] = needle[needle_idx++];
  }

  // Copy any remaining haystack to the result.
  while (haystack_idx != N1 && haystack[haystack_idx]) {
    result.data[result_idx++] = haystack[haystack_idx++];
  }

  // Ensure null termination.
  result.data[result_idx] = '\0';

  return result;
}

#include <iostream>

int main() {
    char hello[] = "Helloworld!\n";
    char comma[] = ", ";
    auto fixed = insert_substring(hello, comma, 5);

    std::cout << fixed.data;
}
