//------------------------------------------------------------------------------
//
// Input "N", output all subsets of set 0..N-1
// You may add labels
//
// Try:
// echo "3" | ./allsubsets
// echo "3 a b c" | ./allsubsets --labels
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#include "combview.hpp"
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

void print_subsets(Config Cfg, int N, int K) {
  if (Cfg.Verbose)
    std::cout << "K = " << K << std::endl;
  auto Cs = combs::comb_view(N, K);
  for (auto C : Cs) {
    for (auto Elt : C)
      std::cout << Elt << " ";
    std::cout << "\n";
  }
}

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);
  int N;
  std::cin >> N;
  for (int K = 1; K < N; ++K)
    print_subsets(Cfg, N, K);
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}