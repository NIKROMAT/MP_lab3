#include <fstream>
#include <vector>

#include <chrono>
#include <experimental/random>

#include "include/prng.h"
#include "include/defs.h"


void check_time (std::ostream&, int);

int main()
{
  std::ofstream time_results("generation_time.csv");
  
  time_results << "Size,Mid_xor,Shu_xor,LCG,randint\n";

  std::vector<int> sizes = {  1'000,   2'000,   3'000,   5'000,    10'000,
                             20'000,  30'000,  50'000,  75'000,   100'000,
                            200'000, 300'000, 500'000, 750'000, 1'000'000};



  for ( auto size : sizes )
    check_time (time_results, size);



}

void check_time (std::ostream& out, int size)
{
  namespace ch = std::chrono;
  using clock  = ch::high_resolution_clock;

  out << size << ',';

  ch::time_point<clock> start, end;
  std::ofstream trash("/dev/null");

  
  {
    mid_xor_PRNG prng(42, MIN_LIM, MAX_LIM);
    start = clock::now();
    prng.generate_sample(trash, size);
    end = clock::now();

    out << ch::duration_cast<ch::microseconds>(end-start).count() << ',';
  }

  
  {
    shuf_xor_PRNG prng(42, MIN_LIM, MAX_LIM);
    start = clock::now();
    prng.generate_sample(trash, size);
    end = clock::now();

    out << ch::duration_cast<ch::microseconds>(end-start).count() << ',';
  }

  
  {
    LCG prng(42, MIN_LIM, MAX_LIM);
    start = clock::now();
    prng.generate_sample(trash, size);
    end = clock::now();

    out << ch::duration_cast<ch::microseconds>(end-start).count() << ',';
  }

  
  {
    std::srand(42);
    start = clock::now();
    for (int i=0; i<size; ++i)
      std::experimental::randint(MIN_LIM, MAX_LIM);
    end = clock::now();

    out << ch::duration_cast<ch::microseconds>(end-start).count() << '\n';
  }
}

