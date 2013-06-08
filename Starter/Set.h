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

#ifndef _STARTER_SET_H_
#define _STARTER_SET_H_


#include "StarterCfgDef.h"
#include "RelOps.h"
#include "RBTree.h"

__NAMESPACE_STARTER___BEGIN__
namespace detail_
{
    template<typename K>
    struct SetNode : public Nodebase
    {
        typedef K   KeyType;
        typedef K   ValueType;

        KeyType     key;

        SetNode(const SetNode<K>& n)
        :   key(n.key)   {}

        SetNode(SetNode<K>&& n)
        :   key(std::move(n.key))   {}

        SetNode(const K& k)
        :   key(k)   {}

        SetNode(K&& rk)
        :   key(rk)   {}

        KeyType& getKey() noexcept
        {   return key; }
    };
}// namespace detail_

template<typename KeyType,
            typename Comparator = std::less<KeyType>,
            typename RepType = Starter::RBTree<detail_::SetNode<KeyType>, Comparator>
            >
class Set : public Starter::RelOps<Set<KeyType, Comparator>>
{
    typedef Set<KeyType, Comparator, RepType>       Self_;
    typedef detail_::SetNode<KeyType>               Node;

public:
    typedef typename RepType::iterator              iterator;
    typedef typename RepType::const_iterator        const_iterator;
    typedef typename RepType::reference             reference;
    typedef typename RepType::const_reference       const_reference;
    typedef typename RepType::pointer               pointer;
    typedef typename RepType::const_pointer         const_pointer;
private:
    RepType core_;

public:
    Set()
     :  core_(),
        insert(core_),
        erase(core_)   {}

    Set(std::initializer_list<KeyType> ini)
     :  core_(),
        insert(core_),
        erase(core_)   {

        for (auto iter = ini.begin(); iter != ini.end(); ++iter)
        { core_.insert(Node(*iter));    }
    }

    Set(const Self_& other)
     :  core_(other.core_),
        insert(core_),
        erase(core_)   {}

    Set(Self_&& other)
     :  core_(std::move(other.core_)),
        insert(core_),
        erase(core_)   {}
public:

    // functor for insertion. Enable insertion like
    //  mytree::inserter(key1, val1)(key2, val2)(key3,val3)...;
    struct InsertionAux
    {
        explicit InsertionAux( RepType& h)
            :   host_(h) {}


        InsertionAux& operator() () {
            return *this;
        }

        template <typename Key_, typename...Keys_>
        typename std::enable_if<std::is_same<KeyType, Key_>::value, InsertionAux&>::type
        operator() (Key_&& k, Keys_&&...restkeys) {
            host_.insert(Node(std::forward<Key_>(k)));
            (*this)(std::forward<Keys_>(restkeys)...);
            return *this;
        }

        InsertionAux& operator() (iterator left, iterator right) {
            while (left != right) {
                host_.insert(Node(left->key));
                ++left;
            }
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux& operator() (std::initializer_list<KeyType> ini) {
            for (auto iter = ini.begin(); iter != ini.end(); ++iter)
            { host_.insert(Node(*iter));    }

            return static_cast<InsertionAux&&>(*this);
        }

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
    Self_& operator= (std::initializer_list<KeyType> init) {
        core_.clear();
        for(auto iter = init.begin(); iter != init.end(); ++iter)
        {   core_.insert(Node(iter->key));  }
    }

    bool operator== (const Self_& other) const noexcept
    {   return core_ == other.core_;    }
    bool operator< (const Self_& other) const noexcept
    {   return core_ < other.core_; }

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
    {   core_.clear();  }

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
    {   return core_.cbegin();  }

    const_iterator cend() const noexcept
    { return core_.cend(); }

//-----------------------------------------------------------------------------
//                  getter
//-----------------------------------------------------------------------------
public:

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

#endif // _STARTER_SET_H_
