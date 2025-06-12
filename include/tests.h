/**
 * @file tests.h
 * @brief Заголовочный файл, содержащий объявления всех функций для вычисления значений и проведения тестов
 */


#ifndef TESTS_H
#define TESTS_H 1


#include <vector>

double get_mean ( std::vector<unsigned int>& );
double get_stdDev ( std::vector<unsigned int>& );
double get_cv ( std::vector<unsigned int>& );

std::pair<double, int> get_chi2 ( std::vector<unsigned int>& );


std::vector<bool> sample_to_bit_sequence ( std::vector<unsigned int>& );

std::vector<unsigned int> get_pattern_counts ( std::vector<bool>&, int );
double get_psi2 ( std::vector<bool>&, int );
double get_apEn  ( std::vector<bool>&, int );


double nist_frequency ( std::vector<bool>& );
double nist_runs ( std::vector<bool>& );
double nist_serial ( std::vector<bool>&, int );
double nist_apEntropy ( std::vector<bool>&, int );
double nist_cusum ( std::vector<bool>& );


#endif // TESTS_H