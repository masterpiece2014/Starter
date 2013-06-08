
/**
 * @file:  PArray.h
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @last modified: 2013.2.1
 * @brief :
 */

#ifndef _STARTER_PARRAY_H_
#define _STARTER_PARRAY_H_

#include <new>
#include<type_traits>

#ifndef __NAMESPACE_STARTER___BEGIN__
#define __NAMESPACE_STARTER___BEGIN__   namespace Starter { /* namespace Starter  */
#define __NAMESPACE_STARTER___END__     } /* namespace Starter  */
#endif

__NAMESPACE_STARTER___BEGIN__


template <typename T, typename Sequence = std::deque<T*>>
class PArray
{

    public:
        typedef T                       value_type;
        typedef T*                      pointer;
        typedef const T*                const_pointer;
        typedef T&                      reference;
        typedef const T&                const_reference;
        typedef pointer                 iterator;
        typedef const_pointer           const_iterator;
        typedef size_t                  size_type;
        typedef ptrdiff_t               difference_type;

	protected:
		Sequence seq;
		M_cleanUp() noexcept {
			for (T*& ptr: seq) {
				delete ptr;
			}
			seq.clear();
		}
    public:
//------------------------------------------------------------------------------------
//                              ctor, operator=
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//                              iterator
//------------------------------------------------------------------------------------
    T*
    begin () noexcept {
        return *seq.begin();
    }
//------------------------------------------------------------------------------------
//                              ctor, operator=
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//                              ctor, operator=
//------------------------------------------------------------------------------------
};



//
// front
// access the first element
//(public member function)
//
//
//
// back
// access the last element
//(public member function)
//
// Iterators
//
//
//
//
// begin
// cbegin
//
//
//
// returns an iterator to the beginning
//(public member function)
//
//
//
// end
//cend
//
//
//
// returns an iterator to the end
//(public member function)
//
//
//
// rbegin
// crbegin
//
//
//
// returns a reverse iterator to the beginning
//(public member function)
//
//
//
// rend
// crend
//
//
//
// returns a reverse iterator to the end
//(public member function)
//
// Capacity
//
//
//
//
// empty
// checks whether the container is empty
//(public member function)
//
//
//
// size
// returns the number of elements
//(public member function)
//
//
//
// max_size
// returns the maximum possible number of elements
//(public member function)
//
// Modifiers
//
//
//
//
// clear
// clears the contents
//(public member function)
//
//
//
// insert
// inserts elements
//(public member function)
//
//
//
// emplace
//
//(C++11)
// constructs element in-place
//(public member function)
//
//
//
// erase
// erases elements
//(public member function)
//
//
//
// push_back
// adds elements to the end
//(public member function)
//
//
//
// emplace_back
//
//(C++11)
// constructs elements in-place at the end
//(public member function)
//
//
//
// pop_back
// removes the last element
//(public member function)
//
//
//
// push_front
// inserts elements to the beginning
//(public member function)
//
//
//
// emplace_front
//
//(C++11)
// constructs elements in-place at the beginning
//(public member function)
//
//
//
// pop_front
// removes the first element
//(public member function)
//
//
//
// resize
// changes the number of elements stored
//(public member function)
//
//
//
// swap
// swaps the contents
//(public member function)

__NAMESPACE_STARTER___END__
#endif // _STARTER_PARRAY_H_







