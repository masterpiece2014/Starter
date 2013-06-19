/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */

#ifndef _STARTER_HASHCONT_H_
#define _STARTER_HASHCONT_H_

#include "StarterCfgDef.h"

#include "Hash.h"

#include <cstdlib>
#include <functional>

__NAMESPACE_STARTER___BEGIN__

namespace detail_
{
    template<typename Node>
    struct HashContainerIterator;

    template<typename Node>
    struct ConstHashContainerIterator;

} // namespace detail_

template<typename Bucket_t, typename Hash>
class HashContainer
{

protected:
    typedef HashContainer<Bucket_t, Hash>                   Self_;
    typedef Bucket_t                                        Bucket_;

    typedef typename Bucket_::Comparator                    Comparator;
    typedef typename Bucket_::iterator                      BucketIter;
    typedef typename Bucket_::const_iterator                ConstBucketIter;

public:

    typedef typename Bucket_::Node                          Node;
    typedef typename Node::KeyType                          KeyType;
    typedef typename Node::ValueType                        ValueType;

    typedef Node&                                           reference;
    typedef const Node&                                     const_reference;
    typedef Node*                                           pointer;
    typedef const Node*                                     const_pointer;
    typedef detail_::HashContainerIterator<Bucket_>         iterator;
    typedef detail_::ConstHashContainerIterator<Bucket_>    const_iterator;

protected:
    typedef std::function<bool(const KeyType&, const KeyType&)> KeysCompType;
    typedef std::function<size_t(const KeyType&)>               HashType;

    size_t          size_;
    size_t          tableSize_;
    Bucket_*        table_;
    HashType        hasher_;
    KeysCompType    comp_;

protected:
    Bucket_* M_getPosition(iterator iter) const noexcept {
        if (iter.table_ == this->table_) {
            if (iter.pos_ < this->size_) {
                return this->table_ + iter.pos_;
            }
            else {
                throw std::out_of_range(" iterator out og range");
            }
        }
        else {
            throw std::out_of_range("iterator from miss matched hash container");
        }
    }
    Bucket_* M_getPosition(size_t idx) const noexcept {
        if (idx < this->size_) {
            return this->table_ + iter.pos_;
        }
        else {
            throw std::out_of_range(" iterator out og range");
        }
    }
public:
    explicit
    HashContainer(size_t tableSz = 1024)
    :   size_(0),
        tableSize_(tableSz),
        hasher_(Hash()),
        comp_(Comparator()) {
        table_ = static_cast<Bucket_*>(calloc(tableSz, sizeof(Bucket_*)));
    }
    ~HashContainer() {
        Bucket_* cur;
        for (size_t idx = 0; idx != tableSize_; ++idx) {
            cur = table_ + idx;
            if (nullptr != cur) {
                cur->~Bucket_();
            }
        }
        ::operator delete(table_);
    }

    template<typename rcNode>
        typename std::enable_if<std::is_same<typename std::decay<rcNode>::type,
                                Node>::value,
        iterator>::type
     exclusiveInsertion(rcNode&& onode) {
         size_t idx = hasher_(onode);
         if ((table_ + idx)->empty()) {
                (table_ + idx)->insert(onode);
         }
         else{
            return iterator(M_getPosition(idx));
         }
     }

    template<typename rcNode>
        typename std::enable_if<std::is_same<typename std::decay<rcNode>::type,
                                Node>::value,
        iterator>::type
    duplicatedInsertion(rcNode&& onode) {
         size_t idx = hasher_(onode);
        (table_ + idx)->insert(onode);
    }

    void erase(iterator iter) {
        --size_;
        (M_getPosition(iter))->~Bucket_();
    }

    bool empty() const noexcept
    {   return size_ == 0;  }

    bool size() const noexcept
    {   return size_;  }

};

///////////////////////////////////////

//////////////////////////////
#include "HashContainer-inl.h"

__NAMESPACE_STARTER___END__


#endif // _STARTER_HASHCONT_H_








