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

  seed  = (product >> 8) & 0xFFFFFFFF;

  // Динамический сдвиг для XorShift
  uint8_t shift = (seed >> 10) & 0x1F; // Сдвиг от 0 до 31
  seed ^= 0x9E3779B9 << (shift % 13 + 1); // Чтобы не было вырождения в 0
  seed ^= seed >> (shift % 17 + 1);

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
// : PRNG(min_lim, max_lim), seed(seed), k((1<<16)+1), b((1<<8)-1), M(1<<31) {}
: PRNG(min_lim, max_lim), seed(seed), k(1'103'515'245), b(12345), M(1<<31) {}


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





////////////////////////////////
//             div            //
////////////////////////////////

/// @brief 
/// @param seed Семя генерации
/// @param min_lim Нижняя  граница генерации
/// @param max_lim Верхняя граница генерации
div_PRNG::div_PRNG (uint32_t seed, uint32_t min_lim, uint32_t max_lim)
: PRNG(min_lim, max_lim), seed(seed), prev(0x9AE77B3D) {}


/// @brief Генерация следующего числа
/// @return Следующее случайное число
uint32_t div_PRNG::generate ()
{
  // Определяем меньшее и большее значения между seed и prev
  uint32_t min = (prev > seed) ? seed : prev;
  uint32_t max = (min != seed) ? seed : prev;

  bool flag = (seed == min); // флаг, чтобы понимать, какие изменения проводятся над переменными

  // Изменения, чтобы не возникло проблем с делением
  if ( min == UINT32_MAX ) min = 0x9AE77B3D;
  if ( min == max ) 
    max = (((unsigned long)max + min) > UINT32_MAX) ? UINT32_MAX : max + min;


  double div = double(min) / max; // Число в диапазоне (0,1)

  seed = div * (1ul << 48); // Извлекаем 32 бита после запятой, начиная с 16 бита (первые 16 битов обрубаются за счёт типа seed)

  if ( seed == 0 ) seed = 0x9AE77B3D; // Избавляемся от выраждения в 0
  prev = (flag ? min : max); // Записываем предыдущее значение seed, включая преобразования

  return result(seed);
}

