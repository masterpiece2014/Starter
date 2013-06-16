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

    extern const unsigned long long PrimeLList[];
    unsigned long long nextPrime(unsigned long long current);

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
//        Bucket_* prev;
//        Bucket_* cur;
//        for (size_t idx = 0; idx != tableSize_; ++idx) {
//            cur = table_ + idx;
//            while (nullptr != cur) {
//                prev = cur;
//                cur = cur->next_;
//                delete prev;
//            }
//        }
//        ::operator delete(table_);
    }

};

#include "HashContainer-inl.h"

__NAMESPACE_STARTER___END__


#endif // _STARTER_HASHCONT_H_








