#ifndef TESTS_H
#define TESTS_H 1


#include <vector>

double get_mean ( std::vector<unsigned int>& );
double get_stdDev ( std::vector<unsigned int>& );
double get_cv ( std::vector<unsigned int>& );

std::pair<double, int> get_chi2 ( std::vector<unsigned int>& );

// double nist_fraction ( std::vector<unsigned int>&, unsigned int );
double nist_fraction ( std::vector<unsigned int>& );


#endif // TESTS_H