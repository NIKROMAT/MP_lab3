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
  std::string dir_path = "./samples/";


  fs::create_directory(dir_path);


  // int sizes[20] =  { 1000,  1000,  1000,  1000,
  std::vector<int> sizes =  { 1000,  1000,  1000,  1000,
                              2000,  2000,  2000,  2000,
                              3000,  3000,  3000,  3000,
                              5000,  5000,  5000,  5000,
                             10000, 10000, 10000, 10000};


  // int seeds[20] =  {       42,  13973739, 323159976, 518977272,
  std::vector<int> seeds =  {       42,  13973739, 323159976, 518977272,
                             667961784, 958390147, 567454690, 292832249,
                             583553826, 722672343, 621406124, 771825685,
                             398112944, 209114256, 242948276, 826041245,
                             573926780, 324066546, 454325408, 147431459};


  uint32_t min_lim = MIN_LIM;
  uint32_t max_lim = MAX_LIM;
  // uint32_t min_lim = 0;
  // uint32_t max_lim = -1;
  // uint32_t max_lim = 5000;

  // std::vector vec(seeds);


  create_samples<mid_xor_PRNG>  (dir_path, "mid_xor",  sizes, seeds, min_lim, max_lim);
  create_samples<shuf_xor_PRNG> (dir_path, "shuf_xor", sizes, seeds, min_lim, max_lim);
  create_samples<LCG>           (dir_path, "lcg",  sizes, seeds, min_lim, max_lim);


  // int seeds[20] =  {       42,  13973739, 323159976, 518977272}; 
  //  Если нужно сравнить те же сиды на разных размерах выборок

  // sample_file.open("./samples/test1.txt");
  // std::string prng_dir;

  // prng_dir = "mid_xor/";
  // fs::create_directory(dir_path + prng_dir);

  // for (int i=0; i<20; ++i)
  // {

  //   sample_file.open(dir_path + prng_dir + "mid_xor_" + std::to_string(i+1) + ".txt");

  //   mid_xor_PRNG prng (seeds[i]);

  //   prng.generate_sample(sample_file, sizes[i]);

  //   // prng.generate_sample(sample_file, sizes[i%4]);  
  //   //  Если нужно сравнить те же сиды на разных размерах выборок

  //   sample_file.close();
  // }



  // std::cout << std::boolalpha << sample_file.fail() << '\n';

  // sample_file.close();


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

    // prng.generate_sample(sample_file, sizes[i%4]);  
    //  Если нужно сравнить те же сиды на разных размерах выборок

    sample_file.close();
  }
}




// int f_main()
// {
//   int seed = 42;

//   // ShufflePRNG sh(42);
  

//   // std::cout << sh.generate() << "\n\n";
//   // std::cout << sh.generate() << "\n\n";
//   // std::cout << sh.generate() << "\n\n";
//   // std::cout << sh.generate() << "\n\n";
//   // std::cout << sh.generate() << "\n\n";




//   MidPRNG       mid     (seed);
//   mid_xor_PRNG  mid_xor (seed);

//   ShufflePRNG   shu     (seed);
//   shuf_xor_PRNG shu_xor (seed);
  
  
//   std::cout << "Middle PRNG:\n";

//   for (int i=0; i<10; ++i)
//   {
//     std::cout << mid.generate() << "\n";
//   }

//   std::cout << "\nMiddle & Xor PRNG:\n";

//   for (int i=0; i<10; ++i)
//   {
//     std::cout << mid_xor.generate() << "\n";
//   }

//   std::cout << "\n\nShuffle PRNG:\n";

//   for (int i=0; i<10; ++i)
//   {
//     std::cout << shu.generate() << "\n";
//   }

//   std::cout << "\nShuffle & Xor PRNG:\n";

//   for (int i=0; i<10; ++i)
//   {
//     std::cout << shu_xor.generate() << "\n";
//   }










//   // int seed = (1<<19)-1;

//   // MidPRNG     mid(seed);
//   // ShufflePRNG shu(seed);
//   // LCG         lcg(seed);



//   // std::cout << "Middle PRNG:\n";

//   // for (int i=0; i<10; ++i)
//   // {
//   //   std::cout << mid.generate() << "\n";
//   // }

//   // std::cout << "\nShuffle PRNG:\n";

//   // for (int i=0; i<10; ++i)
//   // {
//   //   std::cout << shu.generate() << "\n";
//   // }

//   // std::cout << "\nLCG PRNG:\n";

//   // for (int i=0; i<10; ++i)
//   // {
//   //   std::cout << lcg.generate() << "\n";
//   // }
// }