/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */
//
#ifndef _STARTER_HYBIRD_TREE_BUCKET_H_
#define _STARTER_HYBIRD_TREE_BUCKET_H_


#include "StarterCfgDef.h"

#include "RBTree.h"

__NAMESPACE_STARTER___BEGIN__
 // Implementation of a small rbtree as the bucket for hash container

namespace detail_
{
/////////////////////////////////////
//            tiny rb-tree
/////////////////////////////////////
template<typename N, typename Comp>
class HashContainerTreeBucket
{
public:

    typedef HashContainerTreeBucket<N, Comp>                Self_;
    typedef ::Starter::BaseNode                             BaseNode;
    typedef  ::Starter::detail_::RBTreeColor                Color;

    typedef Comp                                            Comparator;
    typedef N                                               Node;
    typedef typename Node::KeyType                          KeyType;
    typedef typename Node::ValueType                        ValueType;

    typedef Node&                                           reference;
    typedef const Node&                                     const_reference;
    typedef Node*                                           pointer;
    typedef const Node*                                     const_pointer;

    typedef ::Starter::detail_::RBTreeIterator<Node>                   iterator;
    typedef ::Starter::detail_::RBTreeConstIterator<Node>              const_iterator;

//=================================
//              ALL DATA
    BaseNode*    head_ = nullptr;
//=================================
public:
    HashContainerTreeBucket(const Self_& other) {
        this->head_ = other.head_;
        this->head_ = other.head_;
        this->head_->left_ = M_clone(other.head_->left_, head_);
        this->head_->right_ = M_clone(other.head_->right_, head_);
    }
    HashContainerTreeBucket() = default;

    HashContainerTreeBucket(Self_&& other) {
        swap(other);
    }
    ~HashContainerTreeBucket() {
        S_strip(head_);
    }

public:
//-----------------------------------------------------------------------------
//                  operator
//-----------------------------------------------------------------------------
    Self_& operator= (const Self_& other) {
        if(likely(this != &other)) {
            this->clear();
            if(likely(0 != other.size_)) {
                this->head_ = other.head_;
                this->head_->left_ = M_clone(other.head_->left_, head_);
                this->head_->right_ = M_clone(other.head_->right_, head_);
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
    bool operator== (const Self_& other) const noexcept {

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
    void swap(Self_& other) noexcept {
        BaseNode* tmpp = this->head_->left_;
        this->head_->left_ = other.head_->left_;
        other.head_->left_ = tmpp;

        tmpp = this->head_->right_;
        this->head_->right_ = other.head_->right_;
        other.head_->right_ = tmpp;

        tmpp = this->head_;
        this->head_ = other.head_;
        other.head_ = tmpp;
    }

public:
    template<typename rcNode>
    static BaseNode*
    S_creatNode(rcNode&& onode, const BaseNode* parent) { // right ref or other qualifiers e.g. const violate ...

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

    static BaseNode*
    S_clone(const BaseNode* node, const BaseNode* parent) {
        if (unlikely(nullptr != node)) {
            BaseNode* newone =
                S_creatNode( *(static_cast<Node*>(const_cast<BaseNode*>(node))), parent);
            newone->left_ = S_clone(node->left_, newone);
            newone->right_ = S_clone(node->right_, newone);
            newone->color_ = node->color_;
            return newone;
        }
        else { return nullptr;}
    }

    static BaseNode*
    S_leftMost(BaseNode*) noexcept;
    static BaseNode*
    S_rightMost(BaseNode*) noexcept;

    void M_rotateLeft(BaseNode* const node) noexcept  {

        BaseNode* const subj = node->right_;
        node->right_ = subj->left_;/////????????????

        if (nullptr != subj->left_) {
             subj->left_->parent_ = node;
        }
        subj->parent_ = node->parent_;
        if (head_ == node) {
            head_ = subj;
        }
        else if (node == node->parent_->left_) {
            node->parent_->left_ = subj;
        }
        else { node->parent_->right_ = subj; }

        subj->left_ = node;
        node->parent_ = subj;
    }

    void M_rotateRight(BaseNode*const node) noexcept {

        BaseNode* const subj = node->left_;
        node->left_ = subj->right_;

        if (nullptr != subj->right_) {
             subj->right_->parent_ = node;
        }
        subj->parent_ = node->parent_;
        if (head_ == node) {
            head_ = subj;
        }
        else  if (node == node->parent_->right_) {
            node->parent_->right_ = subj;
        }
        else { node->parent_->left_ = subj; }

        subj->right_ = node;
        node->parent_ = subj;
    }

    void M_rebalanceAfterInsertion(BaseNode* pnode) noexcept {

        while (pnode != head_
                 && Color::Red == pnode->parent_->color_) {

            BaseNode* const grandpa = pnode->parent_->parent_;
            if (pnode->parent_ == grandpa->left_) {

                BaseNode* const subj = grandpa->right_;
                if (nullptr != subj
                     && Color::Red == subj->color_) {

                    pnode->parent_->color_ = Color::Black;
                    subj->color_ = Color::Black;
                    grandpa->color_ = Color::Red;
                    pnode = grandpa;
                }
                else {
                    if (pnode == pnode->parent_->right_) {
                        pnode = pnode->parent_;
                        M_rotateLeft(pnode);
                    }
                    pnode->parent_->color_ = Color::Black;
                    grandpa->color_ = Color::Red;
                    M_rotateRight(grandpa);
                }
            } // one side
            else {
                BaseNode* const subj = grandpa->left_;
                if (nullptr != subj
                     && Color::Red == subj->color_) {

                    pnode->parent_->color_ = Color::Black;
                    subj->color_ = Color::Black;
                    grandpa->color_ = Color::Red;
                    pnode = grandpa;
                }
                else {
                    if (pnode == pnode->parent_->left_) {
                        pnode = pnode->parent_;
                        M_rotateRight(pnode);
                    }
                    pnode->parent_->color_ = Color::Black;
                    grandpa->color_ = Color::Red;
                    M_rotateLeft(grandpa);
                }
            } // another side
        } // while
        head_->color_ = Color::Black;
    } // endp


    BaseNode* M_rebalanceAndDetach(BaseNode* const node) noexcept {

    BaseNode* subj = node;
    BaseNode* kid = 0;
    BaseNode* sparent = 0;
//------------------------------------------
//              detach
//------------------------------------------
    if (nullptr == subj->left_) {
        kid = subj->right_;
    }
    else if (nullptr ==  subj->right_){
        kid = subj->left_;    // kid is not null.
    }
    else {
        subj = subj->right_;
        while (nullptr != subj->left_) {
            subj = subj->left_;
        }
        kid = subj->right_;
    }

    if (subj != node) {
        node->left_->parent_ = subj;
        subj->left_ = node->left_;
        if (subj != node->right_)
        {
            sparent = subj->parent_;
            if (nullptr != kid) {
                kid->parent_ = subj->parent_;
            }
            subj->parent_->left_ = kid;   // subj must be a child of left_
            subj->right_ = node->right_;
            node->right_->parent_ = subj;
        }
        else { sparent = subj;}


        if (head_ == node) {
            head_ = subj;
        }
        else if (node->parent_->left_ == node) {
            node->parent_->left_ = subj;
        }
        else { node->parent_->right_ = subj;}

        subj->parent_ = node->parent_;
        Color tmpc = subj->color_;
        subj->color_ = node->color_;
        node->color_ = tmpc;
        subj = node;
    }
    else {
        sparent = subj->parent_;
        if (nullptr != kid) {
            kid->parent_ = subj->parent_;
        }

        if (head_ == node) {
            head_ = kid;
        }
        else if (node->parent_->left_ == node) {
            node->parent_->left_ = kid;
        }
        else { node->parent_->right_ = kid;}
    }
//------------------------------------------
//              balacnce
//------------------------------------------
    if (subj->color_ != Color::Red) {
        while (kid != head_
                && (nullptr == kid|| kid->color_ == Color::Black)) {
            if (kid == sparent->left_) {
                BaseNode* tmp = sparent->right_;
                if ( Color::Red == tmp->color_) {

                    tmp->color_ = Color::Black;
                    sparent->color_ = Color::Red;
                    M_rotateLeft(sparent);
                    tmp = sparent->right_;
                }

                if ((nullptr == tmp->left_ || tmp->left_->color_ == Color::Black)
                    && (nullptr == tmp->right_ || tmp->right_->color_ == Color::Black)) {
                    tmp->color_ = Color::Red;
                    kid = sparent;
                    sparent = sparent->parent_;
                }
                else {
                    if ( nullptr == tmp->right_
                            || Color::Black == tmp->right_->color_) {

                        tmp->left_->color_ = Color::Black;
                        tmp->color_ = Color::Red;
                        M_rotateRight(tmp);
                        tmp = sparent->right_;
                    }
                    tmp->color_ = sparent->color_;
                    sparent->color_ = Color::Black;
                    if (nullptr != tmp->right_) {
                        tmp->right_->color_ = Color::Black;
                    }
                    M_rotateLeft(sparent);
                    break;
                }
            }
            else {
                BaseNode* tmp = sparent->left_;
                if ( Color::Red == tmp->color_) {

                    tmp->color_ = Color::Black;
                    sparent->color_ = Color::Red;
                    M_rotateRight(sparent);
                    tmp = sparent->left_;
                }

                if ((nullptr == tmp->right_ || tmp->right_->color_ == Color::Black)
                    && (nullptr == tmp->left_ || tmp->left_->color_ == Color::Black)) {
                    tmp->color_ = Color::Red;
                    kid = sparent;
                    sparent = sparent->parent_;
                }
                else {
                    if ( nullptr == tmp->left_
                            || Color::Black == tmp->left_->color_) {

                        tmp->right_->color_ = Color::Black;
                        tmp->color_ = Color::Red;
                        M_rotateLeft(tmp);
                        tmp = sparent->left_;
                    }
                    tmp->color_ = sparent->color_;
                    sparent->color_ = Color::Black;
                    if (nullptr != tmp->left_) {
                        tmp->left_->color_ = Color::Black;
                    }
                    M_rotateRight(sparent);
                    break;
                }
            } // else
        } // while
        if (nullptr != kid) kid->color_ = Color::Black;
    }
    return subj;
}


public:

    template<typename rcNode>
        typename std::enable_if<std::is_same<typename std::decay<rcNode>::type,
                                Node>::value,
        iterator>::type
     exclusiveInsertion(rcNode&& onode) {

     static_assert(std::is_same<
                   typename std::decay<rcNode>::type,
                   Node>::value, "must be [const] Node with the types");

	BaseNode* parent = head_;
	BaseNode* cur    = parent;
	bool is_left = true;
	while (nullptr != cur) {
		parent = cur;
		is_left = comp_((std::forward<rcNode>(onode)).getKey(),
                        static_cast<Node*>(cur)->getKey());
		if(is_left) {
			cur = cur->left_;
		} else {
			cur = cur->right_;
		}
	}

	if (is_left) {
    	cur = S_creatNode(std::forward<rcNode>(onode), parent);
		parent->left_ = cur;
		if(unlikely(head_ == parent)) { ///////??? easier way ?
			head_->parent_ = cur;
		}
        M_rebalanceAfterInsertion(cur);
	}
    else {
        if (comp_(static_cast<Node*>(parent)->getKey(),
                  (std::forward<rcNode>(onode)).getKey() )) {

            cur = S_creatNode(std::forward<rcNode>(onode), parent);
            parent->right_ = cur;
            M_rebalanceAfterInsertion(cur);
        }
        else {
            cur = parent;
        }
	}
	return iterator(cur);
}

    template<typename rcNode>
        typename std::enable_if<std::is_same<typename std::decay<rcNode>::type,
                                Node>::value,
        iterator>::type
    duplicatedInsertion(rcNode&& onode){

     static_assert(std::is_same<
                   typename std::decay<rcNode>::type,
                   Node>::value, "must be [const] Node with the types");

	BaseNode* parent = head_;
	BaseNode* cur    = parent;
	bool is_left = true;
	while (nullptr != cur) {
		parent = cur;
		is_left = comp_((std::forward<rcNode>(onode)).getKey(),
                        static_cast<Node*>(cur)->getKey());
		if(is_left) {
			cur = cur->left_;
		} else {
			cur = cur->right_;
		}
	}

	if (is_left) {
    	cur = S_creatNode(std::forward<rcNode>(onode), parent);
		parent->left_ = cur;
		if(unlikely(head_ == parent)) { ///////??? easier way ?
			head_->parent_ = cur;
		}
	}
    else {
        cur = S_creatNode(std::forward<rcNode>(onode), parent);
        parent->right_ = cur;
	}
    M_rebalanceAfterInsertion(cur);
	return iterator(cur);
}

    void erase(iterator iter) {
        S_destroyNode(
            M_rebalanceAndDetach(iter.node_)
            );
    }

    void erase(const KeyType& k) {
        iterator iter = lower_bound(k);
        if(unlikely(iter.node_ == head_)) {
            throw std::out_of_range("HashContainerTreeBucket::erase(), cannot find the key");
        }
        else { erase(iter);  }
    }

    iterator detach(iterator iter) noexcept {
        return iterator(
            M_rebalanceAndDetach(iter.node_)
                );
    }

    iterator detach(const KeyType& thekey) noexcept {
        return detach(lower_bound(thekey));
    }

    static void S_strip(BaseNode*& node)  {
        if (nullptr != node) {
            if (nullptr != node->left_) {
                S_strip(node->left_);
            }
            if (nullptr != node->right_) {
                S_strip(node->right_);
            }
            S_destroyNode(node);
            node = nullptr;
        }
    }
//-----------------------------------------------------------------------------
//                  iterator
//-----------------------------------------------------------------------------
public:

    iterator begin() noexcept
    {   return iterator(head_);}

    iterator end() noexcept
    {   return iterator(nullptr); }

    const_iterator cbegin() const noexcept
    {   return const_iterator(head_); }

    const_iterator cend() const noexcept
    { return const_iterator(nullptr); }

    iterator upper_bound(const KeyType& thekey) noexcept{
        BaseNode* curr = head_;
        BaseNode* prev = head_;
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
        BaseNode* curr = head_;
        BaseNode* prev = head_;
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
        BaseNode* curr = head_;
        BaseNode* prev = head_;
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
        BaseNode* curr = head_;
        BaseNode* prev = head_;
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
        if (unlikely(iter.node_ == head_) // empty tree or lower_bound lookup failure
                    || unlikely(comp_(iter.node_->getKey(), thekey))) {
            throw std::out_of_range("RBTRee::at()");
        }
        return static_cast<Node*>(iter.node_)->value;
    }
    const_reference at (const KeyType& thekey) const noexcept {
        iterator iter = lower_bound(thekey);
        if (unlikely(iter.node_ == head_) // empty tree or lower_bound lookup failure
                    || unlikely(comp_(iter.node_->getKey(), thekey))) {
            throw std::out_of_range("RBTRee::at()");
        }
        return static_cast<Node*>(iter.node_)->value;
    }
};

}// namespace detail_

__NAMESPACE_STARTER___END__
#endif // _STARTER_HYBIRD_TREE_BUCKET_H_
