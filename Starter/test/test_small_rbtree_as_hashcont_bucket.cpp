
#include <chrono>
#include <algorithm>
#include <vector>
#include "bwtest/bwtest.h"
#include "HashContainer.h"
#include "TreeMap.h"

using namespace std;
using namespace Starter;

TEST(inti, 1)
{
    cout << boolalpha;
    cout << PrimeTable::isPrime(18446744073709551556ul) << endl;
    cout << sizeof(PrimeTable::PrimeNumbers) << endl;
    for(auto i : PrimeTable::primeBetween(4355707ul, 6456007ul)) {
        cout << i << endl;
    }
    cout << PrimeTable::nextPrime(4611686018427387847ul) << endl;

//    typedef detail_::MapNode<int, int>     NN;
//
//    cout << sizeof(detail_::HashNodeTree<detail_::MapNode<int, int>>) << sizeof(int*)<< endl;
//    detail_::HashNodeTree<detail_::MapNode<int, int>>  hrbt;
//    hrbt.exclusiveInsertion(NN(1, 1));
}

int main() {
    RUN_ALL();
    return 0;
}
