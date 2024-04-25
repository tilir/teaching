//------------------------------------------------------------------------------
//
// Read permutation from stdin and create bst
// Permutation format: N a1 .. aN
// Dump resulting bst to stdout (plenty of options)
//
// try:
// > ./randperm --first=1 --last=10 >& perm
// > ./readbst < perm
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#include "opts.hpp"
#include "tabtree.hpp"
#include <iostream>

namespace {

constexpr bool DEF_BACK = false;
constexpr bool DEF_IBRAC = false;
constexpr bool DEF_DOT = false;
constexpr bool DEF_TOPO = false;
constexpr bool DEF_BATCH = false;
constexpr bool DEF_VERBOSE = false;

enum class InpKind { PERM, BRACES };

enum class DumpKind { EDGELIST, DOT, TOPO };

struct Config {
  bool Back = DEF_BACK;
  bool Batch = DEF_BATCH;
  DumpKind DumpType = DumpKind::EDGELIST;
  InpKind InpType = InpKind::PERM;
  bool Verbose = DEF_VERBOSE;
};

Config parse_cfg(int argc, char **argv) {
  Config Cfg;
  options::Parser OptParser;
  OptParser.template add<int>("back", DEF_BACK, "read input backwards");
  OptParser.template add<int>("batch", DEF_BATCH,
                              "batch mode: single N then one by one");
  OptParser.template add<int>("ibrac", DEF_IBRAC, "input braced string");
  OptParser.template add<int>("dot", DEF_DOT, "output dot file");
  OptParser.template add<int>("topo", DEF_TOPO, "output braced topology");
  OptParser.template add<int>("verbose", DEF_VERBOSE, "a lot of debug output");
  OptParser.parse(argc, argv);

  Cfg.Back = OptParser.exists("back");
  Cfg.Batch = OptParser.exists("batch");
  bool HasDot = OptParser.exists("dot");
  bool HasTopo = OptParser.exists("topo");
  bool HasIbrac = OptParser.exists("ibrac");

  if (HasDot && HasTopo)
    throw std::runtime_error("please seelct -dot or -topo");

  if (HasDot)
    Cfg.DumpType = DumpKind::DOT;

  if (HasTopo)
    Cfg.DumpType = DumpKind::TOPO;

  if (HasIbrac)
    Cfg.InpType = InpKind::BRACES;

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
  std::optional<trees::TabTree<int>> Tree = std::nullopt;

  if (!Cfg.Batch) {
    Is >> N;
    if (!Is) {
      ProcessStatus(Is);
      return false;
    }
  }

  switch (Cfg.InpType) {
  case InpKind::BRACES:
    Tree = trees::read_bst_braced<int>(N, Is, Cfg.Back);
    break;
  default:
    Tree = trees::read_bst_ordered<int>(N, Is, Cfg.Back);
    break;
  }

  if (!Tree) {
    ProcessStatus(Is);
    return false;
  }

  switch (Cfg.DumpType) {
  case DumpKind::DOT:
    Tree->dumpDot(Os);
    break;
  case DumpKind::TOPO:
    Tree->dumpTopo(Os, Cfg.Batch);
    break;
  default:
    Tree->dumpEL(Os);
    break;
  }
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
  std::cout << "\n";
  return -1;
} catch (const std::runtime_error &E) {
  std::cout << "Runtime error: " << E.what() << "\n";
  return -1;
} catch (...) {
  std::cout << "Unknown error\n";
  return -1;
}