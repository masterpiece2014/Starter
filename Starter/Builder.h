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


#ifndef _BUILDER_H_
#define _BUILDER_H_


#include <cstddef>// size_t
#include <cstdlib>// malloc
#include <stdexcept>
#include <utility>// move
#include <new>
using std::size_t;

#ifndef __NAMESPACE_STARTER___BEGIN__
#define __NAMESPACE_STARTER___BEGIN__   namespace starter { /* namespace starter  */
#define __NAMESPACE_STARTER___END__     } /* namespace starter  */
#endif

__NAMESPACE_STARTER___BEGIN__


// fully compatible with std containers.
// provide two mode
// prefer C malloc
// can be used without being intialized, for all functions are static
template<typename T>
class Builder
{
    public:
        typedef T*            pointer;
        typedef const T*      const_pointer;
        typedef T&            reference;
        typedef const T&      const_reference;
        typedef T             value_type;

        typedef size_t     size_type;
        typedef ptrdiff_t  difference_type;

        const static size_t MAX_OBJ_NUM = size_t (-1) / sizeof (T);

        Builder () noexcept = default;
        Builder (const Builder&) noexcept = default;
        template<typename E>
        Builder (const Builder<E>&) noexcept {}

        ~Builder () noexcept = default;

        template<typename E>
        struct rebind {
            typedef Builder<E> other;
        };

        static pointer
        address (reference obj) noexcept {
            return std::addressof(obj);
        }
        static const_pointer
        address (const_reference obj) noexcept {
            return std::addressof(obj);
        }
        static pointer
        allocate (size_t num, const void* = nullptr) {
            if (num > MAX_OBJ_NUM) {
                throw std::bad_alloc();//std::__throw_bad_alloc();
            }
            return static_cast<T*>(::operator new (num * sizeof (T) ) );
        }
        // null ptr is allowed
        static void
        deallocate (T* ptr, size_type = 1) {
            if (ptr != nullptr)
                    ::operator delete (ptr);
        }

        static size_t
        max_size() noexcept {
            return MAX_OBJ_NUM;
        }
        // a C-Style reallc is provide below
        static void
        destroy (T* ptr) noexcept {// Dtor ought not throw!
            ptr->~T();
        }
        // single one, simple one without partial specification.
//        template<typename RAIter_t>
//        static void
//        destroy (RAIter_t left, RAIter_t right) noexcept {
//            if (std::has_trivial_destructor<decltype (*left) >::value)
//                //  if (std::has_trivial_destructor<IterInfer<RAIter_t>::Value_t>::value)
//                //  if (TypeInfer::hasNeglectableDtor(*left)) // do nothing. obj with trivial dtor
//            {
//                ;      //  needs no destruct operation.
//            } else {
//                while (left != right) {
//                    destroy (addressOf (*left) );
//                    left++;
//                }
//            }
//
//        }

        template<typename E, typename ... Args>
        static void
        construct (E* ptr, Args&&... objs) {
            ::new (static_cast<void*>(ptr)) E(std::forward<Args>(objs)...);
        }

// safty on
//        void
//        construct(T* ptr, const T& obj)
//        {
//            ::new(static_cast<void*>(ptr)) T(obj);//!constructed E, diff from stl which constructed T
//        }
};


/////////////// wrapped malloc, calloc and reallc. throw std::bad_alloc() if alloc failed.

__NAMESPACE_STARTER___END__

#endif //_BUILDER_H_

/*
    template<bool>
    struct DestroyAssistant
    {
        template<typename RAIter_t>
        static void
        _destroy(RAIter_t left, RAIter_t right)
        {
            while(left != right)
            {
               // destroy(Eleven::realAddressOf(left));
               destroy(addressOf(*left));
                left++;
            }
        }
    };
    template<>
    struct DestroyAssistant<true>
    {
        template<typename RAIter_t>
        static void
        _destroy(RAIter_t left, RAIter_t right)
        {}  // For obj with trivial destructor, do nothing in destroying
    };

    template<typename RAIter_t>
    static void
    destroy(RAIter_t left, RAIter_t right)
    {
//        DestroyAssistant<
//                std::has_trivial_destructor<IterInformer<RAIter_t>::Value_t>
//            >::_destroy(left, right);

//    typedef typename iterator_traits<_ForwardIterator>::value_type _Value_type;
//      std::_Destroy_aux<__has_trivial_destructor(_Value_type)>::__destroy(__first, __last);
//

    typedef typename IterInfer<RAIter>::Value_t   Entry_t;
    DestroyAssistant<std::has_trivial_destructor<IterInfer<RAIter>::Value_t>>::_destroy(left, right);

    }
*/

