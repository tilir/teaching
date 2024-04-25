//------------------------------------------------------------------------------
//
// Simple view over all correct braced expressions
// Based on TAOCP 7.2.1.6 - P
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
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

#include "dbgs.hpp"

namespace trees {

namespace ranges = std::ranges;
namespace views = std::views;

struct BracedSentinel {};

class BracedGenerator {
  int N;
  int LeftMostClosing, ActiveTail;
  std::string Braces; // we can also go ahead with vector<char>
  bool Proceed;

public:
  explicit BracedGenerator(int NBrac, bool P = true)
      : N(NBrac), LeftMostClosing(2 * (N - 1)), ActiveTail(LeftMostClosing - 1),
        Braces(2 * N, '('), Proceed(P) {
    for (int i = 0; i < N; ++i)
      Braces[2 * i + 1] = ')';
  }

  void restart(bool P) { *this = BracedGenerator(N, P); }

  auto operator*() { return views::all(Braces); }

  BracedGenerator &operator++() {
    // P3: simple case (p == 0)
    dbgs << "\tP3, m = " << LeftMostClosing << std::endl;
    Braces[LeftMostClosing] = ')';
    if (Braces[LeftMostClosing - 1] == ')') {
      LeftMostClosing -= 1;
      Braces[LeftMostClosing] = '(';
      return *this;
    }

    // P4: look up new active tail
    dbgs << "\tP4 start:" << Braces << std::endl;
    ActiveTail = LeftMostClosing - 1;
    int K = 2 * (N - 1);
    while (Braces[ActiveTail] == '(' && ActiveTail >= 0) {
      Braces[ActiveTail] = ')';
      Braces[K] = '(';
      ActiveTail = ActiveTail - 1;
      K = K - 2;
    }

    // P5: increment active tail position
    dbgs << "\tP5 start:" << Braces << std::endl;
    if (ActiveTail > 0)
      Braces[ActiveTail] = '(';
    LeftMostClosing = 2 * (N - 1);

    Proceed = (ActiveTail != -1);
    if (!Proceed)
      restart(false);

    return *this;
  }

  BracedGenerator operator++(int) {
    auto OldVal = *this;
    ++*this;
    return OldVal;
  }

  bool equals(const BracedGenerator &Rhs) const { return Braces == Rhs.Braces; }

  bool proceed() const { return Proceed; }
};

inline bool operator==(const BracedGenerator &Lhs, const BracedGenerator &Rhs) {
  return Lhs.equals(Rhs) && Lhs.proceed() == Rhs.proceed();
}

inline bool operator!=(const BracedGenerator &Lhs, const BracedGenerator &Rhs) {
  return !(Lhs == Rhs);
}

inline bool operator==(const BracedGenerator &Lhs, const BracedSentinel &Rhs) {
  return (Lhs.proceed() == false);
}

inline bool operator!=(const BracedGenerator &Lhs, const BracedSentinel &Rhs) {
  return !(Lhs == Rhs);
}

class braced_view : public ranges::view_interface<braced_view> {
  int N;

public:
  using iterator = BracedGenerator;
  using sentinel = BracedSentinel;
  braced_view(int NBrac) : N(NBrac) {}
  iterator begin() { return iterator{N}; }
  sentinel end() { return sentinel{}; }
};

} // namespace trees