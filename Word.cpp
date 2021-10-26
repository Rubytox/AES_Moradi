#include "Word.hpp"

using namespace std;

Word::Word(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) : _word {{ b0, b1, b2, b3 }}
{}

Word::Word(const Word& word)
{
    for (int i = 0; i < 4; i++)
        _word[i] = word._word[i];
}

Word::Word(const array<uint8_t, 4> &word) : Word(word[0], word[1], word[2], word[3])
{}

Word& Word::operator^=(const Word& other)
{
    _word[0] ^= other._word[0];
    _word[1] ^= other._word[1];
    _word[2] ^= other._word[2];
    _word[3] ^= other._word[3];

    return *this;
}

uint8_t true_modulo(uint8_t a, uint8_t b)
{
    return (b + (a % b)) % b;
}

Word& Word::shiftLeft(int i)
{
    rotate(_word.begin(), _word.begin() + i, _word.end());

    return *this;
}

Word& Word::map(function<void(uint8_t&)> f)
{
    for (auto &el : _word)
        f(el);

    return *this;
}

array<uint8_t, 4> Word::getRawWord() const
{
    return _word;
}