


#include <chrono>
#include <algorithm>
#include <vector>
#include "bwtest/bwtest.h"
#include "AlgosMT.h"

using namespace std;

TEST(mt_qsort, 1)
{
std::vector<uint64_t> vec1, vec2, vec3;
    const uint64_t LEN = (2<<16);
    srand((unsigned)time(0));
    for(uint64_t i = 0; i < LEN; ++i) {
        vec1.push_back(rand() * rand());
    }

    vec2.reserve(vec1.size()); vec2 = vec1; vec3.reserve(vec1.size());vec3 = vec1;
    expect_eq(vec1, vec2);
    expect_eq(vec2, vec3);
    sort(vec3.begin(), vec3.end());
    put_out << "Start:\n";
    /////////////////////////////////////////////////////////////////////
auto t_start1 = std::chrono::high_resolution_clock::now();

         // std::sort(vec1.begin(), vec1.end());
      // Starter::mtSort(vec1.begin(), vec1.end());
      Starter::hySort(vec1.begin(), vec1.end());

     // Starter::quickSort(vec1.begin(), vec1.end());
        // Starter::heapSort(vec1.begin(), vec1.end());
        // Starter::insertionSort(vec1.begin(), vec1.end());
auto t_end1 = std::chrono::high_resolution_clock::now();
/////////////////////////////////////////////////////////////////////
    auto time_1 = std::chrono::duration_cast<std::chrono::microseconds>(t_end1 - t_start1).count();
   put_out << "Time:" << time_1 << endl;
   /////////////////////////////////////////////////////////////////////
auto t_start2 = std::chrono::high_resolution_clock::now();

          std::sort(vec2.begin(), vec2.end());
       // Starter::quickSort(vec2.begin(), vec2.end());
      //   Starter::insertionSort(vec2.begin(), vec2.end());
auto t_end2 = std::chrono::high_resolution_clock::now();
/////////////////////////////////////////////////////////////////////
    auto time_2 = std::chrono::duration_cast<std::chrono::microseconds>(t_end2 - t_start2).count();

   put_out << "Time:" << time_2 << endl;


    expect_true(is_sorted(vec1.begin(), vec1.end()));
    expect_true(is_sorted(vec2.begin(), vec2.end()));

    put_out << ((double)time_1 / (double)time_2) << endl;
}


int main()
{
   // bwtest::setOutput("quickSortToinsertionSort_SwitingPoints_2-18_0-512.txt");
    RUN_ALL();
}
//void f1() noexcept {
//    RUN_ALL();
//}
//
//void f2() noexcept {
//    RUN_ALL();
//}
//    try {
//        thread t1(f1);
//        t1.join();
//        t1 = thread(f2);
//        t1.join();
//        cout <<sizeof(std::thread) << endl;
//    }
//    catch(...) {
//        cout << "some except\n" << endl;
//    }
//TEST(select_best_threshold, 0)
//{
//
//    const uint64_t LEN = (2<<18);
//    const uint64_t Left = 0;
//    const uint64_t Right = 512;
//
//
//    std::vector<uint64_t> vec[2], vec3;
//    srand((unsigned)time(0));
//
//    for(uint64_t i = 0; i < LEN; ++i) {
//        uint64_t t = rand() * rand();
//        vec[0].push_back(t);
//    }
//
//    vec3.reserve(vec[0].size());
//    vec3 = vec[0];
//
//    sort(vec3.begin(), vec3.end());
//
//    vector<pair<int, long>> times;
//
//    //put_out << "Start:\n";
//
//    for(int i = Left; i < Right; ++i) {
//
//    vec[1].reserve(vec[0].size());
//    vec[1] = vec[0];
//
//auto t_start1 = std::chrono::high_resolution_clock::now();
//
//        Starter::quickSort(vec[1].begin(), vec[1].end(), i);
//
//auto t_end1 = std::chrono::high_resolution_clock::now();
//auto time_1 = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end1 - t_start1).count();
//
//        times.push_back(make_pair(i, time_1));
//    }
//
//    sort(times.begin(), times.end(), [](const pair<int, long>& a, const pair<int, long>& b){return a.second < b.second;});
//
//    for(auto& i : times) {
//        put_out << i.second << "  " << i.first << endl;
//    }
//}
