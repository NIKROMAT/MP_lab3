#include <cmath>

#include "../include/tests.h"
#include "../include/defs.h"


#include<iostream>



std::vector<bool> sample_to_bit_sequence ( std::vector<unsigned int>& vec )
{
  int max_len = 1 + std::log2(MAX_LIM); // Находим наибольший бит, который может принять значение 1
                                        // Старшие биты всегда будут 0, что будет нарушать тесты, поэтому обрубаем их

  std::vector<bool> bit_seq;

  for (auto num : vec)
  {
    for (int i=0; i < max_len; ++i)
    {
      bit_seq.push_back(num >> i & 1); // биты хранятся в обратном порядке, т.е. чем меньше индекс, тем младше бит
    }
  }

  return bit_seq;


}

std::vector<unsigned int> get_pattern_counts ( std::vector<bool>& bit_seq, int m )
{
  std::vector<unsigned int> pattern_counts(1<<m, 0);
  
  for (int i=0; i < bit_seq.size(); ++i)
  {
    int k=0;
    for (int j = m-1; j >= 0; --j)  // Проходимся в обратном порядке для эстетичности, чтобы паттерн равный числу I был записан под индексом I
    {
      if (bit_seq.at((i+j) % bit_seq.size()))   k = 2*k + 1;
      else                                      k = 2*k;
    }
    ++pattern_counts[k];
  }

  return pattern_counts;
}


double get_psi2 ( std::vector<bool>& bit_seq, int m )
{
  std::vector<unsigned int> pattern_counts = get_pattern_counts(bit_seq, m);


  int expected = bit_seq.size() / (1 << m);

  double sum = 0;
  for ( auto count : pattern_counts)
  {
    sum += (count - expected) * (count - expected);
  }

  sum = ( sum * (1 << m) ) / bit_seq.size() - bit_seq.size();

  return sum;
}


double get_apEn ( std::vector<bool>& bit_seq, int m )
{
  std::vector<unsigned int> pattern_counts = get_pattern_counts(bit_seq, m);

  double apEn = 0;

  for ( auto count : pattern_counts)
  {
    double p = (double)count / bit_seq.size();
    apEn += (p > std::pow(10,-6)) ? (p * std::log(p)) : 0;
  }

  return apEn;
}


