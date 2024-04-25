//------------------------------------------------------------------------------
//
// Simple view over all permutations, using explicit form (not loops)
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <numeric>
#include <ranges>
#include <vector>

#include "idomain.hpp"

namespace permutations {

namespace ranges = std::ranges;
namespace views = std::views;

struct PermutationsSentinel {};

template <Domain T> class PermutationsGenerator {
  std::vector<T> State;
  using STT = std::vector<T>;
  bool Proceed = true;

public:
  explicit PermutationsGenerator() : State(T::Max() - T::Min() + 1) {
    std::iota(State.begin(), State.end(), T::Min());
  }

  template <std::forward_iterator It>
  PermutationsGenerator(It Begin, It End) : State(Begin, End) {}

  auto operator*() { return views::all(State); }

  PermutationsGenerator &operator++() {
    Proceed = std::next_permutation(State.begin(), State.end());
    if (!Proceed)
      std::iota(State.begin(), State.end(), T::Min());
    return *this;
  }

  PermutationsGenerator operator++(int) {
    auto OldVal = *this;
    ++*this;
    return OldVal;
  }

  bool equals(const PermutationsGenerator &Rhs) const {
    return ranges::equal(State, Rhs.State);
  }

  bool proceed() const { return Proceed; }
};

template <Domain T>
bool operator==(const PermutationsGenerator<T> &Lhs,
                const PermutationsGenerator<T> &Rhs) {
  return Lhs.equals(Rhs) && Lhs.proceed() == Rhs.proceed();
}

template <Domain T>
bool operator!=(const PermutationsGenerator<T> &Lhs,
                const PermutationsGenerator<T> &Rhs) {
  return !(Lhs == Rhs);
}

template <Domain T>
bool operator==(const PermutationsGenerator<T> &Lhs,
                const PermutationsSentinel &Rhs) {
  return (Lhs.proceed() == false);
}

template <Domain T>
bool operator!=(const PermutationsGenerator<T> &Lhs,
                const PermutationsSentinel &Rhs) {
  return !(Lhs == Rhs);
}

template <Domain T>
struct permutations_view : public ranges::view_interface<permutations_view<T>> {
  using iterator = PermutationsGenerator<T>;
  using sentinel = PermutationsSentinel;
  iterator begin() { return iterator{}; }
  sentinel end() { return sentinel{}; }
};

} // namespace permutations