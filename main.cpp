#include <iostream>
#include <iomanip>
#include <array>
#include <cstdint>
#include <Windows.h>

#include "AES.hpp"

#pragma execution_character_set( "utf-8" )

using namespace std;

void printState(const state128_t &state)
{
    cout << hex;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << "0x" << setfill('0') << setw(2) << (int) state[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);


    state128_t masterKey { { { { 0, 1, 2, 3 } },
                             { { 4, 5, 6, 7 } },
                             { { 8, 9, 10, 11 } },
                             { { 12, 13, 14, 15 } } } };
    AES enc(masterKey);
    cout << enc.printRoundKeys() << endl;

    state128_t plaintext { { { { 0, 1, 2, 3 } },
                             { { 4, 5, 6, 7 } },
                             { { 8, 9, 10, 11 } },
                             { { 12, 13, 14, 15 } } } };
    cout << "Plaintext:" << endl;
    printState(plaintext);

    state128_t ciphered = enc.cipher(plaintext);
    cout << "Ciphered:" << endl;
    printState(ciphered);


    return 0;
}