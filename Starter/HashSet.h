/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */

#ifndef _STARTER_HASHSET_H_
#define _STARTER_HASHSET_H_

#include "StarterCfgDef.h"

#include "SetBase.h"
#include "HashContainer.h"


//__NAMESPACE_STARTER___BEGIN__
//
//namespace detail_
//{
//        // to use Set , just custom a node
//    template<typename K>
//    struct SetNode : public BaseNode
//    {
//        typedef K   KeyType;
//        typedef K   ValueType;
//
//        KeyType     key;
//        KeyType&    value;
//
//        SetNode(const SetNode<K>& n)
//        :   key(n.key),
//            value(key)  {}
//
//        SetNode(SetNode<K>&& n)
//        :   key(std::move(n.key)),
//            value(key)  {}
//
//        SetNode(const K& k)
//        :   key(k),
//            value(key)  {}
//
//        SetNode(K&& rk)
//        :   key(rk),
//            value(key)  {}
//
//        KeyType& getKey() const noexcept
//        {   return key; }
//    };
//
//}// namespace detail_
//
//
//template<typename K, typename C = std::less<K>>
//using TreeSet = detail_::GenSet<K,
//                                        C,
//                                        RBTree<detail_::SetNode<const K>, C>
//                                    >;
//
//
//template<typename K, typename C = std::less<K>>
//using MultiTreeSet = detail_::GenMultiSet<K,
//                                        C,
//                                        RBTree<detail_::SetNode<const K>, C>
//                                    >;

__NAMESPACE_STARTER___END__

#endif // _STARTER_HASHSET_H_
