/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */

#ifndef _STARTER_HASHMAP_H_
#define _STARTER_HASHMAP_H_


#include "StarterCfgDef.h"

#include "SetBase.h"
#include "HashContainer.h"

//#include "Hash.h"

__NAMESPACE_STARTER___BEGIN__

struct HashNodeBase
{
    HashNodeBase* next = nullptr;

//    explicit HashNodeBase(const HashNodeBase* pt)
//    :   next

    size_t count() const noexcept {
        HashNodeBase* cur = next;
        size_t ct = 0;
        while (nullptr != cur) {
            cur = cur->next;
            ++ct;
        }
        return ct;
    }
    // explicit construction needed
//    getKey()
};
template<typename K, typename V>
struct HashMapNode : public HashNodeBase
{
    typedef K       KeyType;
    typedef V       ValueType;

    K   key;
    V   value;
    //explicit

};

//namespace detail_
//{
//	template<typename K, typename V>
//	struct MapNode : public BaseNode
//	{
//		typedef K   KeyType;
//		typedef V   ValueType;
//
//        typedef MapNode<K, V>   Self_;
//
//		KeyType     key;
//		ValueType   value;
//
//		template<typename rcKey, typename rcVal>
//		MapNode(rcKey&& ok, rcVal&& ov)
//        :   key(std::forward<rcKey>(ok)),
//			value(std::forward<rcVal>(ov))   {}
//
//		MapNode(const Self_& n)
//        :   key(n.key),
//            value(n.value)   {}
//
//		MapNode(Self_&& n)
//        :   key(std::move(n.key)),
//            value(std::move(n.value))   {}
//
//		KeyType& getKey() const noexcept
//		{   return key;}
//	};
//}
//
//    template<typename K, typename V, typename C = std::less<K>>
//    using HashMap = detail_::GenMap<K, V, C,
//                                            RBTree<detail_::MapNode<const K, V>, C>
//                                        >;
//
//
//    template<typename K, typename V, typename C = std::less<K>>
//    using MultiHashMap = detail_::GenMultiMap<K, V, C,
//                                            RBTree<detail_::MapNode<const K, V>, C>
//                                        >;

__NAMESPACE_STARTER___END__
#endif // _STARTER_HASHMAP_H_
