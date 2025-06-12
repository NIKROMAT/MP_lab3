#include <cmath>
#include <algorithm>

#include "../include/tests.h"
#include "../include/defs.h"

#include "../include/external/cephes.h"


#include<iostream>




double nist_frequency ( std::vector<bool>& bit_seq )
{
  int sum = 0;

  for (auto bit : bit_seq)
  {
    bit ? ++sum : --sum; // Итого: ++sum, если бит - 1, иначе --sum
  }

  double s_obs = std::abs(sum) / std::sqrt(bit_seq.size());
  double p_value = std::erfc(s_obs / std::sqrt(2));

  return p_value;
}


double nist_runs ( std::vector<bool>& bit_seq )
{

  int sum = std::count(bit_seq.begin(), bit_seq.end(), true);

  double pi = (double)sum / bit_seq.size();

  if (fabs(pi - 0.5) >= 2 / std::sqrt(bit_seq.size()))
  {
    std::cerr << ">>> Критерий для runs-теста не выполнен." << std::endl;
    return 0;
  }


  int runs = 1;
  for (int i=1; i < bit_seq.size(); ++i)
  {
    if (bit_seq.at(i) != bit_seq.at(i-1))
      ++runs;
  }

  double erfc_arg = fabs(runs - 2.0 * bit_seq.size() * pi * (1-pi)) / (2.0 * pi * (1-pi) * sqrt(2*bit_seq.size()));
	double p_value = erfc(erfc_arg);

  return p_value;

}


double nist_serial ( std::vector<bool>& bit_seq, int m )
{
  double psi_m   = get_psi2(bit_seq, m);
  double psi_m_1 = get_psi2(bit_seq, m-1);


  double p_value = cephes_igamc(1 << (m-2), (psi_m - psi_m_1)/2);
  return p_value; 
}


double nist_apEntropy ( std::vector<bool>& bit_seq, int m )
{
  double apEn_m  = get_apEn(bit_seq, m);
  double apEn_m1 = get_apEn(bit_seq, m+1);

  double apEn = apEn_m - apEn_m1;
  double chi2 = 2 * bit_seq.size() * (std::log(2) - apEn);

  double p_value = cephes_igamc(1 << (m-1), chi2/2);
  return p_value; 
}


double nist_cusum ( std::vector<bool>& bit_seq )
{
  int S = 0;
  int max_S = 0;


  for ( auto bit : bit_seq )
  {
    bit ? ++S : --S;
    max_S = (std::abs(S) > max_S) ? std::abs(S) : max_S; 
  }

  double p_value = std::erfc(max_S / std::sqrt(2 * bit_seq.size()));

  return p_value;
}


