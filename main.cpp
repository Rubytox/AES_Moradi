#include <iostream>
#include <array>
#include <cstdint>
#include <Windows.h>

#include "AES.hpp"

#pragma execution_character_set( "utf-8" )

using namespace std;

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

    return 0;
}