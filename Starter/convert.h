 ///////////////////////////////////////////////////////////////////////////////
 ////     Copyright 2012 2013 CaiBowen
 ////     All rights reserved.
 ////
 ////     Author: Cai Bowen
 ////       contact/bug report/get new version
 ////           at
 ////       feedback2bowen@outlook.com
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
 * @file: Convert.h
 * @brief: fast and safe conversion between string and other types
 *          illustrated at convert_test.cpp
*/

#ifndef _STARTER_CONVERT_H_
#define _STARTER_CONVERT_H_

//#include "StarterCfgDef.h"


#include <sstream>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <string>
#include <cstdlib>
#include <cstring>


//__NAMESPACE_STARTER___BEGIN__

#undef RANGE_CHEACK
#define RANGE_CHEACK(val, TypeName)\
        val <= static_cast<decltype(val)>(std::numeric_limits<TypeName>::max())\
            ?   (void)0 : throw std::overflow_error("overflow in convertion");\
        val >= static_cast<decltype(val)>(std::numeric_limits<TypeName>::min())\
                ? (void)0 : throw std::underflow_error("underflow in convertion")


//#undef ERROR_MSG
//#define ERROR_MSG(message)\
//(__FILE__ "(" + std::to_string((long long int) __LINE__) + "): " + (message)\
//).c_str()
//
//#undef RANGE_CHEACK
//#define RANGE_CHEACK(val, TypeName)\
//        val <= static_cast<decltype(val)>(std::numeric_limits<TypeName>::max())\
//            ?   (void)0 : throw std::overflow_error(ERROR_MSG("overflow in convertion"));\
//        val >= static_cast<decltype(val)>(std::numeric_limits<TypeName>::min())\
//                ? (void)0 : throw std::underflow_error(ERROR_MSG("underflow in convertion"))

namespace detail_
{
 ///////////////////////////
 /// \brief for custom string, modify this.
 ///    used to enable functions
 ///    custom string must provide below functions
///     constructor: initialize with std::string
///     append(): similar with append() of std::string
///     c_str() :
 ///////////////////////////
    template<typename StrType>
    struct isString
    {
        enum {value = std::is_same<StrType, std::string>::value};
    };
    uint8_t countDigits(uint64_t val) noexcept {
        uint8_t digits = 1;
        while(true) {
            if (val < 10)   return digits;
            if (val < 100)  return digits + 1;
            if (val < 1000) return digits + 2;
            val /= 1000;
            digits += 3;
        }
    }
} // namespace detail_

//---------------------------------------------------------
//                                  float append to string
//----------------------------------------------------------
    template <typename Flo, typename StrType>
        typename std::enable_if<detail_::isString<StrType>::value
                                && std::is_floating_point<Flo>::value,
        void>::type
        appendTo(StrType& str, Flo foo) {
            std::stringstream sstrm;
            sstrm << foo;
            str.append(sstrm.str());
        }
//----------------------------------------------------------
//                                  integer append to string
//----------------------------------------------------------
//-----------------------------------------------
//              unsigned integer append to string
//-----------------------------------------------
    template <typename Int, typename StrType>
        typename std::enable_if<detail_::isString<StrType>::value
                                && std::is_integral<Int>::value
                                && !std::is_signed<Int>::value,
        void>::type
    appendTo(StrType& str, Int val) {
        if (val) {
            uint8_t digit = detail_::countDigits((uint64_t)val);
            char buffer[21];
            buffer[digit] = '\0';
            uint8_t ch = val % 10;
            while(val >= 10) {
                buffer[--digit] = '0' + ch;
                val /= 10;
                ch = val % 10;
            }
            buffer[--digit] = '0' + val;
            str.append(buffer);
        }
        else {
            str.append("0");
        }
    }
//--------------------------------------------
//              signed integer append to string
//--------------------------------------------
    template <typename Int, typename StrType>
        typename std::enable_if<detail_::isString<StrType>::value
                                && std::is_integral<Int>::value
                                && std::is_signed<Int>::value,
        void>::type
    appendTo(StrType& str, Int val) {
        if(!val) {
            str.append("0");
            return;
        }
        else if (val < 0) {
            val = -val;
            char buffer[22];
            uint8_t digit = detail_::countDigits((uint64_t)val);
            buffer[++digit] = '\0';
            uint8_t ch = val % 10;
            while(val >= 10) {
                buffer[--digit] = '0' + ch;
                val /= 10;
                ch = val % 10;
            }
            buffer[--digit] = '0' + val;
            buffer[0] = '-';
            str.append(buffer);
        }
        else {
            uint8_t digit = detail_::countDigits((uint64_t)val);
            char buffer[21];
            buffer[digit] = '\0';
            uint8_t ch = val % 10;
            while(val >= 10) {
                buffer[--digit] = '0' + ch;
                val /= 10;
                ch = val % 10;
            }
            buffer[--digit] = '0' + val;
            str.append(buffer);
        }
    }
//-------------------------------------------------
//                    boolean append to std::string
//-------------------------------------------------
    template<typename StrType>
        typename std::enable_if<detail_::isString<StrType>::value,void>::type
    appendTo(StrType& acceptor, bool boolean, bool toUpperCase = false) {
        if(toUpperCase) {
            if(boolean)
                acceptor.append("TRUE");
            else
                acceptor.append("FALSE");
        }
        else {
            if(boolean)
                acceptor.append("true");
            else
                acceptor.append("false");
        }
    }
//-----------------------------------------------------
//                            c-string append to string
//------------------------------------------------------
    template<typename cCharT, typename StrType>
        typename std::enable_if<
                std::is_convertible<cCharT,const char*>::value
                &&detail_::isString<StrType>::value,
        void>::type
    appendTo( StrType& acceptor, cCharT c_str) {
        auto ptr = static_cast<const char*>(c_str);
        if(ptr)
            acceptor.append(ptr);
    }
//-----------------------------------------------------
//                                char append to string
//-----------------------------------------------------
    template<typename StrType>
    typename std::enable_if<detail_::isString<StrType>::value,
        void>::type
    appendTo(StrType& str, char ch) {
        char c_str[2];
        c_str[0] = ch;
        c_str[1] = '\0';
        str.append(c_str);
    }
//----------------------------------------------------
//                            string append to string
//----------------------------------------------------
    template <typename StrType1, typename StrType2>
        typename std::enable_if<detail_::isString<StrType1>::value
                                && detail_::isString<StrType2>::value,
        void>::type
    appendTo(StrType1& acceptor, StrType2& src) {
        acceptor.append(src);
    }


//---------------------------------------------------------------------
//                                                 integrated appending
//---------------------------------------------------------------------
template <typename StrType>
void
appendTo(StrType& result) {
}
template<typename StrType, typename Num, typename... Oth>
void
appendTo(StrType& str, Num num, Oth...oth) {
    appendTo(str, num);
    appendTo(str, oth...);
}
//-----------------------------------------------------------------------
//                            integers  <<===>>  floating-point numbers
//-----------------------------------------------------------------------

    template<typename TargeType, typename SrcType>
        inline typename std::enable_if<
            std::is_convertible<SrcType, TargeType>::value,
    TargeType>::type
    to(const SrcType& raws) {
        RANGE_CHEACK(raws, TargeType);
        return static_cast<SrcType>(raws);
    }
namespace detail_
{
    const char*
    _skipBlanks(const char* c_str) {
        size_t len = strlen(c_str);
        size_t cur = 0;
        for(;cur != len; ++cur) {
            if(! isspace(c_str[cur])){
                break;
            }
        }
        if (cur == len) {
            throw std::invalid_argument("cannot convert empty string");
        }
        return (const char*)(c_str + cur);
    }

    double
    _reparse(const char* c_str) {
        c_str = _skipBlanks(c_str);
        size_t len = strlen(c_str);
        switch(c_str[0]) {
        case 'e':   if (len == 1)   return 2.7182818284590452353602874713527;
        case 'p':
        case 'P':   if (toupper(c_str[1]) =='I'
                        && c_str[2] == ' ')
                    return 3.1415926535897932384626433832795;
        case 'i':
        case 'I':   if (strncmp(c_str, "INFINITY", 8)
                        ||strncmp(c_str, "infinity", 8))
                    return std::numeric_limits<double>::infinity();
        case 'n':
        case 'N':   if (strncmp(c_str, "NaN", 3)
                        ||strncmp(c_str, "nan", 3))
                  //  return std::numeric_limits<double>::signaling_NaN();
                   return std::numeric_limits<double>::quiet_NaN();
        default : throw std::invalid_argument("unable to parse string to number");
        }// switch
    }
} // namespace detail_
//----------------------------------------------------------------
//                                           String ==>  number
//----------------------------------------------------------------
////////// declaration
    template <typename Number>
    Number
    to(const char*);

    template <typename Number, typename StrType>
        typename std::enable_if<detail_::isString<StrType>::value,
        Number>::type
    to(StrType&);
//////// specification
#define FUNCTION_FACTORY_STR_TO_NUM(Target, Buff, std_func_call)\
    template<> Target\
    to<Target> (const char* c_str) {\
        Buff buf = std_func_call;\
        if (!buf) {\
            return static_cast<Target>\
                (detail_::_reparse(c_str));\
        }\
        else {\
            RANGE_CHEACK(buf, Target);\
            return static_cast<Target>(buf);\
        }\
    }\
    template<>\
    Target\
    to<Target> (std::string& str) {\
        return to<Target>(str.c_str());\
    }

FUNCTION_FACTORY_STR_TO_NUM(signed short, signed long long int, strtol(c_str, NULL, 0))
FUNCTION_FACTORY_STR_TO_NUM(unsigned short, unsigned long long int, strtoul(c_str, NULL, 0))
FUNCTION_FACTORY_STR_TO_NUM(signed int,  signed long long int, strtol(c_str, NULL, 0))
FUNCTION_FACTORY_STR_TO_NUM(unsigned int, unsigned long long int, strtoll(c_str, NULL, 0))
FUNCTION_FACTORY_STR_TO_NUM(signed long long int,  signed long long int, strtoll(c_str, NULL, 0))
FUNCTION_FACTORY_STR_TO_NUM(unsigned long long int, unsigned long long int, strtoull(c_str, NULL, 0))

FUNCTION_FACTORY_STR_TO_NUM(float, long double, strtold(c_str, NULL))
FUNCTION_FACTORY_STR_TO_NUM(double, long double, strtold(c_str, NULL))
FUNCTION_FACTORY_STR_TO_NUM(long double, long double, strtold(c_str, NULL))
#undef FUNCTION_FACTORY_STR_TO_NUM

//----------------------------------------------------------------
//                                           String ==>  boolean
//----------------------------------------------------------------
    template<> bool
    to<bool>(const char* c_str) {
        c_str = detail_::_skipBlanks(c_str);
        size_t len = strlen(c_str);
        switch(c_str[0]) {
        case '0':   if (len == 1)   return false;
        case '1':   if (len == 1)   return true;
        case 't':
        case 'T':   if (len == 1
                        || (strncmp(c_str, "true", 4)
                             ||strncmp(c_str, "TRUE", 4)))
                    return true;
        case 'f':
        case 'F':   if (len == 1
                        || (strncmp(c_str, "false", 5)
                             ||strncmp(c_str, "FALSE", 5)))
                    return false;
        case 'y':
        case 'Y':   if (len == 1
                        || (strncmp(c_str, "yes", 3)
                             ||strncmp(c_str, "YES", 3)))
                    return true;
        case 'n':
        case 'N':   if (len == 1
                        || (strncmp(c_str, "no", 2)
                             ||strncmp(c_str, "NO", 2)))
                    return false;
        case 'o':
        case 'O':
                if (strncmp(c_str, "ON", 2)
                    || strncmp(c_str, "on", 2))
                    return true;
        default: throw std::invalid_argument("unable to parse string to boolean");
        }   // switch
    }

    template<> bool
    to<bool>(std::string& str) {
        return to<bool>(str.c_str());
    }

//----------------------------------------------------------------------
//                                          multi type args  ==>  string
//----------------------------------------------------------------------
    template<typename StrType, typename... Raws>
        typename std::enable_if<detail_::isString<StrType>::value,
        StrType>::type
    to(const Raws&...rawMaterial) {
        StrType str;
        appendTo(str, rawMaterial...);
        return str;
    }

//__NAMESPACE_STARTER___END__
#endif // _STARTER_CONVERT_H_





