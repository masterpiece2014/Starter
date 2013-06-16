 ///////////////////////////////////////////////////////////////////////////////
 ////     Copyright 2012 2013 CaiBowen
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


#ifdef __GNUC__
    #define _STARTER_GCC_VERSION_ __GNUC__*100 + __GNUC_MINOR__*10 + __GNUC_PATCHLEVEL__

    #if _STARTER_GCC_VERSION_ < 470
        #error please update your compiler to GCC 4.7 or higher
    #endif

#else
#ifdef _MSC_VER
    #define _STARTER_MSVC_VERSION_  _MSC_VER

    #if _STARTER_MSVC_VERSION_ < 1700
        #error please update your compiler to GCC 4.7 or higher
    #endif

#endif
#endif


#ifndef _STARTER_CONFIG_H_
#define _STARTER_CONFIG_H_

#pragma once


#undef likely
#undef unlikely

#ifdef __GNUC__
#define likely(x)   (__builtin_expect((x), 1))
#define unlikely(x) (__builtin_expect((x), 0))
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif


#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <cassert>
using std::size_t;
using std::ptrdiff_t;

#ifndef __NAMESPACE_STARTER___BEGIN__
#define __NAMESPACE_STARTER___BEGIN__   namespace Starter { /* namespace Starter  */
#endif
#ifndef __NAMESPACE_STARTER___END__
#define __NAMESPACE_STARTER___END__     } /* namespace Starter  */
#endif

__NAMESPACE_STARTER___BEGIN__

// get real value type of the iterator
template<typename IterType>
struct realType
{
typedef typename std::remove_cv<
            typename std::remove_reference<
                typename std::remove_pointer<
                    typename std::iterator_traits<IterType>::value_type//iterator value_type
                                            >::type//remove pointer
                                        >::type//remove ref
                            >::type
value_type;//// type define
};
////     define ReturnType,
////     if the value type(no const, no volatile, no reference)
////     of the iterator is the same with ValueType
////                                2013.2.10
template <typename IterType,
          typename ValueType,
          typename ReturnType>
struct defType
{
typedef typename
    std::enable_if<
            std::is_same<typename realType<IterType>::value_type, ValueType>::value,// is_same
            ReturnType>::type// enable_if
        type;// type define
};

template<typename...Tn>
struct AreInts;
template<typename T1, typename... Tn>
struct AreInts<T1, Tn...> {
    enum {value = std::is_integral<T1>::value && AreInts<Tn...>::value };
};
template<typename T>
struct AreInts<T> {
    enum {value = std::is_integral<T>::value};
};

template<typename...Tn>
struct AreFloats;
template<typename T1, typename... Tn>
struct AreFloats<T1, Tn...> {
    enum {value = std::is_floating_point<T1>::value && AreFloats<Tn...>::value };
};

template<typename T>
struct AreFloats<T> {
    enum {value = std::is_floating_point<T>::value};
};

template<typename T>
inline T*
addressOf(T& v) {
    return reinterpret_cast<T*>
        (&const_cast<char&>(reinterpret_cast<const volatile char&>(v)));
}

////// useless
//template<typename...Tn>
//struct AreSame;
//template<typename T1, typename... Tn>
//struct AreSame<T1, Tn...> {
//    enum {value = std::is_same<T1>::value && AreSame<Tn...>::value };
//};
//


/////////////////////////////////////////////////////////////////
////////////////    isOneOf ///////////////////////
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

// @author: Andrei Alexandrescu
// Is T one of T1, T2, ..., Tn?
template <class T, class... Ts>
struct IsOneOf {
  enum { value = false };
};

template <class T, class T1, class... Ts>
struct IsOneOf<T, T1, Ts...> {
  enum { value = std::is_same<T, T1>::value || IsOneOf<T, Ts...>::value };
};
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


__NAMESPACE_STARTER___END__



#ifdef _WIN32
#define __STARTER_ON_WINDOWS 1
#endif // _WIN32

#ifdef __MINGW__
#endif

#ifdef _WIN32_WCE
#endif

#ifdef __MINGW32__
#endif

#ifdef __linux__
#endif

#ifdef __APPLE__
#endif

#ifdef __MVS__
#endif

#ifdef defined(__sun) && defined(__SVR4)
#endif

#ifdef _AIX
#endif

#ifdef __hpux
#endif

#ifdef _WIN32_WCE
#endif

#ifdef ANDROID
#endif

#ifdef _MSC_VER
#endif

#ifdef __BORLANDC__
#endif

#ifdef __CYGWIN__
#endif


#endif // _STARTER_CONFIG_H_
