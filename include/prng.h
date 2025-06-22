/**
 * @file prng.h
 * @brief Заголовочный файл, в котором определяются классы генераторов.
 * 
 * Объявляемые классы:
 *  - PRNG
 *  - mid_xor_PRNG
 *  - mul_xor_PRNG
 *  - LCG
 */

#ifndef PRNG_H
#define PRNG_H

#include <cstdint> // для типов int


/// @brief Абстрактный базовый класс генераторов
class PRNG
{

protected:
  const uint32_t min_lim; ///< Нижняя  граница генерации
  const uint32_t max_lim; ///< Верхняя граница генерации

  PRNG (uint32_t, uint32_t); 

  virtual uint32_t generate() = 0; ///< Чистая функция для генерации следующего числа
  
  uint32_t result(uint32_t) const;
  
public:
  void generate_sample(std::ostream&, int);

};



/// @brief Генератор, основанный на среднем квадрата и Xor
class mid_xor_PRNG: public PRNG
{
  uint32_t seed;  ///< Семя генерации 


public:
  mid_xor_PRNG () = delete; ///< Запрет на конструктор по умолчанию 
  mid_xor_PRNG ( uint32_t, uint32_t = 0, uint32_t = -1);  

  uint32_t generate() override;
};


/// @brief Генератор, основанный на произведении и Xor
class mul_xor_PRNG: public PRNG
{
  uint32_t seed;  ///< Семя генерации 


public:
  mul_xor_PRNG () = delete; ///< Запрет на конструктор по умолчанию 
  mul_xor_PRNG ( uint32_t, uint32_t = 0, uint32_t = -1);

  uint32_t generate() override;
};


/// @brief Генератор, основанный на линейном конгруэнтном методе
class LCG: public PRNG
{
  uint32_t seed;  ///< Семя генерации
  uint32_t k;     ///< Коэффициент
  uint32_t b;     ///< Смещение
  uint32_t M;     ///< Модуль


public:
  LCG () = delete;  ///< Запрет на конструктор по умолчанию 
  LCG ( uint32_t, uint32_t = 0, uint32_t = -1);

  uint32_t generate() override;
};



/// @brief Генератор, основанный на делении чисел друг на друга
class div_PRNG: public PRNG
{
  uint32_t seed;  ///< Семя генерации
  uint32_t prev;  ///< Предыдущий результат

public:
  div_PRNG () = delete;  ///< Запрет на конструктор по умолчанию 
  div_PRNG ( uint32_t, uint32_t = 0, uint32_t = -1);

  uint32_t generate() override;
};


#endif // PRNG_H