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


            //    #define BW_RBTREE_DEBUG_

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

template <typename ValueType>
struct RBTreeIterator;

template <typename ValueType>
struct RBTreeConstIterator;

}//namespace detail_


struct BaseNode
{
    detail_::RBTreeColor    color_;
    BaseNode*               parent_;
    BaseNode*               left_;
    BaseNode*               right_;
};

//        head_           the sentry
//        head_.left_     first element, the root
//        head_.right_    right most node, the biggest one
//        head_.parent_

//        should you use RBTree, custom the Node,
//        for example, see TreeMap, TreeSet

template <typename Node_, typename Comparator>
class RBTree : public Starter::RelOps<RBTree<Node_, Comparator>>
{

    static_assert(std::is_base_of<Starter::BaseNode, Node_>::value,
                    "customed node must be derived from \"Starter::BaseNode\"\n");


    static_assert(std::is_same<typename Node_::KeyType,
                              //  typename std::result_of<Node_::getKey>::type
                              decltype(std::declval(Node_::getKey()))
                    >::value,
        " customed node must define type \"KeyType\" and define function \"getKey\"\n");




public:
    typedef Starter::BaseNode                               BaseNode;
    typedef typename detail_::RBTreeColor                   Color;

    typedef Node_                                           Node;
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
            std::is_same<KeyType, decltype(foreign_iterator()->first)>::value
        && std::is_same<ValueType, decltype(foreign_iterator()->second)>::value

//            std::is_same<typename std::map<KeyType, ValueType>::iterator, foreign_iterator>::value
//        or std::is_same<typename std::multimap<KeyType, ValueType>::iterator, foreign_iterator>::value
//        or std::is_same<typename std::unordered_map<KeyType, ValueType>::iterator, foreign_iterator>::value
//        or std::is_same<typename std::unordered_multimap<KeyType, ValueType>::iterator, foreign_iterator>::value

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
        return *this;
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
        return *this;
    }

    bool operator== (const Self_& other) const noexcept {
        if (size_ == other.size_) {
            auto iter = this->cbegin();
            auto iter2 = other.cbegin();
            for (;iter != cend() && iter2 != other.cend();
                    ++iter, ++iter2) {
                if ( comp_(iter->getKey(), iter2->getKey())
                        || comp_(iter2->getKey(), iter->getKey())) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
//        auto cp = [this](const_iterator a , const_iterator b)->bool {
//            return !comp_(a->getKey(), b->getKey())
//                    && !comp_(b->getKey(), a->getKey());
//        };
//        return std::lexicographical_compare(
//                                this->cbegin(), this->cend(),
//                                other.cbegin(), other.cend(),
//                                cp);
    bool operator< (const Self_& other) const noexcept {
        for(auto i = this->cbegin(), j = other.cbegin();
                (j != other.cend() && i != this->cend());
                        ++i, ++j) {
            if ( !comp_(i->getKey(), j->getKey())) {
                return false;
            }
        }
        return true;
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

    template<typename rcNode>
        typename std::enable_if<std::is_same<typename std::decay<rcNode>::type,
                                Node>::value,
        iterator>::type
     exclusiveInsertion(rcNode&& onode);

    template<typename rcNode>
        typename std::enable_if<std::is_same<typename std::decay<rcNode>::type,
                                Node>::value,
        iterator>::type
    duplicatedInsertion(rcNode&& onode);


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


    std::pair<const_iterator, const_iterator>
    equal_range(const KeyType& k) const noexcept {
        const BaseNode* cur = head_.parent_;
        const BaseNode* parent = addressOf(head_);
        std::pair<const_iterator, const_iterator> pr;
        while (nullptr != cur) {
            if (comp_(k, static_cast<const Node*>(cur)->getKey())) {
                parent = cur;
                cur = cur->left_;
            }
            else if(comp_(static_cast<const Node*>(cur)->getKey(), k)) {
                cur = cur->right_;
            }
            else { // find the same key!
                auto j = cur;
                while(nullptr != j->right_
                    && !comp_(static_cast<const Node*>(cur)->getKey(),
                                static_cast<const Node*>(j->right_)->getKey())) {
                    j = j->right_;
                }
                pr.first = const_iterator(cur);
                pr.second = const_iterator(j);
                auto iter = const_iterator(j);
                if(unlikely(1 == size_)) {
                    pr.second = cbegin();//const_iterator(addressOf(head_));
                }
                else {
                    ++iter;
                    pr.second = iter;
                }
                return pr;
            }
        }
        pr.first = const_iterator(parent);
        pr.second = const_iterator(parent);
        return pr;
    }

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
    { return const_iterator(addressOf(head_)); }

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


    iterator find(const KeyType& thekey) noexcept {
        auto i = lower_bound(thekey);
        if (i = end()
            || comp_(thekey, i->getKey()) ) {

            return end();
        }
        else {
            return i;
        }
    }
    const_iterator find(const KeyType& thekey) const noexcept {
        auto i = lower_bound(thekey);
        if (i = end()
            || comp_(thekey, i->getKey()) ) {
            return end();
        }

        else {
            return i;
        }
    }
//-----------------------------------------------------------------------------
//                  getter
//-----------------------------------------------------------------------------
    reference at (const KeyType& thekey) noexcept {
        iterator iter = lower_bound(thekey);
        if (unlikely(iter.node_ == addressOf(head_)) // empty tree or lower_bound lookup failure
                    || unlikely(comp_(iter.node_->getKey(), thekey))) {
            throw std::out_of_range("RBTRee::at()");
        }
        return static_cast<Node*>(iter.node_)->value;
    }

    const_reference at (const KeyType& thekey) const noexcept {
        iterator iter = lower_bound(thekey);
        if (unlikely(iter.node_ == addressOf(head_)) // empty tree or lower_bound lookup failure
                    || unlikely(comp_(iter.node_->getKey(), thekey))) {
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

    const BaseNode* data() const noexcept
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

#include "RBTree-inl.h"
__NAMESPACE_STARTER___END__


#endif // _STARTER_RBTREE_H_



