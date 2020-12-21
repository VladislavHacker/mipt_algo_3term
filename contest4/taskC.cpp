#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cstdint>

int64_t mex(std::vector<size_t> &arr) {
    std::set<int64_t> b(arr.begin(), arr.end());
    for (int64_t i = 0; ; ++i) {
        if (!b.count(i)) {
            return i;
        }
    }
}

void fillGrandiBase(std::vector<int64_t> &grandi, int64_t n) {
    grandi[0] = 0;
    grandi[1] = 0;
    grandi[2] = 1;
    if (n > 2) {
        grandi[3] = 2;
    }
}

int64_t calculateGrandi(std::vector<int64_t> &grandi, int64_t n) {
    if (grandi[n] >= 0) {
        return grandi[n];
    }
    std::vector<size_t> mex_in;
    if (n >= 3) {
        mex_in.push_back(calculateGrandi(grandi, n - 1));
    }
    if (n >= 4) {
        mex_in.push_back(calculateGrandi(grandi, n - 2));
    }
    if (n >= 5) {
        mex_in.push_back(calculateGrandi(grandi, n - 1));
        for (int64_t i = 2; i <= n - 3; ++i) {
            mex_in.push_back(calculateGrandi(grandi, i) ^ calculateGrandi(grandi, n - i - 1));
        }
    }
    grandi[n] = mex(mex_in);
    return grandi[n];
}

std::vector<int64_t> getWinSteps(std::vector<int64_t> &grandi, int64_t n) {
    if (n == 2) {
        return {1, 2};
    }
    if (n == 3) {
        return {2};
    }
    std::vector<int64_t> steps;
    if (grandi[n - 1] == 0) {
        steps.push_back(1);
    }
    if (grandi[n - 2] == 0) {
        steps.push_back(2);
    }
    for (int64_t i = 2; i <= n - 3; ++i) {
        if ((grandi[i] ^ grandi[n - i - 1]) == 0) {
            steps.push_back(i + 1);
        }
    }
    if (grandi[n - 1] == 0) {
        steps.push_back(n);
    }
    if (grandi[n - 2] == 0) {
        steps.push_back(n - 1);
    }
    return steps;
}

void getInput(std::istream& in, int64_t& n) {
    in >> n;
}

void setOutput(
    std::ostream& out,
    const std::string& winner,
    const std::vector<int64_t>& steps
) {
    out << winner << std::endl;
    for (int64_t step : steps) {
        out << step << std::endl;
    }
}

bool isSpecialCase(int64_t n) {
    return n == 2 || n == 3;
}

int main() {
    int64_t n;
    getInput(std::cin, n);

    std::vector<int64_t> grandi(n + 1, -1);
    fillGrandiBase(grandi, n);

    if (!isSpecialCase(n) && calculateGrandi(grandi, n) == 0) {
        setOutput(std::cout, "Mueller", {});
    } else {
        setOutput(std::cout, "Schtirlitz", getWinSteps(grandi, n));
    }

    return 0;
}
