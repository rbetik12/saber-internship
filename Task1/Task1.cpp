#include <iostream>

// 2**64 -> 1 on 63-rd position
#define ULONG_MAX 9223372036854775808

void ToBinary(long long number) {
    uint64_t unumber = number;
    uint8_t bitCount = 0;
    for (uint64_t i = ULONG_MAX; i != 0; i = i >> 1) {
        std::cout << (char)((number & i) ? '1' : '0');

        bitCount += 1;
        if (bitCount % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

int main() {
    long long number = 0;
    std::cin >> number;
    ToBinary(number);
    return 0;
}
