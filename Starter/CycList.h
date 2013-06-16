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
 * @file:  CycList.h
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @last modified: 2013.1.26
 * @brief:  a cyclic, double-linked list, not the bike rider :)
 */

//  brief:
//        1. element's looking-up is two times fast than ordinary one on average.
//                See  _Node* _LkListBase::M_nodeAt(size_t index);
//        2. std::list<T>::iterator is a bidirectional iterator.
//            However, CycList provide a enhanced bidirectional, a iterator that can jump.
//            Operation such as IterA < IterB, IterA - IterB is not available.
//        3. destroying operation in std::list isn't explicit,
//            the list  didn't destroy original objects when list is assigned another list
//            However, original object will be destroyed here with operator=.
//        4. use insertion sort.
//
///////////////////////////////////////////////////////////////////////////
//        About function naming
//          to cope with std::list,  naming followed std style
//          but member functions, valuables are named other way
//
//

#ifndef _STARTER_CYCLIST_H_
#define _STARTER_CYCLIST_H_

#include "StarterCfgDef.h"
#include "RelOps.h"

#include <utility>  // for move, forward, std::hash, initializer_list
#include <iterator> // for iterator_traits
#include <new>      // for placement new
#include <algorithm>// for lexicographical_compare

#include "Hash.h"   // hash other types

__NAMESPACE_STARTER___BEGIN__


/////////////////////////////////////////////////
/// \brief  namespace _detail
///  names :   _NodeBase
///            _CListNode
///            _CycListBase
///            _CycListIterator
///            _ROIterator
///
///   the only exposed name is CycList
 ////////////////////////////////////////////////
namespace _detail {

/////////////////////////////////////////////////
/// \brief base node of the linked list
///   with all functions a node needs
/////////////////////////////////////////////////
template<typename T>
struct _NodeBase
{
    typedef _NodeBase<T>*  _Ptr;

    _Ptr    _next;
    _Ptr    _prev;

    void
    M_attachTo(_Ptr pKnot) noexcept {
        pKnot->_prev->_next = this;
        this->_prev = pKnot->_prev;
        pKnot->_prev = this;
        this->_next = pKnot;
    }
    void
    M_detach() noexcept {
        this->_prev->_next = this->_next;
        this->_next->_prev = this->_prev;
    }
    void
    M_transplantTo (_Ptr& pKnot) noexcept {
        this->M_detach();
        this->M_attachTo(pKnot);
    }
};
// Why this additional layer( NodeBase and Node)?
//  1. to compile statements like this:
//                      CycList<T> ls;
//                      auto data = ls[0];  // ls is empty
//      linkedlist[0] must have an address, though its address is meaningless
//  2. to put an end in this double-linked list without additional allocation for T object.
//     The list holds one node (the BaseNode without data)only.
template<typename T>
struct _CListNode : public _NodeBase<T>
{
    T               _data;
};


/////////////////////////////////////////////////
/// \brief
///
///  CycList Base class.
///  memory operation is preformed here.
///  The CycList wrapped it, provide new interface and iterators.
///
/////////////////////////////////////////////////
template<typename T, typename Allocator>
class _CycListBase
{
    protected:
        typedef _CListNode<T>           _Node;
        typedef _NodeBase<T>            _BaseNode;

        typedef
        	typename Allocator::template rebind<_Node>::other   _NodeAllocator;// Node with data T

        typedef
        	typename Allocator::template rebind<T>::other       _ObjAllocator;

        struct Impl: public _NodeAllocator {
            _BaseNode   __bNode;
            size_t      __size;
        } _impl;

        _NodeAllocator
        M_getNodeAllocator() const noexcept {
            return _NodeAllocator();
        }

        _ObjAllocator
        M_getObjAllocator () const noexcept {
            return _ObjAllocator();
        }
        void
        M_swapWith (_CycListBase& other) noexcept {
            Impl tmp(_impl);
            _impl = other._impl;
            other._impl = tmp;
        }
        void
        M_cleanUp() {
            _Node* ptr = static_cast<_Node*>(_impl.__bNode._next);
            while(ptr != & _impl.__bNode){
                _Node* tmp = ptr;//ptr has to go away before destruction operation
                ptr = static_cast<_Node*>(ptr->_next);
                _impl.destroy(tmp);
                _impl.deallocate(tmp, 1); // second arg "1" is useless
                _impl.__size = 0;
            }
        }


//    std::exception_ptr eptr;
//    try {
//        std::string().at(1); // this generates an std::out_of_range
//    } catch(...) {
//        eptr = std::current_exception(); // capture
//    }
//    handle_eptr(eptr);

        template <typename... Args>
        _Node*
        M_creatNode (Args&&... objs){
            _Node* pNode = _impl.allocate(1);
            try {
                _impl.construct(&pNode->_data, std::forward<Args>(objs)...);
            }
            catch(...) {
                _impl.deallocate(pNode, 0);// if operation failed, clean up.
                throw;
            }
            _impl.__size++;// the only and must-pass way to add new node
            return pNode;
        }

        template <typename... Objs>
        void
        M_insert(_BaseNode* pBNode, Objs&&...objs) {
            _Node* tmp = M_creatNode(std::forward<Objs>(objs)...);
            tmp->M_attachTo(pBNode);
        }

        void
        M_remove (_BaseNode* _bNode) {
            _bNode->M_detach();
            _Node* pNode = static_cast<_Node*>(_bNode);
            _impl.destroy(pNode);
            _impl.deallocate(pNode, 0);
            _impl.__size--;         // // the only must-pass way to remove node
        }
        void
        M_checkRange(size_t idx) const {
            if(idx >= _impl.__size)
                throw std::out_of_range("CycList::M_checkRange");
        }
        //!< list must be initialized before this function is called,
        //!< 'cause it will reverse the whole loop
        void
        M_reverse() noexcept {
            auto turn =
                    [](_BaseNode& node) {
                        auto tmp = node._next;
                        node._next = node._prev;
                        node._prev = tmp;
                    };
            auto head = &_impl.__bNode;
            auto tail = _impl.__bNode._prev;
            while (head != tail) {
                auto last = head;
                head = head->_next;
                turn(*last);
            }
            turn(*tail);
        }
        //!< Note:  returns _BaseNode* instead of _Node*
        _BaseNode*
        M_nodeAt(size_t idx) {
            M_checkRange(idx);
            if (idx > _impl.__size / 2) {
                auto ptr = _impl.__bNode._prev;
                while (idx--) {
                    ptr = ptr->_prev;
                }
                return ptr;
            }
            else {
                auto ptr = _impl.__bNode._next;
                while (idx--) {
                    ptr = ptr->_next;
                }
                return ptr;
            }
        }

    public:
        //!< param bool is an identifier
        _CycListBase(bool)
        : _impl() {
            _impl.__size = 0;
            _impl.__bNode._next =
            _impl.__bNode._prev = &_impl.__bNode;
        }
        //!< only CycList can used it
        _CycListBase() = delete;
        _CycListBase(const _CycListBase&) = delete;
        _CycListBase(_CycListBase&& other) {
            M_swapWith(other);
        }

        //!<  no assignment. use swap only
        _CycListBase&
        operator= (const _CycListBase&) = delete;

        ~_CycListBase() {
            this->M_cleanUp();
        }
};


/////////////////////////////////////////////////
/// \brief
/// a random access iterator alike iterator for CycList
///  operator- and operator<  is not available
///
/////////////////////////////////////////////////
template<typename T>
struct _CycListIterator;

/////////////////////////////////////////////////
/// \brief Read-Only iterator
/////////////////////////////////////////////////
template<typename T>
struct _ROIterator;

}// namespace _detail


/////////////////////////////////////////////////
/// \brief
///     a doubled-linked, cyclic linked-list
///         with quick indexing and build-in sorting
/////////////////////////////////////////////////

// Note:  about  allocators
//   if you used other allocators, e.g. std::allocator
//   some warnings,( -Wzero-as-null-pointer-constant)
//   may be generated duo to the lack of initialization,
//   but it is fine.
//
//   if you cares about warnings, you can also close "-Wzero-as-null-pointer-constant"
// or you can use Builder
//   whose functions are all static.
//template<typename T, typename Allocator = Builder<T>>

template<typename T, typename Allocator = std::allocator<T>>
class CycList : protected _detail::_CycListBase<T, Allocator>, public RelOps<CycList<T, Allocator>>
{
    protected:

        typedef _detail::_CycListBase<T, Allocator>	_Base;
        typedef CycList<T, Allocator>               _Self;

// if T is arithmetic type, use std::hash, otherwise use my own hash function
// because std::hash only support basic types.
//  thought std::hash is capable of hashing std::string,
//  i didn't use std::hash
//
//  hash function is from facebook folly::hash
// algorithm from
//      Paul Hsieh: http://www.azillionmonkeys.com/qed/hash.html
//
        typedef
            std::integral_constant<bool, std::is_arithmetic<T>::value>
                                                    _hashable;
        using _Base::_impl;
        using _Base::M_creatNode;
        using _Base::M_nodeAt;
        using _Base::M_insert;
        using _Base::M_getObjAllocator;
        using _Base::M_remove;
    public:
        typedef T					            Value;
        typedef T*					            Ptr;
        typedef const T*			            cPtr;
        typedef T&					            Ref;
        typedef const T&			            cRef;

        typedef _detail::_CycListIterator<T>	    Iter;
        typedef _detail::_ROIterator<T>	            cIter;

        typedef typename _Base::_Node               Node;
        typedef typename _Base::_BaseNode           BaseNode;

    public:
//------------------------------------------------------------------------------------
//                              constructor, overloaded operator
//------------------------------------------------------------------------------------
        CycList ()
        : _Base (true)   {}
        CycList(size_t sz)
        : _Base (true) {
            while(sz--) {
                emplace_back();
            }
        }
        CycList(size_t sz, cRef filler)
        : _Base (true) {
            while(sz--) {
                push_back(filler);
            }
        }
        template<typename ROIter>
        CycList (ROIter left, ROIter right)
        : _Base (true) {
            splice(begin(), left, right);
        }
        //!< copy constructor
        CycList (const _Self& other)
        : CycList (other.cbegin(), other.cend())   {}
        //!<  move constructor
        CycList (_Self&& other)
        : _Base(std::move(other))  {}

        CycList (std::initializer_list<T> iniList) // do not make reference to that list
        : CycList (iniList.begin(), iniList.end()) {}

        //<! see ~_CycListBase()

        //!<  assign operation
        _Self&
        operator= (_Self& other){
            if(this != &other) {
                M_assignDispatch(other.cbegin(), other.cend());
            }
            return *this;
        }

        //!<    move assign
        _Self&
        operator= (_Self&& other) {
            _Base::M_cleanUp();
            M_swapWith(other);
            return *this;
        }
        _Self&
        operator= (std::initializer_list<T> iniList) {
            M_assignDispatch(iniList.begin(), iniList.end());
            return *this;
        }
        bool
        operator== (const _Self& other) const noexcept;

        bool
        operator < (const _Self& other) {
            return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }
//------------------------------------------------------------------------------------
//                              iterators
//------------------------------------------------------------------------------------
        Iter
        begin() noexcept {
            return Iter(this->_impl.__bNode._next);
        }
        Iter
        end() noexcept {
        	return Iter(&(this->_impl.__bNode));
        }
        cIter
        cbegin() const noexcept {
            return cIter(this->_impl.__bNode._next);
        }
        cIter
        cend() const noexcept {
        	return cIter(&(this->_impl.__bNode));
        }

//------------------------------------------------------------------------------------
//                              getters
//------------------------------------------------------------------------------------
        Ref
        front() noexcept {
            return static_cast<Node*>(this->_impl.__bNode._next)->_data;
        }
        Ref
        back() noexcept {
            return static_cast<Node*>(this->_impl.__bNode._prev)->_data;
        }
        cRef
        front() const noexcept {
            return static_cast<const Node*>(this->_impl.__bNode._next)->_data;
        }
        cRef
        back() const noexcept {
            return static_cast<const Node*>(this->_impl.__bNode._prev)->_data;
        }

        Ref
        at(size_t idx) {
            BaseNode* ptr = M_nodeAt(idx);
            return (static_cast<Node*>(ptr))->_data;
        }
        cRef
        at(size_t idx) const {
            BaseNode* ptr = M_nodeAt(idx);
            return (static_cast<Node*>(ptr))->_data;
        }
        Ref
        operator[] (size_t idx) {
            return at(idx);
        }
        cRef
        operator[] (size_t idx) const {
            return at(idx);
        }
//------------------------------------------------------------------------------------
//                              setters
//------------------------------------------------------------------------------------
        void
        push_back(cRef obj) {
            M_insert(&(this->_impl.__bNode), obj);
        }
        void
        push_back(T&& val) {
            M_insert(&(this->_impl.__bNode), std::move(val));
        }
        void
        push_front(cRef obj) {
            M_insert(this->_impl.__bNode._next, obj);
        }
        void
        push_front(T&& val) {
            M_insert(this->_impl.__bNode._next, std::move(val));
        }

        template <typename... Objs>
        void
        emplace_back(Objs&&...objs) {
            M_insert(&( _impl.__bNode), std::forward<Objs>(objs)...);
        }
        template <typename... Objs>
        void
        emplace_front (Objs&&...objs) {
            M_insert(_impl.__bNode._next, std::forward<Objs>(objs)...);
        }
        /////////////////////////////////////////////////
        /// \brief splice other container into this list
        ///
        /// \param startpos : start position
        /// \param left/right : left/right edge of that range
        /// \return void
        ///
        /////////////////////////////////////////////////

        //!< @see M_spliceDispatch()
        template <typename ROIter>
        void
        splice (Iter startPos, ROIter left, ROIter right) {
            M_spliceDispatch(startPos, left, right);
        }
        void
        splice (Iter startPos, std::initializer_list<T> iniList) {
            splice(startPos, iniList.begin(), iniList.end());
        }

        void
        pop () {
            _Base::M_remove(this->_impl.__bNode._prev);
        }
        void
        pop_front () {
            _Base::M_remove(this->_impl.__bNode._next);
        }

        Iter
        insert (const Iter& iter, cRef obj) {
            Node* tmp = M_creatNode(obj);
            tmp->M_attachTo(iter._pBNode);
            return Iter(tmp);
        }
        Iter//!<  finding the node before creating, in case of exception.
        insert_at (size_t idx, cRef obj) {
            BaseNode* pd = M_nodeAt(idx);// pd is a Base Node pointer, not Node.
            Node* tmp = M_creatNode(obj);
            tmp->M_attachTo(pd);
            return Iter(tmp);
        }

        void
        erase (Iter iter) {
            _Base::M_remove(iter._pBNode);
        }
        void
        erase (Iter left, Iter right) {
            while (left != right) {
                Iter tmp = left++;
                erase(tmp);
            }
        }

        void
        remove_at (int idx) {
            _Base::M_remove(M_nodeAt(idx));
        }
//------------------------------------------------------------------------------------
//                              other
//------------------------------------------------------------------------------------
        size_t
        size() const noexcept {
            return _impl.__size;
        }
        bool
        empty() const noexcept {
            return _impl.__bNode._next == _impl.__bNode._prev;
        }

        void
        clear() {
            _Base::M_cleanUp();
            _impl.__bNode._next =
            _impl.__bNode._prev = & _impl.__bNode;
        }
        void
        swap (_Self& other) noexcept {
            _Base::M_swapWith(other);// all data is in _Base::_impl
        }
        void
        reverse () noexcept {
            _Base::M_reverse();
        }
        void
        remove (cRef obj);
        void
        remove_if (std::function<bool(const T&)>);

        void
        merge (_Self&& other, std::function<bool(const T&, const T&)>);

        void
        merge (_Self& other) {
            merge(std::move(other));
        }
        void
        merge (_Self& other, std::function<bool(const T&, const T&)> cmp) {
            merge(std::move(other), cmp);
        }

        void
        sort (std::function<bool(const T&, const T&)>);

        void
        unique(std::function<bool(const T&, const T&)>);
//--------------------------------------------------------------------------------
//                  modern features
//--------------------------------------------------------------------------------
        BaseNode*  ///!< remember to downcast it before use
        data () noexcept {
            return _impl.__bNode->_next;
        }

        Ptr ///!< user has the responsibility to deallocate this
        to_array () const {
            const size_t objNum = _impl.__size;
            auto builder = M_getObjAllocator();
            cIter left = cbegin();

            Ptr ptr = static_cast<Ptr>(::operator new (sizeof(T) * objNum));
            for (size_t i = 0; i != objNum; i++) {
                builder.construct((ptr + i), *left);
                ++left;
            }
            return ptr;
        }
//        std::vector<T>
//        toVector () const {
//            std::vector<T> vec;
//            vec.reserve(_impl.__size);
//            vec.insert(vec.begin(), cBegin(), cend());
//            return std::move(vec);
//        }
        size_t
        hash_code() const noexcept {
            return M_hashCode(_hashable());
        }

    protected:
        size_t
        M_hashCode(std::true_type) const noexcept;

        size_t
        M_hashCode(std::false_type) const noexcept;
/////////////////////////////////////////////////
/// \brief
///     Implementation of splice.
///     this function deals with iterators that different from CycList::Iter
///     but have the same value_type, type T.
///     function is enabled only if value_type of ROIter is the same with T
/// \param  Iter startPos: insertion start position
/// \param  ROIter: a read-only(const) forward iterator type
/// \return void
///
/////////////////////////////////////////////////
//
// to make debug easier, you can use static_assert instead of std::enable_if
//   M_spliceDispatch(Iter startPos, ROIter left, ROIter right) {
//       static_assert(std::is_same<T, typename iterator_traits<ROIter>::value_type>::value,
//       "CycList::transplant:mismatched types");
//           ...
//   }
        template <typename ROIter>
        typename Starter::defType<ROIter, T, void>::type
        M_spliceDispatch(Iter startPos, ROIter left, ROIter right) {
            for(; left != right; ++left) {
                    insert(startPos, *left);
            }
        }
        //!< distinguish between assign and splice
        template <typename ROIter>
        typename Starter::defType<ROIter, T, void>::type
        M_assignDispatch(ROIter left, ROIter right);
};

//!< implementation of iterators ad some function
#include "CycLis-inl.h"


template <typename T, typename A>
void
swap(CycList<T, A>& jia, CycList<T, A>& yi) noexcept {
    jia.swap(yi);
}

__NAMESPACE_STARTER___END__

#endif // _STARTER_CYCLIST_H_
