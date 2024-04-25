//------------------------------------------------------------------------------
//
// Permutations.
//
// Permutation<T> changes positions of elements from given domain T
//
// Sample permutation in normal form is:
//
// a b c d e f g
// c e f b d g a
//
// it sends a to c, b to e, etc...
//
// Permutation can be expressed in loop form.
// Example from above contains two loops (a c f g) (d b e)
//
// Canonicalization of permutation means:
//   * all single loops are written explicitly
//   * every loop is canonical
//   * loops are sorted by first element in decreasing order
//
// I.e. canonical form of (3 1 6)(5 4) over domain [1, 7) is (4 5)(2)(1 6 3)
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
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <vector>

#include "idomain.hpp"
#include "permloops.hpp"

namespace permutations {

//------------------------------------------------------------------------------
//
// Permutation template
//
//------------------------------------------------------------------------------

template <Domain T> class Permutation {
  std::vector<PermLoop<T>> Loops;

  // ctors/dtors
public:
  // id permutation over domain
  Permutation();

  // permutation over domain with some initial loops
  template <std::random_access_iterator RandIt>
  Permutation(RandIt B, RandIt E) {
    simplify_loops(B, E, back_inserter(Loops));
    sortloops();
#ifdef CHECKS
    check();
#endif
  }

  Permutation(std::initializer_list<PermLoop<T>> Ilist)
      : Permutation(Ilist.begin(), Ilist.end()) {}

  // modifiers
public:
  // left multiply this = lhs * this
  Permutation &lmul(const Permutation &Rhs);

  // right multiply this = this * Rhs
  Permutation &rmul(const Permutation &Rhs);

  // inverted permutation
  void inverse() {
    for (auto &L : Loops)
      L.inverse();
  }

  // selectors
public:
  // apply permutation to elem
  T apply(T elem) const;

  // apply permutation (all loops in direct order) to given table
  template <std::random_access_iterator RandIt>
  void apply(RandIt tbeg, RandIt tend) const {
    for (auto L : Loops)
      L.apply(tbeg, tend);
  }

  // return plain permutation format like 7 1 5 3 2 8 4 6
  std::vector<T> present_as_perm() const {
    std::vector<T> Vec(T::Max());
    std::iota(Vec.begin(), Vec.end(), 1);
    apply(Vec.begin(), Vec.end());
    return Vec;
  }

  // true if permutation contains element
  bool contains(T elem) const;

  // lexicographical less-than
  bool less(const Permutation &Rhs) const {
    size_t Sz = Rhs.Loops.size();
    if (Sz != Loops.size())
      return (Loops.size() < Sz);
    for (size_t I = 0; I != Sz; ++I)
      if (Loops[I] != Rhs.Loops[I])
        return (Loops[I] < Rhs.Loops[I]);
    return false;
  }

  // true if equals
  bool equals(const Permutation &Rhs) const { return Rhs.Loops == Loops; }

  // dump and serialization
public:
  // dump to stream
  std::ostream &dump(std::ostream &) const;
  std::ostream &dump_as_perm(std::ostream &) const;

  // service functions
private:
  // sort loops to canonicalize permutation
  void sortloops();

  // enforce invariants
  void check();
};

//------------------------------------------------------------------------------
//
// Standalone operations
//
//------------------------------------------------------------------------------

template <typename T>
bool operator==(const Permutation<T> &Lhs, const Permutation<T> &Rhs) {
  return Lhs.equals(Rhs);
}

template <typename T>
bool operator<(const Permutation<T> &Lhs, const Permutation<T> &Rhs) {
  return Lhs.less(Rhs);
}

template <typename T>
bool operator!=(const Permutation<T> &Lhs, const Permutation<T> &Rhs) {
  return !operator==(Lhs, Rhs);
}

template <typename T>
static inline std::ostream &operator<<(std::ostream &Os,
                                       const Permutation<T> &Rhs) {
  Rhs.dump(Os);
  return Os;
}

template <typename T>
Permutation<T> product(const Permutation<T> &Lhs, const Permutation<T> &Rhs) {
  Permutation<T> Retval = Lhs;
  Retval.rmul(Rhs);
  return Retval;
}

template <typename T> Permutation<T> invert(Permutation<T> Lhs) {
  Lhs.inverse();
  return Lhs;
}

//------------------------------------------------------------------------------
//
// Ctors/dtors
//
//------------------------------------------------------------------------------

template <Domain T> Permutation<T>::Permutation() {
  for (auto X = T::Min(); X <= T::Max(); ++X)
    Loops.push_back(PermLoop<T>{X});
  sortloops();
#ifdef CHECKS
  check();
#endif
}

//------------------------------------------------------------------------------
//
// Selectors
//
//------------------------------------------------------------------------------

template <Domain T> T Permutation<T>::apply(T Elem) const {
  T Res = std::move(Elem);
  for (auto &L : Loops)
    Res = L.apply(Res);
  return Res;
}

template <Domain T> bool Permutation<T>::contains(T Elem) const {
  return std::any_of(Loops.begin(), Loops.end(), [Elem](const PermLoop<T> &pl) {
    return pl.contains(Elem);
  });
}

template <Domain T> std::ostream &Permutation<T>::dump(std::ostream &Os) const {
  for (auto L : Loops)
    L.dump(Os);
  return Os;
}

template <Domain T>
std::ostream &Permutation<T>::dump_as_perm(std::ostream &Os) const {
  auto Vec = present_as_perm();
  for (auto Elt : Vec)
    Os << Elt << " ";
  return Os;
}

//------------------------------------------------------------------------------
//
// Modifiers
//
//------------------------------------------------------------------------------

template <Domain T>
Permutation<T> &Permutation<T>::lmul(const Permutation<T> &input) {
  Loops.insert(Loops.begin(), input.Loops.begin(), input.Loops.end());
  std::vector<PermLoop<T>> OutLoops;
  simplify_loops(Loops.begin(), Loops.end(), back_inserter(OutLoops));
  Loops.swap(OutLoops);
  sortloops();
#ifdef CHECKS
  check();
#endif
  return *this;
}

template <Domain T>
Permutation<T> &Permutation<T>::rmul(const Permutation<T> &input) {
  Loops.insert(Loops.end(), input.Loops.begin(), input.Loops.end());
  std::vector<PermLoop<T>> OutLoops;
  simplify_loops(Loops.begin(), Loops.end(), back_inserter(OutLoops));
  Loops.swap(OutLoops);
  sortloops();
#ifdef CHECKS
  check();
#endif
  return *this;
}

//------------------------------------------------------------------------------
//
// Service functions
//
//------------------------------------------------------------------------------

template <Domain T> void Permutation<T>::check() {
  if (T::Min() >= T::Max())
    throw std::runtime_error("Domain error");

  if (Loops.empty())
    throw std::runtime_error("Empty permutation");

  for (auto X = T::Min(); X <= T::Max(); ++X)
    if (!contains(X))
      throw std::runtime_error("Every domain element shall be covered");

  for (auto It = Loops.begin(); It != std::prev(Loops.end()); ++It) {
    if (It->smallest() == std::next(It)->smallest())
      throw std::runtime_error("Loops wIth equal elements are here");
    if (It->smallest() < std::next(It)->smallest())
      throw std::runtime_error("Canonical form broken");
  }
}

template <Domain T> void Permutation<T>::sortloops() {
  std::sort(Loops.begin(), Loops.end(),
            [](const PermLoop<T> &Lhs, const PermLoop<T> &Rhs) {
              return (Lhs.smallest() > Rhs.smallest());
            });
}

} // namespace permutations