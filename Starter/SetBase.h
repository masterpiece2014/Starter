
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

__NAMESPACE_STARTER___BEGIN__

namespace detail_
{

template <typename KeyType,
            typename ValueType,
            typename Comparator,
            typename CoreType
            >
class SetBase
{

    typedef SetBase<KeyType, ValueType, Comparator, CoreType>       Self_;
protected:
    typedef typename CoreType::Node                                 Node;
public:

    typedef typename CoreType::iterator              iterator;
    typedef typename CoreType::const_iterator        const_iterator;
    typedef typename CoreType::reference             reference;
    typedef typename CoreType::const_reference       const_reference;
    typedef typename CoreType::pointer               pointer;
    typedef typename CoreType::const_pointer         const_pointer;

protected:
     CoreType core_;

public:
    SetBase()
     :  core_(),
        erase(core_)   {}

    SetBase(std::initializer_list<std::pair<KeyType, ValueType>> ini)
     :  core_(),
        erase(core_)   {

        for (auto iter = ini.begin(); iter != ini.end(); ++iter)
        { core_.insert(Node(iter->first, iter->second));    }
    }

    SetBase(const Self_& other)
     :  core_(other.core_),
        erase(core_)   {}

    template<typename rcMap>
    SetBase(rcMap&& other)
     :  core_(std::forward<rcMap>(other).core_),
        erase(core_)   {}

    SetBase(Self_&& other)
     :  core_(std::move(other.core_)),
        erase(core_)   {}
public:
    struct ErasionAux
    {
        explicit ErasionAux(CoreType& h)
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
        ErasionAux& operator() (const Key_& k, const Keys_&...restkeys) {
            host_.erase(host_.lower_bound(k));
            (*this)(restkeys...);
            return *this;
        }

//        template <typename Key_, typename...Keys_>
//        typename std::enable_if<std::is_same<KeyType, const Key_>::value,
//                               // typename std::decay<KeyType>::type,
//                                //typename std::decay<Key_>::type>::value,
//                ErasionAux&>::type
//        operator() (const Key_& k, const Keys_&...restkeys) {
//            host_.erase(host_.lower_bound(k));
//            (*this)(restkeys...);
//            return *this;
//        }

////////////////////////////////////////////
//      this is to let compiler check if every args are of the key type,
//          but enable_if<> does not work for member functions
//         so the compiler cannot distinguish between one arg function call and muti-args call
//              you can still use erase(k1, k2, k3, ...kn)
//        template <typename Key_, typename...Keys_>
//    typename std::enable_if<
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
       private: CoreType& host_;
    }erase;

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
    {   return core_.begin();}

    const_iterator cend() const noexcept
    { return core_.end(); }

//-----------------------------------------------------------------------------
//                  getter
//-----------------------------------------------------------------------------
public:
    reference at (const KeyType& thekey) noexcept
    {   return core_.at(thekey);    }

    const_reference at (const KeyType& thekey) const noexcept
    {   return core_.at(thekey);    }

    reference front() noexcept
    {   return core_.front();   }
    const_reference front() const noexcept
    {   return core_.front();     }

    const_reference back() const noexcept
    {   return core_.back();     }
    reference back() noexcept
    {   return core_.back();     }

    iterator find(const KeyType& thekey) noexcept
    {   return core_.find(thekey);  }
    const_iterator find(const KeyType& thekey) const noexcept
    {   return core_.find(thekey);  }

    iterator lower_bound(const KeyType& thekey) noexcept
    {   return core_.lower_bound(thekey);   }
    const_iterator lower_bound(const KeyType& thekey) const noexcept
    {   return core_.lower_bound(thekey);   }

    iterator upper_bound(const KeyType& thekey) noexcept
    {   return core_.upper_bound(thekey);   }
    const_iterator upper_bound(const KeyType& thekey) const noexcept
    {   return core_.upper_bound(thekey);   }

    std::pair<const_iterator, const_iterator>
    equal_range(const KeyType& thekey) const noexcept
    {   return core_.equal_range(thekey);   }

    CoreType& data() noexcept
    {   return core_;   }
    const CoreType& data() const noexcept
    {   return core_;   }


};  // SetBase


template <typename KeyType,
            typename ValueType,
            typename Comparator,
            typename CoreType
            >
class GenMap :  public detail_::SetBase<KeyType, ValueType, Comparator, CoreType>,
                public RelOps<GenMap<KeyType, ValueType, Comparator, CoreType>>
{
    typedef detail_::SetBase<KeyType, ValueType, Comparator, CoreType>  Base_;
    typedef GenMap<KeyType, ValueType, Comparator, CoreType>               Self_;

public:
    typedef typename Base_::Node                    Node;

    typedef typename Base_::iterator                iterator;
    typedef typename Base_::const_iterator          const_iterator;
    typedef typename Base_::reference               reference;
    typedef typename Base_::const_reference         const_reference;
    typedef typename Base_::pointer                 pointer;
    typedef typename Base_::const_pointer           const_pointer;

private:
    using Base_::core_;
public:
    using Base_::erase;

    GenMap()
     :  Base_(),
        insert(core_)    {}

    GenMap(std::initializer_list<std::pair<KeyType, ValueType>> ini)
     :  Base_(ini),
        insert(core_)   {}

    GenMap(const Self_& other)
     :  Base_(other),
        insert(core_)   {}

    GenMap(Self_&& other)
     :  Base_(std::move(other)),
        insert(core_)   {}
public:
    // functor for insertion. Enable insertion like
    //  mytree::inserter(key1, val1)(key2, val2)(key3,val3)...;
    struct InsertionAux
    {
        explicit InsertionAux(CoreType& h)
            :   host_(h) {}


//        template<typename rcKey, typename rcVal>
//            typename std::enable_if<std::is_same<typename std::decay<rcKey>::type, KeyType>::value
//                                    && std::is_same<typename std::decay<rcVal>::type, ValueType>::value,
//        InsertionAux&&>::type
//        operator() (rcKey&& k, rcVal&& v) {
//            host_.exclusiveInsertion(Node(std::forward<rcKey>(k) , std::forward<rcVal>(v)));
//            return static_cast<InsertionAux&&>(*this);
//        }

        // another GCC bug, type deduce failure.
        template<typename rcKey, typename rcVal>
        InsertionAux&&  operator() (rcKey&& k, rcVal&& v) {
            host_.exclusiveInsertion(Node(std::forward<rcKey>(k) , std::forward<rcVal>(v)));
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (iterator left, iterator right) {
            while (left != right) {
                host_.exclusiveInsertion(Node(left->key, left->value));
                ++left;
            }
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (std::initializer_list<std::pair<KeyType, ValueType>> init) {
            for (auto iter = init.begin(); iter != init.end(); ++iter) {
                host_.exclusiveInsertion(Node(iter->first, iter->second));
            }
            return static_cast<InsertionAux&&>(*this);
        }
/// !  GCC 4.7.2 Bug
//        template<typename foreign_iterator>
//            typename std::enable_if<
//                    CoreType::template compatible_iterator<foreign_iterator>::value,
//                    InsertionAux&&>::type
//            operator() (foreign_iterator left, foreign_iterator right) {
//
//            while (left != right) {
//                host_.exclusiveInsertion(Node(left->first, left->second));
//                ++left;
//            }
//            return static_cast<InsertionAux&&>(*this);
//        }

       private: CoreType& host_;
    }insert;

    Self_& operator= (const Self_& other) {
        if (likely(this != &other)) {
            this->core_ = other.core_;
        }
        return *this;
    }

    Self_& operator= (Self_&& other) {
        if (unlikely(this == &other)) {
            core_.swap(other.core_);
        }
        return *this;
    }

    Self_& operator= (std::initializer_list<std::pair<KeyType, ValueType>> init) {
        core_.clear();
        for(auto iter = init.begin(); iter != init.end(); ++iter)
            { core_.exclusiveInsertion(Node(iter->first, iter->second)); }
    }

    bool operator== (const Self_& other) const noexcept
    {   return core_ == other.core_;    }

    bool operator< (const Self_& other) const noexcept
    {   return core_ < other.core_; }

    size_t count(const KeyType& thek) const noexcept
    {   return core_.find(thek) == core_.end() ? 0 : 1;}


    reference operator[] (const KeyType& thekey) noexcept {
        iterator i = core_.lower_bound(thekey);
        if(i = Base_::end()) {
            i = core_.duplicatedInsertion(Node(thekey, ValueType()));
        }
        return i->value;
    }
};


template <typename KeyType,
            typename ValueType,
            typename Comparator,
            typename CoreType
            >
class GenMultiMap :  public detail_::SetBase<KeyType, ValueType, Comparator, CoreType>,
                public RelOps<GenMultiMap<KeyType, ValueType, Comparator, CoreType>>
{
    typedef detail_::SetBase<KeyType, ValueType, Comparator, CoreType>      Base_;
    typedef GenMultiMap<KeyType, ValueType, Comparator, CoreType>              Self_;

public:
    typedef typename Base_::Node                    Node;
    typedef typename Base_::iterator                iterator;
    typedef typename Base_::const_iterator          const_iterator;
    typedef typename Base_::reference               reference;
    typedef typename Base_::const_reference         const_reference;
    typedef typename Base_::pointer                 pointer;
    typedef typename Base_::const_pointer           const_pointer;

private:
    using Base_::core_;
public:
    using Base_::erase;

    GenMultiMap()
     :  Base_(),
        insert(core_)    {}

    GenMultiMap(std::initializer_list<std::pair<KeyType, ValueType>> ini)
     :  Base_(ini),
        insert(core_)   {}

    GenMultiMap(const Self_& other)
     :  Base_(other),
        insert(core_)   {}

    GenMultiMap(Self_&& other)
     :  Base_(std::move(other)),
        insert(core_)   {}
public:
    // functor for insertion. Enable insertion like
    //  mytree::inserter(key1, val1)(key2, val2)(key3,val3)...;
    struct InsertionAux
    {
        explicit InsertionAux(CoreType& h)
            :   host_(h) {}

        template<typename rcKey, typename rcVal>
        InsertionAux&& operator() (rcKey&& k, rcVal&& v) {
            host_.duplicatedInsertion(Node(std::forward<rcKey>(k) , std::forward<rcVal>(v)));
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (iterator left, iterator right) {
            while (left != right) {
                host_.duplicatedInsertion(Node(left->key, left->value));
                ++left;
            }
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (std::initializer_list<std::pair<KeyType, ValueType>> init) {
            for (auto iter = init.begin(); iter != init.end(); ++iter) {
                host_.duplicatedInsertion(Node(iter->first, iter->second));
            }
            return static_cast<InsertionAux&&>(*this);
        }
/// !  GCC 4.7.2 Bug
//        template<typename foreign_iterator>
//            typename std::enable_if<
//                    CoreType::template compatible_iterator<foreign_iterator>::value,
//                    InsertionAux&&>::type
//            operator() (foreign_iterator left, foreign_iterator right) {
//
//            while (left != right) {
//                host_.exclusiveInsertion(Node(left->first, left->second));
//                ++left;
//            }
//            return static_cast<InsertionAux&&>(*this);
//        }

       private: CoreType& host_;
    }insert;

    Self_& operator= (const Self_& other) {
        if (likely(this != &other)) {
            this->core_ = other.core_;
        }
        return *this;
    }

    Self_& operator= (Self_&& other) {
        if (unlikely(this == &other)) {
            core_.swap(other.core_);
        }
        return *this;
    }

    Self_& operator= (std::initializer_list<std::pair<KeyType, ValueType>> init) {
        core_.clear();
        for(auto iter = init.begin(); iter != init.end(); ++iter)
            { core_.duplicatedInsertion(Node(iter->first, iter->second)); }
    }

    bool operator== (const Self_& other) const noexcept {
        return core_ == other.core_;
    }

    bool operator< (const Self_& other) const noexcept {
        return core_ < other.core_;
    }
    size_t count(const KeyType& thek) const noexcept {
        auto pa = core_.equal_range(thek);
        size_t c = 0;
        while(pa.first != pa.second) {
            ++pa.first;
            ++c;
        }
        return c;
    }

    reference operator[] (const KeyType& thekey) noexcept {
        iterator i = core_.lower_bound(thekey);
        if(i = Base_::end()) {
            i = core_.duplicatedInsertion(Node(thekey, ValueType()));
        }
        return i->value;
    }
};



template<typename KeyType,
            typename Comparator,
            typename CoreType
            >
class GenSet : public detail_::SetBase<KeyType, KeyType, Comparator, CoreType>,
                public RelOps<GenSet<KeyType, Comparator, CoreType>>
{
    typedef  GenSet<KeyType, Comparator, CoreType>                         Self_;
    typedef  detail_::SetBase<KeyType, KeyType, Comparator, CoreType>   Base_;

public:
    typedef typename Base_::Node                    Node;
    typedef typename Base_::iterator                iterator;
    typedef typename Base_::const_iterator          const_iterator;
    typedef typename Base_::reference               reference;
    typedef typename Base_::const_reference         const_reference;
    typedef typename Base_::pointer                 pointer;
    typedef typename Base_::const_pointer           const_pointer;
private:
    using Base_::core_;

public:
    using Base_::erase;

    GenSet()
     :  Base_(),
        insert(core_)   {}

    GenSet(std::initializer_list<KeyType> ini)
     :  Base_(ini),
        insert(core_)   {}

    GenSet(const Self_& other)
     :  Base_(other),
        insert(core_){}

    GenSet(Self_&& other)
     :  Base_(std::move(other)),
        insert(core_){}
public:

    // functor for insertion. Enable insertion like
    //  mytree::inserter(key1, val1)(key2, val2)(key3,val3)...;
    struct InsertionAux
    {
        explicit InsertionAux( CoreType& h)
            :   host_(h) {}

        InsertionAux&& operator() () {
            return static_cast<InsertionAux&&>(*this);
        }

        template <typename Key_, typename...Keys_>
        InsertionAux&& operator() (const Key_& k, const Keys_&...restkeys) {
            host_.exclusiveInsertion(Node(k));
            (*this)(restkeys...);
            return static_cast<InsertionAux&&>(*this);
        }
//        template <typename Key_, typename...Keys_>
//        typename std::enable_if<std::is_same<KeyType, Key_>::value, InsertionAux&>::type
//
//        operator() (const Key_& k, Keys_&...restkeys) {
//            host_.exclusiveInsertion(Node(k));
//            (*this)(std::forward<Keys_>(restkeys)...);
//            return *this;
//        }

        InsertionAux&& operator() (iterator left, iterator right) {
            while (left != right) {
                host_.exclusiveInsertion(Node(left->getKey()));
                ++left;
            }
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (std::initializer_list<KeyType> ini) {
            for (auto iter = ini.begin(); iter != ini.end(); ++iter)
            { host_.exclusiveInsertion(Node(*iter));    }

            return static_cast<InsertionAux&&>(*this);
        }

       private: CoreType& host_;
    }insert;

    Self_& operator= (const Self_& other) {
        if (likely(this != &other)) {
            this->core_ = other.core_;
        }
        return *this;
    }

    Self_& operator= (Self_&& other) {
        if (unlikely(this == &other)) {
            core_.swap(other.core_);
        }
        return *this;
    }

    Self_& operator= (std::initializer_list<KeyType> init) {
        core_.clear();
        for(auto iter = init.begin(); iter != init.end(); ++iter)
            { core_.exclusiveInsertion(Node(iter->getKey())); }
    }

    bool operator== (const Self_& other) const noexcept {
        return core_ == other.core_;
    }

    bool operator< (const Self_& other) const noexcept {
        return core_ < other.core_;
    }

    size_t count(const KeyType& thek) const noexcept
    {   return lower_bound(thek) == core_.end() ? 0 : 1; }

};


template<typename KeyType,
            typename Comparator,
            typename CoreType
            >
class GenMultiSet : public detail_::SetBase<KeyType, KeyType, Comparator, CoreType>,
                public RelOps<GenMultiSet<KeyType, Comparator, CoreType>>
{
    typedef  GenMultiSet<KeyType, Comparator, CoreType>                         Self_;
    typedef  detail_::SetBase<KeyType, KeyType, Comparator, CoreType>   Base_;

public:
    typedef typename Base_::Node                    Node;
    typedef typename Base_::iterator                iterator;
    typedef typename Base_::const_iterator          const_iterator;
    typedef typename Base_::reference               reference;
    typedef typename Base_::const_reference         const_reference;
    typedef typename Base_::pointer                 pointer;
    typedef typename Base_::const_pointer           const_pointer;
private:
    using Base_::core_;

public:
    using Base_::erase;

    GenMultiSet()
     :  Base_(),
        insert(core_)   {}

    GenMultiSet(std::initializer_list<KeyType> ini)
     :  Base_(ini),
        insert(core_)   {}

    GenMultiSet(const Self_& other)
     :  Base_(other),
        insert(core_){}

    GenMultiSet(Self_&& other)
     :  Base_(std::move(other)),
        insert(core_){}
public:

    // functor for insertion. Enable insertion like
    //  mytree::inserter(key1, val1)(key2, val2)(key3,val3)...;
    struct InsertionAux
    {
        explicit InsertionAux( CoreType& h)
            :   host_(h) {}

        InsertionAux&& operator() () {
            return static_cast<InsertionAux&&>(*this);
        }

        template <typename Key_, typename...Keys_>
        InsertionAux&& operator() (const Key_& k, const Keys_&...restkeys) {
            host_.duplicatedInsertion(Node(k));
            (*this)(restkeys...);
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (iterator left, iterator right) {
            while (left != right) {
                host_.duplicatedInsertion(Node(left->getKey()));
                ++left;
            }
            return static_cast<InsertionAux&&>(*this);
        }

        InsertionAux&& operator() (std::initializer_list<KeyType> ini) {
            for (auto iter = ini.begin(); iter != ini.end(); ++iter)
            { host_.duplicatedInsertion(Node(*iter));    }

            return static_cast<InsertionAux&&>(*this);
        }

       private: CoreType& host_;
    }insert;

    Self_& operator= (const Self_& other) {
        if (likely(this != &other)) {
            this->core_ = other.core_;
        }
        return *this;
    }

    Self_& operator= (Self_&& other) {
        if (unlikely(this == &other)) {
            core_.swap(other.core_);
        }
        return *this;
    }

    Self_& operator= (std::initializer_list<KeyType> init) {
        core_.clear();
        for(auto iter = init.begin(); iter != init.end(); ++iter)
            { core_.duplicatedInsertion(Node(iter->getKey())); }
        return *this;
    }

    bool operator== (const Self_& other) const noexcept {
        return core_ == other.core_;
    }

    bool operator< (const Self_& other) const noexcept {
        return core_ < other.core_;
    }


    size_t count(const KeyType& thek) const noexcept {
        auto pa = core_.equal_range(thek);
        size_t c = 0;
        while(pa.first != pa.second) {
            ++pa.first;
            ++c;
        }
        return c;
    }
};

} // namespace detail_
__NAMESPACE_STARTER___END__

#endif // _STARTER_MAP_H_




