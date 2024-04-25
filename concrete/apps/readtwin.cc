//------------------------------------------------------------------------------
//
// Read permutation from stdin and create twin trees
// Permutation format: N a1 .. aN
// Dump resulting twins to stdout (plenty of options)
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#include "opts.hpp"
#include "twintree.hpp"
#include <iostream>
#include <ranges>

namespace {

namespace ranges = std::ranges;

constexpr bool DEF_BATCH = false;
constexpr bool DEF_BAXTERS = false;
constexpr bool DEF_VERBOSE = false;

struct Config {
  bool Batch = DEF_BATCH;
  bool Baxters = DEF_BAXTERS;
  bool Verbose = DEF_VERBOSE;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("batch", DEF_BATCH,
                              "batch mode: single N then one by one");
  OptParser.template add<int>("baxters", DEF_BAXTERS,
                              "output equivalent Baxter permutation");
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.parse(argc, argv);

  Cfg.Batch = OptParser.exists("batch");
  Cfg.Baxters = OptParser.exists("baxters");
  Cfg.Verbose = OptParser.exists("verbose");
  return Cfg;
}

void ProcessStatus(std::istream &Is) {
  if (Is.bad())
    std::cerr << "I/O error while reading\n";
  else if (!Is.eof() && Is.fail())
    std::cerr << "Bat data encountered\n";
}

// step of batch execution
bool BatchStep(int N, std::istream &Is, std::ostream &Os, const Config &Cfg) {
  std::optional<trees::TwinTree> Tree = std::nullopt;

  if (!Cfg.Batch) {
    Is >> N;
    if (!Is) {
      ProcessStatus(Is);
      return false;
    }
  }

  Tree = trees::read_twin_ordered(N, Is);
  if (!Tree) {
    ProcessStatus(Is);
    return false;
  }

  if (Cfg.Baxters) {
    std::ostream_iterator<int> OsIt(std::cout, " ");
    auto B = Tree->toBaxters(Cfg.Verbose);
    if (!Cfg.Batch)
      std::cout << N << std::endl;
    ranges::copy(B, OsIt);
    std::cout << std::endl;
  } else
    Tree->dumpTable(Os);
  return true;
}

} // namespace

int main(int argc, char **argv) try {
  auto Cfg = parse_cfg(argc, argv);
  int N = 0;
  if (Cfg.Batch) {
    std::cin >> N;
    if (!std::cin) {
      ProcessStatus(std::cin);
      return 0;
    }
    std::cout << N << std::endl;
  }
  while (BatchStep(N, std::cin, std::cout, Cfg)) {
  }
} catch (const trees::tree_error_base &T) {
  std::cout << "Tree error: " << T.what() << " at key: ";
  T.dump_key(std::cout);
  std::cout << std::endl;
  return -1;
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << std::endl;
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}
