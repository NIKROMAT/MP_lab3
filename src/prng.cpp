/**
 * @file prng.cpp
 * @brief Файл, с определением методов классов генераторов
 */

#include <iostream>

#include "../include/prng.h"


////////////////////////////////
//            PRNG            //
////////////////////////////////


/// @brief Конструктор для базового абстрактного класса
/// @param min_lim Нижняя  граница генерации
/// @param max_lim Верхняя граница генерации
PRNG::PRNG ( uint32_t min_lim, uint32_t max_lim )
: min_lim(min_lim), max_lim(max_lim) {}


/// @brief Метод для генерации и записи в поток целой выборки
/// @param out  Поток для записи
/// @param size Размер генерируемой выборки
void PRNG::generate_sample (std::ostream& out, int size)
{
  for (int i=0; i < size; ++i)
  {
    out << this->generate() << std::endl;
  }
}


/// @brief Метод для перевода сгенерированного числа в требуемый диапазон
/// @param result Сгенерированное число
/// @return Сгенерированное число в требуемом диапазоне
uint32_t PRNG::result (uint32_t result) const
{
  return min_lim + result % (1ul + max_lim - min_lim); // поскольку max_lim - верхняя граница включительно, то нужно брать модуль +1
                                                       // (1 - unsigned long, чтобы не возникло переполнения и не было деления на 0)
}




////////////////////////////////
//           mid_xor          //
////////////////////////////////

/// @brief Конструктор класса mid_xor_PRNG
/// @param seed Семя генерации
/// @param min_lim Нижняя  граница генерации
/// @param max_lim Верхняя граница генерации
mid_xor_PRNG::mid_xor_PRNG (uint32_t seed, uint32_t min_lim, uint32_t max_lim)
: PRNG(min_lim, max_lim), seed(seed) {} 


/// @brief Генерация следующего числа
/// @return Следующее случайное число
uint32_t mid_xor_PRNG::generate ()
{
  uint64_t product = seed * seed;

  seed  = (product >> 16) & 0xFFFFFFFF;
  seed ^= 0xAAAAAAAA; // инвертируем каждый 2 бит начиная со старшего, чтобы всё не вырождалось в 0

  return result(seed);
}



////////////////////////////////
//           mul_xor          //
////////////////////////////////

/// @brief 
/// @param seed Семя генерации
/// @param min_lim Нижняя  граница генерации
/// @param max_lim Верхняя граница генерации
mul_xor_PRNG::mul_xor_PRNG (uint32_t seed, uint32_t min_lim, uint32_t max_lim)
: PRNG(min_lim, max_lim), seed(seed) {}


/// @brief Генерация следующего числа
/// @return Следующее случайное число
uint32_t mul_xor_PRNG::generate ()
{
  seed ^= seed << 13;
  seed ^= (seed >> 7) * 0x9AE77B3D;
  seed ^= (seed >> 5) | (seed << 17);

  return result(seed);
}



////////////////////////////////
//             LCG            //
////////////////////////////////

/// @brief 
/// @param seed Семя генерации
/// @param min_lim Нижняя  граница генерации
/// @param max_lim Верхняя граница генерации
LCG::LCG (uint32_t seed, uint32_t min_lim, uint32_t max_lim)
: PRNG(min_lim, max_lim), seed(seed), k((1<<16)+1), b((1<<8)-1), M(1<<31) {}


/// @brief Генерация следующего числа
/// @return Следующее случайное число
uint32_t LCG::generate ()
{
  seed = (seed * k + b) & (M-1); // Если M = 2^N
  // seed = (seed * k + b) % (M); // иначе

  // (k-1) - делится на все простые делители M (2)
  // b и M взаимно простые
  // M делится на 4 и (k-1) делится на 4
  // 
  // Поэтому у этого линейного конгруэнтного генератора макцимальная периодичность  

  return result(seed);
}


