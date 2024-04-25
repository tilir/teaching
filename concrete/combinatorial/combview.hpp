//------------------------------------------------------------------------------
//
// Simple view over all combinations
// Details may be found in Knuth, algorithm R from 7.2.1.3 (vol 4A)
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <concepts>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

#include "dbgs.hpp"

namespace combs {

namespace ranges = std::ranges;
namespace views = std::views;

struct CombinationsSentinel {};

class CombinationsGenerator {
  std::vector<int> Combination;
  int N, K;
  bool Proceed;

public:
  CombinationsGenerator(int Ns, int Ks, bool P = true)
      : Combination(Ks + 1), N(Ns), K(Ks), Proceed(P) {
    std::iota(Combination.begin(), Combination.end(), 0);
    Combination.back() = N;
  }

  void restart(bool P) { *this = CombinationsGenerator(N, K, P); }

  auto operator*() {
    return views::all(
        ranges::subrange(Combination.begin(), std::prev(Combination.end())));
  }

  CombinationsGenerator &operator++() {
    bool skipr4 = false;

    if ((K % 2) == 1) {
      if (Combination[0] + 1 < Combination[1]) {
        Combination[0] += 1;
        return *this;
      }
    } else {
      if (Combination[0] > 0) {
        Combination[0] -= 1;
        return *this;
      }
      skipr4 = true;
    }

    if (K == 1) {
      Proceed = false;
      restart(false);
      return *this;
    }

    int j = 2;

    for (;;) {
      // step R4
      if (!skipr4) {
        assert(Combination[j - 1] == Combination[j - 2] + 1);
        if (Combination[j - 1] >= j) {
          Combination[j - 1] = Combination[j - 2];
          Combination[j - 2] = j - 2;
          break;
        }
        j += 1;
      }

      // step R5
      assert(Combination[j - 2] == j - 2);
      if (Combination[j - 1] + 1 < Combination[j]) {
        Combination[j - 2] = Combination[j - 1];
        Combination[j - 1] += 1;
        break;
      }

      j += 1;
      Proceed = (j <= K);
      if (!Proceed) {
        restart(false);
        break;
      }
      skipr4 = false;
    }
    return *this;
  }

  CombinationsGenerator operator++(int) {
    auto OldVal = *this;
    ++*this;
    return OldVal;
  }

  bool equals(const CombinationsGenerator &Rhs) const {
    return Combination == Rhs.Combination;
  }

  bool proceed() const { return Proceed; }
};

inline bool operator==(const CombinationsGenerator &Lhs,
                       const CombinationsGenerator &Rhs) {
  return Lhs.equals(Rhs) && Lhs.proceed() == Rhs.proceed();
}

inline bool operator!=(const CombinationsGenerator &Lhs,
                       const CombinationsGenerator &Rhs) {
  return !(Lhs == Rhs);
}

inline bool operator==(const CombinationsGenerator &Lhs,
                       const CombinationsSentinel &Rhs) {
  return (Lhs.proceed() == false);
}

inline bool operator!=(const CombinationsGenerator &Lhs,
                       const CombinationsSentinel &Rhs) {
  return !(Lhs == Rhs);
}

class comb_view : public ranges::view_interface<comb_view> {
  int N, K;

public:
  using iterator = CombinationsGenerator;
  using sentinel = CombinationsSentinel;
  comb_view(int Ns, int Ks) : N(Ns), K(Ks) {}
  iterator begin() { return iterator{N, K}; }
  sentinel end() { return sentinel{}; }
};

} // namespace combs
