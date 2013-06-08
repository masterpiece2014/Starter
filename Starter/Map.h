
 ///////////////////////////////////////////////////////////////////////////////
 ////     Copyright 2013 CaiBowen
 ////     All rights reserved.
 ////
 ////     Author: Cai Bowen
 ////       contact/bug report/get new version
 ////           at
 ////       feedback2bowen@outlook.com
 ////
 ////
 ////     Licensed under the Apache License, Version 2.0 (the "License");
 ////     you may not use this file except in compliance with the License.
 ////     You may obtain a copy of the License at
 ////
 ////              http://www.apache.org/licenses/LICENSE-2.0
 ////
 ////     Unless required by applicable law or agreed to in writing, software
 ////     distributed under the License is distributed on an "AS IS" BASIS,
 ////     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ////     See the License for the specific language governing permissions and
 ////     limitations under the License.
 ///////////////////////////////////////////////////////////////////////////////

/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */


#ifndef _STARTER_MAP_H_
#define _STARTER_MAP_H_

#include "StarterCfgDef.h"
#include "RelOps.h"
#include "RBTree.h"

__NAMESPACE_STARTER___BEGIN__

namespace detail_
{
    template<typename K, typename V>
    struct MapNode : public Nodebase
    {
        typedef K   KeyType;
        typedef V   ValueType;

        KeyType     key;
        ValueType   value;

        template<typename rcKey, typename rcVal>
        MapNode(rcKey&& ok, rcVal&& ov)
        :   key(std::forward<rcKey>(ok)),
            value(std::forward<rcVal>(ov))   {}

        template<typename rcNode>
        MapNode(rcNode&& n)
        :   key(std::forward<rcNode>(n).key),
            value(std::forward<rcNode>(n).value)   {}

        KeyType& getKey() noexcept
        {   return key;}
    };
}// namespace detail_


template <typename KeyType,
            typename ValueType,
            typename Comparator = std::less<KeyType>,
            typename RepType = RBTree<typename detail_::MapNode<const KeyType, ValueType>, Comparator>
            >
class Map :  public Starter::RelOps<Map<KeyType, ValueType, RepType>>
{
    typedef Map<KeyType, ValueType, Comparator, RepType>    Self_;
    typedef detail_::MapNode<const KeyType, ValueType>      Node;

public:
    typedef typename RepType::iterator              iterator;
    typedef typename RepType::const_iterator        const_iterator;
    typedef typename RepType::reference             reference;
    typedef typename RepType::const_reference       const_reference;
    typedef typename RepType::pointer               pointer;
    typedef typename RepType::const_pointer         const_pointer;
public:
     RepType core_;

public:
    Map()
     :  core_(),
        insert(core_),
        erase(core_)   {}

    Map(std::initializer_list<std::pair<KeyType, ValueType>> ini)
     :  core_(),
        insert(core_),
        erase(core_)   {

        for (auto iter = ini.begin(); iter != ini.end(); ++iter)
        { core_.insert(Node(iter->first, iter->second));    }
    }


    Map(const Self_& other)
     :  core_(other.core_),
        insert(core_),
        erase(core_)   {}

    template<typename rcMap>
    Map(rcMap&& other)
     :  core_(std::forward<rcMap>(other).core_),
        insert(core_),
        erase(core_)   {}

    Map(Self_&& other)
     :  core_(std::move(other.core_)),
        insert(core_),
        erase(core_)   {}
public:
    // functor for insertion. Enable insertion like
    //  mytree::inserter(key1, val1)(key2, val2)(key3,val3)...;
    struct InsertionAux
    {
        explicit InsertionAux(RepType& h)
            :   host_(h) {}

        template<typename rcKey, typename rcVal>
        InsertionAux&& operator() (rcKey&& k, rcVal&& v) {
            host_.insert(Node(std::forward<rcKey>(k) , std::forward<rcVal>(v)));
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (iterator left, iterator right) {
            while (left != right) {
                host_.insert(Node(left->key, left->value));
                ++left;
            }
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (std::initializer_list<std::pair<KeyType, ValueType>> init) {
            for (auto iter = init.begin(); iter != init.end(); ++iter) {
                host_.insert(Node(iter->first, iter->second));
            }
            return static_cast<InsertionAux&&>(*this);
        }
//
//        template<typename foreign_iterator>
//            typename std::enable_if<
//                    RepType::template compatible_iterator<foreign_iterator>::value,
//                    InsertionAux&&>::type
//            operator() (foreign_iterator left, foreign_iterator right) {
//
//            while (left != right) {
//                host_.insert(Node(left->first, left->second));
//                ++left;
//            }
//            return static_cast<InsertionAux&&>(*this);
//        }

       private: RepType& host_;
    }insert;

    struct ErasionAux
    {
        explicit ErasionAux(RepType& h)
            :   host_(h) {}

        ErasionAux& operator() (iterator iter) {
           host_.erase(iter);
           // return static_cast<InsertionAux&&>(*this);
           return *this;
        }

        ErasionAux& operator() () {
            return *this;
        }

        template <typename Key_, typename...Keys_>
        typename std::enable_if<std::is_same<KeyType, Key_>::value, ErasionAux&>::type
        operator() (const Key_& k, const Keys_&...restkeys) {
            host_.erase(host_.lower_bound(k));
            (*this)(restkeys...);
            return *this;
        }

////////////////////////////////////////////
//      this is to let compiler check if every args are of the key type,
//          but enable_if<> does not work for member functions
//         so the compiler cannot distinguish between one arg function call and muti-args call
//              you can still use erase(k1, k2, k3, ...kn)
//        template <typename Key_, typename...Keys_>
//typename std::enable_if<
//            (sizeof...(Keys_) > 1)
//             std::is_same<KeyType, Key_>::value
//        &&  std::is_same<KeyType,
//            typename std::tuple_element<
//                    sizeof...(Keys_) - 1, std::tuple<Keys_...>
//                    >::type
//         >::value,
//ErasionAux&>::type
//        operator() (const Key_& k, const Keys_&...restkeys) {
//            host_.erase(host_.lower_bound(k));
//            (*this)(restkeys...);
//            return *this;
//        }
////////////////////////////////////////////
        ErasionAux& operator() (iterator left, iterator right) {
            while (left != right) {
                host_.erase(left);
                ++left;
            }
            //return static_cast<InsertionAux&&>(*this);
            return *this;
        }
        ErasionAux& operator() (std::initializer_list<KeyType> init) {
            for (auto iter = init.begin(); iter != init.end(); ++iter) {
                host_.erase(*iter);
            }
            return *this;
        }
       private: RepType& host_;
    }erase;

//-----------------------------------------------------------------------------
//                  operators
//-----------------------------------------------------------------------------
    Self_& operator= (const Self_& other) {
        if (likely(this != &other)) {
            core_ = other.core_;
        }
        return *this;
    }
    Self_& operator= (Self_&& other) {
        if (likely(this != &other)) {
            core_.swap(other.core_);
        }
        return *this;
    }
    Self_& operator= (std::initializer_list<std::pair<KeyType, ValueType>> init) {
        core_.clear();
        for(auto iter = init.begin(); iter != init.end(); ++iter)
            { core_.insert(iter->first, iter->second); }
    }

    bool operator== (const Self_& other) const noexcept {
        return core_ == other.core_;
    }
    bool operator< (const Self_& other) const noexcept {
        return core_ < other.core_;
    }

//-----------------------------------------------------------------------------
//                  iterator
//-----------------------------------------------------------------------------
    size_t size() const noexcept
    { return core_.size(); }

    bool empty() const noexcept
    { return core_.empty(); }

    size_t count(const KeyType& k) const noexcept
    {   return core_.find(k) == core_.end() ? 0 : 1; }

    void clear()
    {   core_.clear();}

    void swap(Self_& other) noexcept
    { core_.swap(other.core_); }
//-----------------------------------------------------------------------------
//                  iterator
//-----------------------------------------------------------------------------
public:
    iterator begin() noexcept
    {   return core_.begin();   }

    iterator end() noexcept
    {   return core_.end(); }

    const_iterator cbegin() const noexcept
    {   return core_.cbegin();}

    const_iterator cend() const noexcept
    { return core_.cend(); }

//-----------------------------------------------------------------------------
//                  getter
//-----------------------------------------------------------------------------
public:
    reference at (const KeyType& thekey) noexcept
    {   return core_.at(thekey);    }

    const_reference at (const KeyType& thekey) const noexcept
    {   return core_.at(thekey);    }

    reference operator[] (const KeyType& thekey) noexcept
    {   return core_.at(thekey);    }

    const_reference operator[] (const KeyType& thekey) const noexcept
    {   return core_.at(thekey);    }

    reference front() noexcept
    {   return core_.front();   }

    reference back() noexcept
    {   return core_.back();     }

    const_reference front() const noexcept
    {   return core_.front();     }

    const_reference back() const noexcept
    {   return core_.back();     }

    RepType& data() noexcept
    {   return core_;   }

    const RepType& data() const noexcept
    {   return core_;   }

};


__NAMESPACE_STARTER___END__

#endif // _STARTER_MAP_H_




