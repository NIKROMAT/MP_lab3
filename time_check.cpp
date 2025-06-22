/**
 * @file time_check.cpp
 * @brief Файл, в котором производится измерение времени генерации выборок разных размеров разными генераторами и одним из стандартных генераторов
 */

#include <fstream>
#include <vector>

#include <chrono>
#include <experimental/random>

#include "include/prng.h"
#include "include/defs.h"


void check_time (std::ostream&, int);


/// @brief Основная функция, в которой происходит измерение времени генерации выборок разных размеров всеми генераторами и одним из стандартных генераторов
/// @return 
int main()
{
  // Файл для записи результатов
  std::ofstream time_results("generation_time.csv");
  
  time_results << "Size,Mid_xor,Mul_xor,LCG,Div,randint\n";

  // Список размеров выборок
  std::vector<int> sizes = {  1'000,   2'000,   3'000,   5'000,    10'000,
                             20'000,  30'000,  50'000,  75'000,   100'000,
                            200'000, 300'000, 500'000, 750'000, 1'000'000};


  // Цикл измерения и записи времени генерации выборок для разных размеров
  for ( auto size : sizes )
    check_time (time_results, size);

}


/// @brief Функция, в которой производится измерение и запись времени генерации выборки для каждого из генераторов
/// @param out Поток, куда будет записан результат
/// @param size Размер генерируемой выборки
void check_time (std::ostream& out, int size)
{
  namespace ch = std::chrono;               // Для удобства
  using clock  = ch::high_resolution_clock; // Для удобства

  // Записываем размер выборки
  out << size << ',';

  ch::time_point<clock> start, end; // Метки начала и конца измерения
  std::ofstream trash("/dev/null"); // Мусорка, чтобы не хранить сгенерированные выборки

  
  // Замер для генератора mid_xor
  {
    mid_xor_PRNG prng(42, MIN_LIM, MAX_LIM);  // Создание генератора
    start = clock::now();                     // Начало измерения
    prng.generate_sample(trash, size);        // Генерация выборки
    end = clock::now();                       // Конец измерения

    out << ch::duration_cast<ch::microseconds>(end-start).count() << ','; // Запись результата
  }

  
  // Замер для генератора mul_xor
  {
    mul_xor_PRNG prng(42, MIN_LIM, MAX_LIM);  // Создание генератора
    start = clock::now();                     // Начало измерения
    prng.generate_sample(trash, size);        // Генерация выборки
    end = clock::now();                       // Конец измерения

    out << ch::duration_cast<ch::microseconds>(end-start).count() << ','; // Запись результата
  }

  
  // Замер для генератора LCG
  {
    LCG prng(42, MIN_LIM, MAX_LIM);           // Создание генератора
    start = clock::now();                     // Начало измерения
    prng.generate_sample(trash, size);        // Генерация выборки
    end = clock::now();                       // Конец измерения

    out << ch::duration_cast<ch::microseconds>(end-start).count() << ','; // Запись результата
  }

  
  // Замер для генератора div
  {
    div_PRNG prng(42, MIN_LIM, MAX_LIM);      // Создание генератора
    start = clock::now();                     // Начало измерения
    prng.generate_sample(trash, size);        // Генерация выборки
    end = clock::now();                       // Конец измерения

    out << ch::duration_cast<ch::microseconds>(end-start).count() << ','; // Запись результата
  }

  
  // Замер для генератора randint
  {
    std::srand(42);                                 // Задание семени
    start = clock::now();                           // Начало измерения
    for (int i=0; i<size; ++i)                      // Генерация выборки
      std::experimental::randint(MIN_LIM, MAX_LIM);
    end = clock::now();                             // Конец измерения

    out << ch::duration_cast<ch::microseconds>(end-start).count() << '\n'; // Запись результата
  }
}

