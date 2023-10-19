#pragma once

#include<iostream>
#include<algorithm>
#include<sstream>

namespace {
    // Задание псевдонимов для основного используемого типа и типа для индексов в циклах
    using index_type = size_t;
    using main_type = int64_t;
    // Заданный алфавит
    char number_to_char(const main_type& symbol);
    // Алгоритм быстрого возведения в степень чисел
    main_type fast_degree(const main_type& number, const main_type& degree, const main_type& mod);
    // Сложение большого числа и малого числа в заданной системе счисления
    void add_up_numeral_system(std::vector<main_type>& number, const main_type& base, main_type& plus);
    // Умножение большого числа на основание старой системы счисления
    void multiply_numeral_system(std::vector<main_type>& number, const main_type& new_base, const main_type& base);
    // Получение многочлена заданного строкой
    std::vector<main_type> string_to_polynomial(std::string& input);
    // Нормализация многочлена, чтобы не было ни отрицательных членов, ни членов выше заданного mod
    void normalise_polynomial(std::vector<main_type>& number, const main_type& base,
                              const std::vector<main_type>& polynomial);
    // Умножение двух многочленов
    std::vector<main_type> multiply_polynomials(const std::vector<main_type>& left, const std::vector<main_type>& right,
                                                const main_type& base, const std::vector<main_type>& polynomial);
    // Алгоритм быстрого возведения многочлена в степень
    std::vector<main_type> polynomial_fast_degree(const std::vector<main_type>& number, const main_type& degree,
                                                  const main_type& mod, const std::vector<main_type>& polynomial);
    // Ввод шифра и его дешифровка в большое число в заданной системе счисления
    std::vector<main_type> input_cipher(const std::vector<main_type>& polynomial, const main_type& mod,
                                        const main_type& private_key);
    // Перевод числа из одной системы счисления в другую
    std::vector<main_type> translation_numeral_system(std::vector<main_type>& message, const main_type& base,
                                                      const main_type& mod);
    // Вывод полученного сообщения
    void output_message(const std::vector<main_type>& encrypted);
}