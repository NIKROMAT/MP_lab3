/**
 * @file get_results.cpp
 * @brief Файл для вычисления измерений и проведения тестов над выборками
 * 
 * В этом файле происходит вычисление Среднего, стандартного отклонения, коэффициента вариации, тест хи-квадрат, а также 5 nist-тестов.
 * 
 * Среди nist-тестов:
 *  - Частотный побитовый тест
 *  - Тест на последовательность одинаковых битов
 *  - Тест на периодичность
 *  - Тест приблизительной энтропии
 *  - Тест кумулятивных сумм
 * 
 */

#include <filesystem>

#include <fstream>
#include <iostream>

#include <vector>
#include <string>
#include <algorithm>

#include "include/tests.h"
#include "include/defs.h"



namespace fs = std::filesystem;


void process_sample (std::istream&, std::vector<std::string>&);

/// @brief Основная функция, в которой произвоится вычисление всех значений и проведение тестов для всех выборок всех генераторов
int main ()
{

  std::ifstream sample_file;
  std::ofstream result_file;


  // Цикл прохождения по содержимому главной папки с выборками
  for (auto const& dir_entry : fs::directory_iterator(SAMPLES_DIR))
  {
    // Обрабатываем только папки с выборками генераторов
    if (!dir_entry.is_directory()) continue;

    // Получаем имя генератора
    std::string subdir_name = dir_entry.path().filename().string();
    
    std::cout << "Processing " << subdir_name << std::endl;

    // Подготавливаем файл для записи результатов
    result_file.open(SAMPLES_DIR + subdir_name + "/" + subdir_name + "_results.csv");
    result_file << "No,Size,"
                   "Mean,"
                   "StdDev,"
                   "CV,"
                   "Chi2,Chi2 df,"
                   "NIST frequency,frequency status,"
                   "NIST runs,runs status,"
                   "NIST serial,serial block size,serial status,"
                   "NIST approximate entropy,ApEn block size,ApEn status,"
                   "NIST cusum,cusum status\n";


    // Проход по файлам папки произвользый, поэтому чтобы сохранить очерёдность будем временно сохранять результаты в вектор
    std::vector< std::pair< int,std::vector<std::string> >> result_vector;



    // Цикл для прохода по всем выборкам генератора
    for (auto const& subdir_entry : fs::directory_iterator(SAMPLES_DIR + subdir_name))
    {
      // Файл для записи результата игнорируем
      if (subdir_entry.path().extension().string() == ".csv") continue;

      // Открываем файл выборки
      sample_file.open(subdir_entry.path());

      std::string subdir_entry_stem = subdir_entry.path().stem().string();

      std::cout << "> Processing " << subdir_entry_stem << std::endl;


      // Вектор для временного хранения реультатов
      std::pair<int,std::vector<std::string>> sample_result;

      sample_result.first = std::stoi(subdir_entry_stem.substr(subdir_entry_stem.rfind("_") + 1));

      // Обрабатываем выборку
      process_sample(sample_file, sample_result.second);
      
      // Сохраняем результат
      result_vector.push_back(sample_result);


      sample_file.close();
    }


    // Сортируем результаты по увеличению номера выборки
    std::sort( result_vector.begin(), result_vector.end(), [](auto& el1, auto& el2){ return el1.first < el2.first; } );

    // Записываем результаты в файл 
    for (auto& pair : result_vector)
    {
      result_file << pair.first;

      for (auto& val : pair.second)
      {
        result_file << ',' << val; 
      }
      result_file << std::endl; 
    }

    // Закрываем файл результатов генератора
    result_file.close();

    std::endl(std::cout);
  }
  
  
  std::endl(std::cout);

}


/// @brief Функция для полной обработки выбороки генератора
/// @param sample_file Файл выборки
/// @param vector Вектор для хранения найденных результатов
void process_sample (std::istream& sample_file, std::vector<std::string>& vector)
{
  // Записываем выборку в вектор для удобства
  std::vector<unsigned int> sample;
  unsigned int num;

  while (true)
  {
    sample_file >> num;
    if (sample_file.eof()) break;

    sample.push_back(num);
  }

  
  
  vector.push_back( std::to_string(sample.size()) );      // Записываем размер выборки
  vector.push_back( std::to_string(get_mean(sample)) );   // Записываем среднее выборки
  vector.push_back( std::to_string(get_stdDev(sample)) ); // Записываем стандартное отклонение выборки
  vector.push_back( std::to_string(get_cv(sample)) );     // Записываем коэффициент вариации выборки

  auto chi2_pair = get_chi2(sample);  // Проводим хи-квадрат тест
  vector.push_back( chi2_pair.first  == -1 ? "N/A" : std::to_string(chi2_pair.first)  );  // Записываем результат или N/A в случае ошибки
  vector.push_back( chi2_pair.second == -1 ? "N/A" : std::to_string(chi2_pair.second) );  // Записываем количество степеней свободы или N/A в случае ошибки


  // Переводим вектор чисел в последовательность бит для проведения nist-тестов
  std::vector<bool> bit_sequence = sample_to_bit_sequence(sample);  

  double frequency = nist_frequency(bit_sequence);  // Проводим частотный побитовый тест
  vector.push_back( std::to_string(frequency) );              // Записываем полученное значение p_value
  vector.push_back( (frequency >= ALPHA) ? "PASS" : "FAIL" ); // Записываем результат теста

  double runs = nist_runs(bit_sequence);  // Проводим тест на последовательность одинаковых битов
  vector.push_back( std::to_string(runs) );               // Записываем полученное значение p_value
  vector.push_back( (runs >= ALPHA) ? "PASS" : "FAIL" );  // Записываем результат теста

  double serial = nist_serial(bit_sequence, SERIAL_M); // Проводим тест на периодичность
  vector.push_back( std::to_string(serial) );               // Записываем полученное значение p_value
  vector.push_back( std::to_string(SERIAL_M) );             // Записываем длину рассматриваемого блока
  vector.push_back( (serial >= ALPHA) ? "PASS" : "FAIL" );  // Записываем результат теста

  double apEntropy = nist_apEntropy(bit_sequence, AP_ENTROPY_M);  // Проводим тест приблизительной энтропии
  vector.push_back( std::to_string(apEntropy) );              // Записываем полученное значение p_value
  vector.push_back( std::to_string(AP_ENTROPY_M) );           // Записываем длину рассматриваемого блока
  vector.push_back( (apEntropy >= ALPHA) ? "PASS" : "FAIL" ); // Записываем результат теста

  double cusum = nist_cusum(bit_sequence);  // Проводим тест кумулятивных сумм
  vector.push_back( std::to_string(cusum) );              // Записываем полученное значение p_value
  vector.push_back( (cusum >= ALPHA) ? "PASS" : "FAIL" ); // Записываем результат теста


}


