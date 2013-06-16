


#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <cstddef>
#include <cstdint>

#include "bwtest/bwtest.h"
#include "Algos.h"

using namespace std;
//
//partial_sort()
TEST(misc, 1) {
    std::vector<int> v{5, 6, 4, 3, 2, 6, 7, 9, 3};
std::vector<int> v2{5, 6, 4, 3, 2, 6, 7, 9, 3};
//    std::nth_element(v.begin(), v.begin() + v.size()/2, v.end());
//    std::cout << "The median is " << v[v.size()/2] << '\n';
    std::partial_sort(v.begin(), v.begin() + v.size()/2, v.end());
   // std::nth_element(v.begin(), v.begin()+1, v.end(), std::greater<int>());
   // std::cout << "The second largest element is " << v[1] << '\n';
    Starter::applyOn(v.begin(), v.end(), [](int a)->void{cout << a << "  " ;});


    cout << "\n\n";
        Starter::partialSort(v2.begin(), v2.end(), 4, [](const int& a, const int& b)->bool{return a < b;});
   // std::nth_element(v.begin(), v.begin()+1, v.end(), std::greater<int>());
   // std::cout << "The second largest element is " << v[1] << '\n';
    Starter::applyOn(v2.begin(), v2.end(), [](int a)->void{cout << a << "  ";});
}

TEST(part, 1) 
{
        std::vector<uint64_t> vec1, vec2;

    const uint32_t LEN = (2<12);
    srand((unsigned)time(0));

    for(uint32_t i = 0; i < LEN; ++i) {
        uint64_t t = rand() * rand();
        vec1.push_back(t);
        vec2.push_back(t);
    }

    auto less = [](const uint64_t& a)->bool{return a < 2222222;};

        auto i1 = Starter::getParted(vec1.begin(), vec1.end(), less);
        auto i2 = std::partition(vec2.begin(), vec2.end(), less);

        expect_eq(vec1, vec2);
        expect_eq(*i1, *i2);
        expect_eq(i1 - vec1.begin(), i2 - vec2.begin());
}

TEST(test_insertionSort, 1)
{
    list<decltype(rand())>  ls1;
    list<decltype(rand())>  ls2;
    double tmp;

    const size_t LOOP = 512;

    for(size_t i = 0; i != LOOP; ++i) {
        tmp = rand();
        ls1.push_back(tmp);
        ls2.push_back(tmp);
    }
   // std::sort(ls1.begin(), ls1.end());
    ls1.sort();
   // cout << "\n==========\n";
    Starter::insertionSort(ls2.begin(), ls2.end());
//cout << "\n==============\n";
  //  for(size_t i = 0; i != LOOP; ++i) {
   //         cout << ls1[i] << "   " << ls2[i] << endl;
  //  }
    expect_eq(ls1, ls2);
}


TEST(test_merge, 1)
{
    vector<decltype(rand())>  vec1;
    vector<decltype(rand())>  vec2;
    vector<decltype(rand())>  vec_dest;
    vector<decltype(rand())>  vec_dest2;
    std::list<decltype(rand())> ls;

    const size_t LOOP = 10;
    for(size_t i = 0; i != LOOP; ++i) {
        vec1.push_back(rand());
        vec2.push_back(rand());
        ls.push_back(rand());
    }

expect_eq(Starter::distance(vec1.begin(), vec1.end()),
          std::distance(vec1.begin(), vec1.end()));

expect_eq(Starter::distance(ls.begin(), ls.end()),
          std::distance(ls.begin(), ls.end()));

vec_dest.resize(vec1.size() + vec2.size());
vec_dest2.resize(vec_dest.size());

std::sort(vec1.begin(), vec1.end());
std::sort(vec2.begin(), vec2.end());

//    std::cout << "v1 : ";
//    std::copy(vec1.begin(), vec1.end(),
//              std::ostream_iterator<int>(std::cout, " "));
//    std::cout << '\n';
//
//    std::cout << "v2 : ";
//    std::copy(vec2.begin(), vec2.end(),
//              std::ostream_iterator<int>(std::cout, " "));
//    std::cout << '\n';


    std::merge(vec1.begin(), vec1.end(),
               vec2.begin(), vec2.end(),
               vec_dest.begin());

    Starter::merge(vec1.begin(), vec1.end(),
               vec2.begin(), vec2.end(),
               vec_dest2.begin());

//    for (int i = 0; i != vec_dest.size(); ++i) {
//        cout << vec_dest[i] << "   " << vec_dest2[i] << endl;
//    }

    expect_eq(vec_dest, vec_dest2);
}


TEST(test_mergeSort, 1)
{
    vector<decltype(rand())>  vec1;
    vector<decltype(rand())>  vec2;
    double tmp;

    const size_t LOOP = 100;

    for(size_t i = 0; i != LOOP; ++i) {
        tmp = rand();
        vec1.push_back(tmp);
        vec2.push_back(tmp);
    }
    std::sort(vec1.begin(), vec1.end());

    Starter::mergeSort(vec2.begin(), vec2.end());

//    for(size_t i = 0; i != LOOP; ++i) {
//            cout << vec1[i] << "   " << vec2[i] << endl;
//    }
    expect_eq(vec1, vec2);
}

TEST(test_quickSort, 1)
{

    vector<decltype(rand())>  vec1;
    vector<decltype(rand())>  vec2;
    double tmp;

    const size_t LOOP = 100;

    for(size_t i = 0; i != LOOP; ++i) {
        tmp = rand();
        vec1.push_back(tmp);
        vec2.push_back(tmp);
    }
    std::sort(vec1.begin(), vec1.end());

    Starter::quickSort(vec2.begin(), vec2.end());

//    for(size_t i = 0; i != LOOP; ++i) {
//            cout << vec1[i] << "   " << vec2[i] << endl;
//    }

    expect_eq(vec1, vec2);
}

TEST(test_bucketSort, 1)
{
    list<decltype(rand())>  ls1;
    list<decltype(rand())>  ls2;
    double tmp;

    const size_t LOOP = 1024;

    for(size_t i = 0; i != LOOP; ++i) {
        tmp = rand();
        ls1.push_back(tmp);
        ls2.push_back(tmp);
    }
   // std::sort(ls1.begin(), ls1.end());
    ls1.sort();
    Starter::bucketSort<decltype(ls1.begin()), 0, RAND_MAX>(ls2.begin(), ls2.end());

//    for(size_t i = 0; i != LOOP; ++i) {
//            cout << ls1[i] << "   " << ls2[i] << endl;
//    }
    expect_eq(ls1, ls2);

}


TEST(test_heap_sort_priv, 1)
{
//    int* arr = (int*)malloc(10 * sizeof(int));
//    int a[10] = {16, 4, 10, 14, 7, 9, 3, 2, 8, 1};
//    for(int i = 0; i != 10 ; ++i) {
//        *(arr + i) = a[i];
//    }
//    Starter::detail_::maxHeapify(arr, 4, 9);
//    Starter::detail_::maxHeapify(arr, 3, 9);
//    Starter::detail_::maxHeapify(arr, 1, 9);
//    Starter::detail_::maxHeapify(arr, 0, 9);
//    Starter::detail_::maxHeapify(arr, 1, 9);
//
//   Starter::detail_::buildHeap(arr, 10);
//
//   ssf_xchg(*arr, *(arr + 9), int);
//   Starter::detail_::maxHeapify(arr, 0, 9);///////////////////
//
//   ssf_xchg(*arr, *(arr + 8), int);
//   Starter::detail_::maxHeapify(arr, 0, 8);
//
//   ssf_xchg(*arr, *(arr + 7), int);
//   Starter::detail_::maxHeapify(arr, 0, 7);
//
//   ssf_xchg(*arr, *(arr + 6), int);
//   Starter::detail_::maxHeapify(arr, 0, 6);
//
//   ssf_xchg(*arr, *(arr + 5), int);
//   Starter::detail_::maxHeapify(arr, 0, 5);
//
//   ssf_xchg(*arr, *(arr + 4), int);
//   Starter::detail_::maxHeapify(arr, 0, 4);
//
//   ssf_xchg(*arr, *(arr + 3), int);
//   Starter::detail_::maxHeapify(arr, 0, 3);
//
//   ssf_xchg(*arr, *(arr + 2), int);
//   Starter::detail_::maxHeapify(arr, 0, 2);
//
//   ssf_xchg(*arr, *(arr + 1), int);
//   Starter::detail_::maxHeapify(arr, 0, 1);
//
//    Starter::heapSort(arr, arr + 10);
//    for(int i = 0; i != 10 ; ++i) {
//        cout << *(arr + i) << "\t" << a[i] << endl;
//    }
//    free(arr);
//    arr = nullptr;
}

TEST(test_heapSort, 1)
{
    vector<decltype(rand())>  vec1;
    vector<decltype(rand())>  vec2;
    double tmp;

    const size_t LOOP = 128;

    for(size_t i = 0; i != LOOP; ++i) {
        tmp = rand();
        vec1.push_back(tmp);
        vec2.push_back(tmp);
    }
    std::sort(vec1.begin(), vec1.end());

    Starter::heapSort(vec2.begin(), vec2.end());

//    for(size_t i = 0; i != LOOP; ++i) {
//            cout << vec1[i] << "   " << vec2[i] << endl;
//    }
    expect_eq(vec1, vec2);


}

TEST(test_introSort, 1)
{
    vector<decltype(rand())>  vec1;
    vector<decltype(rand())>  vec2;
    double tmp;

    const size_t LOOP = 12;

    for(size_t i = 0; i != LOOP; ++i) {
        tmp = rand();
        vec1.push_back(tmp);
        vec2.push_back(tmp);
    }
    std::sort(vec1.begin(), vec1.end());

  //  cout <<"log " << Starter::detail_::log2(300) << endl;

    Starter::insertionSort(vec2.begin(), vec2.end());

    for(size_t i = 0; i != LOOP; ++i) {
          //  cout << vec1[i] << "   " << vec2[i] << endl;
    }
    expect_eq(vec1, vec2);

}

struct tester
{
    tester(){}
    tester(int i): data(i)  {}
    int data;
    double dump[100];
    virtual void greet(){}
    ~tester(){}
};

TEST(test_class_sorting, 1)
{
    vector<tester>  vec1;
    vector<tester>  vec2;
    auto descending = [](const tester& a, const tester& b)->bool
              {return a.data > b.data;};
    auto ascending = [](const tester& a, const tester& b)->bool
              {return a.data < b.data;};

    auto equals = [](const tester& a, const tester& b)->bool
              {return a.data == b.data;};


    double tmp;

    const size_t LOOP = 128;
//////////////  load
    for(size_t i = 0; i != LOOP; ++i) {
        tmp = rand();
        vec1.push_back(tester(tmp));
        vec2.push_back(tester(tmp));
    }

///////////////     insertion sort
    std::sort(vec1.begin(), vec1.end(), descending);
    Starter::insertionSort(vec2.begin(), vec2.end(), descending);


    auto i1 = vec1.begin();
    auto i2 = vec2.begin();
    for(;i1 != vec1.end() && i2 != vec2.end() && equals(*i1, *i2);
            ++i1, ++i2)
            {
              //  cout << i1->data << "  " << i2->data << endl;
            }

    expect_true(i1 == vec1.end() && i2 == vec2.end());
//    expect_eq(i1, vec1.end());
//    expect_eq(i2, vec2.end());

///////////////     merge sort
    std::sort(vec1.begin(), vec1.end(), ascending);
    Starter::mergeSort(vec2.begin(), vec2.end(), ascending);

    i1 = vec1.begin();
    i2 = vec2.begin();
    for(;i1 != vec1.end() && i2 != vec2.end() && equals(*i1, *i2);
            ++i1, ++i2)
            {
              //  cout << i1->data << "  " << i2->data << endl;
            }
    expect_true(i1 == vec1.end() && i2 == vec2.end());
//    expect_eq(i1, vec1.end());
//    expect_eq(i2, vec2.end());
///////////////     quick sort
    std::sort(vec1.begin(), vec1.end(), descending);
    Starter::quickSort(vec2.begin(), vec2.end(), descending);

    i1 = vec1.begin();
    i2 = vec2.begin();
    for(;i1 != vec1.end() && i2 != vec2.end() && equals(*i1, *i2);
            ++i1, ++i2)
            {
           // cout << i1->data << "  " << i2->data << endl;
            }
    expect_true(i1 == vec1.end() && i2 == vec2.end());

/////////////     bucket sort
{
    vector<int>  vec1;
    vector<int>  vec2;
    double tmp;
    const size_t LOOP = 64;
    for(size_t i = 0; i != LOOP; ++i) {
        tmp = rand();
        vec1.push_back(tmp);
        vec2.push_back(tmp);
    }

    auto descending = [](const int& a, const int& b)->bool{ return a > b;};
    std::sort(vec1.begin(), vec1.end(), descending);

    Starter::bucketSort(vec2.begin(), vec2.end(), descending);

    auto i1 = vec1.begin();
    auto i2 = vec2.begin();
    for(;i1 != vec1.end() && i2 != vec2.end() && equals(*i1, *i2);
            ++i1, ++i2)
            {
           // cout << i1->data << "  " << i2->data << endl;
            }
    expect_true(i1 == vec1.end() && i2 == vec2.end());

}

///////////////     heap sort
    std::sort(vec1.begin(), vec1.end(), ascending);
    Starter::heapSort(vec2.begin(), vec2.end(), ascending);

    i1 = vec1.begin();
    i2 = vec2.begin();
    for(;i1 != vec1.end() && i2 != vec2.end() && equals(*i1, *i2);
            ++i1, ++i2)
            {
           // cout << i1->data << "  " << i2->data << endl;
            }
    expect_true(i1 == vec1.end() && i2 == vec2.end());

}


int main()
{
    RUN_ALL();
   // REPORT_ALL();
    return 0;
}









