//------------------------------------------------------------------------------
//
// Output random correctly nested braced expression
// Format is: (()()()) or 1 1 0 1 0 1 0 0 by option
//
// try:
// > ./randbraces --n=5
// > ./randbraces --n=5 --ones
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#include "dice.hpp"
#include "opts.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

namespace {

constexpr int DEF_N = 5;
constexpr int DEF_SEED = 0;
constexpr bool DEF_ONES = false;
constexpr bool DEF_SEED_EXISTS = false;
constexpr bool DEF_VERBOSE = false;

struct Config {
  int N = DEF_N;
  int Seed = DEF_SEED;
  bool ZeroOne = DEF_ONES;
  bool SeedExists = DEF_SEED_EXISTS;
  bool Verbose = DEF_VERBOSE;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("n", DEF_N, "number of brace pairs");
  OptParser.template add<int>("ones", DEF_ONES,
                              "Use 1 and 0 instead of ( and )");
  OptParser.template add<int>("seed", DEF_SEED,
                              "optional seed to be reproducible");
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.parse(argc, argv);

  Cfg.ZeroOne = OptParser.exists("ones");
  Cfg.SeedExists = OptParser.exists("seed");
  Cfg.Verbose = OptParser.exists("verbose");
  Cfg.N = OptParser.get<int>("n") * 2;
  Cfg.Seed = OptParser.get<int>("seed");
  return Cfg;
}

// Knuth, 7.2.1.6.W
std::vector<bool> bitstring(int n) {
  assert((n % 2) == 0);

  int p = n / 2, q = n / 2, m = 1;
  std::vector<bool> res(n);

  while (q != 0) {
    for (;;) {
      int x = rands::Dice::generate(0, (q + p) * (q - p + 1) - 1);
      assert(q + p == n - m + 1);
      if (x < (q + 1) * (q - p)) {
        q -= 1;
        assert(res[m - 1] == false);
        m += 1;
        break;
      }
      p -= 1;
      res[m - 1] = true;
      m += 1;
    }
  }

  return res;
}

} // namespace

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);
  if (!Cfg.SeedExists) {
    auto Seed = rands::Dice::init();
    if (Cfg.Verbose)
      std::cout << "Seed not provided. Using: " << Seed << "\n";
  } else
    rands::Dice::init(Cfg.Seed);

  std::vector<bool> V = bitstring(Cfg.N);
  std::cout << V.size() / 2 << "\n";

  if (Cfg.ZeroOne) {
    std::ostream_iterator<int> OsIt(std::cout, " ");
    std::transform(V.begin(), V.end(), OsIt, [](bool S) { return S ? 1 : 0; });
  } else {
    std::ostream_iterator<char> OsIt(std::cout, "");
    std::transform(V.begin(), V.end(), OsIt,
                   [](bool S) { return S ? '(' : ')'; });
  }
  std::cout << "\n";
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}