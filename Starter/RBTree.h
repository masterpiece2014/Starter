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


#ifndef _STARTER_RBTREE_H_
#define _STARTER_RBTREE_H_


#ifdef __GUNC__
#pragma GCC system_header
#endif




                // #define BW_RBTREE_DEBUG_

#ifdef BW_RBTREE_DEBUG_

#include <utility>
#include <algorithm>
#include <iostream>
using namespace std;
#include "bwtest/bwtest.h"
#define __YELL__    put_out << __FUNCTION__ << "  " << __LINE__ << std::endl << std::endl;
#else
#define __YELL__
#endif




#include <functional>// less
#include <new>
#include <initializer_list>
#include <map>//
#include <unordered_map>//
#include<type_traits>
#include <iterator>
#include <algorithm>


#include "StarterCfgDef.h"
#include "RelOps.h"

__NAMESPACE_STARTER___BEGIN__


namespace detail_
{

enum RBTreeColor{Red = false, Black = true};

struct Nodebase
{
    RBTreeColor color_;
    Nodebase* parent_;
    Nodebase* left_;
    Nodebase* right_;
};


template <typename ValueType>
struct RBTreeIterator;

template <typename ValueType>
struct RBTreeConstIterator;

}//namespace detail_

//        head_           the sentry
//        head_.left_     first element, the root
//        head_.right_    right most node, the biggest one
//        head_.parent_

//        should you use RBTree, custom the Node,
//        for example, see Map, Set

template <typename Node,
            typename Comparator = std::less<typename Node::KeyType>
            >
class RBTree : public Starter::RelOps<RBTree<Node, Comparator>>
{
public:
    typedef detail_::Nodebase                               BaseNode;
    typedef typename detail_::RBTreeColor                   Color;
    typedef typename Node::KeyType                          KeyType;
    typedef typename Node::ValueType                        ValueType;

    typedef Node&                                           reference;
    typedef const Node&                                     const_reference;
    typedef Node*                                           pointer;
    typedef const Node*                                     const_pointer;

    typedef detail_::RBTreeIterator<Node>                   iterator;
    typedef detail_::RBTreeConstIterator<Node>              const_iterator;


protected:
    typedef RBTree<Node, Comparator>                            Self_;

    typedef std::function<bool(const KeyType&, const KeyType&)> KeysCompFuncType;


    // to cope with std::map, std::unordered map, ...
    template <typename foreign_iterator>
    struct compatible_iterator
    {
        enum{ value =
            std::is_same<typename std::map<KeyType, ValueType>::iterator, foreign_iterator>::value
        or std::is_same<typename std::multimap<KeyType, ValueType>::iterator, foreign_iterator>::value
        or std::is_same<typename std::unordered_map<KeyType, ValueType>::iterator, foreign_iterator>::value
        or std::is_same<typename std::unordered_multimap<KeyType, ValueType>::iterator, foreign_iterator>::value
        };
    };

//=================================
//              ALL DATA
protected:
    size_t              size_;
    KeysCompFuncType    comp_;
    BaseNode            head_;
//=================================

public:
    RBTree()
    :   size_(0),
        comp_(Comparator()) {
        head_.parent_   = nullptr;
        head_.color_    = Color::Red;
    }
    RBTree(KeysCompFuncType comp)
    :   size_(0),
        comp_(comp){

        head_.parent_   = nullptr;
        head_.color_    = Color::Red;
    }

    RBTree(const Self_& other) {
        if (likely(0 != other.size_)) {
            this->size_ = other.size_;
            this->comp_ = other.comp_;
            this->head_.color_ = other.head_.color_;
            this->head_.parent_ = S_creatNode(*(static_cast<Node*>(other.head_.parent_)),
                                              addressOf(head_));

            this->head_.parent_->color_ = other.head_.parent_->color_;
            this->head_.parent_->left_ = M_clone(other.head_.parent_->left_, head_.parent_);
            this->head_.parent_->right_ = M_clone(other.head_.parent_->right_, head_.parent_);

            this->head_.left_ = S_leftMost(head_.parent_);
            this->head_.right_ = S_rightMost(head_.parent_);
        }
    }
    RBTree(Self_&& other) {
        if (likely(0 != other.size_)) {
            swap(other);
        }
    }

    ~RBTree() {
        clear();
    }

public:
//-----------------------------------------------------------------------------
//                  operator
//-----------------------------------------------------------------------------
    Self_& operator= (const Self_& other) {
        if(likely(this != &other)) {
            this->clear();
            if(likely(0 != other.size_)) {
                this->size_ = other.size_;
                this->comp_ = other.comp_;
                this->head_.color_ = other.head_.color_;
                this->head_.parent_ = S_creatNode(*(static_cast<Node*>(other.head_.parent_)),
                                              addressOf(head_));

                this->head_.parent_->color_ = other.head_.parent_->color_;
                this->head_.parent_->left_ = M_clone(other.head_.parent_->left_, head_.parent_);
                this->head_.parent_->right_ = M_clone(other.head_.parent_->right_, head_.parent_);

                this->head_.left_ = S_leftMost(head_.parent_);
                this->head_.right_ = S_rightMost(head_.parent_);
            }
        }
    }
    Self_& operator= (Self_&& other) {
        if(likely(this != &other)) {
            this->clear();
            swap(other);
        }
        return *this;
    }
    Self_& operator= (std::initializer_list<Node> init) {
        this->clear();
        for(auto iter = init.begin(); iter != init.end(); ++iter) {
            insert(*iter);
        }
    }

    bool operator== (const Self_& other) const noexcept {
        auto cp = [this](iterator a , iterator b)->bool {
            return not comp_(a->key, b->key)
                    and not comp_(b->getKey(), a->getKey());
        };
        return std::lexicographical_compare(
                                this->begin(), this->end(),
                                other.begin(), other.end(),
                                cp);
    }
    bool operator< (const Self_& other) const noexcept {
        for(auto i = begin(), j = other.begin(); i != end(); ++i, ++j) {
            if (comp_(i->key, j->key))
                return true;
            else if((comp_(j->getKey(), i->getKey())));
                return false;
        }
        return false;
    }

    void swap(Self_&) noexcept;

protected:

    template<typename rcNode>
    static BaseNode*
    S_creatNode(rcNode&& onode, BaseNode* parent) { // right ref or other qualifiers e.g. const violate ...

        auto node = static_cast<Node*>(::operator new(sizeof(Node)));
        try {
            ::new(static_cast<void*>(node)) Node (std::forward<rcNode>(onode));
        }
        catch(...) {
            ::operator delete(node);
            throw;
        }
        node->left_ = nullptr;
        node->right_ = nullptr;
        node->parent_ = parent;
        node->color_ = Color::Red;

        return static_cast<BaseNode*>(node);
    }

    static void
    S_destroyNode(BaseNode* node)  {
        Node* const bnode = static_cast<Node*>(node);
        bnode->~Node();
        ::operator delete(bnode);
    }

    BaseNode* M_clone(const BaseNode* node, BaseNode* parent) {
        if (nullptr != node) {
            BaseNode* newone =
                S_creatNode( *(static_cast<Node*>(const_cast<BaseNode*>(node))), parent);
            newone->left_ = M_clone(node->left_, newone);
            newone->right_ = M_clone(node->right_, newone);
            newone->color_ = node->color_;
            return newone;
        }
        else { return nullptr;}
    }

    static BaseNode*
    S_leftMost(BaseNode*) noexcept;
    static BaseNode*
    S_rightMost(BaseNode*) noexcept;

    void M_rotateLeft(BaseNode* const) noexcept;
    void M_rotateRight(BaseNode*const) noexcept;

    void M_rebalanceAfterInsertion(BaseNode*) noexcept;
    BaseNode* M_rebalanceAndDetach(BaseNode* const) noexcept;

public:

//	template<typename...rcNode>
//	iterator insert(rcNode&&... onode);

template<typename rcNode>
iterator insert(rcNode&& onode)
 {
     static_assert(std::is_same<
                   typename std::decay<rcNode>::type,
                   Node>::value, "must be [const] Node with the types");
	BaseNode* parent = &head_;
	BaseNode* cur    = head_.parent_;
	bool isLeftLeaf = true;
	while(nullptr != cur) {
		parent = cur;
		if(comp_(  (std::forward<rcNode>(onode)).getKey(),
                static_cast<Node*>(cur)->getKey())) {

			cur = cur->left_;
			isLeftLeaf = true;
		} else {
			cur = cur->right_;
			isLeftLeaf = false;
		}
	}
	cur = S_creatNode(std::forward<rcNode>(onode), parent);

	if(isLeftLeaf) {
		parent->left_ = cur;
		if(unlikely(&head_ == parent)) { ///////??? easier way ?
			head_.parent_ = cur;
			head_.right_ = cur;
		} else if(parent == head_.left_) {
			head_.left_ = cur;
		}
	}
	else {
		parent->right_ = cur;
		if(parent == head_.right_) {
			head_.right_ = cur;
		}
	}
	M_rebalanceAfterInsertion(cur);
	++this->size_;
	return iterator(cur);
}



    void erase(iterator iter) {
        --size_;
        S_destroyNode(
            M_rebalanceAndDetach(iter.node_)
            );
    }

    void erase(const KeyType& k) {
        iterator iter = lower_bound(k);
        if(unlikely(iter.node_ == &head_)) {
            throw std::out_of_range("RBTree::erase(), cannot find the key");
        }
        else { erase(iter);  }
    }

    iterator detach(iterator iter) noexcept {
      --size_;
        return iterator(
            M_rebalanceAndDetach(iter.node_)
                );
    }

    iterator detach(const KeyType& thekey) noexcept {
        return detach(lower_bound(thekey));
    }
protected:
    void static
    S_forward_iterate(BaseNode* node, std::function<void(const Node*)> visit);

    void static
    S_forward_iterate(BaseNode* node, std::function<void(Node*)> visit);

public:
    //  read-only
    void foreach(std::function<void(const Node*)> visit) const
    {   S_forward_iterate(head_.parent_, visit);}

    void foreach(std::function<void(Node*)> visit)
    {   S_forward_iterate(head_.parent_, visit); }

protected:
    static void S_strip(BaseNode*& node);
public:
    void clear() {
        S_strip(head_.parent_);
        this->size_ = 0;
    }

    void setComparator(KeysCompFuncType comp) noexcept {
        if (0 == this->size_) {
            this->comp_ = comp;
        }
        else {
            throw::std::logic_error("setComparator()::cannot set comparator after initialization.");
        }
    }

public:
    size_t size() const noexcept
    { return this->size_; }

    bool empty() const noexcept
    { return 0 == this->size_; }

    size_t count(const KeyType& k) const noexcept
    {   return ( end() == lower_bound(k) ? 0 : 1 ); }

//-----------------------------------------------------------------------------
//                  iterator
//-----------------------------------------------------------------------------
public:
    iterator root() noexcept
    { return iterator(head_.parent_); }

    const_iterator root() const noexcept
    { return const_iterator(head_.parent_); }

    iterator begin() noexcept
    {   return iterator(head_.left_);}

    iterator end() noexcept
    {   return iterator(addressOf(this->head_)); }

    const_iterator cbegin() const noexcept
    {   return const_iterator(head_.left_); }

    const_iterator cend() const noexcept
    { return const_iterator(addressOf(this->head_)); }

    iterator max() noexcept
    {   return iterator(head_.right_);}

    const_iterator max() const noexcept
    {   return const_iterator(head_.right_);}

    iterator upper_bound(const KeyType& thekey) noexcept{
        BaseNode* curr = head_.parent_;
        BaseNode* prev = addressOf(head_);
        while (nullptr != curr) {
            if (comp_(thekey, static_cast<Node*>(curr)->getKey())) {
                prev = curr;
                curr = curr->left_;
            }
            else {
                curr = curr->right_;
            }
        }
        return iterator(prev);
    }

    iterator lower_bound(const KeyType& thekey) noexcept {
        BaseNode* curr = head_.parent_;
        BaseNode* prev = addressOf(head_);
        while (nullptr != curr) {
            if (not comp_(static_cast<Node*>(curr)->getKey(), thekey)) {
                prev = curr;
                curr = curr->left_;
            }
            else {
                curr = curr->right_;
            }
        }
        return iterator(prev);
    }
    const_iterator upper_bound(const KeyType& thekey) const noexcept {
        BaseNode* curr = head_.parent_;
        BaseNode* prev = addressOf(head_);
        while (nullptr != curr) {
            if (comp_(thekey, static_cast<Node*>(curr)->getKey())) {
                prev = curr;
                curr = curr->left_;
            }
            else {
                curr = curr->right_;
            }
        }
        return iterator(prev);
    }

    const_iterator lower_bound(const KeyType& thekey) const noexcept {
        BaseNode* curr = head_.parent_;
        BaseNode* prev = addressOf(head_);
        while (nullptr != curr) {
            if (not comp_(static_cast<Node*>(curr)->getKey(), thekey)) {
                prev = curr;
                curr = curr->left_;
            }
            else {
                curr = curr->right_;
            }
        }
        return iterator(prev);
    }
//-----------------------------------------------------------------------------
//                  getter
//-----------------------------------------------------------------------------
    reference at (const KeyType& thekey) noexcept {
        iterator iter = lower_bound(thekey);
        if (unlikely(iter.node_ == addressOf(head_)) // empty tree or lower_bound lookup failure
                    || unlikely(comp_(iter.node_->key, thekey))) {
            throw std::out_of_range("RBTRee::at()");
        }
        return static_cast<Node*>(iter.node_)->value;
    }

    const_reference at (const KeyType& thekey) const noexcept {
        iterator iter = lower_bound(thekey);
        if (unlikely(iter.node_ == addressOf(head_)) // empty tree or lower_bound lookup failure
                    || unlikely(comp_(iter.node_->key, thekey))) {
            throw std::out_of_range("RBTRee::at()");
        }
        return static_cast<Node*>(iter.node_)->value;
    }

    reference front() noexcept
    {   return static_cast<Node*>(head_.left_)->value;  }

    reference back() noexcept
    {   return static_cast<Node*>(head_.right_)->value; }

    const_reference front() const noexcept
    {   return static_cast<Node*>(head_.left_)->value;  }

    const_reference back() const noexcept
    {   return static_cast<Node*>(head_.right_)->value; }

    static int height(iterator theroot) noexcept {
        return (nullptr == theroot.node_ ? -1
                :  (1 + std::max(height(iterator(theroot.node_->left_)),
                            height(iterator(theroot.node_->right_))))
                );
    }

    static int black_count(iterator theleaf) noexcept {
        if (nullptr == theleaf.node_) {
            return 0;
        }
        else {
            int blackcount = Color::Black == theleaf.node_->color_ ?
                                1 : 0;
            return root() == theleaf ?
                blackcount : blackcount + black_count(iterator(theleaf.node_->parent_));
        }
    }

    BaseNode* data() noexcept
    {   return head_.parent_; }

#ifdef BW_RBTREE_DEBUG_
public:

    void down_iterate(BaseNode* node,
                std::function<void(Node*)> visit)  {


        put_out << "\n ( root:";
        visit(static_cast<Node*>(node));

        if (node->left_) {

            put_out << " left: ";
            down_iterate(node->left_, visit);
        }

        if (node->right_) {
            put_out << " right: ";
            down_iterate(node->right_, visit);
        }

        put_out << ')';//<<  std::endl;

    }
#endif // debug

};

#include "rbtree-inl.h"
__NAMESPACE_STARTER___END__


#endif // _STARTER_RBTREE_H_



