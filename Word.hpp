#pragma once

#include <cstdint>
#include <array>
#include <functional>
#include <algorithm>

class Word
{
public:
    Word(uint8_t b0 = 0, uint8_t b1 = 0, uint8_t b2 = 0, uint8_t b3 = 0);
    Word(const Word& word);
    Word(const std::array<uint8_t, 4> &word);

    Word& operator^=(const Word& other);
    friend Word operator^(Word lhs, const Word& rhs);

    Word& shiftLeft(int i);

    Word& map(std::function<void(uint8_t&)> f);

    std::array<uint8_t, 4> getRawWord() const;

private:
    std::array<uint8_t, 4> _word;

};

inline Word operator^(Word lhs, const Word& rhs)
{
    lhs ^= rhs;
    return lhs;
}