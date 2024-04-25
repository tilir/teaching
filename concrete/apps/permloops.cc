//------------------------------------------------------------------------------
//
// Input permutation, output loop format
// Permutation format: N a1 .. aN
// Loops are like: (ai aj)(ax ay az av)
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#include "permloops.hpp"
#include "dice.hpp"
#include "opts.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

namespace {

constexpr bool DEF_VERBOSE = false;

struct Config {
  bool Verbose = DEF_VERBOSE;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.parse(argc, argv);

  Cfg.Verbose = OptParser.exists("verbose");
  return Cfg;
}

} // namespace

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);

  int N;
  std::cin >> N;

  using Dom = permutations::IDomain<int>;
  Dom::init(N);

  std::vector<Dom> Vec(N);

  for (int I = 0; I < N; ++I)
    std::cin >> Vec[I];

  using PL = permutations::PermLoop<Dom>;
  std::vector<PL> Loops;

  permutations::create_loops(Vec.begin(), Vec.end(), std::back_inserter(Loops));

  for (auto &&L : Loops)
    std::cout << L;
  std::cout << std::endl;
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}