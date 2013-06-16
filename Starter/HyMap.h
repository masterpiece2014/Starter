


#ifndef _STARTER_HYBIRD_MAP_H_
#define _STARTER_HYBIRD_MAP_H_

#include "StarterCfgDef.h"
#include "SetBase.h"
#include "HashContainer.h"
#include "HyMap-treebucket.h"
//#include "Hash.h"

__NAMESPACE_STARTER___BEGIN__

    template<typename K, typename V, typename C = std::less<K>, typename Hash = std::hash<K>>//??????????>
    using HyMap = detail_::GenMap<K, V, C,
                    HashContainer<
                        detail_::HashContainerTreeBucket<detail_::MapNode<const K, V>, C>,
                        Hash>
                    >;

    template<typename K, typename V, typename C = std::less<K>, typename Hash = std::hash<K>>//??????????>
    using HyMultiMap = detail_::GenMultiMap<K, V, C,
                    HashContainer<
                        detail_::HashContainerTreeBucket<detail_::MapNode<const K, V>, C>,
                        Hash>
                    >;

__NAMESPACE_STARTER___END__
#endif // _STARTER_HYBIRD_MAP_H_
