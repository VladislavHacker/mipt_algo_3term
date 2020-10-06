#include <bitset>
#include <iostream>
#include <limits>
#include <cstdint>
#include <unordered_map>
#include <vector>

const int32_t ST_MAX = std::numeric_limits<int32_t>::max(); // max int32_t
const int32_t ALPHABET_SIZE = 26; // alphabet size
const int32_t TEXT_SIZE = 2000005; // max text size

/* Bohr vertex class with getters and setters */
class BohrVertex {
public:

    BohrVertex (int32_t parent, char symbol);

    const int32_t& getLetterChild (const char& symbol) const;

    void setNextLetterChild (const char& symbol, const int32_t& vertex);

    const int32_t& getSuffix () const;

    void setSuffix (const int32_t &suffix);

    const int32_t& getGoodSuffix () const;

    void setGoodSuffix (const int32_t& suffix);

    const int32_t& getParent () const;

    void setParent (const int32_t& parent);

    const int32_t& getAutomateMove (const char& symbol) const;

    void setAutomateMove (const char& symbol, int32_t vertex);

    const char& getSymbol () const;

    void setTrueFlag ();

    const bool& getFlag () const;

    const int32_t& getPatternNumber () const;

    void setPatternNumber (const int32_t& number);

private:

    std::vector<int32_t> children_;
    std::vector<int32_t> automate_move_;
    int32_t suffix_link_;
    int32_t good_suffix_link_;
    bool flag_;
    char symbol_;
    int32_t parent_;
    int32_t pattern_number_;

};

BohrVertex::BohrVertex (
    int32_t parent,
    char symbol
) :
        parent_(parent),
        symbol_(symbol),
        suffix_link_(ST_MAX),
        good_suffix_link_(ST_MAX),
        pattern_number_(ST_MAX),
        flag_(false)
{

    children_.assign(ALPHABET_SIZE, ST_MAX);
    automate_move_.assign(ALPHABET_SIZE, ST_MAX);

}

const int32_t& BohrVertex::getLetterChild (const char& symbol) const {
    return children_[symbol];
}

void BohrVertex::setNextLetterChild (const char& symbol, const int32_t& vertex) {
    children_[symbol] = vertex;
}

const int32_t& BohrVertex::getSuffix () const {
    return suffix_link_;
}

void BohrVertex::setSuffix (const int32_t &suffix) {
    suffix_link_ = suffix;
}

const int32_t& BohrVertex::getGoodSuffix () const {
    return good_suffix_link_;
}

void BohrVertex::setGoodSuffix (const int32_t &suffix) {
    good_suffix_link_ = suffix;
}


const int32_t& BohrVertex::getParent () const {
    return parent_;
}

void BohrVertex::setParent (const int32_t &parent) {
    parent_ = parent;
}

const int32_t& BohrVertex::getPatternNumber () const {
    return pattern_number_;
}

void BohrVertex::setPatternNumber (const int32_t &num) {
    pattern_number_ = num;
}

const int32_t& BohrVertex::getAutomateMove (const char& symbol) const {
    return automate_move_[symbol];
}

void BohrVertex::setAutomateMove (const char& symbol, int32_t vertex) {
    automate_move_[symbol] = vertex;
}

void BohrVertex::setTrueFlag () {
    flag_ = true;
}

const bool& BohrVertex::getFlag() const {
    return flag_;
}

const char& BohrVertex::getSymbol() const {
    return symbol_;
}

/* Bohr class for Aho-Korasik algorithm */
class Bohr {
public:

    Bohr ();

    const int32_t& next(const int32_t& vertex, const char& );

    void addVertex (const int32_t& parent, const char& );

    void addString (const std::string &s);

    const int32_t& getSuffixLink (const int32_t& vertex);

    const int32_t& getGoodSuffixLink (const int32_t& vertex);

    const int32_t& getAutomateMove (const int32_t& vertex, const char& i);

    void check (
        std::ostream &out,
        const std::string &s,
        const int32_t& v,
        const int32_t& i,
        std::bitset<2000005> &bits,
        std::unordered_map<std::string,  std::vector<int32_t>> &patterns_positions,
        std::vector<int32_t> &result,
        const int32_t& pattern_len
    );

    void findPositions (
        std::ostream &out,
        const std::string& s,
        std::unordered_map<std::string,  std::vector<int32_t>> &patterns_positions,
        std::vector<int32_t> &result,
        int32_t pattern_len
    );

private:

    std::vector<BohrVertex> bohr_;
    std::vector<std::string> patterns_;

};

Bohr::Bohr() {
    bohr_.emplace_back(0, 120);
}

/* create vertex */
void Bohr::addVertex (const int32_t& parent, const char& i) {
    bohr_.emplace_back(parent, i);
    bohr_[parent].setNextLetterChild(i, bohr_.size() - 1);
}

/* next vertex getter */
const int32_t& Bohr::next (const int32_t& vertex, const char& i) {
    return bohr_[vertex].getLetterChild(i);
}

/* add string to Bohr */
void Bohr::addString (const std::string &s) {
    int32_t num = 0;
    for (int32_t i = 0; i < s.length(); ++i) {
        const char& ch = s[i];
        char index = static_cast<char>(ch - 'a');
        if (next(num, index) == ST_MAX) {
            addVertex(num, index);
        }
        num = next(num, index);
    }

    patterns_.push_back(s);
    bohr_[num].setTrueFlag();
    bohr_[num].setPatternNumber(patterns_.size() - 1);
}

/* suffix link getter */
const int32_t& Bohr::getSuffixLink(const int32_t& vertex) {
    if (bohr_[vertex].getSuffix() == ST_MAX) {
        if (vertex == 0 || bohr_[vertex].getParent() == 0) {
            bohr_[vertex].setSuffix(0);
        } else {
            bohr_[vertex].setSuffix (
                getAutomateMove (
                    getSuffixLink(bohr_[vertex].getParent()),
                    bohr_[vertex].getSymbol()
                )
            );
        }
    }
    return bohr_[vertex].getSuffix();
}

/* compressed suffix link getter */
const int32_t& Bohr::getGoodSuffixLink(const int32_t& vertex) {
    if (bohr_[vertex].getGoodSuffix() == ST_MAX) {
        int32_t u = getSuffixLink(vertex);
        if (u == 0) {
            bohr_[vertex].setGoodSuffix(0);
        } else {
            bohr_[vertex].setGoodSuffix(
                bohr_[u].getFlag() ? u : getGoodSuffixLink(u)
            );
        }
    }
    return bohr_[vertex].getGoodSuffix();
}

/* automate step */
const int32_t& Bohr::getAutomateMove(const int32_t& vertex, const char& i) {
    if (bohr_[vertex].getAutomateMove(i) == ST_MAX) {
        if (bohr_[vertex].getLetterChild(i) != ST_MAX) {
            bohr_[vertex].setAutomateMove (
                i,
                bohr_[vertex].getLetterChild(i)
            );
        } else if (vertex == 0) {
            bohr_[vertex].setAutomateMove(i, 0);
        } else {
            bohr_[vertex].setAutomateMove(
                i,
                getAutomateMove(getSuffixLink(vertex), i)
            );
        }
    }
    return bohr_[vertex].getAutomateMove(i);
}

/* check positions and write to stream results */
void Bohr::check (
    std::ostream &out,
    const std::string &s,
    const int32_t& v,
    const int32_t& i,
    std::bitset<TEXT_SIZE> &bits,
    std::unordered_map<std::string,  std::vector<int32_t>> &patterns_positions,
    std::vector<int32_t> &result,
    const int32_t& pattern_len
) {
    int32_t u = v;
    int32_t patterns_count = patterns_.size();
    int32_t patterns_last = patterns_count - 1;
    int32_t s_len = s.length();
    while (u != 0) {
        if (bohr_[u].getFlag()) {
            const std::string& pat = patterns_[bohr_[u].getPatternNumber()];
            int32_t pos = i - pat.length() + 1;
            const std::vector<int32_t>& pat_pos = patterns_positions[pat];
            for (int32_t it = 0; it < pat_pos.size(); ++it) {
                const int32_t& my_p = pat_pos[it];
                if (pos >= my_p) {
                    int32_t index =  pos - my_p;
                    ++result[index];
                    if (
                        bohr_[u].getPatternNumber() == patterns_last &&
                        result[index] == patterns_count &&
                        index <= s_len - pattern_len && !bits.test(index)
                    ) {
                        out << index << " ";
                        bits.set(index);
                        result[index] = 0;
                    }
                }
            }
        }
        u = getGoodSuffixLink(u);
    }
    if (patterns_.empty() && i <= s_len - pattern_len) {
        out << i << " ";
    }
}

void Bohr::findPositions (
    std::ostream &out,
    const std::string &s,
    std::unordered_map<std::string, std::vector<int32_t>> &patterns_positions,
    std::vector<int32_t> &result,
    int32_t pattern_len
) {
    int32_t now = 0;
    std::bitset<TEXT_SIZE> bits;
    int32_t s_len = s.length();
    for (int32_t i = 0; i < s_len; ++i){
        now = getAutomateMove(now, static_cast<char>(s[i] - 'a'));
        check(out, s, now, i, bits, patterns_positions, result, pattern_len);
    }
}

void fillBohrWithPattern (
    Bohr& bohr,
    std::string &pattern,
    std::unordered_map<std::string, std::vector<int32_t>> &patterns_positions,
    bool &f
) {
    std::string current;
    int32_t pos = 0;

    if (pattern[pattern.length() - 1] != '?') {
        pattern += '?';
        f = true;
    }

    for (int32_t i = 0; i < pattern.size(); ++i) {
        const char &ch = pattern[i];
        if (ch != '?') {
            current += ch;
        } else if (!current.empty()){
            bohr.addString(current);
            patterns_positions[current].push_back(pos - current.length());
            current.clear();
        }
        ++pos;
    }

    if (!current.empty()) {
        bohr.addString(current);
        --pos;
        patterns_positions[current].push_back(pos - current.length());
        current.clear();
    }
}

void search (
    std::ostream &out,
    std::string &pattern,
    const std::string &s
) {

    Bohr bohr;
    std::unordered_map<std::string, std::vector<int32_t>> patterns_positions;
    bool f = false; // true if we have fictive '?' in the end

    fillBohrWithPattern(bohr, pattern, patterns_positions, f);

    std::vector<int32_t> result(s.length(), 0);
    bohr.findPositions(
        out,
        s,
        patterns_positions,
        result,
        pattern.length() - static_cast<int32_t>(f)
    );

}

void getInput (
    std::istream &in,
    std::string &pattern,
    std::string &s
) {
    in >> pattern >> s;
}

int main() {

    std::string pattern;
    std::string s;
    /* reserve space to increase performance (we needn't reallocate space) */
    s.reserve(TEXT_SIZE);
    pattern.reserve(5000);
    /* cin&cout optimization */
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    getInput(std::cin, pattern, s);
    search(std::cout, pattern, s);

    return 0;

}
