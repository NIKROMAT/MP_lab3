#include <filesystem>

#include <fstream>
#include <iostream>

#include <string>
#include <vector>

#include "include/prng.h"
#include "include/defs.h"


namespace fs = std::filesystem;

template <class PRNG>
void create_samples (std::string, std::string, std::vector<int>&, std::vector<int>&, uint32_t = 0, uint32_t = -1);

int main()
{
  std::ofstream sample_file;
  // std::string SAMPLES_DIR = "./samples/";


  fs::create_directory(SAMPLES_DIR);


  std::vector<int> sizes =  { 1000,  1000,  1000,  1000,
                              2000,  2000,  2000,  2000,
                              3000,  3000,  3000,  3000,
                              5000,  5000,  5000,  5000,
                             10000, 10000, 10000, 10000};


  std::vector<int> seeds =  {       42,  13973739, 323159976, 518977272,
                             667961784, 958390147, 567454690, 292832249,
                             583553826, 722672343, 621406124, 771825685,
                             398112944, 209114256, 242948276, 826041245,
                             573926780, 324066546, 454325408, 147431459};



  create_samples<mid_xor_PRNG>  (SAMPLES_DIR, "mid_xor",  sizes, seeds, MIN_LIM, MAX_LIM);
  create_samples<shuf_xor_PRNG> (SAMPLES_DIR, "shuf_xor", sizes, seeds, MIN_LIM, MAX_LIM);
  create_samples<LCG>           (SAMPLES_DIR, "lcg",      sizes, seeds, MIN_LIM, MAX_LIM);


}


template <class PRNG>
void create_samples (std::string path, std::string prng_name,
                     std::vector<int>& sizes, std::vector<int>& seeds,
                     uint32_t min_lim, uint32_t max_lim)
{
  int mod;

       if (sizes.size() == seeds.size())     mod = sizes.size();
  else if (sizes.size() % seeds.size() == 0) mod = sizes.size() / seeds.size();
  else 
  {
    std::cerr << "Неправильные размерности набора размеров и семян генераторов.";
    return;
  }



  fs::create_directory(path + prng_name);

  for (int i=0; i < sizes.size(); ++i)
  {

    std::ofstream sample_file(path + prng_name + "/" + prng_name + "_" + std::to_string(i+1) + ".txt");

    PRNG prng (seeds.at(i % mod), min_lim, max_lim);

    prng.generate_sample(sample_file, sizes.at(i));

    sample_file.close();
  }
}

