#include"main.h"

namespace {
    // Заданный алфавит
    char number_to_char(const main_type& symbol) {
        if (symbol <= 9)
            return (char) (symbol + 48);
        if (symbol <= 35)
            return (char) (symbol + 55);
        if (symbol <= 61)
            return (char) (symbol + 61);
        if (symbol == 62)
            return 32;
        if (symbol == 63)
            return 46;
        return '\n';
    }
    // Алгоритм быстрого возведения в степень чисел
    main_type fast_degree(const main_type& number, const main_type& degree, const main_type& mod) {
        if (degree == 0) {
            return 1;
        }
        if (degree % 2 == 1) {
            return (fast_degree(number, degree - 1, mod) * number) % mod;
        }
        main_type root = fast_degree(number, degree >> 1, mod);
        return (root * root) % mod;
    }
    // Сложение большого числа и малого числа в заданной системе счисления
    void add_up_numeral_system(std::vector<main_type>& number, const main_type& base, main_type& plus) {
        main_type temp = 0;
        std::vector<main_type> memory;
        while (plus > 0) {
            memory.push_back(plus % base);
            plus /= base;
        }
        for (index_type i = 0; i < std::max(number.size(), memory.size()) || temp; ++i) {
            if (i == number.size()) {
                number.push_back(0);
            }
            number[i] += temp + (i < memory.size() ? memory[i] : 0);
            temp = number[i] >= base;
            if (temp) {
                number[i] -= base;
            }
        }
    }
    // Умножение большого числа на основание старой системы счисления
    void multiply_numeral_system(std::vector<main_type>& number, const main_type& new_base, const main_type& base) {
        main_type temp = 0;
        main_type memory;
        for (index_type i = 0; i < number.size() || temp; ++i) {
            if (i == number.size()) {
                number.push_back(0);
            }
            memory = temp + number[i] * base;
            temp = memory / new_base;
            number[i] = memory - new_base * temp;
        }
    }
    // Получение многочлена заданного строкой
    std::vector<main_type> string_to_polynomial(std::string& input) {
        std::stringstream text(input);
        std::string number;
        std::vector<main_type> output;
        while (std::getline(text, number, ' ')) {
            output.push_back(std::stoi(number));
        }
        return output;
    }
    // Нормализация многочлена, чтобы не было ни отрицательных членов, ни членов выше заданного mod
    void normalise_polynomial(std::vector<main_type>& number, const main_type& base,
                              const std::vector<main_type>& polynomial) {
        while (number.size() >= polynomial.size()) {
            main_type multiplier = number.back();
            index_type number_size = number.size();
            index_type polynomial_size = polynomial.size();
            for (index_type i = 1; i <= polynomial_size; ++i) {
                number[number_size - i] = (number[number_size - i] - polynomial[polynomial_size - i] * multiplier) % base;
                if (number[number_size - i] < 0) {
                    number[number_size - i] += base;
                }
            }
            while (number.size() > 1 && number.back() == 0) {
                number.pop_back();
            }
        }
    }
    // Умножение двух многочленов
    std::vector<main_type> multiply_polynomials(const std::vector<main_type>& left, const std::vector<main_type>& right,
                                                const main_type& base, const std::vector<main_type>& polynomial) {
        std::vector<main_type> result(left.size() + right.size());
        for (index_type i = 0; i < left.size(); ++i) {
            for (index_type j = 0; j < right.size(); ++j) {
                result[i + j] = (result[i + j] + left[i] * right[j]) % base;
            }
        }
        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }
        normalise_polynomial(result, base, polynomial);
        return result;
    }
    // Алгоритм быстрого возведения многочлена в степень
    std::vector<main_type> polynomial_fast_degree(const std::vector<main_type>& number, const main_type& degree,
                                                  const main_type& mod, const std::vector<main_type>& polynomial) {
        if (degree == 0) {
            return {1};
        }
        if (degree % 2 == 1) {
            std::vector<main_type> temp = polynomial_fast_degree(number, degree - 1, mod, polynomial);
            return multiply_polynomials(temp, number, mod, polynomial);
        }
        std::vector<main_type> root = polynomial_fast_degree(number, degree >> 1, mod, polynomial);
        return multiply_polynomials(root, root, mod, polynomial);
    }
    // Ввод шифра и его дешифровка в большое число в заданной системе счисления
    std::vector<main_type> input_cipher(const std::vector<main_type>& polynomial, const main_type& mod,
                                        const main_type& private_key) {
        std::string input;
        const main_type polynomial_size = (main_type)polynomial.size() - 1;
        const main_type edge = 1e18;
        const main_type ord = fast_degree(mod, polynomial_size, edge) - 2;
        std::vector<main_type> message;
        while (std::getline(std::cin, input, '\n') && !input.empty()) {
            std::vector<main_type> input_1 = string_to_polynomial(input);
            std::getline(std::cin, input, '\n');
            std::vector<main_type> input_2 = string_to_polynomial(input);
            input_1 = polynomial_fast_degree(input_1, private_key, mod, polynomial);
            input_1 = polynomial_fast_degree(input_1, ord, mod, polynomial);
            input_1 = multiply_polynomials(input_2, input_1, mod, polynomial);
            while (input_1.size() < polynomial_size) {
                input_1.push_back(0);
            }
            message.insert(message.end(), input_1.begin(), input_1.end());
        }
        return message;
    }
    // Перевод числа из одной системы счисления в другую
    std::vector<main_type> translation_numeral_system(std::vector<main_type>& message, const main_type& base,
                                                      const main_type& mod) {
        std::vector<main_type> encrypted(1);
        for (index_type i = message.size(); i >= 1; --i) {
            multiply_numeral_system(encrypted, base, mod);
            add_up_numeral_system(encrypted, base, message[i - 1]);
        }
        return encrypted;
    }
    // Вывод полученного сообщения
    void output_message(const std::vector<main_type>& encrypted) {
        for (main_type i : encrypted) {
            std::cout << number_to_char(i);
        }
    }
}

int main() {
    // Задаём основание для системы счисления, соответствующей алфавиту из условия
    const main_type base = 64;
    // Приём соответственно модуля поля, неприводимого многочлена и приватного ключа
    std::string input;
    std::getline(std::cin, input);
    const main_type mod = std::stoi(input);
    std::getline(std::cin, input);
    std::vector<main_type> polynomial = string_to_polynomial(input);
    std::getline(std::cin, input);
    const main_type private_key = std::stoi(input);
    // Приём передаваемого шифра и расшифровка по схеме Эль-Гамаля
    std::vector<main_type> message = input_cipher(polynomial, mod, private_key);
    // Перевод полученного шифра из заданной системы счисления в систему счисления с имеющимся алфавитом
    std::vector<main_type> encrypted = translation_numeral_system(message, base, mod);
    // Вывод передаваемого сообщения
    output_message(encrypted);
    return 0;
}
