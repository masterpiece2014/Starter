/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */

#ifndef _STARTER_RBTREE_H_
#error this file is the partial implementation of RBTree\
        it may only be included by rbtree.h\
        and cannot be used separately

#endif

// Implementation of rbtree.h static functions and iterators

template <typename Node, typename Comparator>
inline void
RBTree<Node, Comparator>::
    swap(Self_& other) noexcept {
        size_t tmps = this->size_;
        this->size_ = other.size_;
        other.size_ = tmps;

        KeysCompFuncType tmpf = this->comp_;
        this->comp_ = other.comp_;
        other.comp_ = tmpf;

        Color tmpc = this->head_.color_;
        this->head_.color_ = other.head_.color_;
        other.head_.color_ = tmpc;

        BaseNode* tmpp = this->head_.left_;
        this->head_.left_ = other.head_.left_;
        other.head_.left_ = tmpp;

        tmpp = this->head_.right_;
        this->head_.right_ = other.head_.right_;
        other.head_.right_ = tmpp;

        tmpp = this->head_.parent_;
        this->head_.parent_ = other.head_.parent_;
        other.head_.parent_ = tmpp;
    }


template <typename Node, typename Comparator>
template<typename rcNode>
inline typename std::enable_if<std::is_same<typename std::decay<rcNode>::type,
                                            Node>::value,
            typename RBTree<Node, Comparator>::iterator>::type
RBTree<Node, Comparator>::
    exclusiveInsertion(rcNode&& onode) {

     static_assert(std::is_same<
                   typename std::decay<rcNode>::type,
                   Node>::value, "must be [const] Node with the types");

	BaseNode* parent = &head_;
	BaseNode* cur    = head_.parent_;
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
		if(unlikely(&head_ == parent)) { ///////??? easier way ?
			head_.parent_ = cur;
			head_.right_ = cur;
		}
        else if(parent == head_.left_) {
			head_.left_ = cur;
		}
        M_rebalanceAfterInsertion(cur);
        ++this->size_;
	}
    else {
        if (comp_(static_cast<Node*>(parent)->getKey(),
                  (std::forward<rcNode>(onode)).getKey() )) {

            cur = S_creatNode(std::forward<rcNode>(onode), parent);
            parent->right_ = cur;
            if(parent == head_.right_) {
                head_.right_ = cur;
            }
            M_rebalanceAfterInsertion(cur);
            ++this->size_;
        }
        else {
            cur = parent;
        }
	}
	return iterator(cur);
}



template <typename Node, typename Comparator>
template<typename rcNode>
inline typename std::enable_if<std::is_same<typename std::decay<rcNode>::type,
                                            Node>::value,
            typename RBTree<Node, Comparator>::iterator>::type
RBTree<Node, Comparator>::
    duplicatedInsertion(rcNode&& onode) {

     static_assert(std::is_same<
                   typename std::decay<rcNode>::type,
                   Node>::value, "must be [const] Node with the types");

	BaseNode* parent = &head_;
	BaseNode* cur    = head_.parent_;
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
		if(unlikely(&head_ == parent)) { ///////??? easier way ?
			head_.parent_ = cur;
			head_.right_ = cur;
		}
        else if(parent == head_.left_) {
			head_.left_ = cur;
		}
	}
    else {
        cur = S_creatNode(std::forward<rcNode>(onode), parent);
        parent->right_ = cur;
        if(parent == head_.right_) {
            head_.right_ = cur;
        }
	}
    M_rebalanceAfterInsertion(cur);
    ++this->size_;
	return iterator(cur);
}
//-----------------------------------------------------------------------------
//                  tree balancing
//-----------------------------------------------------------------------------

template <typename Node, typename Comparator>
inline typename RBTree<Node, Comparator>::BaseNode*
RBTree<Node, Comparator>::
    S_leftMost(BaseNode* node) noexcept {
        while (nullptr != node->left_)
            node = node->left_;
        return node;
}


template <typename Node, typename Comparator>
inline typename RBTree<Node, Comparator>::BaseNode*
RBTree<Node, Comparator>::
    S_rightMost(BaseNode* node) noexcept {
        while (nullptr != node->right_)
            node = node->right_;
        return node;
    }


template <typename Node, typename Comparator>
inline void
RBTree<Node, Comparator>::
    M_rotateLeft(BaseNode* const node) noexcept {

        BaseNode* const subj = node->right_;
        node->right_ = subj->left_;/////????????????

        if (nullptr != subj->left_) {
             subj->left_->parent_ = node;
        }
        subj->parent_ = node->parent_;
        if (head_.parent_ == node) {
            head_.parent_ = subj;
        }
        else if (node == node->parent_->left_) {
            node->parent_->left_ = subj;
        }
        else { node->parent_->right_ = subj; }

        subj->left_ = node;
        node->parent_ = subj;
    }


template <typename Node, typename Comparator>
inline void
RBTree<Node, Comparator>::
    M_rotateRight(BaseNode*const node) noexcept {

        BaseNode* const subj = node->left_;
        node->left_ = subj->right_;

        if (nullptr != subj->right_) {
             subj->right_->parent_ = node;
        }
        subj->parent_ = node->parent_;
        if (head_.parent_ == node) {
            head_.parent_ = subj;
        }
        else  if (node == node->parent_->right_) {
            node->parent_->right_ = subj;
        }
        else { node->parent_->left_ = subj; }

        subj->right_ = node;
        node->parent_ = subj;
    }


template <typename Node, typename Comparator>
inline void
RBTree<Node, Comparator>::
    M_rebalanceAfterInsertion(BaseNode* pnode) noexcept {

        while (pnode != head_.parent_
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

        head_.parent_->color_ = Color::Black;
} // endp


template <typename Node, typename Comparator>
inline  typename RBTree<Node, Comparator>::BaseNode*
RBTree<Node, Comparator>::
    M_rebalanceAndDetach(BaseNode* const node) noexcept {

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


        if (head_.parent_ == node) {
            head_.parent_ = subj;
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

        if (head_.parent_ == node) {
            head_.parent_ = kid;
        }
        else if (node->parent_->left_ == node) {
            node->parent_->left_ = kid;
        }
        else { node->parent_->right_ = kid;}

        if (head_.left_ == node) {
            if (nullptr == node->right_)        // node->left_ must be null also
                head_.left_ = node->parent_;
            else { head_.left_ = S_leftMost(kid); }
        }
        if (head_.right_ == node) {
            if (nullptr == node->left_) {
                head_.right_ = node->parent_;
            }
            else {head_.right_ = S_rightMost(kid); }
        }
    }
//------------------------------------------
//              balacnce
//------------------------------------------
    if (subj->color_ != Color::Red) {
        while (kid != head_.parent_
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



template <typename Node, typename Comparator>
inline void
RBTree<Node, Comparator>::
    S_forward_iterate(BaseNode* node, std::function<void(const Node*)> visit) {
        if (nullptr != node->left_)
            S_forward_iterate(node->left_, visit);

        visit(const_cast<const Node*>(static_cast<Node*>(node)));

        if (nullptr != node->right_)
            S_forward_iterate(node->right_, visit);
    }

template <typename Node, typename Comparator>
inline void
RBTree<Node, Comparator>::
    S_forward_iterate(BaseNode* node, std::function<void(Node*)> visit) {
        if (nullptr != node->left_)
            S_forward_iterate(node->left_, visit);

        visit(static_cast<Node*>(node));

        if (nullptr != node->right_)
            S_forward_iterate(node->right_, visit);
    }




template <typename Node, typename Comparator>
inline void
RBTree<Node, Comparator>::
    S_strip(BaseNode*& node) {
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

namespace detail_
{

template <typename Node>
struct RBTreeIterator
{
    typedef RBTreeIterator<Node>                Self_;

    typedef typename Node::ValueType            value_type; // concept requires, to cope with stl algorithms
    typedef typename Node::KeyType              key_type; // concept requires, to cope with stl algorithms
    typedef Node&                               reference;
    typedef Node*                               pointer;
    typedef RBTreeIterator<Node>                iterator;
    typedef std::random_access_iterator_tag
                                        iterator_category;// concept requires, to cope with stl algorithms
    typedef ptrdiff_t                       difference_type;


      BaseNode*        node_;    // data.

      RBTreeIterator()
      : node_(nullptr) { }

      explicit
      RBTreeIterator( BaseNode* node)
      : node_(node) { }

      RBTreeIterator(const Self_& iter)
      : node_(iter.node_) { }

      reference operator*()
      { return *(static_cast<Node*>(node_)); }

      pointer operator->()
      { return static_cast<Node*>(node_); }

      Self_& operator++() noexcept {
          if (nullptr != node_->right_) {
            node_ = node_->right_;
            while(nullptr != node_->left_)
                node_ = node_->left_;
          }
          else {
                BaseNode* parent = node_->parent_;
                while (node_ == parent->right_) {//
                    node_ = parent;
                    parent = parent->parent_;
                } //  if (node_->right_ != parent)
                node_ = parent;
          }
          return *this;
      }
      Self_ operator++(int) noexcept {
          iterator tmp(*this);
          this->operator++();
          return tmp;
      }

      Self_& operator--() noexcept {
          if (nullptr != node_->left_) {
                BaseNode* smaller = node_->left_;
                while (nullptr != smaller->right_)
                    smaller = smaller->right_;
                node_ = smaller;
          }
          else {
                BaseNode* parent = node_->parent_;
                while (node_ == parent->left_) {
                    node_ = parent;
                    parent = parent->parent_;
                }
                node_ = parent;
          }
          return *this;
      }
      Self_ operator--(int) noexcept {
          iterator tmp(*this);
          this->operator--();
          return tmp;
      }

      Self_ operator+ (size_t step) noexcept {
          iterator tmp(*this);
          while (step--) {
            ++tmp;
          }
          return tmp;
      }
      Self_ operator- (size_t step) noexcept {
          iterator tmp(*this);
          while (step--) {
            --tmp;
          }
          return tmp;
      }
      Self_& operator+= (size_t step) noexcept {
          while (step--) {
            this->operator++();
          }
          return *this;
      }
      Self_& operator-= (size_t step) noexcept {
          while (step--) {
            this->operator--();
          }
          return *this;
      }

      Self_ operator[] (int step) noexcept {
          iterator tmp(*this);
          if (step > 0) {
            while (step--) {
                ++tmp;
            }
          }
          else {
                while (step++) {
                --tmp;
            }
          }
          return static_cast<Node*>(tmp.node_);
      }

      bool operator== (const Self_& other) const noexcept
      { return node_ == other.node_; }

      bool operator!= (const Self_& other) const noexcept
      { return node_ != other.node_; }

      bool is_leaf() const noexcept {
            return (nullptr == node_->left_
                    && nullptr == node_->right_);
      }
      bool is_black() const noexcept
      {  return RBTreeColor::Black == node_->color_;    }

      void to_left() const noexcept
      { this->node_ = node_->left_;}

      void to_right() const noexcept
      { this->node_ = node_->right_;}

      void to_parent() const noexcept
      { this->node_ = node_->parent_;}
};


template <typename Node>
struct RBTreeConstIterator
{
    typedef RBTreeConstIterator<Node>           iterator;
    typedef RBTreeConstIterator<Node>           Self_;

    typedef typename Node::ValueType            value_type; // concept requires, to cope with stl algorithms
    typedef typename Node::KeyType              key_type; // concept requires, to cope with stl algorithms
    typedef const Node&                         reference;
    typedef const Node*                         pointer;
    typedef std::bidirectional_iterator_tag
                                                iterator_category;// concept requires, to cope with stl algorithms
    typedef ptrdiff_t                           difference_type;


    const BaseNode*        node_;    // data.

      RBTreeConstIterator()
      : node_(nullptr) { }

      explicit
      RBTreeConstIterator(const BaseNode* node)
      : node_(node) { }

      RBTreeConstIterator(const iterator& iter)
      : node_(iter.node_) { }

      RBTreeConstIterator(RBTreeIterator<Node>& iter)
      : node_(iter.node_) { }

      reference operator*() const
      { return *(static_cast<Node*>(node_)); }

      pointer operator->() const
      { return static_cast<const Node*>(node_); }

      Self_& operator++() noexcept {
          if (nullptr != node_->right_) {
            node_ = node_->right_;
            while(nullptr != node_->left_)
                node_ = node_->left_;
          }
          else {
                BaseNode* parent = node_->parent_;
                while (node_ == parent->right_) {//
                    node_ = parent;
                    parent = parent->parent_;
                } //  if (node_->right_ != parent)
                node_ = parent;
          }
          return *this;
      }
      Self_ operator++(int) noexcept {
          iterator tmp(*this);
          this->operator++();
          return tmp;
      }

      Self_& operator--() noexcept {
          if (nullptr != node_->left_) {
                BaseNode* smaller = node_->left_;
                while (nullptr != smaller->right_)
                    smaller = smaller->right_;
                node_ = smaller;
          }
          else {
                BaseNode* parent = node_->parent_;
                while (node_ == parent->left_) {
                    node_ = parent;
                    parent = parent->parent_;
                }
                node_ = parent;
          }
          return *this;
      }
      Self_ operator--(int) {
          iterator tmp(*this);
          this->operator--();
          return tmp;
      }


      Self_ operator+ (size_t step) {
          iterator tmp(*this);
          while (step--) {
            ++tmp;
          }
          return tmp;
      }
      Self_ operator- (size_t step) {
          iterator tmp(*this);
          while (step--) {
            --tmp;
          }
          return tmp;
      }
      const Self_& operator+= (size_t step) {
          while (step--) {
            this->operator++();
          }
          return *this;
      }
      const Self_& operator-= (size_t step) {
          while (step--) {
            this->operator--();
          }
          return *this;
      }

      Self_ operator[] (int step) {
          iterator tmp(*this);
          if (step > 0) {
            while (step--) {
                ++tmp;
            }
          }
          else {
                while (step++) {
                --tmp;
            }
          }
          return static_cast<Node*>(tmp.node_);
      }

      bool operator== (const Self_& other) const noexcept
      { return node_ == other.node_; }

      bool operator!= (const Self_& other) const noexcept
      { return node_ != other.node_; }

      bool is_leaf() const noexcept {
            return (nullptr == node_->left_
                    && nullptr == node_->right_);
      }
      bool is_black() const noexcept
      {  return RBTreeColor::Black == node_->color_;    }

      void to_left() const noexcept
      { this->node_ = node_->left_;}

      void to_right() const noexcept
      { this->node_ = node_->right_;}

      void to_parent() const noexcept
      { this->node_ = node_->parent_;}
};

}// namespace detail_




