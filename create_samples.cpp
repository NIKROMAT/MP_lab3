/**
 * @file create_samples.cpp
 * @brief Файл, предназначенный для генерации выборок разными генераторами
 * 
 * В этом файле генерируются выборки с помощью различных генераторов и записываются в соответствующе папки в папке ./samples
 * 
 */



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



/// @brief Основная функция, в которой происходит генерация выборок всеми генераторами
int main()
{
  std::ofstream sample_file;  

  // Создать папку для выборок, если её нет
  fs::create_directory(SAMPLES_DIR); 


  // Размеры выборок
  std::vector<int> sizes =  { 1000,  1000,  1000,  1000,
                              2000,  2000,  2000,  2000,
                              3000,  3000,  3000,  3000,
                              5000,  5000,  5000,  5000,
                             10000, 10000, 10000, 10000};

  // Семена генераторов
  std::vector<int> seeds =  {       42,  13973739, 323159976, 518977272,
                             667961784, 958390147, 567454690, 292832249,
                             583553826, 722672343, 621406124, 771825685,
                             398112944, 209114256, 242948276, 826041245,
                             573926780, 324066546, 454325408, 147431459};


  // Функции, в которых происходит генерация выборок для каждого из генераторов
  create_samples<mid_xor_PRNG> (SAMPLES_DIR, "mid_xor", sizes, seeds, MIN_LIM, MAX_LIM);
  create_samples<mul_xor_PRNG> (SAMPLES_DIR, "mul_xor", sizes, seeds, MIN_LIM, MAX_LIM);
  create_samples<LCG>          (SAMPLES_DIR, "lcg",     sizes, seeds, MIN_LIM, MAX_LIM);
  create_samples<div_PRNG>     (SAMPLES_DIR, "div",     sizes, seeds, MIN_LIM, MAX_LIM);


  std::endl(std::cout);
}

/// @brief Функция для генерации выборок для одного из генераторов
/// @tparam PRNG Класс используемого генератора. Наследник класса PRNG
/// @param path Путь в котором будет создана папка с выборками
/// @param prng_name Название генератора
/// @param sizes Список размеров выборок
/// @param seeds Список семян для генерации выборок. Размер списка должен делить количество размеров выборок
/// @param min_lim Нижняя граница генерации
/// @param max_lim Верхняя граница генерации
template <class PRNG>
void create_samples (std::string path, std::string prng_name,
                     std::vector<int>& sizes, std::vector<int>& seeds,
                     uint32_t min_lim, uint32_t max_lim)
{

  
  int mod;
  
  // Проверка корректности размеров списков seeds и sizes
  if (sizes.size() == seeds.size())     mod = sizes.size();
  else if (sizes.size() % seeds.size() == 0) mod = sizes.size() / seeds.size();
  else 
  {
    std::cerr << "! Неправильные размерности набора размеров и семян генераторов." << std::endl;
    return;
  }
  
  
  std::cout << "Generating samples by " + prng_name << std::endl;

  // Создание папки для выборок
  fs::create_directory(path + prng_name); 

  // Цикл создания выборок
  for (int i=0; i < sizes.size(); ++i)
  {

    // Создать файл выборки
    std::ofstream sample_file(path + prng_name + "/" + prng_name + "_" + std::to_string(i+1) + ".txt");

    // Создать генератор с заданным семенем и границами
    PRNG prng (seeds.at(i % mod), min_lim, max_lim);

    // Создать и записать выборку
    prng.generate_sample(sample_file, sizes.at(i));

    // Закрыть файл выборки
    sample_file.close();
    
    std::cout << "> Complete " + prng_name + "_" + std::to_string(i+1) + ".txt" << std::endl;
  }

  std::endl(std::cout);
}

