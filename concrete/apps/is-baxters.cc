//------------------------------------------------------------------------------
//
// Input permutation, output is it baxter or not
// Permutation format: N a1 .. aN
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#include "baxters.hpp"
#include "dice.hpp"
#include "opts.hpp"
#include "permutations.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;

namespace {

constexpr bool DEF_BATCH = false;
constexpr bool DEF_COMBINE = false;
constexpr bool DEF_FILTER = false;
constexpr bool DEF_VERBOSE = false;

struct Config {
  bool Batch = DEF_BATCH;
  bool Combine = DEF_COMBINE;
  bool Filter = DEF_FILTER;
  bool Verbose = DEF_VERBOSE;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("batch", DEF_BATCH, "batch mode");
  OptParser.template add<int>("combine", DEF_BATCH,
                              "output combined perm and if Baxters");
  OptParser.template add<int>("filter", DEF_BATCH,
                              "output perm itself if Baxters or nothing");
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.parse(argc, argv);

  Cfg.Batch = OptParser.exists("batch");
  Cfg.Combine = OptParser.exists("combine");
  Cfg.Filter = OptParser.exists("filter");
  Cfg.Verbose = OptParser.exists("verbose");
  return Cfg;
}

void ProcessStatus(std::istream &Is) {
  if (Is.bad())
    std::cerr << "I/O error while reading\n";
  else if (!Is.eof() && Is.fail())
    std::cerr << "Bat data encountered\n";
}

using Dom = permutations::IDomain<int>;

template <std::forward_iterator It> void OutPerm(It Start) {
  for (int I = 0; I < Dom::Max(); ++I) {
    std::cout << *Start++;
    if (I != Dom::Max() - 1)
      std::cout << " ";
  }
}

template <std::forward_iterator It>
void OutBaxters(It Start, Config Cfg, bool Baxters) {
  if (!Cfg.Filter && !Cfg.Combine) {
    std::cout << Baxters << std::endl;
    return;
  }

  if (!Cfg.Filter && Cfg.Combine) {
    std::cout << (Baxters ? "+ " : "- ");
    OutPerm(Start);
    std::cout << std::endl;
    return;
  }

  // filtered output only if Baxters
  if (Baxters) {
    OutPerm(Start);
    std::cout << std::endl;
  }
}

// read permutation in given domain, check if Baxters
bool Check(Config Cfg) {
  int N = Dom::Max();
  std::vector<Dom> Vec(N);
  for (int I = 0; I < N; ++I) {
    std::cin >> Vec[I];
    if (!std::cin) {
      ProcessStatus(std::cin);
      return false;
    }
  }

  bool IsBaxters = permutations::isBaxters(Vec.begin(), Vec.end());
  OutBaxters(Vec.begin(), Cfg, IsBaxters);
  return true;
}

bool DomReinit() {
  int N;
  std::cin >> N;
  if (!std::cin) {
    ProcessStatus(std::cin);
    return false;
  }
  Dom::init(N);
  return true;
}

} // namespace

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);

  // if we are filtering or combining we need make things machine-readable
  // by producing max in first line
  bool NeedMax = Cfg.Filter || Cfg.Combine;

  if (!DomReinit())
    return -1;

  if (NeedMax)
    std::cout << Dom::Max() << std::endl;

  for (;;) {
    // read permutation and check if Baxters
    if (!Check(Cfg))
      break;

    // if not batch mode we need to read N before next in file
    if (!Cfg.Batch) {
      if (!DomReinit())
        break;

      // and probably output new N
      if (NeedMax)
        std::cout << Dom::Max() << std::endl;
    }
  }
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}