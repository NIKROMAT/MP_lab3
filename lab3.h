// #include <vector>
#include <cstdint> // для типов int

// typedef uint32_t;
// typedef uint64_t;

// class PRNG
// {

// public:
//   virtual int generate() = 0;

// };


// class MidPRNG: public PRNG
class MidPRNG
{
  uint32_t base;
  uint32_t seed;


public:
  MidPRNG () = delete;
  MidPRNG ( uint32_t );

  uint32_t generate();
};


class ShufflePRNG
{
  uint32_t seed;


public:
  ShufflePRNG () = delete;
  ShufflePRNG ( uint32_t );

  uint32_t generate();
};


class LCG
{
  uint64_t seed;
  uint64_t k;
  uint64_t b;
  uint64_t M;


public:
  LCG () = delete;
  // LCG ( uint64_t, uint64_t, uint64_t, uint64_t );
  LCG ( uint64_t );

  uint64_t generate();
};


