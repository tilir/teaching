//------------------------------------------------------------------------------
//
// Twin tree is really thin wrapper around tabulated tree
// it keeps nothing it is just topology
//
// about why this is required, see
// see https://www-cs-faculty.stanford.edu/~knuth/programs/twintree-to-baxter.w
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <iterator>
#include <ranges>
#include <vector>

#include "tabtree.hpp"

namespace trees {

namespace ranges = std::ranges;
namespace views = std::views;

class TwinTree {
  int T0 = -1, T1 = -1;
  std::vector<int> D;
  std::vector<int> L0, L1, R0, R1;

public:
  // ctor from permutation
  template <typename It>
  explicit TwinTree(It VBegin, int N)
      : D(N, 0), L0(N, 0), L1(N, 0), R0(N, 0), R1(N, 0) {
    TabTree<int> Forward(N);
    for (int I = 0; I < N; ++I)
      Forward.addSearchOrder(VBegin[I]);
    Forward.read_structure(T0, L0.begin(), R0.begin(), D.begin());
    std::reverse(VBegin, VBegin + N);
    TabTree<int> Backward(N);
    for (int I = 0; I < N; ++I)
      Backward.addSearchOrder(VBegin[I]);
    Backward.read_structure(T1, L1.begin(), R1.begin(), D.begin());
    std::reverse(D.begin(), D.end());
    T1 = N - 1 - T1;
    std::reverse(L1.begin(), L1.end());
    std::reverse(R1.begin(), R1.end());

    // fix pointers after reverse
    auto TransFunc = [N](int K) { return (K == -1) ? -1 : N - 1 - K; };
    ranges::transform(L1, L1.begin(), TransFunc);
    ranges::transform(R1, R1.begin(), TransFunc);
  }

  // pretty dump with nodes and zeroes
  void dumpTable(std::ostream &Os) const {
    std::ostream_iterator<int> OsIt(Os, " ");
    Os << D[T0] << " " << D[T1] << "\n";
    ranges::copy(D, OsIt);
    Os << "\n";
    auto TransFunc = [this](int N) { return (N == -1) ? 0 : D[N]; };
    // auto TransFunc = [this](int N) { return N; };
    ranges::transform(L0, OsIt, TransFunc);
    Os << "\n";
    ranges::transform(R0, OsIt, TransFunc);
    Os << "\n";
    ranges::transform(L1, OsIt, TransFunc);
    Os << "\n";
    ranges::transform(R1, OsIt, TransFunc);
    Os << "\n";
  }

  // Algorithm is destructive, tree after it in consistent but unpredictable
  // state. Returns permutation which is guaranteed to be Baxters.
  std::vector<int> toBaxters(bool Verbose = false) {
    int N = D.size();
    std::vector<std::pair<int, int>> Parent(N, {0, 0});
    for (int K = 0; K < N; ++K) {
      if (L1[K] != -1)
        Parent[L1[K]] = std::make_pair(K, 1);
      if (R1[K] != -1)
        Parent[R1[K]] = std::make_pair(K, -1);
    }
    std::vector<int> Baxters;
    Baxters.reserve(N);
    assert(T0 == 0);
    for (;;) {
      if (Verbose)
        std::cout << "p elt = " << D[T0] << std::endl;
      Baxters.push_back(D[T0]);
      auto [I, LR] = Parent[T0];
      if (LR == 0)
        break;

      if (Verbose) {
        std::cout << "parent elt is = " << D[I] << " : " << LR << std::endl;
        std::cout << "elt L[" << T0 << "] = " << (L0[T0] != -1 ? D[L0[T0]] : 0)
                  << std::endl;
        std::cout << "elt R[" << T0 << "] = " << (R0[T0] != -1 ? D[R0[T0]] : 0)
                  << std::endl;
      }

      if (LR > 0) {
        if (Verbose)
          std::cout << "Removing L: " << I << "th elt: " << L1[I] << std::endl;
        L1[I] = -1;
        if (R0[T0] == -1)
          T0 = L0[T0];
        else {
          L0[I] = L0[T0];
          T0 = R0[T0];
        }
      } else {
        if (Verbose)
          std::cout << "Removing R: " << I << "th elt: " << R1[I] << std::endl;
        R1[I] = -1;
        if (L0[T0] == -1) {
          T0 = R0[T0];
        } else {
          R0[I] = R0[T0];
          T0 = L0[T0];
        }
      }
    }
    return Baxters;
  }
};

inline std::optional<TwinTree> read_twin_ordered(int N, std::istream &Is) {
  auto OptVec = trees::read_order<int>(N, Is);
  if (!OptVec)
    return std::nullopt;
  auto &Vec = *OptVec;
  TwinTree Ret(Vec.begin(), Vec.size());
  return Ret;
}

} // namespace trees