/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */

#ifndef STARTER_TREEMAP_H_
#define STARTER_TREEMAP_H_


#include "StarterCfgDef.h"


#include "SetBase.h"
#include "RBTree.h"

__NAMESPACE_STARTER___BEGIN__

namespace detail_
{
	template<typename K, typename V>
	struct MapNode : public BaseNode
	{
		typedef K   KeyType;
		typedef V   ValueType;

        typedef MapNode<K, V>   Self_;

		KeyType     key;
		ValueType   value;

		template<typename rcKey, typename rcVal>
		MapNode(rcKey&& ok, rcVal&& ov)
        :   key(std::forward<rcKey>(ok)),
			value(std::forward<rcVal>(ov))   {}

		MapNode(const Self_& n)
        :   key(n.key),
            value(n.value)   {}

		MapNode(Self_&& n)
        :   key(std::move(n.key)),
            value(std::move(n.value))   {}

		KeyType& getKey() const noexcept
		{   return key;}
	};
}


    template<typename K, typename V, typename C = std::less<K>>
    using TreeMap = detail_::GenMap<K, V, C,
                                            RBTree<detail_::MapNode<const K, V>, C>
                                        >;


    template<typename K, typename V, typename C = std::less<K>>
    using MultiTreeMap = detail_::GenMultiMap<K, V, C,
                                            RBTree<detail_::MapNode<const K, V>, C>
                                        >;

__NAMESPACE_STARTER___END__
#endif // STARTER_TREEMAP_H_
