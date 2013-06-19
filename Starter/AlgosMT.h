


#ifndef _STARTER_THREADER_ALGORITHEMS_H_
#define _STARTER_THREADER_ALGORITHEMS_H_

#include "StarterCfgDef.h"

#if _STARTER_GCC_VERSION_ < 480
#error thread library needed, so please update your compiler to GCC 4.8 or higher
#endif

#include <thread>
#include <future>
#include <functional>
#include "Algos.h"
#warning "multi-threaded functions here are experimental, and are unsafe!"


#include "bwtest/bwtest.h"


#include <algorithm>

__NAMESPACE_STARTER___BEGIN__


namespace detail_{

const uint16_t hardware_threads = std::thread::hardware_concurrency();

    enum{switch_point = 16};

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type
    > void
    hySortLoop(RAIter left, RAIter right, size_t depth) {

        while (right - left  > switch_point) {
            if (0 == depth) {
                return heapSort(left, right);
            }
            --depth;
            auto p1 = smartPartition(left, right);
            hySortLoop(left, p1, depth);
            left = p1;
        }
    }

} // namespace detail_

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type
    > void
    hySort(RAIter left, RAIter right) {
        size_t length = right - left;
        if (length < detail_::switch_point) {
            return heapSort(left, right);
        }
        const uint16_t depth = detail_::log2(length);
        RAIter pivot1 = detail_::smartPartition(left, right);
        RAIter pivot0;

        RAIter pivot[5];
        std::thread tds[3]; // three new threads
        // 0    1   2   3   4
        pivot[0] = left;
        pivot[4] = right;
        --pivot[0];
        //  partition 0 ~ 4
        pivot[2] = detail_::smartPartition(pivot[0] + 1, pivot[4]);
        // partition 0 ~2
        tds[0] = std::thread(
            [&]()   {
                pivot[1] = detail_::smartPartition(pivot[0] + 1, pivot[2]);
            }
        );
        // partition 2 ~ 4
        pivot[3] = detail_::smartPartition(pivot[2] + 1, pivot[4]);
        // sync!
        tds[0].join();

        for(int i = 0; i < 3; ++i) {
            tds[i] = std::thread(
                [&]()   {
                    std::sort(pivot[i] + 1, pivot[i + 1]);
                }
            );
        }
        std::sort(pivot[3] + 1, pivot[4]);
        for (auto& i : tds) {
            i.join();
        }
    }
//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type
//    > void
//    mtSort(RAIter left, RAIter right, size_t threshold = 64) {
//
//        size_t length = right - left;
//        if (length < threshold) {
//            return insertionSort(left, right);
//        }
//    // left     0   1   2   right
//        starter_xchg(*(left + (right - left)/2), *left, ValType); // ok
//        RAIter pivot1 = getParted(left, right, [&](const ValType& a){return a < *left;});
//        starter_xchg(*left, *pivot1, ValType);
//
//        RAIter pivot0, pivot2;
//        std::thread t1( // ok
//            [&]()    {
//                starter_xchg(*(left + (pivot1 - left)/2), *left, ValType);
//                pivot0 = getParted(left, right, [&](const ValType& a){return a < *left;});
//                starter_xchg(*left, *pivot0, ValType);
//            }
//        );
//        std::thread t2(
//            [&]()    {
//                starter_xchg(*(pivot1 + (right - pivot1)/2), *pivot1, ValType);
//                pivot2 = getParted(pivot1, right, [&](const ValType& a){return a < *pivot1;});
//                starter_xchg(*pivot1, *pivot2, ValType);
//            }
//        );
//        t1.join();
//        t2.join();
//    // left     0   1   2   right
//        std::thread t3(_MT_SORT_INTERVAL(left, pivot0));
//        std::thread t4(_MT_SORT_INTERVAL(pivot0, pivot1));
//        std::thread t5(_MT_SORT_INTERVAL(pivot1, pivot2));
//        std::thread t6(_MT_SORT_INTERVAL(pivot2, right));
//
//        t3.join();
//        t4.join();
//        t5.join();
//        t6.join();
//    }

//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type
//    > void
//    MT_quickSort(RAIter left, RAIter right, const uint16_t threshold, const uint8_t currentThreads) {
//
//        size_t length = right - left;
//        if (length < threshold) {
//            return insertionSort(left, right);
//        }
//
//        starter_xchg(*(left + length/2), *left, ValType);
//
//        RAIter pivot = getParted(left, right, [&](const ValType& a){return a < *left;});
//
//        starter_xchg(*left, *pivot, ValType);
//
//        if (currentThreads < g_hardware_threads) {
//
//            std::async(std::launch::async,
//                        [&]() {
//                            MT_quickSort(left, pivot, threshold, currentThreads + 1);
//                        }
//                    );
//            quickSort(pivot + 1, right);
//        }
//        else {
//            quickSort(left, pivot);
//            quickSort(pivot + 1, right);
//        }
//    }
//
//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type
//    > void
//    mtSort(RAIter left, RAIter right, const uint16_t threshold = 64) {
//        MT_quickSort(left, right,  threshold, 0);
//    }
//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type
//    > void
//    MT_sort(RAIter left, RAIter right, const size_t threshold = 64) {
/////
/////         detach cause segment fault
/////         join    is slower than std::sort
/////
//        const size_t length = right - left;
//        if (length < threshold) {
//            return insertionSort(left, right);
//        }
/////////////////////////////////
//        starter_xchg(*(left + length/2), *left, ValType);
//
//        RAIter last = left;
//        for (RAIter cur = left + 1; cur != right; ++cur) {
//            if (*cur < *left) {
//                ++last;
//                starter_xchg(*last, *cur, ValType);
//            }
//        }
//        starter_xchg(*left, *last, ValType);
//
//        std::thread t1(detail_::QuickSort_block<RAIter>(), left, last);
//        std::thread t2(detail_::QuickSort_block<RAIter>(), (last + 1), right);
//        t1.join();
//        t2.join();
//    }


//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type
//    > void
//    MT_quickSort(RAIter left, RAIter right, size_t threshold = 64) {
//
//        size_t length = right - left;
//        if (length < threshold) {
//            return insertionSort(left, right);
//        }
//
//        RAIter pivots[3];
////            left   1   0   2  right
//        ValType   p[3];
//
//        p[0] = *(left + length/2);
//        auto less1 = [&](const ValType& a)->bool{return a < p[0];};
//        pivots[0] = Starter::getParted(left, right, less1);
//
//        p[1] = *(left + length/4);
//        auto less2 = [&](const ValType& a)->bool{return a < p[1];};
//        pivots[1] = Starter::getParted(left, pivots[0], less2);
//
//        p[2] = *(left + length * 3/4);
//        auto less3 = [&](const ValType& a)->bool{return a < p[2];};
//        pivots[2] = Starter::getParted(pivots[0], right, less3);
//
//       // starter_xchg(*left, *last, ValType);
//        std::thread t1(detail_::QuickSort_block<RAIter>(), left, pivots[1]);
//
//        std::thread t2(detail_::QuickSort_block<RAIter>(), pivots[1] + 1, pivots[0]);
//
//        std::thread t3(detail_::QuickSort_block<RAIter>(), pivots[0] + 1, pivots[2]);
//
//        std::thread t4(detail_::QuickSort_block<RAIter>(), pivots[2] + 1, right);
//__YELL__ // die here!!!!!!!!!!!!!!!!!!!
//        t4.join();
//        t3.join();
//        t2.join();
//        t1.join();
//__YELL__
//    }


//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type
//    > void
//    MT_mergeSort (RAIter left, RAIter right, size_t min_length_pre_thread = 32) {
//        if (right - left < 2) {
//            return;
//        }
//
//        const size_t length = Starter::distance(left, right);
//
//        const uint16_t possible_max_thread_num =
//                (length + min_length_pre_thread - 1) / min_length_pre_thread;
//
//
//        const uint16_t mun_thread = possible_max_thread_num < g_hardware_threads ?
//                                        possible_max_thread_num : g_hardware_threads;
//
//        std::vector<std::thread> threads(mun_thread);
//
//        const size_t step = length / mun_thread;
//
//        RAIter block_start = left;
//        for(uint16_t i = 0; i < mun_thread - 1; ++i) {// reserve last thread
//            RAIter block_end = block_start;
//            Starter::toJump(block_end, step);
//            threads[i] = std::thread(Starter::mergeSort, block_start, block_end);
//            block_start = block_end;
//        }
//
//
//        RAIter mid = left + (right - left) / 2;
//
//        mergeSort(left, mid);// note that mid as the right edge of this interval is unreachable
//        mergeSort(mid, right);
//
//        Starter::merge(left, mid, mid, right, left);
//    }
/////////////////////////
//    for(unsigned long i = 0; i < (num_threads - 1); ++i)
//    {
//        Iterator block_end = block_start;
//        std::advance(block_end, block_size);
//        threads[i] = std::thread(
//                         accumulate_block<Iterator, T>(),
//                         block_start, block_end, std::ref(results[i]));
//        block_start = block_end;
//    }
//
//    accumulate_block<Iterator, T>()(
//                block_start, last, results[num_threads - 1]
//        );
//
//    std::for_each(threads.begin(), threads.end(),
//                  std::mem_fn(&std::thread::join));
//
//    return std::accumulate(results.begin(), results.end(), init);
////----------------------------------------------------------------------------
////                      merge sort
////----------------------------------------------------------------------------
//
//    template <typename BidIter,
//                typename ValType = typename std::iterator_traits<BidIter>::value_type,
//                    typename Comp = std::function<bool(const ValType&, const ValType&)>// need c++11
//    > void
//    MT_merge (BidIter left1, BidIter right1,
//                BidIter left2, BidIter right2, BidIter destination, Comp comp) {
//
//        ptrdiff_t len1 = Starter::distance(left1, right1);
//        ptrdiff_t len2 = Starter::distance(left2, right2);
//        ValType* arr = new ValType[len1 + len2];
//        ValType* dest = arr;
//        while (left1 != right1 && left2 != right2) {
//            if (comp(*left1, *left2)) {
//                *dest = starter_move(*left1, ValType);
//                ++left1;
//            }
//            else {
//                *dest = starter_move(*left2, ValType);
//                ++left2;
//            }
//            ++dest;
//        }
//        while (left1 != right1) {
//            *dest = starter_move(*left1, ValType);
//            ++dest;
//            ++left1;
//        }
//        while (left2 != right2) {
//            *dest = starter_move(*left2, ValType);
//            ++dest;
//            ++left2;
//        }
//        ValType* start = arr;
//        while (start != dest) {
//            *destination = starter_move(*start, ValType);
//            ++start;
//            ++destination;
//        }
//        delete [] arr;
//    }
//
//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type,
//                typename Comp = std::function<bool(const ValType&, const ValType&)>// need c++11
//    > void
//    MT_mergeSort (RAIter left, RAIter right, Comp comp) {
//        if (right - left < 2)
//            return;
//        RAIter mid = left + (right - left) / 2;
//        mergeSort(left, mid, comp);
//        mergeSort(mid, right, comp);
//        Starter::merge(left, mid, mid, right, left, comp);
//    }



//----------------------------------------------------------------------------
//                      quick sort
//----------------------------------------------------------------------------
//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type
//    > void
//    MT_quickSort(RAIter left, RAIter right) {
//
//         if (UNLIKELY(right - left) < 2)//////////////
//                return;
//
//        starter_xchg(*(left + (right-left)/2), *left, ValType);
//
//        RAIter last = left;
//        for (RAIter cur = left + 1; cur != right; ++cur)
//            if (*cur < *left) {
//                ++last;
//                starter_xchg(*last, *cur, ValType);
//            }
//
//        starter_xchg(*left, *last, ValType);
//
//
//        if(last - left > 64) {
//            RAIter le = left;
//            RAIter las = last;
//            std::thread tds(mt_quick_sort, le, las);
//          //  mt_quick_sort(left, last);
//        }
//        else {
//            quickSort(left, last);
//        }
//        if(right - last > 64) {
//            mt_quick_sort(last + 1, right);
//        }
//        else {
//            quickSort(last + 1, right);
//        }
//    }

__NAMESPACE_STARTER___END__
#endif // _STARTER_THREADER_ALGORITHEMS_H_


