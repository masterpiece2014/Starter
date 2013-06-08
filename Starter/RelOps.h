/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .h
 * @last modified:
 * @brief:
 */


#ifndef _STARTER_RELOPS_H_
#define _STARTER_RELOPS_H_


#include "StarterCfgDef.h"

__NAMESPACE_STARTER___BEGIN__

template<typename T>
struct RelOps
{
    bool operator!= (const T& other)const noexcept {
        return !this->operator==(other);
    }
    bool operator<= (const T& other) const noexcept {
        return *this == other || *this < other;
    }
    bool operator> (const T& other) const noexcept {
        return ! (*this <= other);
    }
    bool operator>= (const T& other) const noexcept {
        return ! (*this < other);
    }
};

__NAMESPACE_STARTER___END__
#endif // _STARTER_RELOPS_H_
