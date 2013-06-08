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
    friend bool operator!= (const T& jia, const T& yi) noexcept {
        return !jia == yi;
    }
    friend bool operator<= (const T& jia, const T& yi) noexcept {
        return (jia < yi || jia == yi);
    }
    friend bool operator> (const T& jia, const T& yi) noexcept {
        return yi < jia;
    }
    friend bool operator>= (const T& jia, const T& yi) noexcept {
        return !(jia < yi);
    }
};

__NAMESPACE_STARTER___END__
#endif // _STARTER_RELOPS_H_
