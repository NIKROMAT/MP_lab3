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


// double get_chi2 ( std::vector<unsigned int>& vec, int bin_number )
std::pair<double,int> get_chi2 ( std::vector<unsigned int>& vec )
{
  if (vec.empty())
    return {-1,-1};

  // int bin_number = (int)std::ceil()
  
      // std::cout << "\tHERE !\n" << std::flush;
  

  auto min_val = *std::min_element(vec.begin(), vec.end());
  auto max_val = *std::max_element(vec.begin(), vec.end());
  

  // float Scott_width = 3.49 * get_stdDev(vec) / std::pow(vec.size(), 1./3); // Ширина промежутка по правилу Скотта
  
  // int bin_number =  Scott_width >= 1. ? /* Проверка на случай, если правило Скотта плохо сработало */ \
                       std::ceil((max_val - min_val) / Scott_width):  /* Необходимое по правилу Скотта количество промежутков*/ \
                       1 + std::log2(vec.size()); /* Необходимое по правилу Стерджерса количество промежутков (работает всегда, но не идеально)*/
  int bin_number =  1 + std::log2(vec.size());
  
  float bin_width = (float)(max_val-min_val)/bin_number; // Фактическая ширина промежутка

  float expected = (float)vec.size() / bin_number;

      // std::cout << "\tHERE !\n" << std::flush;

      // std::cout << "\t scott wight: " << Scott_width << "\n" << std::flush;
      // std::cout << "\t bin number: " << bin_number << "\n" << std::flush;
      // std::cout << "\t bin width: " << bin_width << "\n" << std::flush;

  std::vector<int> observed(bin_number, 0);

      // std::cout << "\tHERE !\n" << std::flush;


  for ( auto& val : vec )
  {
    int bin_ind = (int) ((val - min_val) / bin_width);
    if (bin_ind >= bin_number) --bin_ind;

    ++observed.at(bin_ind);

  }

  double chi2 = 0;

  for ( auto& obs : observed)
  {
    chi2 += (obs - expected) * (obs - expected) / expected;
  }

  return {chi2, bin_number-1};

}


