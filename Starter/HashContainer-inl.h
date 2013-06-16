



/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */

#ifndef _STARTER_HASHCONT_H_
#error this file is the partial implementation of HashContainer\
        it may only be included by HashContainer.h\
        and cannot be used separately

#endif



namespace detail_
{
    template<typename Bucket>
    struct HashContainerIterator
    {
        explicit
        HashContainerIterator(Bucket* n, size_t p)
        :   table_(n),
            pos_(p)  {}

        Bucket* table_;
        size_t pos_;
    };

    template<typename Bucket>
    struct ConstHashContainerIterator
    {};

} // detail_





