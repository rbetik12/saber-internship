#include <iostream>
#include <functional>
#include <chrono>
#include "List.h"

void Test3(List& list) {
    list.Add("head", nullptr);
    
    list.AddRand(0, list.head);
}

void Test2(List& list) {
    list.Add("head", nullptr);
    list.Add("second", nullptr);
    list.Add("third", nullptr);
    list.Add("tail", nullptr);
}

void Test1(List& list) {
    list.Add("head", nullptr);
    list.Add("tail", nullptr);
}

void Test0(List& list) {
    list.Add("head", nullptr);
    list.Add("second", nullptr);
    list.Add("third", list.head);
    list.Add("tail", nullptr);

    list.AddRand(3, list.head);
    list.AddRand(0, list.tail);
}

#define TESTS_AMOUNT 4

int main() {
    List list;
    List deserializedList;
    FILE* file = nullptr;
    std::function<void(List&)> tests[TESTS_AMOUNT];
    tests[0] = Test0;
    tests[1] = Test1;
    tests[2] = Test2;
    tests[3] = Test3;

    for (int i = 0; i < TESTS_AMOUNT; i++) {
        std::cout << "====================================Test " << i << "====================================" << std::endl;
        list.Clear();
        deserializedList.Clear();
        tests[i](list);

        file = fopen("list.bin", "wb");
        if (file == nullptr) {
            std::cout << "Can't open file to write" << std::endl;
            exit(EXIT_FAILURE);
        }

        list.Serialize(file);
        fclose(file);

        std::cout << "Serialized list:" << std::endl;
        list.Print();

        file = fopen("list.bin", "rb");
        if (file == nullptr) {
            std::cout << "Can't open file to read" << std::endl;
            exit(EXIT_FAILURE);
        }

        deserializedList.Deserialize(file);
        std::cout << "Deserialized list:" << std::endl;
        deserializedList.Print();
        fclose(file);
    }
    return 0;
}