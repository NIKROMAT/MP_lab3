#include "prng.h"
// #include <iostream>


void PRNG::generate_sample (std::ostream& out, int size)
{
  for (int i=0; i < size; ++i)
  {
    out << this->generate() << std::endl;
  }
}





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
  // std::cout << "prev seed: " << seed << '\n';


  uint32_t num1 = (seed<<8) + (seed>>24);
  uint32_t num2 = (seed>>8) + (seed<<24);

  // std::cout << "num1: " << num1 << '\n';
  // std::cout << "num2: " << num2 << '\n';
  
  seed = num1 + num2;
  // std::cout << "seed: " << seed << '\n';

  return seed;
}


LCG::LCG (uint32_t seed)
: seed(seed), k((1<<16)+1), b((1<<8)-1), M(1<<31) {}

uint32_t LCG::generate ()
{
  seed = (seed * k + b) & (M-1); // Если M = 2^N
  // seed = (seed * k + b) % (M); // иначе

  return seed;
}



mid_xor_PRNG::mid_xor_PRNG (uint32_t seed)
: seed(seed) {} 

uint32_t mid_xor_PRNG::generate ()
{
  uint64_t product = seed * seed;

  // base = seed;
  seed  = (product >> 16) & 0xFFFFFFFF;
  seed ^= 0xAAAAAAAA; // инвертируем каждый 2 бит начиная со старшего, чтобы всё не вырождалось в 0

  return seed;
}



shuf_xor_PRNG::shuf_xor_PRNG (uint32_t seed)
: seed(seed) {}

uint32_t shuf_xor_PRNG::generate ()
{
  uint32_t num1 = (seed<<8 + seed>>24) ^ 0xAAAAAAAA; // инвертируем у смещённого числа каждый 2 бит начиная со старшего
  uint32_t num2 = (seed>>8 + seed<<24) ^ 0x55555555; // инвертируем у смещённого числа каждый 2 бит начиная с  младшего

  // seed ^= num1 + num2;
  seed = num1 + num2;

  return seed;
}