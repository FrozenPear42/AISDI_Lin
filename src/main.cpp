#include <cstddef>
#include <cstdlib>
#include <string>
#include <random>
#include "../CODEine/benchmark.h"
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
    //prepend<Vector<int>>(12222);
    //append<Vector<int>>(12222);
    //prepend<List<int>>(12222);
    //append<List<int>>(12222);

/*    std::cout << "Append" << std::endl;

    bmk::benchmark<> append_bm;
    append_bm.run("Vector", 10, append<Vector<int>>, "Elements",
                  {100, 1000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000});
    append_bm.run("List", 10, append<List<int>>, "Elements",
                  {100, 1000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000});
    append_bm.serialize("Append", "append.txt");

    std::cout << "Prepend" << std::endl;

    bmk::benchmark<> prepend_bm;
    prepend_bm.run("Vector", 1, prepend<Vector<int>>, "Elements",
                   {100, 1000, 10000, 50000, 100000, 500000, 1000000});
    prepend_bm.run("List", 1, prepend<List<int>>, "Elements",
                   {100, 1000, 10000, 50000, 100000, 500000, 1000000});
    prepend_bm.serialize("Prepend", "prepend.txt");

    std::cout << "PopFirst" << std::endl;

    bmk::benchmark<> pop_first_bm;
    pop_first_bm.run("Vector", 1, popFirst<Vector<int>>, "Elements",
                     {100, 1000, 10000, 50000, 100000, 500000, 1000000});
    pop_first_bm.run("List", 1, popFirst<List<int>>, "Elements",
                     {100, 1000, 10000, 50000, 100000, 500000, 1000000});
    pop_first_bm.serialize("PopFirst", "popfirst.txt");

    std::cout << "Random" << std::endl;
*/
    bmk::benchmark<> random_insert_bm;
    random_insert_bm.run("Vector", 1, randomInsert<Vector<int>>, "Elements",
                         {100, 1000, 10000, 20000, 40000, 50000, 100000});
    random_insert_bm.run("List", 1, randomInsert<List<int>>, "Elements",
                         {100, 1000, 10000, 20000, 40000, 50000, 100000});
    random_insert_bm.serialize("RandomInsert", "randominsert.txt");

    return 0;
}
