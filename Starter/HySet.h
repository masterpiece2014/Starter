

#ifndef _STARTER_HYBIRD_SET_H_
#define _STARTER_HYBIRD_SET_H_

#include "StarterCfgDef.h"
#include "SetBase.h"
#include "HashContainer.h"
#include "HyMap-treebucket.h"
//#include "Hash.h"

__NAMESPACE_STARTER___BEGIN__

    template<typename K, typename C = std::less<K>, typename Hash = std::hash<K>>//??????????>
    using HySet = detail_::GenSet<K, C,
                    HashContainer<
                        detail_::HashContainerTreeBucket<detail_::SetNode<const K>, C>,
                        Hash>
                    >;

    template<typename K, typename C = std::less<K>, typename Hash = std::hash<K>>//??????????>
    using HyMultiSet = detail_::GenMultiSet<K, C,
                    HashContainer<
                        detail_::HashContainerTreeBucket<detail_::SetNode<const K>, C>,
                        Hash>
                    >;


__NAMESPACE_STARTER___END__
#endif // _STARTER_HYBIRD_SET_H_
