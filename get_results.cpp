#include <filesystem>

#include <fstream>
#include <iostream>

#include <vector>
#include <string>
#include <algorithm>

#include "include/tests.h"



namespace fs = std::filesystem;


// void process_sample (std::ostream&, std::istream&);
// std::string process_sample (std::ostream&, std::istream&);
// std::vector<std::string> process_sample (std::istream&);
void process_sample (std::istream&, std::vector<std::string>&);


int main ()
{
  std::string dir_path = "./samples/";
  // fs::directory_iterator dir_iter
  std::ifstream sample_file;
  std::ofstream result_file;


  for (auto const& dir_entry : fs::directory_iterator(dir_path))
  {
    if (!dir_entry.is_directory()) continue;

    std::string subdir_name = dir_entry.path().filename().string();
    
    std::cout << "Processing " << subdir_name << "\n" << std::flush;

    result_file.open(dir_path + subdir_name + "/" + subdir_name + "_results.csv");
    result_file << "No,Size,Mean,StdDev,CV,Chi2,df\n";

    std::vector< std::pair< int,std::vector<std::string> >> result_vector;
    // std::vector<std::string> result_vector;

    // std::cout << "RESULT FILE OPENED\n";




    for (auto const& subdir_entry : fs::directory_iterator(dir_path + subdir_name))
    {
      if (subdir_entry.path().extension().string() == ".csv") continue;

      sample_file.open(subdir_entry.path());

      // std::cout << "SAMPLE FILE OPENED (" << subdir_entry.path().filename() << ")\n";
      
      std::string subdir_entry_stem = subdir_entry.path().stem().string();

      std::cout << "> Processing " << subdir_entry_stem << "\n" << std::flush;


      std::pair<int,std::vector<std::string>> sample_result;

      sample_result.first = std::stoi(subdir_entry_stem.substr(subdir_entry_stem.rfind("_") + 1));

      // std::cout << "\tHERE !\n" << std::flush;
      
      process_sample(sample_file, sample_result.second);
      
      // std::cout << "\tHERE !\n" << std::flush;

      result_vector.push_back(sample_result);

      // result_file << subdir_entry_stem.substr(subdir_entry_stem.rfind("_") + 1) << ',';
      // std::string sample_result = subdir_entry_stem.substr(subdir_entry_stem.rfind("_") + 1) + ',';
      // std::string sample_result = subdir_entry_stem.substr(subdir_entry_stem.rfind("_") + 1) + ',';
      // std::cout << "\'No\' printed in RESULT file\n";

      // process_sample(result_file, sample_file);
      // sample_result += process_sample(result_file, sample_file);
      // std::cout << "Other printed in RESULT file\n";


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


// double get_mean ( std::vector<unsigned int>& vec )
// {
//   double sum = 0;

//   for ( auto& num : vec )
//   {
//     sum += num;
//   }

//   return sum / vec.size();
// }


// double get_stdDev ( std::vector<unsigned int>& vec )
// {
//   unsigned long sum = 0;
//   auto mean = get_mean(vec);
  
//   for ( auto& num : vec )
//   {
//     sum += (num - mean) * (num - mean);
//   }

//   return std::sqrt(sum / vec.size());
// } 


// double get_cv ( std::vector<unsigned int>& vec )
// {
//   return get_stdDev(vec) / get_mean(vec);
// }


// // double get_chi2 ( std::vector<unsigned int>& vec, int bin_number )
// std::pair<double,int> get_chi2 ( std::vector<unsigned int>& vec )
// {
//   if (vec.empty())
//     return {-1,-1};

//   // int bin_number = (int)std::ceil()
  
//       // std::cout << "\tHERE !\n" << std::flush;
  

//   auto min_val = *std::min_element(vec.begin(), vec.end());
//   auto max_val = *std::max_element(vec.begin(), vec.end());
  

//   // float Scott_width = 3.49 * get_stdDev(vec) / std::pow(vec.size(), 1./3); // Ширина промежутка по правилу Скотта
  
//   // int bin_number =  Scott_width >= 1. ? /* Проверка на случай, если правило Скотта плохо сработало */ \
//                        std::ceil((max_val - min_val) / Scott_width):  /* Необходимое по правилу Скотта количество промежутков*/ \
//                        1 + std::log2(vec.size()); /* Необходимое по правилу Стерджерса количество промежутков (работает всегда, но не идеально)*/
//   int bin_number =  1 + std::log2(vec.size());
  
//   float bin_width = (float)(max_val-min_val)/bin_number; // Фактическая ширина промежутка

//   float expected = (float)vec.size() / bin_number;

//       // std::cout << "\tHERE !\n" << std::flush;

//       // std::cout << "\t scott wight: " << Scott_width << "\n" << std::flush;
//       // std::cout << "\t bin number: " << bin_number << "\n" << std::flush;
//       // std::cout << "\t bin width: " << bin_width << "\n" << std::flush;

//   std::vector<int> observed(bin_number, 0);

//       // std::cout << "\tHERE !\n" << std::flush;


//   for ( auto& val : vec )
//   {
//     int bin_ind = (int) ((val - min_val) / bin_width);
//     if (bin_ind >= bin_number) --bin_ind;

//     ++observed.at(bin_ind);

//   }

//   double chi2 = 0;

//   for ( auto& obs : observed)
//   {
//     chi2 += (obs - expected) * (obs - expected) / expected;
//   }

//   return {chi2, bin_number-1};

// }

// void process_sample (std::ostream& result_file, std::istream& sample_file)
// std::string process_sample (std::ostream& result_file, std::istream& sample_file)
// std::vector<std::string> process_sample (std::istream& sample_file)
void process_sample (std::istream& sample_file, std::vector<std::string>& vector)
{
  std::vector<unsigned int> sample;
  unsigned int num;

  // int step = 0;
  // char *tmp;
  while (true)
  {
    sample_file >> num;
    if (sample_file.eof()) break;

    // std::cout << "Step " << ++step << '\n';
    // std::cout << "> Got " << num << '\n';
    // std::cin >> tmp;

    sample.push_back(num);
  }

  // std::cout << ">>> Got vector of " << sample.size() << " elements\n";
  
  
  // double mean   = get_mean(sample);
  // double stdDev = get_stdDev(sample);
  // double cv     = get_cv(sample);
  
  vector.push_back( std::to_string(sample.size()) );
  vector.push_back( std::to_string(get_mean(sample)) );
  vector.push_back( std::to_string(get_stdDev(sample)) );
  vector.push_back( std::to_string(get_cv(sample)) );

  auto chi2_pair = get_chi2(sample);
  vector.push_back( chi2_pair.first  == -1 ? "N/A" : std::to_string(chi2_pair.first)  );
  vector.push_back( chi2_pair.second == -1 ? "N/A" : std::to_string(chi2_pair.second) );
  
  
  // std::cout << ">>> Got sample's MEAN\n";
  // std::cout << ">>> Got sample's STD DEV\n";
  // std::cout << ">>> Got sample's CV\n";

  // В result_file данные помещаются в порядке: Size,Mean,StdDev,CV

  // std::vector<std::string> vector = 
  // result_file << sample.size() << ',' << mean << ',' << stdDev << ',' << cv << std::endl;
  // return std::to_string(sample.size()) + ',' + std::to_string(mean) + ',' + std::to_string(stdDev) + ',' + std::to_string(cv) + '\n';


}
