//------------------------------------------------------------------------------
//
// Simplify work with randomness
//
//------------------------------------------------------------------------------
//
// This file is licensed after LGPL v3
// Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
//
//------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <memory>
#include <random>

namespace rands {

// simple dice to roll
class Dice {
  std::mt19937 Engine;
  static std::unique_ptr<Dice> PImpl;
  Dice(unsigned Seed) { Engine.seed(Seed); }

public:
  // version with no seed: use random device
  static auto init() {
    static std::random_device Rd;
    auto Seed = Rd();
    init(Seed);
    return Seed;
  }

  // version with user-provided seed
  static void init(unsigned Seed) {
    struct MakeUniqueEnabler : public Dice {
      MakeUniqueEnabler(unsigned Seed) : Dice(Seed) {}
    };
    PImpl = std::make_unique<MakeUniqueEnabler>(Seed);
  }

  template <typename T> static T generate(T Min, T Max) {
    if (!PImpl)
      throw std::runtime_error("Please initialize random generator");
    std::uniform_int_distribution<T> Dist(Min, Max);
    return Dist(PImpl->Engine);
  }

  template <typename T> T operator()(T Min, T Max) {
    return generate(Min, Max);
  }

  template <typename T, typename It>
  static void rand_initialize(It Begin, It End, T Min, T Max) {
    std::generate(Begin, End, [Min, Max] { return generate(Min, Max); });
  }

  template <typename It> static void rand_shuffle(It Begin, It End) {
    if (!PImpl)
      throw std::runtime_error("Please initialize random generator");
    std::shuffle(Begin, End, PImpl->Engine);
  }
};

inline std::unique_ptr<Dice> Dice::PImpl;

} // namespace rands