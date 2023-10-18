#include<iostream>
#include<algorithm>
#include<sstream>

std::vector<int64_t> poly;
int64_t edge = 1e18;

char number_to_char(uint64_t symbol) {
    if (symbol <= 9)
        return (char)(symbol + 48);
    if (symbol <= 35)
        return (char)(symbol + 55);
    if (symbol <= 61)
        return (char)(symbol + 61);
    if (symbol == 62)
        return 32;
    if (symbol == 63)
        return 46;
    return '\n';
}

int64_t fast_degree(const int64_t& num, const int64_t& deg, const int64_t& module) {
    if (deg == 0) {
        return 1;
    }
    if (deg % 2 == 1) {
        return (fast_degree(num, deg - 1, module) * num) % module;
    }
    int64_t ans = fast_degree(num, deg >> 1,  module);
    return (ans * ans) % module;
}

void numeral_system_plus(std::vector<int64_t>& number, int64_t& base, int64_t plus) {
    std::vector<int64_t> mem;
    int64_t temp = 0;
    while (plus > 0) {
        mem.push_back(plus % base);
        plus = plus / base;
    }
    for (size_t i = 0; i < std::max(number.size(), mem.size()) || temp; ++i) {
        if (i == number.size()) {
            number.push_back(0);
        }
        number[i] += temp + (i < mem.size() ? mem[i] : 0);
        temp = number[i] >= base;
        if (temp)  {
            number[i] -= base;
        }
    }
}

void numeral_system_mult(std::vector<int64_t>& number, const int64_t& new_base, const int64_t base) {
    int64_t temp = 0;
    int64_t num;
    for (size_t i = 0; i < number.size() || temp; ++i) {
        if (i == number.size()) {
            number.push_back(0);
        }
        num = temp + number[i] * base;
        temp = num / new_base;
        number[i] = num - new_base * temp;
    }
}

void string_to_polynom(std::string& input, std::vector<int64_t>& vec) {
    std::stringstream in(input);
    std::string num;
    while (std::getline(in, num, ' ')) {
        vec.push_back(std::stoi(num));
    }
}

void poly_normalise(std::vector<int64_t>& num, const int64_t base) {
    while (num.size() >= poly.size()) {
        int64_t mult = num.back();
        size_t n = num.size();
        size_t m = poly.size();
        for (size_t i = 1; i <= m; ++i) {
            num[n - i] = (num[n - i] - poly[m - i] * mult) % base;
            if (num[n - i] < 0) {
                num[n - i] += base;
            }
        }
        while (num.size() > 1 && num.back() == 0) {
            num.pop_back();
        }
    }
}

std::vector<int64_t> poly_mult(std::vector<int64_t>& left, std::vector<int64_t>& right, const int64_t& base) {
    std::vector<int64_t> result(left.size() + right.size(), 0);
    for (int i = 0; i < left.size(); ++i) {
        for (int j = 0; j < right.size(); ++j) {
            result[i + j] = (result[i + j] + left[i] * right[j]) % base;
        }
    }
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    poly_normalise(result, base);
    return result;
}

std::vector<int64_t> poly_fast_degree(std::vector<int64_t>& num, const int64_t deg, const int64_t& module) {
    if (deg == 0) {
        std::vector<int64_t> temp(1, 1);
        return temp;
    }
    if (deg % 2 == 1) {
        std::vector<int64_t> mem = num;
        std::vector<int64_t> temp = poly_fast_degree(num, deg - 1, module);
        return poly_mult(temp, mem, module);
    }
    std::vector<int64_t> ans = poly_fast_degree(num, deg >> 1,  module);
    return poly_mult(ans, ans, module);
}

int main() {
    int64_t mod, base = 64;
    int64_t private_key;
    std::vector<int64_t> inp_1, inp_2;
    std::string input;
    std::getline(std::cin, input);
    mod = std::stoi(input);
    std::getline(std::cin, input);
    string_to_polynom(input, poly);
    std::getline(std::cin, input);
    private_key = std::stoi(input);
    int64_t n = poly.size() - 1;
    std::vector<int64_t> message;
    while (std::getline(std::cin, input, '\n') && !input.empty()) {
        string_to_polynom(input, inp_1);
        std::getline(std::cin, input, '\n');
        string_to_polynom(input, inp_2);
        inp_1 = poly_fast_degree(inp_1, private_key, mod);
        inp_1 = poly_fast_degree(inp_1, fast_degree(mod, n, edge) - 2, mod);
        inp_1 = poly_mult(inp_2, inp_1, mod);
        while (inp_1.size() < n) {
            inp_1.push_back(0);
        }
        message.insert(message.end(), inp_1.begin(), inp_1.end());
        inp_1.clear();
        inp_2.clear();
    }
    std::vector<int64_t> encrypted(1, 0);
    for (uint64_t i = message.size(); i >= 1; --i) {
        numeral_system_mult(encrypted, base, mod);
        numeral_system_plus(encrypted, base, message[i - 1]);
    }
    for (int64_t i : encrypted) {
        std::cout << number_to_char(i);
    }
}
