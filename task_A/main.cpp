#include <iostream>
#include <vector>
#include <string>

std::vector<size_t> zFunction (const std::string &s) {
    size_t n = s.length();
    std::vector<size_t> z (n, 0);
    size_t left = 0;
    size_t right = 0;

    for (size_t i = 1; i < n; ++i) {
        if (i <= right) {
            z[i] = std::min(right - i + 1, z[i - left]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > right) {
            left = i;
            right = i + z[i] - 1;
        }
    }
    return z;
}

void findPositions (
    std::vector <size_t> &result,
    std::string &find_template,
    std::string &text
) {
    std::string z_str = find_template + '#' + text;
    std::vector<size_t> z = zFunction(z_str);
    for (size_t i = find_template.length(); i < z_str.length(); ++i) {
        if (z[i] == find_template.length()) {
            result.push_back(i - find_template.length() - 1);
        }
    }
}

void getInput (
    std::istream &in,
    std::string &find_template,
    std::string &text
) {
    in >> find_template;
    in >> text;
}

void setOutput (
    std::ostream &out,
    std::vector<size_t> &positions
) {
     for (auto& pos : positions) {
         out << pos << " ";
     }
}

int main() {

    std::string s, p;
    getInput(std::cin, s, p);

    std::vector <size_t> result;
    findPositions(result, s, p);

    setOutput(std::cout, result);

    return 0;
}
