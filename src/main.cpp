#include <cstddef>
#include <cstdlib>
#include <string>
#include <random>
#include "Vector.h"
#include "LinkedList.h"
#include <iostream>

namespace {

    template<typename T>
    using List = aisdi::LinkedList<T>;

    template<typename T>
    using Vector = aisdi::Vector<T>;

}

template<typename Collection>
void append(int pCount) {
    std::cout << pCount << std::endl;
    Collection col;
    for (int i = 0; i < pCount; i++) {
        col.append(i);
    }
}

template<typename Collection>
void prepend(int pCount) {
    std::cout << pCount << std::endl;
    Collection col;
    for (int i = 0; i < pCount; i++) {
        col.prepend(i);
    }
}

template<typename Collection>
void popFirst(int pCount) {
    std::cout << pCount << std::endl;
    Collection col;

    for (int i = 0; i < pCount; i++) {
        col.append(i);
    }
    for (int i = 0; i < pCount; i++) {
        col.popFirst();
    }
}


template<typename Collection>
void randomInsert(int pCount) {
    std::cout << pCount << std::endl;
    Collection collection;
    std::random_device rand;
    std::default_random_engine eng(rand());
    std::uniform_int_distribution<int> distr(0, pCount);

    for (int i = 0; i < pCount; i++) {
        int val = distr(eng);
        auto it = collection.begin() + val;
        collection.insert(it, i);
    }
}

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    auto start = clock();
    prepend<Vector<int>>(100000);
    auto end = clock();
    std::cout << "Prepend Vector: " << ((float) (end - start)) / CLOCKS_PER_SEC << std::endl;


    start = clock();
    prepend<List<int>>(100000);
    end = clock();
    std::cout << "Prepend List: " << ((float) (end - start)) / CLOCKS_PER_SEC << std::endl;

    start = clock();
    randomInsert<Vector<int>>(100000);
    end = clock();
    std::cout << "RandomInsert Vector: " << ((float) (end - start)) / CLOCKS_PER_SEC << std::endl;


    start = clock();
    randomInsert<List<int>>(100000);
    end = clock();
    std::cout << "RandomInsert List: " << ((float) (end - start)) / CLOCKS_PER_SEC << std::endl;


    return 0;
}
