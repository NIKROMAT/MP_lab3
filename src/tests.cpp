#include <cmath>
#include <algorithm>

#include "../include/tests.h"


double get_mean ( std::vector<unsigned int>& vec )
{
  double sum = 0;

  for ( auto& num : vec )
  {
    sum += num;
  }

  return sum / vec.size();
}


double get_stdDev ( std::vector<unsigned int>& vec )
{
  unsigned long sum = 0;
  auto mean = get_mean(vec);
  
  for ( auto& num : vec )
  {
    sum += (num - mean) * (num - mean);
  }

  return std::sqrt(sum / vec.size());
} 


double get_cv ( std::vector<unsigned int>& vec )
{
  return get_stdDev(vec) / get_mean(vec);
}


std::pair<double,int> get_chi2 ( std::vector<unsigned int>& vec )
{
  if (vec.empty())
    return {-1,-1};

  auto min_val = *std::min_element(vec.begin(), vec.end());
  auto max_val = *std::max_element(vec.begin(), vec.end());
  
  int bin_number =  1 + std::log2(vec.size());
  float bin_width = (float)(max_val-min_val)/bin_number; // Фактическая ширина промежутка

  float expected = (float)vec.size() / bin_number;
  std::vector<int> observed(bin_number, 0);


  for ( auto val : vec )
  {
    int bin_ind = (int) ((val - min_val) / bin_width);
    if (bin_ind >= bin_number) --bin_ind;

    ++observed.at(bin_ind);
  }

  
  double chi2 = 0;

  for ( auto obs : observed)
  {
    chi2 += (obs - expected) * (obs - expected) / expected;
  }

  return {chi2, bin_number-1};

}


