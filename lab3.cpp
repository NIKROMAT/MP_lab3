#include "lab3.h"


MidPRNG::MidPRNG (uint32_t seed)
: base((1<<29)-1), seed(seed) {} // base = 2^29-1

uint32_t MidPRNG::generate ()
{
  uint64_t product = base * seed;

  base = seed;
  seed = (product >> 16) & 0xffffffff;

  return seed;
}


ShufflePRNG::ShufflePRNG (uint32_t seed)
: seed(seed) {}

uint32_t ShufflePRNG::generate ()
{
  uint32_t num1 = seed<<8 + seed>>24;
  uint32_t num2 = seed>>8 + seed<<24;

  seed = num1 + num2;

  return seed;
}


LCG::LCG (uint64_t seed)
: seed(seed), k((1<<16)+1), b((1<<8)-1), M(1ul<<63) {}

uint64_t LCG::generate ()
{
  seed = (seed * k + b) & (M-1); // Если M = 2^N
  // seed = (seed * k + b) % (M); // иначе

  return seed;
}