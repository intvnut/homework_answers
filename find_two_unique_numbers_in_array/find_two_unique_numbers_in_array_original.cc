#include <algorithm>
#include <iterator>
#include <utility>

template <typename InputIt>
auto find_first_two_unique(InputIt first, InputIt after_last)
    -> std::pair<decltype(*first), decltype(*first)> {
  const auto& first_unique = *first;

  std::advance(first, 1);

  auto next_unique_it =
    std::find_if(first, after_last, [&first_unique](auto second) {
        return first_unique < second || second < first_unique;
      });

  if (next_unique_it == after_last) {
    // What do you want to do here?  One option: Return first value twice.
    return {first_unique, first_unique};
  }

  return {first_unique, *next_unique_it};
}
