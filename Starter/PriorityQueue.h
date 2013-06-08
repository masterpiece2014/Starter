/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */

#ifndef PRIORITYQUEUE_H_INCLUDED
#define PRIORITYQUEUE_H_INCLUDED

 ///////////////////////////////////////////////////////////////////////////////
 ////     Copyright 2012 2013 CaiBowen
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


#include <functional>
#include <vector>
#include <type_traits>
#include "ssfunc.h"

#ifndef __NAMESPACE_STARTER___BEGIN__
#define __NAMESPACE_STARTER___BEGIN__   namespace starter { /* namespace starter  */
#define __NAMESPACE_STARTER___END__     } /* namespace starter  */
#endif

__NAMESPACE_STARTER___BEGIN__

template <typename T,
            typename Comp = std::less<T>,
            typename Container = std::vector<T>
            >
class PrioQueue
{
private:
    Container cont_;
    Comp comp_;
public:

    PrioQueue()
    : cont_()   {}

    template <typename Iter>
    PrioQueue(Iter left, Iter right)
    :   cont_(left, right)  {
        static_assert(std::is_same< typename std::iterator_traits<Iter>::type, T>::value,
                       "mismatched type");
    }

    bool empty() const noexcept {
        return cont_.empty();
    }
    size_t size() const noexcept {
        return cont_.size();
    }

    T& top() {
        return cont_.front();
    }

    const T& top() const {
        return cont_.front();
    }

    void push(const T& val) {
        cont_.push_back(val);
        ssf::detail_::buildHeap(cont_.begin(), cont_.size());
    }

    void pop() {
        T t = cont_.front();    // swap
        cont_.front() = cont_.back();
        cont_.back() = t;

        cont_.pop_back();

        //  half the time
        ssf::detail_::maxHeapify(cont_.begin(), 0, cont_.size());

      //  ssf::detail_::buildHeap(cont_.begin(), cont_.size());
    }

};
__NAMESPACE_STARTER___END__

#endif // PRIORITYQUEUE_H_INCLUDED






