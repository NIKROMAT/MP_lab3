#include <filesystem>

#include <fstream>
#include <iostream>

#include <vector>
#include <string>
#include <algorithm>

#include "include/tests.h"
#include "include/defs.h"



namespace fs = std::filesystem;


void process_sample (std::istream&, std::vector<std::string>&);


int main ()
{

  std::ifstream sample_file;
  std::ofstream result_file;


  for (auto const& dir_entry : fs::directory_iterator(SAMPLES_DIR))
  {
    if (!dir_entry.is_directory()) continue;

    std::string subdir_name = dir_entry.path().filename().string();
    
    std::cout << "Processing " << subdir_name << "\n" << std::flush;

    result_file.open(SAMPLES_DIR + subdir_name + "/" + subdir_name + "_results.csv");
    result_file << "No,Size,"
                   "Mean,"
                   "StdDev,"
                   "CV,"
                   "Chi2,Chi2 df,"
                   "NIST frequency,frequency status,"
                   "NIST runs,runs status,"
                   "NIST serial,serial block size,serial status,"
                   "NIST approximate entropy,ApEn block size,ApEn status,"
                   "NIST cusum,cusum status\n";

    std::vector< std::pair< int,std::vector<std::string> >> result_vector;




    for (auto const& subdir_entry : fs::directory_iterator(SAMPLES_DIR + subdir_name))
    {
      if (subdir_entry.path().extension().string() == ".csv") continue;

      sample_file.open(subdir_entry.path());

      std::string subdir_entry_stem = subdir_entry.path().stem().string();

      std::cout << "> Processing " << subdir_entry_stem << "\n" << std::flush;


      std::pair<int,std::vector<std::string>> sample_result;

      sample_result.first = std::stoi(subdir_entry_stem.substr(subdir_entry_stem.rfind("_") + 1));

      process_sample(sample_file, sample_result.second);
      
      result_vector.push_back(sample_result);


      sample_file.close();
    }


    std::sort( result_vector.begin(), result_vector.end(), [](auto& el1, auto& el2){ return el1.first < el2.first; } );

    for (auto& pair : result_vector)
    {
      result_file << pair.first;

      for (auto& val : pair.second)
      {
        result_file << ',' << val; 
      }
      result_file << std::endl; 
    }

    result_file.close();
  }

}



void process_sample (std::istream& sample_file, std::vector<std::string>& vector)
{
  std::vector<unsigned int> sample;
  unsigned int num;

  while (true)
  {
    sample_file >> num;
    if (sample_file.eof()) break;

    sample.push_back(num);
  }

  
  
  double mean   = get_mean(sample);
  double stdDev = get_stdDev(sample);
  double cv     = get_cv(sample);
  
  vector.push_back( std::to_string(sample.size()) );
  vector.push_back( std::to_string(get_mean(sample)) );
  vector.push_back( std::to_string(get_stdDev(sample)) );
  vector.push_back( std::to_string(get_cv(sample)) );

  auto chi2_pair = get_chi2(sample);
  vector.push_back( chi2_pair.first  == -1 ? "N/A" : std::to_string(chi2_pair.first)  );
  vector.push_back( chi2_pair.second == -1 ? "N/A" : std::to_string(chi2_pair.second) );


  std::vector<bool> bit_sequence = sample_to_bit_sequence(sample);

  double frequency = nist_frequency(bit_sequence);
  vector.push_back( std::to_string(frequency) );
  vector.push_back( (frequency >= ALPHA) ? "PASS" : "FAIL" );

  double runs = nist_runs(bit_sequence);
  vector.push_back( std::to_string(runs) );
  vector.push_back( (runs >= ALPHA) ? "PASS" : "FAIL" );

  double serial = nist_serial(bit_sequence, SERIAL_M);
  vector.push_back( std::to_string(serial) );
  vector.push_back( (serial >= ALPHA) ? "PASS" : "FAIL" );

  double apEntropy = nist_apEntropy(bit_sequence, AP_ENTROPY_M);
  vector.push_back( std::to_string(apEntropy) );
  vector.push_back( (apEntropy >= ALPHA) ? "PASS" : "FAIL" );

  double cusum = nist_cusum(bit_sequence);
  vector.push_back( std::to_string(cusum) );
  vector.push_back( (cusum >= ALPHA) ? "PASS" : "FAIL" );


}


