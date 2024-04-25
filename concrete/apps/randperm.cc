//------------------------------------------------------------------------------
//
// Output random permutation (default is 1 .. N)
// Permutation format: N a1 .. aN
//
// try:
// > ./randperm --last=10
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

constexpr int DEF_FIRST = 1;
constexpr int DEF_LAST = 10;
constexpr int DEF_SEED = 0;
constexpr bool DEF_SEED_EXISTS = false;
constexpr bool DEF_VERBOSE = false;

struct Config {
  int First = DEF_FIRST;
  int Last = DEF_LAST;
  int Seed = DEF_SEED;
  bool SeedExists = DEF_SEED_EXISTS;
  bool Verbose = DEF_VERBOSE;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("first", DEF_FIRST, "starting permutation value");
  OptParser.template add<int>("last", DEF_LAST, "ending permutation value");
  OptParser.template add<int>("seed", DEF_SEED,
                              "optional seed to be reproducible");
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.parse(argc, argv);

  Cfg.SeedExists = OptParser.exists("seed");
  Cfg.Verbose = OptParser.exists("verbose");
  Cfg.First = OptParser.get<int>("first");
  Cfg.Last = OptParser.get<int>("last");
  Cfg.Seed = OptParser.get<int>("seed");
  return Cfg;
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
  std::vector<int> V(Cfg.Last - Cfg.First + 1);
  std::iota(V.begin(), V.end(), Cfg.First);
  rands::Dice::rand_shuffle(V.begin(), V.end());

  std::cout << V.size() << "\n";
  std::ostream_iterator<int> OsIt(std::cout, " ");
  std::copy(V.begin(), V.end(), OsIt);
  std::cout << "\n";
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}