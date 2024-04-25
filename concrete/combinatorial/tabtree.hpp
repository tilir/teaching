//------------------------------------------------------------------------------
//
// Tabulated tree: binary tree with left and right vectors
// optimized for almost immutable trees
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <concepts>
#include <iostream>
#include <map>
#include <optional>
#include <ranges>
#include <stack>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace trees {

namespace ranges = std::ranges;

struct tree_error_base : public std::runtime_error {
  virtual void dump_key(std::ostream &) const = 0;
  tree_error_base(const char *Msg) : std::runtime_error(Msg) {}
  const char *what() const noexcept override {
    return std::runtime_error::what();
  }
};

template <typename T> struct tree_error : public tree_error_base {
  T Key;
  tree_error(const char *Msg, T K) : tree_error_base(Msg), Key(K) {}
  void dump_key(std::ostream &Os) const override { Os << Key; }
};

// tabulated tree class
template <typename T> class TabTree {
  std::vector<int> Left, Right;
  std::vector<T> Data;
  int Root = -1;
  int FreePos = 0;

  // Pos is right or left link in parent
  template <std::random_access_iterator It> void addNode(It Pos, T Dt) {
    assert(FreePos < Data.size());
    *Pos = FreePos;
    Data[FreePos] = std::move(Dt);
    FreePos += 1;
  }

  // ineffective, but really not a big deal: used for dot ouput only
  // may be in future I will rewrite better
  template <std::random_access_iterator It>
  void setRanks(It Ranks, int N) const {
    if (Left[N] != -1) {
      Ranks[Left[N]] = Ranks[N] + 1;
      setRanks(Ranks, Left[N]);
    }
    if (Right[N] != -1) {
      Ranks[Right[N]] = Ranks[N] + 1;
      setRanks(Ranks, Right[N]);
    }
  }

  // like dumpEL but unordered, with nil node and dot specifics
  // mark left links red, right links blue
  void dumpELDot(std::ostream &os) const {
    for (int I = 0; I < Data.size(); ++I) {
      os << Data[I] << " -- ";
      if (Left[I] != -1)
        os << Data[Left[I]] << " [color = red]\n";
      else
        os << "nil [style=dotted]\n";

      os << Data[I] << " -- ";
      if (Right[I] != -1)
        os << Data[Right[I]] << " [color = blue]\n";
      else
        os << "nil [style=dotted]\n";
    }
  }

  template <typename F> void visitInord(F Visitor) {
    if (Root == -1)
      return;
    assert(Root == 0);
    std::stack<int> S;
    int N = Root;
    while (!S.empty() || N != -1) {
      if (N != -1) {
        S.push(N);
        N = Left[N];
      } else {
        N = S.top();
        Visitor(Data[N]);
        S.pop();
        N = Right[N];
      }
    }
  }

  template <std::random_access_iterator It>
  It lookupInsertPosition(It L, It R, const T &Dt) const {
    assert(Root == 0);
    int N = Root;
    for (;;) {
      if (Dt == Data[N])
        throw tree_error<T>("Error: duplicate key insertion", Dt);
      if (Dt < Data[N]) {
        if (L[N] == -1)
          return L + N;
        N = L[N];
      } else {
        if (R[N] == -1)
          return R + N;
        N = R[N];
      }
    }
    __builtin_unreachable();
  }

  // main idea: putting in stack node for right and left link
  template <std::random_access_iterator It>
  void reconstructTopology(It Start, int Sz) {
    std::stack<std::pair<int, bool>> S;
    Root = 0;
    int CurNode = 0;
    assert(Start[0] == true);
    S.push({Root, false});
    S.push({Root, true});

    for (int N = 1; N < Sz; ++N) {
      auto Parent = S.top();
      S.pop();
      auto Elt = Start[N];
      if (Elt) {
        CurNode += 1;
        if (Parent.second) {
          assert(Left[Parent.first] == -1);
          Left[Parent.first] = CurNode;
        } else {
          assert(Right[Parent.first] == -1);
          Right[Parent.first] = CurNode;
        }
        S.push({CurNode, false});
        S.push({CurNode, true});
      }
      assert(!S.empty());
    }

    S.pop();
    assert(S.empty());
  }

public:
  explicit TabTree(int Sz, int Rt = -1)
      : Left(Sz, -1), Right(Sz, -1), Data(Sz), Root(Rt) {}

  // construct TabTree from proper braced sequence like (()())()
  // equivalent permutation will be 1 .. N+1 where N is number of braces
  template <std::random_access_iterator It> requires requires(It I) {
    // clang-format off
    { *I } -> std::convertible_to<bool>;
    // clang-format on
  }
  explicit TabTree(It Start, int Sz)
      : Left(Sz / 2, -1), Right(Sz / 2, -1), Data(Sz / 2) {
    reconstructTopology(Start, Sz);

    T CurVal = 1;
    visitInord([&CurVal](T &Data) { Data = CurVal++; });
  }

  void addSearchOrder(T Dt) {
    // if Root is -1 this is first node, add to 0 and set Root to 0.
    if (Root == -1) {
      assert(FreePos == 0);
      Data[0] = std::move(Dt);
      Root = 0;
      FreePos += 1;
      return;
    }

    auto It = lookupInsertPosition(Left.begin(), Right.begin(), Dt);
    addNode(It, std::move(Dt));
  }

  // dump tree as edge list
  // edges are ordered by first and then by second vertex data
  // prepend with nedges and root vertex data
  void dumpEL(std::ostream &Os) const {
    std::vector<std::pair<T, T>> Els;
    Els.reserve(Data.size() * 2);
    for (int I = 0; I < Data.size(); ++I) {
      if (Left[I] != -1)
        Els.emplace_back(Data[I], Data[Left[I]]);
      if (Right[I] != -1)
        Els.emplace_back(Data[I], Data[Right[I]]);
    }

    if (Els.empty()) {
      Os << "[niltree]\n";
      return;
    }

    ranges::sort(Els, {}, [](auto P) { return P.second; });
    ranges::stable_sort(Els, {}, [](auto P) { return P.first; });

    Os << Els.size() << "\n";
    Os << Data[Root] << "\n";
    for (const auto &El : Els)
      Os << El.first << " -- " << El.second << "\n";
  }

  // dump tree as a dot format to visualize
  // mark left links red, right links blue
  void dumpDot(std::ostream &Os) const {
    Os << "graph {\n";
    if (Root == -1) {
      Os << "}\n";
      return;
    }

    // pretty tree: determine same-rank elements
    assert(Root == 0);
    std::vector<int> Ranks(Data.size(), -1);
    Ranks[Root] = 0;
    Os << "{ rank=source " << Data[Root] << "}\n";
    Os << "{ rank=sink nil }\n";
    setRanks(Ranks.begin(), 0);
    std::multimap<int, int> MRanks;
    for (int I = 0; I < Data.size(); ++I)
      MRanks.insert({Ranks[I], I});
    auto MaxRankIt = std::max_element(Ranks.begin(), Ranks.end());
    if (MaxRankIt != Ranks.end()) {
      for (int I = 1; I < *MaxRankIt; ++I) {
        auto [It, Ite] = MRanks.equal_range(I);
        if (It != Ite) {
          Os << "{ rank=same ";
          for (auto EltIt = It; EltIt != Ite; ++EltIt)
            Os << Data[EltIt->second] << " ";
          Os << " }\n";
        }
      }
    }

    dumpELDot(Os);
    Os << "}\n";
  }

  std::vector<bool> readTopology() const {
    std::vector<bool> Ret;
    if (Root == -1)
      return Ret;
    assert(Root == 0);
    Ret.reserve(Data.size() * 2);
    int Cursor = 0;

    std::stack<int> S;
    Ret.push_back(true);
    S.push(Right[Root]);
    S.push(Left[Root]);

    while (!S.empty()) {
      auto Cur = S.top();
      S.pop();
      if (Cur != -1) {
        Ret.push_back(true);
        S.push(Right[Cur]);
        S.push(Left[Cur]);
      } else {
        Ret.push_back(false);
      }
    }
    Ret.pop_back();
    return Ret;
  }

  void dumpTopo(std::ostream &Os, bool Batch = false) const {
    std::vector<bool> V = readTopology();
    assert((V.size() % 2) == 0);
    if (!Batch)
      Os << V.size() / 2 << std::endl;
    std::ostream_iterator<char> OsIt(Os, "");
    std::transform(V.begin(), V.end(), OsIt,
                   [](bool S) { return S ? '(' : ')'; });
    Os << std::endl;
  }

  // dump table structure
  template <std::random_access_iterator It, std::random_access_iterator DIt>
  void read_structure(int &R, It LBegin, It RBegin, DIt DBegin) const {
    R = Root;
    std::copy(Left.begin(), Left.end(), LBegin);
    std::copy(Right.begin(), Right.end(), RBegin);
    std::copy(Data.begin(), Data.end(), DBegin);
  }
};

// this is complicated, because () reversed is not )(
template <std::random_access_iterator It>
void brace_reverse(It VBegin, It VEnd) {
  std::reverse(VBegin, VEnd);
  std::transform(VBegin, VEnd, VBegin, [](bool B) { return !B; });
}

// parse proper braces and read the tree
template <typename T>
std::optional<trees::TabTree<T>> read_bst_braced(int N, std::istream &Is,
                                                 bool Back) {
  int M = 0;
  N = N * 2;
  std::vector<bool> Vec(N + 1); // 2N + 1 to fit mandatory last 0
  while (M < N) {
    char c;
    Is >> c;
    if (!Is)
      return std::nullopt;
    if (std::isspace(c))
      continue;
    switch (c) {
    case '(':
    case ')':
      Vec[M] = (c == '(');
      M += 1;
      break;
    default:
      throw std::runtime_error("Unknown input symbol");
    }
  }

  // (())() is encoded to 1100100, being reversed need to have ()(()) 1011000
  // so we are reversing [110010]0 to [010011]0, then transforming to
  // [101100]0 and we are done.
  if (Back)
    brace_reverse(Vec.begin(), std::prev(Vec.end()));

  trees::TabTree<T> Ret(Vec.begin(), N);
  return Ret;
}

// parse proper braces and read N and then the tree
template <typename T>
std::optional<trees::TabTree<T>> read_bst_braced(std::istream &Is, bool Back) {
  int N;
  Is >> N;
  if (!Is)
    return std::nullopt;
  return read_bst_braced<T>(N, Is, Back);
}

template <typename T>
std::optional<std::vector<T>> read_order(int N, std::istream &Is) {
  std::vector<T> Vec(N);
  for (int I = 0; I < N; ++I) {
    Is >> Vec[I];
    if (!Is)
      return std::nullopt;
  }
  return Vec;
}

// parse permutation and read the tree
template <typename T>
std::optional<TabTree<T>> read_bst_ordered(int N, std::istream &Is, bool Back) {
  auto OptVec = read_order<T>(N, Is);
  if (!OptVec)
    return std::nullopt;
  auto &Vec = *OptVec;

  if (Back)
    std::reverse(Vec.begin(), Vec.end());

  TabTree<T> Ret(N);
  for (int I = 0; I < N; ++I)
    Ret.addSearchOrder(Vec[I]);
  return Ret;
}

// parse permutation and read N and then the tree
template <typename T>
std::optional<trees::TabTree<T>> read_bst_ordered(std::istream &Is, bool Back) {
  int N;
  Is >> N;
  if (!Is)
    return std::nullopt;
  return read_bst_ordered<T>(N, Is, Back);
}

} // namespace trees