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

/**
 * @file:  SArray.h
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @last modified: 2013.2.12
 * @brief: small (smart)array.
 *         dynamic array with various optimization
 */
//////////////////////////////////////////////////////////////////////////////////////////////////
////
////    Traditional realloc in C is designed to perform
////      a in-place reallocation or allocaion-memcpy-free cycle.
////    C++ do not support such in-place reallocation which can be cheap.
////
////    When memory block is larger than a certain size
////      there is a high chance that a in-place reallocation can be done,
////      that is, original memory block grow with out normal
////      allocate-memcpy-free cycle.
////
////    In GCC, the possibility of in-place realloc
////      is no less than 0.97 when memory block is larger than a bound
////      the limits is 4096 bytes in my machine
////    ( my machine: 64-bit Ubuntu Linux 12.04, GCC 4.72
////        i also had test in other platform i can access to
////        see test results on MS Windows 8 with MinGW 4.72
////                and on MS Windows 8 with VC 11
////    )
////
#ifndef _STARTER_SARRAY_H_
#define _STARTER_SARRAY_H_
#pragma once

#include "StarterCfgDef.h"
#include "RelOps.h"

#include <utility>  // for move, forward, hash
#include <iterator> // for iterator_traits
#include <new>      // for placement new
#include <stdexcept>// for out_of_rang error
#include <algorithm>// for lexicographical_compare

#include "Hash.h"

__NAMESPACE_STARTER___BEGIN__

namespace _detail {

//////////////////////////////////////////////////////////////////////////////////////////////////
////    1. memory mould:
////
////        |-> left capacity <-||->      right capacity            <-|
////        |-> left space    <-||->      right space               <-|
////        ____________________ ______________________________________
////        --------------------OOOOOOOOOOOOOOOOO----------------------
////        ^                   ^               ^                     ^
////        _leftStorage       _left (data)   _right              _rightStorage
////
////                            |->  size     <-|
////         <---- push_front              push_back ----->
//////////////////////////////////////////////////////////////////////////////////////////////////
////
////    2. allocation and data movement
////
////        there are two different memory allocation inside this array
////                ::operator new / ::operator delete
////            for objects, especially objects big and has remote pointer
////        and
////                malloc/memcpy/free
////            for trivial data
////
////        they are identified by manip_bit
////                true_type for manipulating bits
////                false_type for manipulatiing objects
////
////    The corresponding operations are
////
////    if data is trivial (or is_trivially_copyable)
////                if requried left space == current left space
////                                        ===>    realloc
////                else
////                                        ===> malloc->memcpy->free
////
////    else
////                if  is_nothrow_move_constructible (or is_move_constructible)
////                    or is not copy constructible
////                                        ===>  move
////
////                else
////                                        ===>  copy
////
////
////        Object must be copy-constructible or move-constructible
////            and the move constructor ought to be exception free,
////            otherwise, depend on the behavior of the move-constructor,
////            old objects may be lost (cannot be restored) when exception is thrown
////            in the construction of new objects.
////        That is why move-relocation does not catch exception.
////
////        When exception is throw in copy-relocation, the only guarantee provided is that
////            already-constructed objects being destryed.
//////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>
class _SArrayBase
{
    protected:
    typedef _SArrayBase<T>  _Self;

//    typedef std::integral_constant<bool,
//        std::is_trivially_copyable<T>::value>           manip_bit;
    //!< is_trivial is a stronger condition
    typedef std::integral_constant<bool,
        std::is_trivial<T>::value>                      manip_bit;// C-styled bits manipulation.

//    typedef std::integral_constant<bool,
//        std::is_nothrow_move_constructible<T>::value
//        || !std::is_copy_constructible<T>::value>      use_move;
    typedef std::integral_constant<bool,
        std::is_move_constructible<T>::value
        || !std::is_copy_constructible<T>::value>      use_move;

        struct Impl {

            T* _leftStorage;
            T* _left;
            T* _right;
            T* _rightStorage;

            void M_shiftTo(T* dest, size_t leftSp, size_t rightSp) noexcept {
                ptrdiff_t distance = _right- _left;

                _leftStorage = dest;
                _left = _leftStorage + leftSp;
                _right = _left + distance;
                _rightStorage = _left + rightSp;
            }
            void M_swap (Impl& other) noexcept {
                auto swap = [](T*& a, T*& b){ T* tmp = a; a = b; b = tmp;};
                swap(this->_leftStorage, other._leftStorage);
                swap(this->_left, other._left);
                swap(this->_right, other._right);
                swap(this->_rightStorage, other._rightStorage);
            }

            Impl (size_t leftSp, size_t rightSp, std::false_type)
            :   _leftStorage(static_cast<T*>(::operator new((leftSp + rightSp) * sizeof(T)))),
                _left(_leftStorage  + leftSp),
                _right(_left),
                _rightStorage(_left + rightSp)      {}

            Impl (size_t leftSp, size_t rightSp, std::true_type)
            :   _leftStorage(static_cast<T*>(malloc((leftSp + rightSp) * sizeof(T)))),
                _left(_leftStorage  + leftSp),
                _right(_left),
                _rightStorage(_left + rightSp)      {}

            Impl (Impl& other) {
                M_swap(other);
            }

        } _impl;

        void M_destroy (T* left, T* right) {
            constexpr bool callDtor = !std::has_trivial_destructor<T>::value;
            D_destroy(left, right,
                      std::integral_constant<bool, callDtor>());
        }
        void D_destroy(T* left, T* right, std::true_type) {
            for (; left != right; ++left) {
                    left->~T();
            }
        }
        void D_destroy(T* left, T* right, std::false_type) {}

        void M_deallocate(T* ptr, std::true_type) {
            free(ptr);
        }
        void M_deallocate(T* ptr, std::false_type) {
            ::operator delete(ptr);
        }
        //!<  this is for operator[], can refer to position that currently has no data.
        //!< just like the way with a build-in array
        void M_checkIndex (int idx) const {

            if (_impl._left + idx < _impl._leftStorage
                || _impl._left + idx >= _impl._rightStorage )

                __throw_range_error("SArray::M_checkIndex");
        }
        //!< this is for at(), can refer to data only
        void M_checkRange(size_t idx) const {
            if (_impl._left + idx >= _impl._right) {
                __throw_out_of_range("SArray::M_checkRange");
            }
        }

        /////////////////////////////////////////////////
        /// \brief  grow to two directions
        /// \param leftGrowth: additional space needed.
        /// \return void
        /////////////////////////////////////////////////
        void M_grow (size_t leftGrowth, size_t rightGrowth) {
            M_expand(leftGrowth + _impl._left - _impl._leftStorage,
                             rightGrowth + _impl._rightStorage - _impl._left,
                             manip_bit());
        }
        /////////////////////////////////////////////////
        /// \brief true_type: manipulate bits.
        /// \param leftSp: total left space (original left space and growth).
        /// \return void
        /////////////////////////////////////////////////
        void M_expand (size_t leftSp, size_t rightSp, std::true_type) {
            if (_impl._leftStorage + leftSp == _impl._left) {   // try in-place expansion
                void* ptr_bit = realloc(_impl._leftStorage, (leftSp + rightSp ) * sizeof(T));
                if (ptr_bit == _impl._leftStorage) {            // succeed
                    _impl._rightStorage = _impl._left + rightSp;
                }
                else if (ptr_bit) {
                    _impl.M_shiftTo((T*)ptr_bit, leftSp, rightSp);
                }
                else {
                    __throw_bad_alloc();
                }
            }
            else {
                void* ptr_bit = malloc((leftSp + rightSp) * sizeof(T));
           //     void* start = ptr_bit + (_impl._right - _impl._left) * sizeof(T);
                void* start = ptr_bit + leftSp * sizeof(T);
                if ( ! ptr_bit) {
                    __throw_bad_alloc();
                }   // do not use memmove
                memcpy(start,
                       (void*)_impl._left,
                       (_impl._right - _impl._left) * sizeof(T));
                free(_impl._leftStorage);
                _impl.M_shiftTo((T*)ptr_bit, leftSp, rightSp);
            }
        }
        //!< false_type: manipulate non-trivial objects
        void M_expand (size_t leftSp, size_t rightSp, std::false_type) {
            T* ptr = static_cast<T*>
                        (::operator new ((leftSp + rightSp) * sizeof(T)));
            M_relocate(ptr + leftSp, _impl._left, _impl._right, use_move());
          //  M_destroy(_impl._left, _impl._right);// necessary?
            ::operator delete(_impl._leftStorage);
            _impl.M_shiftTo(ptr, leftSp, rightSp);
        }
    protected:
//        template <typename ROIter>
//        typename std::enable_if<
//            std::is_same<T, typename std::iterator_traits<ROIter>::value_type>::value,
//             void>::type

        template <typename Iter>
        typename Starter::defType<Iter, T, void>::type
        M_relocate (T* start, Iter left, Iter right, std::false_type) {
            T* cur = start;
            try {
                for (; left != right; ++left, ++cur) {
                    ::new(static_cast<void*>(cur)) T(*left);
                }
            }
            catch(...) {
                M_destroy(start, cur);
                throw;
            }
        }
        template <typename Iter>
        typename Starter::defType<Iter, T, void>::type
        M_relocate (T* start, Iter left, Iter right, std::true_type) {
            T* cur = start;
            for (; left != right; ++left, ++cur) {
                    ::new(static_cast<void*>(cur)) T(std::move(*left));
            }
        }
//        template <typename Iter>
//            enable_if<std::is_same<T, std::iterator_traits<Iter>::value_type>::value,
//        void>::type
//        M_relocate(T* start, Iter left, Iter right, std::true_type) {
//            M_relocate(start, std::make_move_iterator(left), std::make_move_iterator(right), std::false_type());
//        }
        /// \param start : higher bound of this interval
        template <typename Iter>
        typename Starter::defType<Iter, T, void>::type
        M_relocate_backward (T* start, Iter left, Iter right, std::true_type) {
            T* cur = start;
            while (left != right) {
                --cur;
                --right;
                new(static_cast<void*>(cur)) T(std::move(*right));
            }
        }
        template <typename Iter>
        typename Starter::defType<Iter, T, void>::type
        M_relocate_backward (T* start, Iter left, Iter right, std::false_type) {
            T* cur = start;
            try {
                while (left != right) {
                    --cur;
                    --right;
                    new(static_cast<void*>(cur)) T(*right);
                }
            }
            catch(...) {
                M_destroy(cur, start);
                throw;
            }
        }

        void M_makeGap(T* leftEdge, size_t width) {
            if (_impl._right + width > _impl._rightStorage) {
                M_grow(0, _impl._right + width - _impl._rightStorage);
            }
            M_relocate_backward(_impl._right + width, leftEdge, _impl._right, use_move());
            _impl._right += width;
        }

        _SArrayBase (size_t leftSp, size_t rightSp)
        :   _impl(leftSp, rightSp, manip_bit())     {}

        _SArrayBase (_Self&& other)
        :   _impl(other._impl) {}

        ~_SArrayBase() {
            M_destroy(_impl._left, _impl._right);
            M_deallocate(_impl._leftStorage, manip_bit());
        }
};

} // namespace _detail
template <typename T>
class SArray : protected _detail::_SArrayBase<T>, public RelOps<SArray<T>> 
{
  //  typedef SArray<T, std::function<size_t(size_t)>>    _Self;
        typedef SArray<T>                               _Self;
        typedef _detail::_SArrayBase<T>                 _Base;
        typedef typename _Base::manip_bit               manip_bit;
        typedef typename _Base::use_move                use_move;

    public:
        typedef T                                       value_type;
        typedef T*                                      pointer;
        typedef const T*                                const_pointer;
        typedef T&                                      reference;
        typedef const T&                                const_reference;
        typedef T*                                      iterator;
        typedef const T*                                const_iterator;
        typedef std::reverse_iterator<iterator>         reverse_iterator;
        typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    protected:
        using _Base::_impl;
        using _Base::M_grow;
        using _Base::M_expand;
        using _Base::M_destroy;
        using _Base::M_relocate;
        using _Base::M_relocate_backward;
        using _Base::M_checkRange;
        using _Base::M_checkIndex;


        //!< calculate next right capacity base on current right capacity.
        //!< see memory mould on top of the file.
        std::function<size_t(size_t)>   // in general, current right capacity is more than 1
            _capacityR;
        //!< calculate next left capacity base on current size.
        std::function<size_t(size_t)>
            _capacityL;

    public:
//-----------------------------------------------------------------------------
//                              constructors
//-----------------------------------------------------------------------------

        SArray (size_t initCapL, size_t initCapR)// A
        :   _Base(initCapL, initCapR),
            _capacityR([](size_t currentCapR)->size_t{
                        return currentCapR * sizeof(T) < g_inPlcaeExpansionBound ?
                            currentCapR * 2 : (currentCapR * 3 + 1) / 2;}),
            _capacityL([](size_t currentSz)->size_t{return currentSz * 9 / 5;})
        {}

        SArray (size_t initCapL, size_t initCapR, const T& obj)// B
        :   SArray(initCapL, initCapR) {
            std::uninitialized_fill_n(begin(), initCapR, obj);
            _impl._right = _impl._rightStorage;
        }

        SArray ()
        :   SArray((64/sizeof(T))+1, (64/sizeof(T))+1)   {} // c A

        SArray (size_t initCap)
        :   _Base((64/sizeof(T))+1, initCap),
            _capacityR([](size_t currentCapR)->size_t{      // D
                        return currentCapR * sizeof(T) < g_inPlcaeExpansionBound ?
                            currentCapR * 2 : (currentCapR * 3 + 1) / 2;}),
            _capacityL([](size_t currentSz)->size_t{return currentSz * 9 / 5;})
        {}

//        SArray (size_t initCap, const T& filler)
//        :   SArray((64/sizeof(T))+1, initCap, filler)   {} //e B

        //!< param\ bool : a tag use to distinguish with Ctor A
        template <typename Iter>
        SArray (Iter left, Iter right, bool)
        :   SArray(std::distance(left, right)) {// F A

            static_assert(std::is_same<T,
                                typename Starter::realType<Iter>::value_type>::value,
                                "iterator value type mismatched");

            std::uninitialized_copy(left, right, _impl._left);
            _impl._right = _impl._rightStorage;
        }

        SArray (std::initializer_list<T> ls)
        :   SArray(ls.begin(), ls.end(), true)    {} //g F

        SArray (_Self& other)                   // h
        :   _Base(other.capacityL(), other.capacityR()) {
            std::uninitialized_copy(other.begin(), other.end(), begin());
            _impl._right = _impl._left + (other._impl._right - other._impl._left);
        }

        SArray (_Self&& other)
        :   _Base(std::move(other))     {}  // I
//-----------------------------------------------------------------------------
//                              overloaded operators
//-----------------------------------------------------------------------------
        _Self&
        operator= (const _Self& other) {
            if (&other != this) {
                assign(other.begin(), other.end());
            }
            return *this;
        }
        _Self&
        operator= (_Self&& other) {
         //   M_destroy(_impl._left, _impl._right);
         //   _Base::M_deallocate(_impl._leftStorage, manip_bit());///???????????????????????
            _Base::M_swap(other);
            return *this;
        }
        _Self&
        operator= (std::initializer_list<T> ls) {
            assign(ls.begin(), ls.end());
            return *this;
        }
//-----------------------------------------------------------------------------
//                              setters
//-----------------------------------------------------------------------------
        void setGrowPolicy (std::function<size_t(size_t)> newPoli) noexcept {
            this->_capacityR = newPoli;
        }
        void setBackwardGrowPolicy (std::function<size_t(size_t)> newPoli) noexcept {
            this->_capacityL = newPoli;
        }

        template<typename Iter>
        typename Starter::defType<Iter, T, void>::type
        assign (Iter left, Iter right) {
            T* cur = _impl._left;
            for (; cur != _impl._right
                    && left != right;
                    ++left, ++cur) {
                *cur = *left;
            }
            if (cur != _impl._right) {
                M_destroy(cur, _impl._right);
                _impl._right = cur;
            }
            else {
                for (;left != right; ++left) {
                    emplace_back(*left);
                }
            }
        }
        void assign(std::initializer_list<T> ls) {
            assign(ls.begin(), ls.end());
        }
        void push_back (const T& obj) {
            if (_impl._right == _impl._rightStorage) {
                M_expand(_impl._left - _impl._leftStorage,  // stand still
                         _capacityR(this->capacityR()),
                         manip_bit());
            }
            ::new(static_cast<void*>(_impl._right++)) T(obj);
        }
        /////////////////////////////////////////////////
        ///
        /// NOTE:
        ///     If you use push_front() to store the first data in this array
        ///     that is, call push_front() before all other setters.
        ///     the first piece of memory that should store that data will not be used,
        ///     because _impl._left drawbacks first, leaving the empty memory empty.
        ///     if you call capacityL(), you will find that capacityL() is one less than your expectation
        ///     you can find that one in capacityR()
        ///     The same thing happens to std::deque too,
        ///     but you cannot find it because deque has no capacity().
        /////////////////////////////////////////////////
        void push_front (const T& obj) {
            if (_impl._left == _impl._leftStorage) {
                M_expand(_capacityL(this->size()),//use _capacityR in case currentL is 0.
                         _impl._rightStorage - _impl._left, // stand still
                         manip_bit());
            }
            ::new(static_cast<void*>(-- _impl._left)) T(obj);
        }
        template<typename ...Args>
        void
        emplace_back(Args&&...args) {
            if (_impl._right == _impl._rightStorage) {
                M_expand(_impl._left - _impl._leftStorage,  // stand still
                         _capacityR(this->capacityR()),
                         manip_bit());
            }
            ::new(static_cast<void*>(_impl._right++)) T(std::forward<Args>(args)...);
        }
        template<typename ...Args>
        void
        emplace_front(Args&&...args) {
            if (_impl._left == _impl._leftStorage) {
                M_expand(_capacityL(this->size()),//use _capacityR in case currentL is 0.
                         _impl._rightStorage - _impl._left, // stand still
                         manip_bit());
            }
            ::new(static_cast<void*>(-- _impl._left)) T(std::forward<Args>(args)...);
        }

        iterator insert (T* start, const T& obj) {
            size_t pos = start - _impl._left;
            M_checkRange(pos - 1);
            if (_impl._right == _impl._rightStorage) {

                M_expand(_impl._left - _impl._leftStorage,  // stand still
                         _capacityR(this->capacityR()),
                         manip_bit());
            }
            T* edge = _impl._left + pos;
            _Base::M_makeGap(edge, 1);
            ::new((void*)(edge)) T(obj);
            return edge;
        }

        template<typename Iter>
        typename Starter::defType<Iter, T, iterator>::type
        insert (T* start, Iter left, Iter right) {
            size_t pos = start - _impl._left;
            M_checkRange(pos - 1); // allow insertion at end
            size_t len = std::distance(left, right);
            while (_impl._right + len >= _impl._rightStorage) {
                M_expand(_impl._left - _impl._leftStorage,  // stand still
                        _capacityR(this->capacityR()),
                        manip_bit());
            }
            T* edge = _impl._left + pos;
            _Base::M_makeGap(edge, len);
            return std::uninitialized_copy(left, right, edge);
        }

        iterator insert (T* start, std::initializer_list<T> ls) {
            return insert(start, ls.begin(), ls.end());
        }

        void erase (T* pos) {
            M_checkRange(pos - _impl._left);
            pos->~T();
            M_relocate(pos, pos + 1, _impl._right, use_move());
            -- _impl._right;
        }
        //!< fast erase will choose to move the smaller data block
        //!< left space will chang
        void fast_erase (T* pos) {
            M_checkRange(pos - _impl._left);
            pos->~T();
            if (_impl._right - pos < pos - _impl._left) {
                M_relocate(pos, pos + 1, _impl._right, use_move());
                -- _impl._right;
            }
            else {
                M_relocate_backward(pos + 1, _impl._left, pos, use_move());
                ++ _impl._left;
            }
        }
        void erase (T* left, T* right) {
            M_checkRange(left - _impl._left);
            M_checkRange(right - _impl._left - 1);
            M_destroy(left, right);
            M_relocate(left, right, _impl._right, use_move());
            _impl._right -= right - left;
        }
        void fast_erase (T* left, T* right) {
            M_checkRange(left - _impl._left);
            M_checkRange(right - _impl._left - 1);
            M_destroy(left, right);
            if (left - _impl._left > _impl._right - right) {
                M_relocate(left, right, _impl._right, use_move());
                _impl._right -= right - left;
            }
            else {
                M_relocate_backward(right, _impl._left, left, use_move());
                _impl._left += right - left;
            }
        }

        void pop_back () {
            (--_impl._right)->~T();
        }
        //!< pop_front() affects capacityL() and capacityR()
        void pop_front () {
            (_impl._left++)->~T();
        }
        void resize (size_t leftSp, size_t rightSp) {
            assert(0 < (leftSp + rightSp) && (leftSp + rightSp < max_size()));
            if(leftSp == capacityL() && rightSp == capacityR()){
                return;
            }
            T* rightBound = _impl._left + rightSp;
            if (rightBound < _impl._right) {
                M_destroy(rightBound, _impl._right);
                _impl._right = rightBound;
            }
            M_expand(leftSp, rightSp, manip_bit());
        }
        void resize (size_t rightSz) {
            resize(_impl._left - _impl._leftStorage, rightSz);
        }
        void shrink_to_fit () {
            M_expand(0, _impl._right - _impl._left, manip_bit());///////////////////
        }

        void clear () noexcept {
            M_destroy(_impl._left, _impl._right);
            _impl._right = _impl._left;
        }

//-----------------------------------------------------------------------------
//                              getters
//-----------------------------------------------------------------------------
        reference front () {
            return * _impl._left;
        }
        reference back () {
            return * (_impl._right - 1);
        }
        const_reference front () const {
            return * _impl._left;
        }
        const_reference back () const {
            return * (_impl._right - 1);
        }
        //!< can access to empty space
        reference operator[] (int idx) {
            M_checkIndex(idx);
            return *(_impl._left + idx);
        }
        reference at (size_t idx) {
            M_checkRange(idx);
            return *(_impl._left + idx);
        }
        const_reference operator[] (int idx) const {
            M_checkIndex(idx);
            return *(_impl._left + idx);
        }
        const_reference at (size_t idx) const {
            M_checkRange(idx);
            return *(_impl._left + idx);
        }
        //!< data does not always return address of the first object.
        pointer data () noexcept {
            return _impl._leftStorage;
        }
        const_pointer data () const noexcept {
            return _impl._leftStorage;
        }
//-----------------------------------------------------------------------------
//                              iterators
//-----------------------------------------------------------------------------
        iterator begin () noexcept {
            return _impl._left;
        }
        iterator end () noexcept {
            return _impl._right;
        }
        const_iterator cbegin () const noexcept {
            return _impl._left;
        }
        const_iterator cend () const noexcept {
            return _impl._right;
        }
        reverse_iterator rbegin () noexcept {
            return reverse_iterator(end());
        }
        reverse_iterator rend () noexcept {
            return reverse_iterator(begin());
        }
        const_reverse_iterator crbegin () const noexcept {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator crend () const noexcept {
            return const_reverse_iterator(begin());
        }
//-----------------------------------------------------------------------------
//                              others
//-----------------------------------------------------------------------------
        // sizeof(char) is supposed to be 1 byte,
        // otherwish change second param to
        //  (_impl._right - _impl._left) * sizeof(T) / sizeof(char)
        uint64_t
        hash_code () const noexcept {
            return StarterHash::fnv64_buf(
                static_cast<void*>(_impl._left),
                (_impl._right - _impl._left) * sizeof(T));
        }
     //!< distinguish between data()
        T*  to_array() noexcept {
            return _impl._left;
        }
        const_pointer to_array() const noexcept {
            return _impl._left;
        }

        size_t size () const noexcept {
            return _impl._right - _impl._left;
        }

        size_t capacityR () const noexcept {
            return _impl._rightStorage  -_impl._left;
        }

        size_t capacityL () const noexcept {
            return _impl._left - _impl._leftStorage;
        }

        size_t capacity () const noexcept {
            return _impl._rightStorage - _impl._leftStorage;
        }

        bool empty () const noexcept {
            return _impl._right == _impl._left;
        }

        size_t max_size () const noexcept {
            return size_t(-1) / sizeof(T);
        }
//-------------------------------------------
//                  comparator
//-------------------------------------------
        bool operator== (const _Self& other) const noexcept;

        bool operator!= (const _Self& other) const noexcept {
            return !operator ==(other);
        }

        bool operator < (const _Self& other) const noexcept {
            return std::lexicographical_compare(cbegin(), cend(), other.cbegin(), other.cend());
        }
        bool operator<= (const _Self& other) const noexcept {
            return operator < (other) || operator == (other);
        }

        bool operator> (const _Self& other) const noexcept {
            return !operator<= (other);
        }
        bool operator>= (const _Self& other) const noexcept {
            return !operator< (other);
        }
    protected:
};

//template<typename T, std::function<size_t(size_t)>>
template<typename T>
bool
//SArray<T, std::function<size_t(size_t)> comp>
SArray<T>::
operator== (const _Self& other) const noexcept {
    if (this != &other) {
		const_iterator myH = cbegin();
		const_iterator othrH = other.cbegin();
		const_iterator myButt = cend();
		const_iterator othrButt = other.cend();
		for (; (*myH) == (*othrH) && myH != myButt && othrH != othrButt;
		++myH, ++othrH) {}

		return myH == myButt && othrH == othrButt;
	}
    return true;
}

__NAMESPACE_STARTER___END__

#endif


