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
#if __GNUC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ < 7
#error please update to GCC 4.7 or higher
#endif
#endif

//#ifdef _MSC_VER
//#if _MSC_VER < 1700
//#error please update to MS VC 14 or higher
//#endif
//#endif // _MSC_VER

#ifndef _STARTER_CONFIG_H_
#define _STARTER_CONFIG_H_
#pragma once

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

//# elif defined(__MINGW__) || defined(__MINGW32__)
//# define GTEST_GCC_VER_ \
//    (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)
//
#ifdef __GNUC__
#define _STARTER_GCC_VERSION (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)
#endif // __GNUC__


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



#ifdef _STARTER_GCC_VERSION
//#pragma message("using g++")
#include <bits/functexcept.h>
using std::__throw_bad_alloc;
using std::__throw_out_of_range;
using std::__throw_range_error;
#else
#define __throw_bad_alloc throw\
                            std::bad_alloc
#define __throw_out_of_range(x) throw\
                            std::out_of_range(x)
#define __throw_range_error(x) throw\
                            std::range_error(x)
#endif

#ifdef _MSC_VER
#pragma message("using MS VC")
#endif


__NAMESPACE_STARTER___BEGIN__

const uint16_t g_inPlcaeExpansionBound = 4096;

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


#endif // _STARTER_CONFIG_H_
