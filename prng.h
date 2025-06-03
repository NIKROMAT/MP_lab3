// #include <vector>
#include <cstdint> // для типов int
#include <iostream>

// typedef uint32_t;
// typedef uint64_t;

class PRNG
{

public:
  // virtual uint32_t generate() = 0;
  virtual uint32_t generate() = 0;

  void generate_sample(std::ostream&, int);

};


class MidPRNG: public PRNG
// class MidPRNG
{
  uint32_t base;
  uint32_t seed;


public:
  MidPRNG () = delete;
  MidPRNG ( uint32_t );

  uint32_t generate() override;
};


class ShufflePRNG: public PRNG
{
  uint32_t seed;


public:
  ShufflePRNG () = delete;
  ShufflePRNG ( uint32_t );

  uint32_t generate() override;
};


class LCG: public PRNG
{
  uint32_t seed;
  uint32_t k;
  uint32_t b;
  uint32_t M;


public:
  LCG () = delete;
  // LCG ( uint32_t, uint32_t, uint32_t, uint32_t );
  LCG ( uint32_t );

  uint32_t generate() override;
};



class mid_xor_PRNG: public PRNG
{
  uint32_t seed;


public:
  mid_xor_PRNG () = delete;
  mid_xor_PRNG ( uint32_t );

  uint32_t generate() override;
};


class shuf_xor_PRNG: public PRNG
{
  uint32_t seed;


public:
  shuf_xor_PRNG () = delete;
  shuf_xor_PRNG ( uint32_t );

  uint32_t generate() override;
};