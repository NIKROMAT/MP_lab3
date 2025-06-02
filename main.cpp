#include <iostream>
#include "lab3.h"


int main()
{
  // int seed = 42;
  int seed = (1<<19)-1;

  MidPRNG     mid(seed);
  ShufflePRNG shu(seed);
  LCG         lcg(seed);



  std::cout << "Middle PRNG:\n";

  for (int i=0; i<10; ++i)
  {
    std::cout << mid.generate() << "\n";
  }

  std::cout << "\nShuffle PRNG:\n";

  for (int i=0; i<10; ++i)
  {
    std::cout << shu.generate() << "\n";
  }

  std::cout << "\nLCG PRNG:\n";

  for (int i=0; i<10; ++i)
  {
    std::cout << lcg.generate() << "\n";
  }
}