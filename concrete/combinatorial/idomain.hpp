//------------------------------------------------------------------------------
//
// Integral domain
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#include <cassert>
#include <concepts>
#include <iostream>

namespace permutations {

//------------------------------------------------------------------------------
//
// Integral domain -- simplect version, nonfixed bound (need to call init)
//
//------------------------------------------------------------------------------

template <typename T> concept Domain = requires(T D) {
  T::Min();
  T::Max();
  typename T::type;
};

template <std::integral T> class IDomain {
  static T Maximum;
  static T Minimum;
  T Val;

public:
  using type = T;

  static void init(T Max, T Min = 1) { Maximum = Max; }
  static T Max() { return Maximum; }
  static T Min() { return Minimum; }

  constexpr IDomain(T V = 1) : Val(V) { assert(V >= Minimum && V <= Maximum); }
  constexpr operator T() const { return Val; }

  // do not define this: ambiguity vs builtin operator can happen
  // constexpr auto operator<=>(const IDomain &) const = default;
};

template <std::integral T> inline T IDomain<T>::Maximum = 1;
template <std::integral T> inline T IDomain<T>::Minimum = 1;

template <std::integral T>
inline std::ostream &operator<<(std::ostream &Os, const IDomain<T> &Elt) {
  T Underlying = Elt;
  Os << Underlying;
  return Os;
}

template <std::integral T>
inline std::istream &operator>>(std::istream &Is, IDomain<T> &Elt) {
  T Underlying;
  Is >> Underlying;
  if (Is)
    Elt = Underlying;
  return Is;
}

} // namespace permutations

//------------------------------------------------------------------------------
//
// Hash function to behave like underlying for unordered containers
//
//------------------------------------------------------------------------------

namespace std {
template <integral T> struct hash<permutations::IDomain<T>> {
  size_t operator()(const permutations::IDomain<T> &K) const {
    hash<T> H;
    return H(K);
  }
};
} // namespace std
