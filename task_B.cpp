#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

std::vector <int64_t> getOddVector (const std::string &s) {
    
    std::vector <int64_t> v(s.size(), 0);
    size_t n = s.size();
    int64_t l = 0;
    int64_t r = -1;
    
    for (int64_t i = 0; i < n; ++i) {
        int64_t k;
        if (i > r) {
            k = 1;
        } else {
            k = std::min(v[l + r - i], r - i + 1);
        }
        while (i + k < n && i - k >= 0 && s[i + k] == s[i - k]) {
            ++k;
        }
        v[i] = k;
        if (i + k - 1 > r) {
            l = i - k + 1;
            r = i + k - 1;
        }
    }
    
    return v;
    
}

std::vector <int64_t> getEvenVector (const std::string &s) {
    
    std::vector <int64_t> v(s.size(), 0);
    size_t n = s.size();
    int64_t l = 0;
    int64_t r = -1;
    
    for (int64_t i = 0; i < n; ++i) {
        int64_t k;
        if (i > r) {
            k = 0;
        } else {
            k = std::min(v[l + r - i + 1], r - i + 1);
        }
        while (i + k < n && i - k - 1 >= 0 && s[i + k] == s[i - k - 1]) {
            ++k;
        }
        v[i] = k;
        if (i + k - 1 > r) {
            l = i - k;
            r = i + k - 1;
        }
    }
    
    return v;
    
}

size_t manakerAlgorithm (const std::string &s) {
    
    std::vector <int64_t> d1 = getOddVector(s);
    std::vector <int64_t> d2 = getEvenVector(s);
    size_t result = 0;
    
    for (size_t i = 0; i < s.size(); ++i) {
        result += d1[i] + d2[i];
    }
    
    return result - s.size();
    
}

void getInput (
    std::istream &in,
    std::string &s
) {
    in >> s;
}

void setOutput (
    std::ostream &out,
    const size_t &result
) {
    out  << result;
}

int main() {
    
    std::string s;
    getInput(std::cin, s);

    setOutput(std::cout, manakerAlgorithm(s));
    
    return 0;
}
