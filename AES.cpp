#include "AES.hpp"

using namespace std;

AES::AES(state128_t key): _masterKey(key)
{
    // Build rcons
    _rcons.push_back(1);
    for (int i = 1; i < 10; i++) {
        uint8_t latest = _rcons[i - 1];
        if (latest < 0x80)
            _rcons.push_back(2 * latest);
        else
            _rcons.push_back((2 * latest) ^ 0x11B);
    }

    KeySchedule();
}

state128_t AES::cipher(state128_t plaintext)
{
    _state = plaintext;

    AddRoundKey(0);
    for (int i = 1; i < 10; i++) {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(i);
    }
    SubBytes();
    ShiftRows();
    AddRoundKey(10);

    return _state;
}

void AES::AddRoundKey(int round_num)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _state[i][j] ^= _roundKeys[round_num][i][j];
        }
    }
}

void AES::SubBytes()
{
    for_each(_state.begin(), _state.end(), [this](array<uint8_t, 4> &column) {
        for_each(column.begin(), column.end(), [this](uint8_t &value) {
            value = S_BOX[value];
        });
    });
}

void AES::ShiftRows()
{
    // If _state is lines then columns we can use the shift
    int i = 0;
    for (auto &line : _state) {
        Word tmp(line);
        tmp.shiftLeft(i++);
        line = tmp.getRawWord();
    }
}

void AES::MixColumns()
{
    for (int j = 0; j < 4; j++) {
        array<uint8_t, 4> column { 
            _state[0][j], 
            _state[1][j], 
            _state[2][j], 
            _state[3][j] 
        };
        _state[0][j] = 2 * column[0] + 3 * column[1] + 1 * column[2] + 1 * column[3];
        _state[1][j] = 1 * column[0] + 2 * column[1] + 3 * column[2] + 1 * column[3];
        _state[2][j] = 1 * column[0] + 1 * column[1] + 2 * column[2] + 3 * column[3];
        _state[3][j] = 3 * column[0] + 1 * column[1] + 1 * column[2] + 2 * column[3];
    }
}

Word& AES::RotWord(Word& word)
{
    return word.shiftLeft(1);
}

Word& AES::RotWord(Word&& word)
{
    return RotWord(word);
}

Word& AES::SubWord(Word& word)
{
    return word.map([this](uint8_t &el) { el = S_BOX[el]; });
}


void AES::KeySchedule()
{
    int R = 11;
    int N = 4;

    vector<Word> flatRoundKeys;
    for (int i = 0; i < 4 * R; i++) {
        if (i < N)
            flatRoundKeys.push_back(Word(_masterKey[i]));
        else if (i >= N && i % N == 0)
            flatRoundKeys.push_back(
                flatRoundKeys[i - N]
                ^ SubWord(RotWord(Word(flatRoundKeys[i - 1])))
                ^ Word(_rcons[i / N - 1])
            );
        else
            flatRoundKeys.push_back(flatRoundKeys[i - N] ^ flatRoundKeys[i - 1]);
        
        // If next i == 0[4] then we just finished a word
        if ((i + 1) % 4 == 0) {
            state128_t roundKey;
            roundKey[0] = flatRoundKeys[i-3].getRawWord();
            roundKey[1] = flatRoundKeys[i-2].getRawWord();
            roundKey[2] = flatRoundKeys[i-1].getRawWord();
            roundKey[3] = flatRoundKeys[i].getRawWord();
            _roundKeys.push_back(roundKey);
        }
    }
}

template<typename T>
string int_to_hex(T i)
{
    stringstream stream;
    stream << "0x"
           << std::setfill('0') << std::setw(sizeof(T) * 2)
           << std::hex << (int)i;
    return stream.str();
}

string AES::printKey(const state128_t& key)
{
    stringstream stream;
    for (const auto &line : key) {
        for (const auto element : line) {
            stream << int_to_hex(element) << " ";
        }
    }
    stream << endl;
    return stream.str();
}

string AES::printRoundKeys() const
{
    stringstream stream;
    int index = 0;
    for (const auto &key : _roundKeys) {
        stream << "Round key n°" << index++
               << ": " << printKey(key)
               << endl;
    }
    return stream.str();
}