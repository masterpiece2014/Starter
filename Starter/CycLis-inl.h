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
 * @file:  CycList_inl.h
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @last modified: 2013.2.6
 */

#ifndef _STARTER_CYCLIST_H_
#error this file is the partial implementation of Cyclist\
        it may only be included by Cyclist.h\
        and cannot be used separately

#endif

namespace _detail {


template<typename T>
struct _CycListIterator final
{
    typedef _CycListIterator<T>     _Iter;
    typedef _CListNode<T>           _Node;
    typedef _NodeBase<T>            _BaseNode;

    // std requirements
    typedef typename std::bidirectional_iterator_tag
                            iterator_category;
    typedef T               value_type;
    typedef ptrdiff_t       difference_type;
    typedef T*              pointer;
    typedef T&              reference;

    _BaseNode*    _pBNode;

    _CycListIterator() = delete;

    explicit
    _CycListIterator(_BaseNode* node_)
    : _pBNode(node_) {}

    _CycListIterator(const _CycListIterator&) = default;

    ~_CycListIterator() = default;

    reference
    operator* () const {
        return (static_cast<_Node*>(_pBNode))->_data;//Note: data is T, not ptr
    }
    pointer
    operator-> () const {
        return &((static_cast<_Node*>(_pBNode))->_data);
    }

    _Iter&
    operator++ () {
        _pBNode = _pBNode->_next;
        return *this;
    }
    _Iter///////     _Iter& for ++iter and _Iter for iter++
    operator++ (int) {
        _Iter tmp(*this);
        _pBNode = _pBNode->_next;
        return tmp;
    }
    // When iterator is assigned linkedlist.begin(),
    //      iterator-- || --iterator points at end() which is empty actually,
    //      here, end() return data form a node casted from a BaseNode which has no data,
    //      thus the value of end() is unpredictable and meaningless
    // In a short, in this CycList, the last and first node: _imple.__bNode is counted
    //      and the relevant operation like "iterator + index", "iterator[6] " passes (if will) on it.
    _Iter&
    operator-- () {
        _pBNode = _pBNode->_prev;
        return *this;
    }
    _Iter
    operator-- (int) {
        _Iter tmp(*this);
        _pBNode = _pBNode->_prev;
        return tmp;
    }
    _Iter
    operator+ (size_t steps) const {
        _Iter foot(*this);
        while (steps--) {
            ++foot;
        }
        return foot;
    }
    _Iter
    operator- (size_t steps) const {
        _Iter foot(*this);
        while (steps--) {
            --foot;
        }
        return foot;
    }
    _Iter&
    operator+= (size_t steps) {
        while (steps--) {
            this->operator++();
        }
        return*this;
    }
    _Iter&
    operator -= (size_t steps) {
        while (steps--) {
            this->operator--();
        }
        return*this;
    }
    reference
    operator[] (size_t steps) {
        this->operator+= (steps);
        return static_cast<_Node*>(this->_pBNode)->_data;
    }
    bool
    operator == (const _Iter& other) const noexcept {
        return this->_pBNode == other._pBNode;
    }
    bool
    operator != (const _Iter& other) const noexcept {
        return this->_pBNode != other._pBNode;
    }
//    size_t
//    operator- (const _Iter& other) const noexcept {
//        size_t counter = 0;
//        for (_Iter pos(*this); pos != other; --pos) {
//            ++counter;
//        }
//        return counter;
//    }
//    bool
//    operator < (const _Iter& other) const noexcept {
//        _Iter my(*this),they(other);
//        return they - my < my - they;
//    }
};


template<typename T>
struct _ROIterator final
{
    typedef _ROIterator<T>         _ROIter;
    typedef _CycListIterator<T>              _NormalIter;
    typedef const _CListNode<T>            _Node;
    typedef const _NodeBase<T>        _BaseNode;


    // std requirements
    typedef typename std::bidirectional_iterator_tag
                                iterator_category;
    typedef T                   value_type;
    typedef ptrdiff_t           difference_type;
    typedef const T*            pointer;
    typedef const T&            reference;


    _BaseNode*    _pBNode;

    _ROIterator() = delete;

    explicit
    _ROIterator(_BaseNode* node_)
    : _pBNode(node_) {}

    explicit
    _ROIterator(const _NormalIter& nIter)
    : _pBNode(nIter._pBNode) {}

    _ROIterator(const _ROIterator&) = default;
    ~_ROIterator() = default;

    reference
    operator* () const {
        return (static_cast<_Node*>(_pBNode))->_data;//Note: data is T, not ptr
    }
    pointer
    operator-> () const {
        return &((static_cast<_Node*>(_pBNode))->_data);
    }
    ////////////

    _ROIter&
    operator++ () {
        _pBNode = _pBNode->_next;
        return *this;
    }
    _ROIter//    _ROIter& for ++iter and _ROIter for iter++
    operator++ (int) {
        _ROIter tmp(*this);
        _pBNode = _pBNode->_next;
        return tmp;
    }
    _ROIter&
    operator-- () {
        _pBNode = _pBNode->_prev;
        return *this;
    }
    _ROIter
    operator-- (int) {
        _ROIter tmp(*this);
        _pBNode = _pBNode->_prev;
        return tmp;
    }
    _ROIter
    operator+ (size_t steps) const {
        _ROIter foot(*this);
        while (steps--) {
            ++foot;
        }
        return foot;
    }
    _ROIter
    operator- (size_t steps) const {
        _ROIter foot(*this);
        while (steps--) {
            --foot;
        }
        return foot;
    }
    _ROIter&
    operator+= (size_t steps) {
        while (steps--) {
            this->operator++();
        }
        return*this;
    }
    _ROIter&
    operator -= (size_t steps) {
        while (steps--) {
            this->operator--();
        }
        return*this;
    }
    reference
    operator[] (size_t steps) {
        this->operator+= (steps);
        return static_cast<_Node*>(this->_pBNode)->_data;
    }
    bool
    operator == (const _ROIter& other) const noexcept {
        return this->_pBNode == other._pBNode;
    }
    bool
    operator != (const _ROIter& other) const noexcept {
        return this->_pBNode != other._pBNode;
    }
};


} // namespace detail


//----------------------------------------------------------------------------------
//                  algorithms
//----------------------------------------------------------------------------------
template< typename T, typename Allocator>
template <typename ROIter>
typename Starter::defType<ROIter, T, void>::type
CycList<T, Allocator>::
M_assignDispatch(ROIter left, ROIter right) {
    Iter myLeft = begin();
    Iter myRight = end();
    for (; left != right && myLeft != myRight;
            ++myLeft, ++ left) {
      //  *myLeft = *left;
      // this is what std::list does, but i thought is inappropriate
      // there is no destroying, no copy-construction
      // i promoted it as this
        _Base::M_getObjAllocator().destroy(
                        std::addressof(static_cast<Node*>(myLeft._pBNode)->_data));

        _Base::M_getObjAllocator().construct(
                        std::addressof(static_cast<Node*>(myLeft._pBNode)->_data),
                        *left);

    }
    if (myLeft == myRight) {
        splice(end(), left, right);
    } else {
        erase (myLeft, myRight);
    }
}


template< typename T, typename Allocator>
bool
CycList<T, Allocator>::
operator== (const _Self& other) const noexcept {
	if (this != & other) {
		cIter myLeft = cbegin();
		cIter myRight = cend();
		cIter othersLeft = other.cbegin();
		cIter othersRight = other.cend();
		while (*myLeft == *othersLeft
				&& myLeft != myRight
				&& othersLeft != othersRight
			  ) {
			++myLeft;
			++othersLeft;
		}
		return myLeft == myRight && othersLeft == othersRight;
	}
	return true;
}

template< typename T, typename Allocator>
void
CycList<T, Allocator>::
remove (cRef obj) {
    Iter left = begin();
    Iter right = end();
    Iter mark = end();
    Iter last = left;
    while (left != right) {
        last = left++;
        if (*last == obj) {
            if (&*last == &obj) {
                mark = last;
            }
            else {
                M_remove(last._pBNode);
            }
        }
    }
    if (mark != end()) {
        M_remove(mark._pBNode);
    }
}
template< typename T, typename Allocator>
void
CycList<T, Allocator>::
remove_if (std::function<bool(const T&)> pred) {
    Iter left = begin();
    Iter right = end();
    while (left != right) {
        Iter last = left++;
        if (pred(*last)) {
            M_remove(last._pBNode);
        }
    }
}

template< typename T, typename Allocator>
void
CycList<T, Allocator>::
merge (_Self&& other,
       std::function<bool(const T&, const T&)>
        comp = [](const T& a, const T& b)->bool{return a < b;}
       ) {
    if (this != & other) {
		Iter myLeft = begin();
		Iter myRight = end();
		Iter othersLeft = other.begin();
		Iter othersRight = other.end();
		while (myLeft != myRight && othersLeft != othersRight) {
            if (comp (*othersLeft, *myLeft)) {
                Iter next = othersLeft++;
                next._pBNode->M_transplantTo(myLeft._pBNode);
            }
            else {
                ++myLeft;
            }
        }
        while (othersLeft != othersRight) {
            Iter one = othersLeft++;
            one._pBNode->M_transplantTo(myRight._pBNode);
        }
        this->_impl.__size += other._impl.__size;
    }
}

template< typename T, typename Allocator>
void
CycList<T, Allocator>::
unique( std::function<bool(const T&, const T&)>
        equals = [](const T& a, const T& b)->bool{return a == b;}
        ) {

    if ( !empty()) {
        Iter left = begin();
        Iter right = end();
        Iter last = left;
        while (left != right) {
            last = left++;
            if(equals(*last, *left)) {
                M_remove(last._pBNode);
            }
        }
    }
}
//!< take advantage of the counter _impl.__size
template< typename T, typename Allocator>
void
CycList<T, Allocator>::
sort ( std::function<bool(const T&, const T&)>
        comp = [](const T& a, const T& b)->bool{return a < b;}
        ) {

    if (_impl.__size > 1) {
        Iter cur = begin();
        Iter node = ++begin();
        Iter last = node;
        auto unordered = _impl.__size - 1;
        while (unordered--) {
            auto ordered = _impl.__size - unordered;
            last = node++;
            while (--ordered && comp(*cur, *last)) {
                ++cur;
            }
            if (ordered) {
                last._pBNode->M_transplantTo(cur._pBNode);
            }
            cur = begin();
        }
    }
}

template< typename T, typename Allocator>
inline size_t
CycList<T, Allocator>::
M_hashCode (std::true_type) const noexcept {

    cIter left = cbegin();
	cIter right = cend();
	std::hash<T> hasher;//!< for arithmetic type, use std::hash
	size_t hashCode = hasher(*left);
	++left;
	for (; left != right; ++left) {
		hashCode = StarterHash::hash_combine(hashCode, hasher(*left));
	}
	return hashCode;
}

//  uint32_t hsieh_hash32_buf(const void* buf, int len)
/*
 *  hash function is from facebook folly::hash
 * algorithm from
 *      Paul Hsieh: http://www.azillionmonkeys.com/qed/hash.html
 */
template< typename T, typename Allocator>
size_t
CycList<T, Allocator>::
M_hashCode (std::false_type) const noexcept {
//        do not use const_cast, static_cast or reinterpret_cast
//      like this:
//  auto hash32bit = [](cIter iter)->uint32_t{return StarterHash::hsieh_hash32_buf(const_cast<void*>(std::addressof(*iter)), sizeof(T));};
//          they will not work even if qualifier 'const' is removed
    auto hash32bit = [](cIter iter)->uint32_t{
        return
        StarterHash::hsieh_hash32_buf((void*)std::addressof(*iter), sizeof(T));
    };

    cIter left = cbegin();
	cIter right = cend();

	uint32_t hashCode = hash32bit(left);
    ++left;
    for (; left != right; ++left) {
		hashCode = StarterHash::hash_combine(hashCode, hash32bit(left));
	}
	return hashCode;
}




