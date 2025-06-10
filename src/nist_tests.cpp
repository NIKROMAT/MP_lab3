#include <cmath>
#include <algorithm>

#include <bitset>

#include "../include/tests.h"
#include "../include/defs.h"






double nist_fraction ( std::vector<unsigned int>& vec )
{
  std::size_t max_len = 1 + std::ceil(std::log2(MAX_LIM));

  int sum = 0;

  for (auto num : vec)
  {
    // std::bitset<max_len>
    // std::bitset<(std::size_t)1 + std::ceil(std::log2(MAX_LIM))>
  }
}

