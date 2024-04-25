//------------------------------------------------------------------------------
//
// Permutation loops.
//
// Permutation loop like (a c d) encodes permutation
//
// a b c d e
// c b d a e
//
// i.e. fixes umentioned points and permutes a --> c --> d --> a
//
// Loops (a c d), (d a c) and (c d a) are equivalent. First is normal, i.e.
// it has smallest element first and serves as class representative
//
// Main assumptions are:
// * loop is non-empty
// * all elements in loop are unique
// * first element is smallest one
//
// use -DCHECKS to build with expensive runtime checks of this assumptions on
// creation and on any modification
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "idomain.hpp"

namespace permutations {

//------------------------------------------------------------------------------
//
// Permloop template
//
//------------------------------------------------------------------------------

template <Domain T> class PermLoop {
  std::vector<T> Loop;

  // traits
public:
  using value_type = T;

  // ctors/dtors
public:
  // from initializer list
  PermLoop(std::initializer_list<T> ls);

  // from begin-end range (vector, list, etc)
  template <std::forward_iterator FwdIter> PermLoop(FwdIter b, FwdIter e);

  // Modifiers
public:
  // add element to permutation loop
  void add(T x);

  // inverse permutation loop:
  // (a b c) to (a c b), etc
  void inverse();

  // Getters
public:
  // get smallest element in loop
  T smallest() const { return Loop.front(); }

  // return true if loop contains given element
  bool contains(T Val) const {
    return std::any_of(Loop.begin(), Loop.end(), Val);
  }

  // apply loop to given element
  T apply(T x) const;

  // permute on table with loop
  // this is more effective then element-wise application
  template <std::random_access_iterator RandIt>
  void apply(RandIt tbeg, RandIt tend) const;

  // inherit eq comparisons from vector
  bool operator==(const PermLoop &) const = default;

  // lexicographical less-than
  bool less(const PermLoop &Rhs) const {
    size_t Sz = Rhs.Loop.size();
    if (Loop.size() != Sz)
      return Loop.size() < Sz;
    for (size_t I = 0; I != Sz; ++I)
      if (Loop[I] != Rhs.Loop[I])
        return Loop[I] < Rhs.Loop[I];
    return false;
  }

  // size of loop
  size_t size() const { return Loop.size(); }

  // Serialization and dumps
public:
  // dump to given stream
  std::ostream &dump(std::ostream &) const;

  // return as string
  std::string to_string() const {
    std::ostringstream Buffer;
    dump(Buffer);
    return Buffer.str();
  }

  // return as vector
  std::vector<T> to_vector() const { return Loop; }

  // Service functions
private:
  // CHECK postcondition consistency after modification
  void check();

  // roll to canonical: smallest element first
  void reroll() {
    auto Smallest = std::min_element(Loop.begin(), Loop.end());
    std::rotate(Loop.begin(), Smallest, Loop.end());
  }
};

//------------------------------------------------------------------------------
//
// Standalone operations
//
//------------------------------------------------------------------------------

template <Domain T>
bool operator<(const PermLoop<T> &Lhs, const PermLoop<T> &Rhs) {
  return Lhs.less(Rhs);
}

template <Domain T>
std::ostream &operator<<(std::ostream &Os, const PermLoop<T> &Rhs) {
  Rhs.dump(Os);
  return Os;
}

// creates array of loops from permutation given by table
// say: [d, c, e, g, b, f, a] with type CharDomain<a, g>
// gives: [(a, d, g), (b, c, e), (f)]
template <std::random_access_iterator RandIt,
          std::input_or_output_iterator OutIt>
void create_loops(RandIt B, RandIt E, OutIt O);

// products all input loops over [start, fin) to minimize them
// for example:
// (a, c, f, g) (b, c, d) (a, e, d) (f, a, d, e) (b, g, f, a, e)
// simplifies to:
// (a, d, g) (b, c, e) (f)
// see TAOCP, Alg. 1.3.3B
template <std::random_access_iterator RandIt,
          std::input_or_output_iterator OutIt>
void simplify_loops(RandIt B, RandIt E, OutIt O);

//------------------------------------------------------------------------------
//
// Ctors/dtors
//
//------------------------------------------------------------------------------

template <Domain T>
PermLoop<T>::PermLoop(std::initializer_list<T> Ls) : Loop(Ls) {
  reroll();
#ifdef CHECKS
  check();
#endif
}

// from begin-end range (vector, list, etc)
template <Domain T>
template <std::forward_iterator FwdIter>
PermLoop<T>::PermLoop(FwdIter B, FwdIter E) : Loop(B, E) {
  reroll();
#ifdef CHECKS
  check();
#endif
}

//------------------------------------------------------------------------------
//
// Modifiers
//
//------------------------------------------------------------------------------

template <Domain T> void PermLoop<T>::add(T Val) {
  Loop.push_back(Val);
  reroll();
#ifdef CHECKS
  check();
#endif
}

template <Domain T> void PermLoop<T>::inverse() {
  if (Loop.size() < 3)
    return;
  std::reverse(std::next(Loop.begin()), Loop.end());
#ifdef CHECKS
  check();
#endif
}

//------------------------------------------------------------------------------
//
// Selectors
//
//------------------------------------------------------------------------------

template <Domain T> T PermLoop<T>::apply(T Val) const {
  auto It = std::find(Loop.begin(), Loop.end(), Val);
  if (It == Loop.end())
    return Val;
  auto Nxt = std::next(It);
  if (Nxt == Loop.end())
    return *Loop.begin();
  return *Nxt;
}

template <Domain T>
template <std::random_access_iterator RandIt>
void PermLoop<T>::apply(RandIt B, RandIt E) const {
  assert(T::Max() > T::Min());
  assert(Loop.front() >= T::Min());
  assert(Loop.back() <= T::Max());
  assert(E - B == T::Max() - T::Min() + 1);
  size_t Nxt = Loop.front() - T::Min();
  T Tmp = B[Nxt];
  for (auto L : Loop) {
    size_t Prev = Nxt;
    Nxt = L - T::Min();
    if (L == Loop.front())
      continue;
    B[Prev] = B[Nxt];
  }
  B[Nxt] = Tmp;
}

//------------------------------------------------------------------------------
//
// Dump and printing
//
//------------------------------------------------------------------------------

template <Domain T> std::ostream &PermLoop<T>::dump(std::ostream &Os) const {
  for (const auto &L : Loop) {
    if (L == Loop.front())
      Os << "(";
    Os << L;
    if (L == Loop.back())
      Os << ")";
    else
      Os << " ";
  }
  return Os;
}

//------------------------------------------------------------------------------
//
// Service functions
//
//------------------------------------------------------------------------------

template <Domain T> void PermLoop<T>::check() {
  // check any elements
  if (Loop.empty())
    throw std::runtime_error("PermLoop shall be non-empty");

  // check unique elements
  std::unordered_set<T> Uniqs(Loop.begin(), Loop.end());
  if (Uniqs.size() != Loop.size())
    throw std::runtime_error("PermLoop elements shall be unique");

  // check minimal element is first
  auto Smallest = std::min_element(Loop.begin(), Loop.end());
  if (*Smallest != Loop.front())
    throw std::runtime_error("Unnormalized state detected");
}

//------------------------------------------------------------------------------
//
// Standalone operations
//
//------------------------------------------------------------------------------

template <std::random_access_iterator RandIt,
          std::input_or_output_iterator OutIt>
void create_loops(RandIt B, RandIt E, OutIt O) {
  using T = typename std::decay<decltype(*B)>::type;
  static_assert(Domain<T>);
  using OutT = typename OutIt::container_type::value_type;
  std::vector<bool> Marked(E - B, false);

  for (auto Mit = Marked.begin(); Mit != Marked.end();
       Mit = std::find(Mit + 1, Marked.end(), false)) {
    auto Relt = Mit - Marked.begin();
    auto P = static_cast<T>(T::Min() + Relt);
    OutT Perm = {P};
    Marked[Relt] = true;
    for (T Nxt = B[Relt]; Nxt != P; Nxt = B[Nxt - T::Min()]) {
      Perm.add(Nxt);
      Marked[Nxt - T::Min()] = true;
    }
    *O = Perm;
    O++;
  }
}

template <std::random_access_iterator RandIt,
          std::input_or_output_iterator OutIt>
void simplify_loops(RandIt B, RandIt E, OutIt O) {
  using T = typename std::decay<decltype(*B)>::type::value_type;
  std::vector<T> Table(T::Max() - T::Min() + 1, T::Min());
  std::iota(Table.begin(), Table.end(), T::Min());
  for (auto Loopit = std::make_reverse_iterator(E);
       Loopit != std::make_reverse_iterator(B); ++Loopit)
    Loopit->apply(Table.begin(), Table.end());
  create_loops(Table.begin(), Table.end(), O);
}

} // namespace permutations