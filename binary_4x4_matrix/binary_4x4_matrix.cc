#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

int main() {
  constexpr auto rm = 1152939097061330944ull;
  constexpr auto cm = 8738ull;
  auto rd = std::random_device{};
  auto re = std::default_random_engine{rd()};
  auto ud = std::uniform_int_distribution<decltype(0ull)>{};
  const auto bm = ud(re) & 1229782938247303441ull;
  const auto rs = (bm * 4369) & 8070573679429316608ull;
  const auto cs = (bm * 281479271743489ull) >> 47;
  const auto mx = [](auto v, const auto xm) {
    const auto x4 = xm*4; while (!(v & x4)) v += xm;
    auto m = v & x4, t = m; while (t) m = t, t &= t - 1;
    return 2*!(m & 4611756387171600384ull) +
           !(m & 4611686019501162624ull);
  };
  const auto mr = mx(rs, rm), mc = mx(cs, cm);
  auto os = std::ostringstream{};
  os << std::hex << std::setw(16) << std::setfill('0') << bm;
  const auto bs = os.str();
  const auto ms = bs.substr(0,4) + '\n' + bs.substr(4,4) + '\n'
                + bs.substr(8,4) + '\n' + bs.substr(12.4) + '\n';
  std::cout << ms << "Max row: " << mr << "\nMax col: " << mc << '\n';
}
