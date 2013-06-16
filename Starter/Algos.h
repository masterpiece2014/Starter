 ///////////////////////////////////////////////////////////////////////////////
 ////     Copyright 2013 CaiBowen
 ////     All rights reserved.
 ////
 ////     Author: Cai Bowen
 ////       contact/bug report/get new version
 ////           at
 ////       feedback2bowen@outlook.com
 ////
 ////
 ////     Licensed under the Apache License, Version 2.0 (the "License");
 ////     you may not use this file except in compliance with the License.
 ////     You may obtain a copy of the License at
 ////
 ////              http://www.apache.org/licenses/LICENSE-2.0
 ////
 ////     Unless required by applicable law or agreed to in writing, software
 ////     distributed under the License is distributed on an "AS IS" BASIS,
 ////     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ////     See the License for the specific language governing permissions and
 ////     limitations under the License.
 ///////////////////////////////////////////////////////////////////////////////

/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */

#ifndef _STARTER_ALGORITHMS_H_
#define _STARTER_ALGORITHMS_H_

#ifdef __GUNC__
#pragma GCC system_header
#endif

#include "StarterCfgDef.h"

#include "bwtest/bwtest.h"

#include <iterator>// for iterator_traits
#include <limits>
#include <functional>
#include <list>
__NAMESPACE_STARTER___BEGIN__


    #define starter_move(obj, TypeName)\
                static_cast<TypeName&&>(obj)

//#else

//            #define starter_move(obj, TypeName)\
//                                obj

//#endif

#define starter_xchg(jia, yi, TypeName)\
        {\
            TypeName tmp = starter_move(jia, TypeName);\
            jia = starter_move(yi, TypeName);\
            yi = starter_move(tmp, TypeName);\
        }

#define starter_swap(jia, yi, bin,TypeName)\
            bin = starter_move(jia, TypeName);\
            jia = starter_move(yi, TypeName);\
            yi = starter_move(bin, TypeName);


//----------------------------------------------------------------------------
//                      for iterators
//----------------------------------------------------------------------------
/////////////////////////   compile-time polymorphic  /////////////////////////

    // random iterator
    template <typename Iter,
                typename Steps,
                typename category = typename std::iterator_traits<Iter>::iterator_category
    > inline typename std::enable_if<std::is_same<category, typename std::random_access_iterator_tag>::value,
        ptrdiff_t>::type
    toJump(Iter& iter, Steps steps) {
        iter = iter + steps;
    }

        // not random
    template <typename Iter,
                typename Steps,
                typename category = typename std::iterator_traits<Iter>::iterator_category
    > inline typename std::enable_if<!std::is_same<category, typename std::random_access_iterator_tag>::value,
        ptrdiff_t>::type
    toJump(Iter& iter, Steps steps) {
        if (steps > 0)
            while (steps)
                ++iter;
        else
            while(steps)
                --iter;
    }

    // random iterator
    template <typename Iter,
                typename category = typename std::iterator_traits<Iter>::iterator_category
    > inline typename std::enable_if<std::is_same<category, typename std::random_access_iterator_tag>::value,
        ptrdiff_t>::type
    distance(Iter left, Iter right) {
       // cout << " random" << endl;
        return right - left;
    }

        // not random
    template <typename Iter,
                typename category = typename std::iterator_traits<Iter>::iterator_category
    > inline typename std::enable_if<!std::is_same<category, typename std::random_access_iterator_tag>::value,
        ptrdiff_t>::type
    distance(Iter left, Iter right) {
        ptrdiff_t dis = 0;
        while (left != right) {
            ++left;
            ++dis;
        }
       // cout << " not random" << endl;
        return dis;
    }


//----------------------------------------------------------------------------
//                      miscellany: part one
//----------------------------------------------------------------------------
/*
    template <typename BidIter,
                    typename ValType = typename std::iterator_traits<BidIter>::value_type,
                typename UnaryFunc = std::function<void(const ValType&)>
    > void
    applyOn(const BidIter left, const BidIter right, UnaryFunc func) {
        while (left != right) {
            func(*left);
            ++left;     // it's better separate the *op and ++op,
        }           //because write them together may cause trouble for the compilers
    }               //and it cannot decide which one goes first

    template <typename BidIter,
                typename ValType = typename std::iterator_traits<BidIter>::value_type,
                typename BinaryFunc = std::function<void(const ValType&, const ValType&)>
    > void
    applyOn(const BidIter left, const BidIter right, const BidIter nextToleft, BinaryFunc func) {
        while (left != right) {
            func(*left, *nextToleft);
            ++left;
            ++nextToleft;
        }
    }
*/
    template <typename BidIter,
                    typename ValType = typename std::iterator_traits<BidIter>::value_type,
                typename UnaryFunc = std::function<void(ValType&)>
    > inline void
    applyOn(BidIter left, BidIter right, UnaryFunc func) {
        while (left != right) {
            func(*left);
            ++left;     // it's better separate the *op and ++op,
        }           //because write them together may cause trouble for the compilers
    }               //and it cannot decide which one goes first

    template <typename BidIter,
                typename ValType = typename std::iterator_traits<BidIter>::value_type,
                typename BinaryFunc = std::function<void(ValType&, ValType&)>
    > inline void
    applyOn(BidIter left, BidIter right, BidIter nextToleft, BinaryFunc func) {
        while (left != right) {
            func(*left, *nextToleft);
            ++left;
            ++nextToleft;
        }
    }
//----------------------------------------------------------------------------
//                      insertion sort
//----------------------------------------------------------------------------
/**< some extra work is done to fit bid-rational iterator */

    template <typename BidIter,
                typename ValType = typename std::iterator_traits<BidIter>::value_type// not supported by C++98.
    > void
    insertionSort(BidIter left, BidIter right) {
        --right;
        if (unlikely(right == left))
            return;
        for (BidIter cur = left; cur != right; ++cur) {
            BidIter adv(cur);
            ++adv;
            if (*adv < *cur) {
                ValType tmp = starter_move(*adv, ValType);
                *adv = starter_move(*cur, ValType);
                BidIter last(cur);
                BidIter last_adv(cur);
                while (tmp < *last && last_adv != left) {
                    last_adv = last;
                    --last;
                    *last_adv = starter_move(*last, ValType);
                }
                *last_adv = starter_move(tmp, ValType);
            }
        }
    }


    template <typename BidIter,
                typename ValType = typename std::iterator_traits<BidIter>::value_type,// not supported by C++98.
                    typename Comp = std::function<bool(const ValType&, const ValType&)>
    > void
    insertionSort(BidIter left, BidIter right, Comp comp) {
        --right;
        for (BidIter cur = left; cur != right; ++cur) {
            BidIter adv(cur);
            ++adv;
            if (comp(*adv, *cur)) {
                ValType tmp = starter_move(*adv, ValType);
                *adv = starter_move(*cur, ValType);
                BidIter last(cur);
                BidIter last_adv(cur);
                while (comp(tmp, *last) && last_adv != left) {
                    last_adv = last;
                    --last;
                    *last_adv = starter_move(*last, ValType);
                }
                *last_adv = starter_move(tmp, ValType);
            }
        }
    }

//----------------------------------------------------------------------------
//                      merge
//----------------------------------------------------------------------------

    template <typename BidIter,
                typename ValType = typename std::iterator_traits<BidIter>::value_type
    > void
    merge (BidIter left1, BidIter right1,
                BidIter left2, BidIter right2, BidIter destination) {

        ptrdiff_t len1 = Starter::distance(left1, right1);
        ptrdiff_t len2 = Starter::distance(left2, right2);
        ValType* arr = new ValType[len1 + len2];

        ValType* dest = arr;
        while (left1 != right1 && left2 != right2) {
            if (*left1 < *left2) {
                *dest = starter_move(*left1, ValType);
                ++left1;
            }
            else {
                *dest = starter_move(*left2, ValType);
                ++left2;
            }
            ++dest;
        }
        while (left1 != right1) {
            *dest = starter_move(*left1, ValType);
            ++dest;
            ++left1;
        }
        while (left2 != right2) {
            *dest = starter_move(*left2, ValType);
            ++dest;
            ++left2;
        }
        ValType* start = arr;
        while (start != dest) {
            *destination = starter_move(*start, ValType);
            ++start;
            ++destination;
        }
        delete [] arr;
    }

    template <typename BidIter,
                typename ValType = typename std::iterator_traits<BidIter>::value_type,
                    typename Comp = std::function<bool(const ValType&, const ValType&)>// need c++11
    > void
    merge (BidIter left1, BidIter right1,
                BidIter left2, BidIter right2, BidIter destination, Comp comp) {

        ptrdiff_t len1 = Starter::distance(left1, right1);
        ptrdiff_t len2 = Starter::distance(left2, right2);
        ValType* arr = new ValType[len1 + len2];
        ValType* dest = arr;
        while (left1 != right1 && left2 != right2) {
            if (comp(*left1, *left2)) {
                *dest = starter_move(*left1, ValType);
                ++left1;
            }
            else {
                *dest = starter_move(*left2, ValType);
                ++left2;
            }
            ++dest;
        }
        while (left1 != right1) {
            *dest = starter_move(*left1, ValType);
            ++dest;
            ++left1;
        }
        while (left2 != right2) {
            *dest = starter_move(*left2, ValType);
            ++dest;
            ++left2;
        }
        ValType* start = arr;
        while (start != dest) {
            *destination = starter_move(*start, ValType);
            ++start;
            ++destination;
        }
        delete [] arr;
    }

//----------------------------------------------------------------------------
//                      merge sort
//----------------------------------------------------------------------------

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type
    > void
    mergeSort (RAIter left, RAIter right) {
        if (right - left < 2)
            return;
        RAIter mid = left + (right - left) / 2;
        mergeSort(left, mid);// note that mid in the right edge of the interval is unreachable
        mergeSort(mid, right);
        Starter::merge(left, mid, mid, right, left);
    }


    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type,
                typename Comp = std::function<bool(const ValType&, const ValType&)>// need c++11
    > void
    mergeSort (RAIter left, RAIter right, Comp comp) {
        if (right - left < 2)
            return;
        RAIter mid = left + (right - left) / 2;
        mergeSort(left, mid, comp);
        mergeSort(mid, right, comp);
        Starter::merge(left, mid, mid, right, left, comp);
    }


//----------------------------------------------------------------------------
//                      bucket sort
//----------------------------------------------------------------------------
namespace detail_
{
    template<typename Contor, typename Val>
    void bk_helper_insert(Contor& contor, const Val& val) {
        if(unlikely(contor.empty())) {
            contor.push_back(val);
            return;
        }
        auto cur = contor.begin();
        while( *cur < val && cur != contor.end())
            ++cur;

        contor.insert(cur, val);
//    cout << "\n\n========== <<" <<  val << "=======\n";
//        for(auto i : contor) {
//            cout << i <<  endl;
//        }
//
//    cout << "\n\n========== <<" <<  val << "=======\n";
    }

    template<typename Contor, typename Val, typename Comp = std::function<bool(const Val&, const Val&)>
    > void
    bk_helper_insert(Contor& contor, const Val& val, Comp comp) {
        if(unlikely(contor.empty())) {
            contor.push_back(val);
            return;
        }
        auto cur = contor.begin();
        while(comp(*cur, val) && cur != contor.end()) {
            ++cur;
        }

        contor.insert(cur, val);
    }

    template<typename Contor, typename Iter>
    void bk_helper_add(Iter& iter, const Contor& cont) {
        if(unlikely(cont.empty()))
            return;
        for(auto cur = cont.cbegin();
                cur != cont.cend(); ++cur) {
            *iter = *cur;
            ++iter;
        }
    }

}//namespace detail_

/*
    initially, MAX and MIN is placed at normal args list (...), that is , they are normal parameters,
    but this will cause problems because when function<> is introduced, compiler is unable to
    distinguish between bucketSort with function<> and the one without that.
    compiler ought to be able to deduce that, let's hope GCC 5.X will do better.

    Hence, now the function<> has to be moved to the template args list <...>
    to make user comfortable, more code is written here.
    so if needed, you can specified the MAX and MIN like this:
        bucketSort<IterType, 10, 100>(iterA, iterB);
    rather than
        bucketSort<IterType, ValueType, 10, 100>(iterA, iterB);
*/

    template <typename FwIter,
                const typename std::iterator_traits<FwIter>::value_type MIN =
                        std::numeric_limits<typename std::iterator_traits<FwIter>::value_type>::min(),
                const typename std::iterator_traits<FwIter>::value_type MAX =
                        std::numeric_limits<typename std::iterator_traits<FwIter>::value_type>::max(),
                size_t BUCKETNUM = 10,
            typename ValType = typename std::iterator_traits<FwIter>::value_type,
            typename Contor = typename std::list<ValType>
    > void
    bucketSort(FwIter left, FwIter right){

        Contor  buckets[BUCKETNUM];
        FwIter cur;
                                       // cout << MIN <<MAX << endl;
        for (cur = left; cur != right; ++cur) {
                                      //  cout << "\n " << (*cur - MIN) * BUCKETNUM / MAX << "      " << *cur << endl;
            size_t idx = (*cur - MIN) * BUCKETNUM / (MAX - MIN);

            detail_::bk_helper_insert(buckets[idx], *cur);
        }
        cur = left;
        for(size_t i = 0; i != BUCKETNUM; ++i) {
            detail_::bk_helper_add(cur, buckets[i]);
        }
    }


    template <typename FwIter,
                const typename std::iterator_traits<FwIter>::value_type MIN =
                        std::numeric_limits<typename std::iterator_traits<FwIter>::value_type>::min(),
                const typename std::iterator_traits<FwIter>::value_type MAX =
                        std::numeric_limits<typename std::iterator_traits<FwIter>::value_type>::max(),
                size_t BUCKETNUM = 10,
            typename ValType = typename std::iterator_traits<FwIter>::value_type,
            typename Contor = std::list<ValType>,
            typename Comp = std::function<bool(const ValType&, const ValType&)>
    > void
    bucketSort(FwIter left, FwIter right, Comp comp){

        Contor  buckets[BUCKETNUM];
        FwIter cur;
                                       // cout << MIN <<MAX << endl;
        for (cur = left; cur != right; ++cur) {
                                      //  cout << "\n " << (*cur - MIN) * BUCKETNUM / MAX << "      " << *cur << endl;
          //  detail_::bk_helper_insert(buckets[(*cur - MIN) * BUCKETNUM / MAX], *cur, comp);

            size_t idx = (*cur - MIN) * BUCKETNUM / (MAX - MIN);

            detail_::bk_helper_insert(buckets[idx], *cur, comp);

        }
        cur = left;
        for(size_t i = 0; i != BUCKETNUM; ++i) {
            detail_::bk_helper_add(cur, buckets[i]);
        }

    }


//----------------------------------------------------------------------------
//                      heap functions and heap sort
//----------------------------------------------------------------------------

namespace detail_
{

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type// not supported by C++98.
    > inline void
    maxHeapify(RAIter v, size_t cur, size_t heapSz) {
        size_t le = cur * 2 + 1;
        size_t ri = cur * 2 + 2;/////////////////////////////////////////////////////////////////

        size_t max =  (le < heapSz && *(v + le) > *(v + cur)) ? le : cur;
        if (ri < heapSz && *(v + ri) > *(v + max))
            max = ri;

        if (max != cur) {
            starter_xchg(*(v + cur), *(v + max), ValType);
            maxHeapify(v, max, heapSz);
        }
    }

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type// not supported by C++98.
    > inline void
    buildHeap(RAIter left, size_t heapSz) {
        for (size_t i = heapSz/2; i != 0; --i) {
            maxHeapify(left, i - 1, heapSz);
        }
    }

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type// not supported by C++98.
    > inline void
    minHeapify(RAIter v, size_t cur, size_t heapSz) {
        size_t le = cur * 2 + 1;
        size_t ri = cur * 2 + 2;/////////////////////////////////////////////////////////////////

        size_t min =  (le < heapSz && *(v + le) < *(v + cur)) ? le : cur;
        if (ri < heapSz && *(v + ri) < *(v + min))
            min = ri;

        if (min != cur) {
            starter_xchg(*(v + cur), *(v + min), ValType);
            minHeapify(v, min, heapSz);
        }
    }

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type// not supported by C++98.
    > inline void
    buildMinHeap(RAIter left, size_t heapSz) {
        for (size_t i = heapSz/2; i != 0; --i) {
            minHeapify(left, i - 1, heapSz);
        }
    }

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type,
                    typename Comp = std::function<bool(const ValType&, const ValType&)>
    > inline void
    heapify(RAIter v, size_t cur, size_t heapSz, Comp comp) {
        size_t le = cur * 2 + 1;
        size_t ri = cur * 2 + 2;/////////////////////////////////////////////////////////////////

        size_t max =  (le < heapSz && comp(*(v + cur), *(v + le) )) ? le : cur;

        if (ri < heapSz && comp(*(v + max), *(v + ri)))
            max = ri;

        if (max != cur) {
            starter_xchg(*(v + cur), *(v + max), ValType);
            heapify(v, max, heapSz, comp);
        }
    }

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type,
                    typename Comp = std::function<bool(const ValType&, const ValType&)>
    > inline void
    buildMyOwnHeap(RAIter left, size_t heapSz, Comp comp) {
        for (size_t i = heapSz/2; i != 0; --i) {
            heapify(left, i - 1, heapSz, comp);
        }
    }

} // namespace detail_

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type// not supported by C++98.
    > void
    heapSort(RAIter left, RAIter right) {
        size_t heapSz = right - left;
        detail_::buildHeap(left, heapSz);
        for (size_t i = --heapSz; i != 0; --i) {
            starter_xchg(*left, *(left + i), ValType);
            detail_::maxHeapify(left, 0, i);
        }
    }


    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type,// not supported by C++98.
                    typename Comp = std::function<bool(const ValType&, const ValType&)>
    > void
    heapSort(RAIter left, RAIter right, Comp comp) {

        size_t heapSz = right - left;
        detail_::buildMyOwnHeap(left, heapSz, comp);
        for (size_t i = --heapSz; i != 0; --i) {
            starter_xchg(*left, *(left + i), ValType);
            detail_::heapify(left, 0, i, comp);
        }
    }



    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type
    > void
    partialSort(RAIter left, RAIter right, size_t num) {
        detail_::buildMinHeap(left, right - left);
        for (size_t i = --num; i != 0; --i) {
            starter_xchg(*left, *(left + i), ValType);
            detail_::minHeapify(left, 0, i);
        }
    }

        template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type,
                    typename Comp = std::function<bool(const ValType&, const ValType&)>
    > void
    partialSort(RAIter left, RAIter right, size_t num, Comp comp) {
        detail_::buildMyOwnHeap(left, right - left, comp);
        for (size_t i = --num; i != 0; --i) {
            starter_xchg(*left, *(left + i), ValType);
            detail_::heapify(left, 0, i, comp);
        }
    }



//   starter_xchg(*arr, *(arr + 9), int);
//   Starter::detail_::maxHeapify(arr, 0, 9);///////////////////
//

namespace detail_
{
    const size_t thresHold = 16;

    size_t log2(size_t n) {
        size_t log = 0;
        while ( n >>= 1)  ++log;
        return log;
    }

}// namespace detail_


//----------------------------------------------------------------------------
//                      quick sort
//----------------------------------------------------------------------------

    template <typename BidIter,
                typename Pred,
                typename ValType = typename std::iterator_traits<BidIter>::value_type
    > BidIter
    getParted(BidIter left, BidIter right, Pred comp) {
        if (unlikely(left == right)) {
            return left;
        }
        while (comp(*left)) {
            ++left;
            if (left == right) {
                return left;
            }
        }
        BidIter last = left++;
        while (left != right) {
            if (comp(*left)) {
                starter_xchg(*last, *left, ValType);
                ++last;
            }
            ++left;
        }
        return last;
    }

    template <typename RAIter,
                typename ValType = typename std::iterator_traits<RAIter>::value_type
    > void
    quickSort(RAIter left, RAIter right) {

        size_t length = right - left;
        if(unlikely(length < 2)) {
            return;
        }
        starter_xchg(*(left + length/2), *left, ValType);

        RAIter last = left;
        for (RAIter cur = left + 1; cur != right; ++cur)
            if (*cur < *left) {
                ++last;
                starter_xchg(*last, *cur, ValType);
            }

        starter_xchg(*left, *last, ValType);
        quickSort(left, last);
        quickSort(last + 1, right);
    }

//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type,// not supported by C++98.
//                    typename Comp = std::function<bool(const ValType&, const ValType&)>// need c++11
//    > void
//    quickSort(RAIter left, RAIter right, Comp comp) {
//
//        size_t length = right - left;
//        if(unlikely(length < 2)) {
//            return;
//        }
//
//        starter_xchg(*(left + length/2), *left, ValType);
//
//        RAIter last = left;
//        for (RAIter cur = left + 1; cur != right; ++cur) {
//            if (comp(*cur, *left)) {
//                ++last;
//                starter_xchg(*last, *cur, ValType);
//            }
//        }
//        starter_xchg(*left, *last, ValType);
//        quickSort(left, last, comp);
//        quickSort(last + 1, right, comp);
//    }

//    template <typename RAIter,
//                typename ValType = typename std::iterator_traits<RAIter>::value_type// not supported by C++98.
//    > void
//    introSort(RAIter left, RAIter right) {
//        if (right != left) {
//            detail_::introspectiveSortLoops(left, right, 2*log2(right - left));
//            insertionSort(left, right);
//        }
//    }



//----------------------------------------------------------------------------
//                      miscellany: part two
//----------------------------------------------------------------------------


__NAMESPACE_STARTER___END__

#endif // _STARTER_ALGORITHMS_H_







