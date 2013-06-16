


/////////////////////////////////////////////////
////
////    this file is from Facebook folly
////
////
/////////////////////////////////////////////////



/*
 * Copyright 2012 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FOLLY_BASE_HASH_H_
#define FOLLY_BASE_HASH_H_

#include "StarterCfgDef.h"

#include <type_traits>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <cstring>
#include <stdint.h>
#include <string>
#include <utility>
#include <system_error>
#include <bitset>
#include <memory>
#include <typeindex>
#include <thread>

//#include "TreeMap.h"
//#include "HyMap.h"
//#include "TreeSet.h"
//#include "HySet.h"
///////////////////////////////////////////// HashMap
//#include "SArray.h"
//#include "CycList.h"

__NAMESPACE_STARTER___BEGIN__

namespace StarterHash {


/*
 * Various hashing functions.
 */

// This is a general-purpose way to create a single hash from multiple
// hashable objects. It relies on std::hash<T> being available for all
// relevant types and combines those hashes in an order-dependent way
// to yield a new hash.

// Never used, but gcc demands it.
inline size_t hash_combine() {
  return 0;
}

// This is the Hash128to64 function from Google's cityhash (available
// under the MIT License).  We use it to reduce multiple 64 bit hashes
// into a single hash.
inline size_t hash_128_to_64(const size_t upper, const size_t lower) {
  // Murmur-inspired hashing.
  const size_t kMul = 0x9ddfea08eb382d69ULL;
  size_t a = (lower ^ upper) * kMul;
  a ^= (a >> 47);
  size_t b = (upper ^ a) * kMul;
  b ^= (b >> 47);
  b *= kMul;
  return b;
}

template <typename T, typename... Ts>
size_t hash_combine(const T& t, const Ts&... ts) {
  size_t seed = std::hash<T>()(t);
  if (sizeof...(ts) == 0) {
    return seed;
  }
  size_t remainder = hash_combine(ts...);
  return hash_128_to_64(seed, remainder);
}

//////////////////////////////////////////////////////////////////////

/*
 * Thomas Wang 64 bit mix hash function
 */

inline uint64_t twang_mix64(uint64_t key) {
  key = (~key) + (key << 21);  // key *= (1 << 21) - 1; key -= 1;
  key = key ^ (key >> 24);
  key = key + (key << 3) + (key << 8);  // key *= 1 + (1 << 3) + (1 << 8)
  key = key ^ (key >> 14);
  key = key + (key << 2) + (key << 4);  // key *= 1 + (1 << 2) + (1 << 4)
  key = key ^ (key >> 28);
  key = key + (key << 31);  // key *= 1 + (1 << 31)
  return key;
}

/*
 * Inverse of twang_mix64
 *
 * Note that twang_unmix64 is significantly slower than twang_mix64.
 */

inline uint64_t twang_unmix64(uint64_t key) {
  // See the comments in jenkins_rev_unmix32 for an explanation as to how this
  // was generated
  key *= 4611686016279904257U;
  key ^= (key >> 28) ^ (key >> 56);
  key *= 14933078535860113213U;
  key ^= (key >> 14) ^ (key >> 28) ^ (key >> 42) ^ (key >> 56);
  key *= 15244667743933553977U;
  key ^= (key >> 24) ^ (key >> 48);
  key = (key + 1) * 9223367638806167551U;
  return key;
}

/*
 * Thomas Wang downscaling hash function
 */

inline uint32_t twang_32from64(uint64_t key) {
  key = (~key) + (key << 18);
  key = key ^ (key >> 31);
  key = key * 21;
  key = key ^ (key >> 11);
  key = key + (key << 6);
  key = key ^ (key >> 22);
  return (uint32_t) key;
}

/*
 * Robert Jenkins' reversible 32 bit mix hash function
 */

inline uint32_t jenkins_rev_mix32(uint32_t key) {
  key += (key << 12);  // key *= (1 + (1 << 12))
  key ^= (key >> 22);
  key += (key << 4);   // key *= (1 + (1 << 4))
  key ^= (key >> 9);
  key += (key << 10);  // key *= (1 + (1 << 10))
  key ^= (key >> 2);
  // key *= (1 + (1 << 7)) * (1 + (1 << 12))
  key += (key << 7);
  key += (key << 12);
  return key;
}

/*
 * Inverse of jenkins_rev_mix32
 *
 * Note that jenkinks_rev_unmix32 is significantly slower than
 * jenkins_rev_mix32.
 */

inline uint32_t jenkins_rev_unmix32(uint32_t key) {
  // These are the modular multiplicative inverses (in Z_2^32) of the
  // multiplication factors in jenkins_rev_mix32, in reverse order.  They were
  // computed using the Extended Euclidean algorithm, see
  // http://en.wikipedia.org/wiki/Modular_multiplicative_inverse
  key *= 2364026753U;

  // The inverse of a ^= (a >> n) is
  // b = a
  // for (int i = n; i < 32; i += n) {
  //   b ^= (a >> i);
  // }
  key ^=
    (key >> 2) ^ (key >> 4) ^ (key >> 6) ^ (key >> 8) ^
    (key >> 10) ^ (key >> 12) ^ (key >> 14) ^ (key >> 16) ^
    (key >> 18) ^ (key >> 20) ^ (key >> 22) ^ (key >> 24) ^
    (key >> 26) ^ (key >> 28) ^ (key >> 30);
  key *= 3222273025U;
  key ^= (key >> 9) ^ (key >> 18) ^ (key >> 27);
  key *= 4042322161U;
  key ^= (key >> 22);
  key *= 16773121U;
  return key;
}

/*
 * Fowler / Noll / Vo (FNV) Hash
 *     http://www.isthe.com/chongo/tech/comp/fnv/
 */

const uint32_t FNV_32_HASH_START = 216613626UL;
const uint64_t FNV_64_HASH_START = 14695981039346656037ULL;

inline uint32_t fnv32(const char* s,
                      uint32_t hash = FNV_32_HASH_START) {
  for (; *s; ++s) {
    hash += (hash << 1) + (hash << 4) + (hash << 7) +
            (hash << 8) + (hash << 24);
    hash ^= *s;
  }
  return hash;
}

inline uint32_t fnv32_buf(const void* buf,
                          int n,
                          uint32_t hash = FNV_32_HASH_START) {
  const char* char_buf = reinterpret_cast<const char*>(buf);

  for (int i = 0; i < n; ++i) {
    hash += (hash << 1) + (hash << 4) + (hash << 7) +
            (hash << 8) + (hash << 24);
    hash ^= char_buf[i];
  }

  return hash;
}

inline uint32_t fnv32(const std::string& str,
                      uint64_t hash = FNV_32_HASH_START) {
  return fnv32_buf(str.data(), str.size(), hash);
}

inline uint64_t fnv64(const char* s,
                      uint64_t hash = FNV_64_HASH_START) {
  for (; *s; ++s) {
    hash += (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) +
      (hash << 8) + (hash << 40);
    hash ^= *s;
  }
  return hash;
}

inline uint64_t fnv64_buf(const void* buf,
                          int n,
                          uint64_t hash = FNV_64_HASH_START) {
  const char* char_buf = reinterpret_cast<const char*>(buf);

  for (int i = 0; i < n; ++i) {
    hash += (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) +
      (hash << 8) + (hash << 40);
    hash ^= char_buf[i];
  }
  return hash;
}

inline uint64_t fnv64(const std::string& str,
                      uint64_t hash = FNV_64_HASH_START) {
  return fnv64_buf(str.data(), str.size(), hash);
}

/*
 * Paul Hsieh: http://www.azillionmonkeys.com/qed/hash.html
 */

#define get16bits(d) (*((const uint16_t*) (d)))

inline uint32_t hsieh_hash32_buf(const void* buf, int len) {
  const char* s = reinterpret_cast<const char*>(buf);
  uint32_t hash = len;
  uint32_t tmp;
  int rem;

  if (len <= 0 || buf == 0) {
    return 0;
  }

  rem = len & 3;
  len >>= 2;

  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (s);
    tmp    = (get16bits (s+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    s  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }

  /* Handle end cases */
  switch (rem) {
  case 3:
    hash += get16bits(s);
    hash ^= hash << 16;
    hash ^= s[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2:
    hash += get16bits(s);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1:
    hash += *s;
    hash ^= hash << 10;
    hash += hash >> 1;
  }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}

#undef get16bits

inline uint32_t hsieh_hash32(const char* s) {
  return hsieh_hash32_buf(s, std::strlen(s));
}

inline uint32_t hsieh_hash32_str(const std::string& str) {
  return hsieh_hash32_buf(str.data(), str.size());
}


template<class Key>
struct hasher;

template<> struct hasher<int32_t> {
  size_t operator()(int32_t key) const {
    return StarterHash::jenkins_rev_mix32(uint32_t(key));
  }
};

template<> struct hasher<uint32_t> {
  size_t operator()(uint32_t key) const {
    return StarterHash::jenkins_rev_mix32(key);
  }
};

template<> struct hasher<int64_t> {
  size_t operator()(int64_t key) const {
    return StarterHash::twang_mix64(uint64_t(key));
  }
};

template<> struct hasher<uint64_t> {
  size_t operator()(uint64_t key) const {
    return StarterHash::twang_mix64(key);
  }
};
//////////////////////////////////////////////////////////////////////
} // namespace StarterHash

namespace detail_
{
template <typename Key>
struct canUseSTL_Hash
{
    enum{ value = std::is_fundamental<Key>::value
                    ||std::is_same<Key, std::string>::value
                    ||std::is_same<Key, std::u16string>::value
                    ||std::is_same<Key, std::u32string>::value
                    ||std::is_same<Key, std::wstring>::value

                    ||std::is_same<Key, std::error_code>::value
                    ||std::is_same<Key, std::unique_ptr<Key>>::value
                    ||std::is_same<Key, std::shared_ptr<Key>>::value
                    ||std::is_same<Key, std::type_index>::value
                    ||std::is_same<Key, std::vector<bool>>::value
                    ||std::is_same<Key, std::thread::id>::value
        };
};
} // namespace detail_

    template <typename Key>
    typename std::enable_if<detail_::canUseSTL_Hash<Key>::value,
        size_t>::type
    hashCode(const Key& k) noexcept {
        return std::hash<Key>()(k);
    }

    template <typename T1, typename T2>
    size_t
    hashCode(const std::pair<T1, T2>& k) noexcept {
        return StarterHash::hash_combine(hashCode(k.first), hashCode(k.second));
    }

    template <typename Key, typename ValueType = typename Key::value_type>
    typename std::enable_if<(std::is_same<Key, std::vector<ValueType>>::value
                            ||std::is_same<Key, std::list<ValueType>>::value
                            ||std::is_same<Key, std::deque<ValueType>>::value

                            ||std::is_same<Key, std::set<ValueType>>::value
                            ||std::is_same<Key, std::multiset<ValueType>>::value
                            ||std::is_same<Key, std::unordered_set<ValueType>>::value
                            ||std::is_same<Key, std::unordered_multiset<ValueType>>::value
//                            ||std::is_same<Key, ::Starter::TreeSet<ValueType>>::value
//                            ||std::is_same<Key, ::Starter::TreeMultiSet<ValueType>>::value
//                            ||std::is_same<Key, Starter::HySet<ValueType>>::value
//                            ||std::is_same<Key, Starter::HyMultiSet<ValueType>>::value
//                            ||std::is_same<Key, Starter::SArray<ValueType>>::value
//                            ||std::is_same<Key, Starter::CycList<ValueType>>::value
                            ),
        size_t>::type
    hashCode(const Key& k) noexcept {
        size_t seed = StarterHash::FNV_32_HASH_START;
        for(const auto& i : k) {
            seed = StarterHash::hash_combine(seed, hashCode(i));
        }
        return seed;
    }

template <typename Cont,
            typename KeyType = typename Cont::value_type::first_type,
            typename ValueType = typename Cont::value_type::second_type>
    typename std::enable_if<(std::is_same<Cont, std::map<KeyType, ValueType>>::value
                            ||std::is_same<Cont, std::multimap<KeyType, ValueType>>::value
                            ||std::is_same<Cont, std::unordered_map<KeyType, ValueType>>::value
                            ||std::is_same<Cont, std::unordered_multimap<KeyType, ValueType>>::value                          ||std::is_same<Key, Starter::SArray<ValueType>>::value
//                            ||std::is_same<Cont, Starter::TreeMap<KeyType, ValueType>>::value
//                            ||std::is_same<Cont, Starter::HyMap<KeyType, ValueType>>::value
//                            ||std::is_same<Cont, Starter::HyMultiMap<KeyType, ValueType>>::value
//                            ||std::is_same<Cont, Starter::TreeMultiMap<KeyType, ValueType>>::value
                            ),
        size_t>::type
    hashCode(const Cont& k) noexcept {
        size_t seed = StarterHash::FNV_32_HASH_START;
        for(const auto& i : k) {
            seed = StarterHash::hash_combine(seed, hashCode(i));
        }
        return seed;
    }

namespace PrimeTable
{
    const unsigned long long PrimeNumbers[] = //256 + 48 + 1
    {
        2ul, 3ul, 5ul, 7ul, 11ul, 13ul, 17ul, 19ul, 23ul, 29ul, 31ul,
        37ul, 41ul, 43ul, 47ul, 53ul, 59ul, 61ul, 67ul, 71ul, 73ul, 79ul,
        83ul, 89ul, 97ul, 103ul, 109ul, 113ul, 127ul, 137ul, 139ul, 149ul,
        157ul, 167ul, 179ul, 193ul, 199ul, 211ul, 227ul, 241ul, 257ul,
        277ul, 293ul, 313ul, 337ul, 359ul, 383ul, 409ul, 439ul, 467ul,
        503ul, 541ul, 577ul, 619ul, 661ul, 709ul, 761ul, 823ul, 887ul,
        953ul, 1031ul, 1109ul, 1193ul, 1289ul, 1381ul, 1493ul, 1613ul,
        1741ul, 1879ul, 2029ul, 2179ul, 2357ul, 2549ul, 2753ul, 2971ul,
        3209ul, 3469ul, 3739ul, 4027ul, 4349ul, 4703ul, 5087ul, 5503ul,
        5953ul, 6427ul, 6949ul, 7517ul, 8123ul, 8783ul, 9497ul, 10273ul,
        11113ul, 12011ul, 12983ul, 14033ul, 15173ul, 16411ul, 17749ul,
        19183ul, 20753ul, 22447ul, 24281ul, 26267ul, 28411ul, 30727ul,
        33223ul, 35933ul, 38873ul, 42043ul, 45481ul, 49201ul, 53201ul,
        57557ul, 62233ul, 67307ul, 72817ul, 78779ul, 85229ul, 92203ul,
        99733ul, 107897ul, 116731ul, 126271ul, 136607ul, 147793ul,
        159871ul, 172933ul, 187091ul, 202409ul, 218971ul, 236897ul,
        256279ul, 277261ul, 299951ul, 324503ul, 351061ul, 379787ul,
        410857ul, 444487ul, 480881ul, 520241ul, 562841ul, 608903ul,
        658753ul, 712697ul, 771049ul, 834181ul, 902483ul, 976369ul,
        1056323ul, 1142821ul, 1236397ul, 1337629ul, 1447153ul, 1565659ul,
        1693859ul, 1832561ul, 1982627ul, 2144977ul, 2320627ul, 2510653ul,
        2716249ul, 2938679ul, 3179303ul, 3439651ul, 3721303ul, 4026031ul,
        4355707ul, 4712381ul, 5098259ul, 5515729ul, 5967347ul, 6456007ul,
        6984629ul, 7556579ul, 8175383ul, 8844859ul, 9569143ul, 10352717ul,
        11200489ul, 12117689ul, 13109983ul, 14183539ul, 15345007ul,
        16601593ul, 17961079ul, 19431899ul, 21023161ul, 22744717ul,
        24607243ul, 26622317ul, 28802401ul, 31160981ul, 33712729ul,
        36473443ul, 39460231ul, 42691603ul, 46187573ul, 49969847ul,
        54061849ul, 58488943ul, 63278561ul, 68460391ul, 74066549ul,
        80131819ul, 86693767ul, 93793069ul, 101473717ul, 109783337ul,
        118773397ul, 128499677ul, 139022417ul, 150406843ul, 162723577ul,
        176048909ul, 190465427ul, 206062531ul, 222936881ul, 241193053ul,
        260944219ul, 282312799ul, 305431229ul, 330442829ul, 357502601ul,
        386778277ul, 418451333ul, 452718089ul, 489790921ul, 529899637ul,
        573292817ul, 620239453ul, 671030513ul, 725980837ul, 785430967ul,
        849749479ul, 919334987ul, 994618837ul, 1076067617ul, 1164186217ul,
        1259520799ul, 1362662261ul, 1474249943ul, 1594975441ul, 1725587117ul,
        1866894511ul, 2019773507ul, 2185171673ul, 2364114217ul, 2557710269ul,
        2767159799ul, 2993761039ul, 3238918481ul, 3504151727ul, 3791104843ul,
        4101556399ul, 4294967291ul,
        6442450933ul, 8589934583ul, 12884901857ul, 17179869143ul,
        25769803693ul, 34359738337ul, 51539607367ul, 68719476731ul,
        103079215087ul, 137438953447ul, 206158430123ul, 274877906899ul,
        412316860387ul, 549755813881ul, 824633720731ul, 1099511627689ul,
        1649267441579ul, 2199023255531ul, 3298534883309ul, 4398046511093ul,
        6597069766607ul, 8796093022151ul, 13194139533241ul, 17592186044399ul,
        26388279066581ul, 35184372088777ul, 52776558133177ul, 70368744177643ul,
        105553116266399ul, 140737488355213ul, 211106232532861ul, 281474976710597ul,
        562949953421231ul, 1125899906842597ul, 2251799813685119ul,
        4503599627370449ul, 9007199254740881ul, 18014398509481951ul,
        36028797018963913ul, 72057594037927931ul, 144115188075855859ul,
        288230376151711717ul, 576460752303423433ul,
        1152921504606846883ul, 2305843009213693951ul,
        4611686018427387847ul, 9223372036854775783ul,
        18446744073709551557ul, 18446744073709551557ul
    };

    bool isPrime(unsigned long long num) noexcept {
        size_t left = 0;
        size_t right = 304;
        size_t median = (right + left) / 2;
        while(right != left) {
            if (PrimeNumbers[median] < num) {
                left = median + 1;
            }
            else if(PrimeNumbers[median] > num) {
                right = median;
            }
            else {
                return true;
            }
            median = (right + left) / 2;
        }
        return false;
    }

    std::vector<unsigned long long>
    primeBetween(unsigned long long lower, unsigned long long upper) {

        std::vector<unsigned long long> vec;
        size_t left = 0;
        while(PrimeNumbers[left++] < lower);
        size_t right = 303;
        while(PrimeNumbers[right--] > upper);
        left--;
        right++;
        for (size_t i = left; i <= right; ++i) {
            vec.push_back(PrimeNumbers[i]);
        }
        return vec;
    }
    std::vector<unsigned long long>
    primeUnder(unsigned long long upper) {
        return primeBetween(2, upper);
    }
        std::vector<unsigned long long>
    primeAbove(unsigned long long lower) {
        return primeBetween(lower, 18446744073709551557ul);
    }

    unsigned long long
    prevPrime(unsigned long long current) {
        size_t i = 304;
        while(i--) {
            if(current > PrimeNumbers[i]) {
                return PrimeNumbers[i];
            }
        }
        return PrimeNumbers[i];
    }
    unsigned long long
    nextPrime(unsigned long long current) {
        size_t i = 1;
        for(; i != 304; ++i) {
            if(current < PrimeNumbers[i]) {
                return PrimeNumbers[i];
            }
        }
        return PrimeNumbers[i];
    }
} // PrimeTable

__NAMESPACE_STARTER___END__

#endif
