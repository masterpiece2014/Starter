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



/**
 * @file:  SArray_unit_test.cpp
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @last modified: 2013.2.13
 * @brief :
 */

#include "UnitTestHeaders.h"

#include "SArray.h"

#include "Hash.h"

using namespace Starter;

    #define DO_PRINT
    #undef DO_PRINT

    #ifdef DO_PRINT
    #define msg(x) cout << x << "  id:"<< id << "   mk:"<< mk << endl;
    #else
    #define msg(x)
    #endif
struct TestClass {

    int fillerr[50];
    double mk;
    int id;
    static int ct;

    TestClass () : TestClass(0.0) {
        msg("default Ctor ");
    }

    TestClass(double mkk): mk(mkk){
        id = ++ ct;
        msg("db_arg Ctor");
    }
    TestClass (const TestClass& other) {
        id = ++ ct;
        mk = other.mk;
        msg(" copy Ctor ");
    }
    TestClass (TestClass&& other) {
        id = ++ ct;
        mk = other.mk;
        msg(" move Ctor ");
    }
    TestClass& operator= (const TestClass& other) {
        msg(" default assign ");
        if(&other != this){
            mk = other.mk;
        }
        return *this;
    }

    TestClass& operator= (TestClass&& other) {
        mk = other.mk;
        msg(" move assign ");
        return *this;
    }
    bool operator == (const TestClass& other) const noexcept {
        return (this->mk - other.mk) < 0.00001;
    }
    virtual void greet() const {
        msg("greetings! ");
    }
    virtual ~TestClass() {
        msg("Dtor  ");
    }
};
int TestClass::ct = 0;


using ClassArray = SArray<TestClass>;
using TrivialArray = SArray<double>;
using StringArray = SArray<std::string>;

void test_constructor() {
    ClassArray sa0;
    ClassArray sa1(5);

    ClassArray sa2(5, 10);
         //   assert_has_caught(sa2[sa2.capacityR()], std::range_error& e);
            EXPECT_throw(sa2[sa2.capacityR()], std::range_error);


    ClassArray sa3(5, 10, TestClass(555.0));
         //   assert(fabs(sa3[9].mk - 555) < 0.0001);

//	if (std::is_integral<decltype (sa3[9].mk) >::value &&std::is_integral<decltype (555.0) >::value)
//    {
//        ( (0 == sa3[9].mk - 555.0) ? (void) 0 : _assert ("0 == sa3[9].mk - 555.0", "SArray_unit_test.cpp", 95));
//    }
//    else if (std::is_floating_point<decltype (sa3[9].mk) >::value && std::is_floating_point<decltype (555.0) >::value)
//    {
//        ( (fabs (sa3[9].mk - 555.0) <= 0.000001) ? (void) 0 : _assert ("fabs(sa3[9].mk - 555.0) <= 0.000001", "SArray_unit_test.cpp", 95));
//            cout <<"should\n";
//    }
//    else { cout << "fuck";}//static_assert(false, "uncomparable types");};
//


            EXPECT_eq(sa3[9].mk, 555);
//         cout << std::boolalpha;
//            if(std::is_floating_point<decltype(sa3[9].mk)>::value
//               && std::is_floating_point<decltype(555.0)>::value) {
//                 cout << "fuck"<<endl;
//               }
        //  cout << std::is_same<decltype(sa3[9].mk), double>::value <<endl;
        //  cout << std::is_same<decltype(555.0),double>::value << endl;
          //  assert_has_caught(sa3.at(sa2.capacityR()), std::out_of_range& e);
             //   expect_throw_any(sa3.at(sa2.capacityR()));
           //     expect_throw(sa3.at(sa2.capacityR()), std::out_of_range);

    ClassArray sa4(sa3);
            assert(sa4 == sa3);

    ClassArray sa5(sa1.begin(), sa1.end(), true);
            assert(sa5 == sa1);

    ClassArray sa6{ TestClass(10),TestClass(20),TestClass(30),TestClass(40)};
            assert( fabs(sa6[0].mk - 10) < 0.0001);
            assert( fabs(sa6[3].mk - 40) < 0.0001);

        // move constructor
    ClassArray sa7(SArray<TestClass>{ TestClass(10),TestClass(20),TestClass(30),TestClass(40)});
            assert(sa6 == sa7);
}

void test_setters() {

    StringArray arr{" initstring 1", " initstring 2", " initstring 3", " initstring 4"};

    arr[0] = "test string a";
            assert(arr[0] == "test string a");

    arr[arr.size() - 1] = "test string b, at end";
            assert(arr.at(3) == "test string b, at end");

    arr.push_back("test string c");
            assert(arr[arr.size() - 1] == "test string c");
    arr.pop_back();
            assert(arr[arr.size() - 1] == "test string b, at end");

    arr.push_front("pushed front string");
            assert(arr[0] == "pushed front string");
    arr.pop_front();
            assert(arr[0] == "test string a");
//==========================================================================

    StringArray arr2;
    arr2.assign({"assign string 1", "assign string 2", "assign string 3", "assign string 4", "assign string 5"});
            assert(arr2.size() == 5);
            assert(arr2[2] == "assign string 3");

    arr2.emplace_back("emplace back");
            assert(arr2[arr2.size() - 1] == "emplace back");

    arr2.emplace_front("emplace front");
            assert(arr2[0] == "emplace front");

    arr2.insert(arr2.begin(), "insert front");
            assert(arr2.front() == "insert front");

    arr2.insert(arr2.end(), "insert back");
            assert(arr2.back() == "insert back");

    arr2.insert(arr2.end(), {"insert using initializer_list 1",
                        "insert using initializer_list 2",
                        "insert using initializer_list 3"});

            assert(arr2.back() == "insert using initializer_list 3");

    arr2.erase((arr2.end() - 1));
            assert(arr2.back() == "insert using initializer_list 2");

    arr2.erase(arr2.begin());
            assert(arr2.front() == "emplace front");

    arr2.fast_erase(arr2.begin());
            assert(arr2.front() == "assign string 1");

    arr2.erase(arr2.begin(), arr2.end());
            assert(arr2.size() == 0);
//==========================================================================

    StringArray arr3(arr);
    arr3.insert(arr3.begin(), (arr.begin() + 1), arr.end());
            assert(arr3.size() == 7);

    auto capaL = arr3.capacityL();
    arr3.resize(100);
            assert(arr3.capacityL() == capaL);
            assert(arr3.capacityR() == 100);
    arr3.resize(1, 50);
            assert(arr3.capacityL() == 1);
            assert(arr3.capacityR() == 50);

    arr3.shrink_to_fit();
            assert(arr3.size() == arr3.capacity());
}

void test_comparator() {

    TrivialArray arr{1.11, 2.22, 3.33, 4.44, 5.55, 6.66};
   TrivialArray arr2{1.11, 2.22, 4.33, 3.44, 5.55, 6.66};
        assert(arr != arr2);
        assert(arr < arr2);
        assert(arr2 > arr);
        assert(arr <= arr2);
        assert(arr2 >= arr);

        assert(std::distance(arr.begin(), arr.end()) == 6);

    std::sort(arr2.begin(), arr2.end());
//    for(auto i : arr2) {
//        cout << i << endl;
//    }
       // assert(arr2 == arr);
}

void test_hash() {
    const int SIZE = 1024;
    void* ptr = malloc(SIZE * sizeof(double));

    TrivialArray arr(0, SIZE), arrMirror;
    for(int i = 0; i != SIZE; ++i)  {
        auto tmp = rand();
        arr.push_back(tmp);
        arrMirror.push_back(tmp);
        *((double*)ptr + i) = tmp;
    }
            assert(arr.size() == SIZE);

    uint64_t true_hash = StarterHash::fnv64_buf(ptr, SIZE * sizeof(double));
    free(ptr);
    ptr = nullptr;

            assert(arr.hash_code() == true_hash);

    TrivialArray arr2(arr);
            assert(arr== arr2);
            assert(arr.hash_code() == arr2.hash_code());


            assert(arr.hash_code() == arrMirror.hash_code());
    arrMirror[arrMirror.size() - 1] -= 1;
            assert(arr.hash_code() != arrMirror.hash_code());
}

//int runAllTest()
//{
//	try {
//	    const int LOOP = 32;
//
//	//    for(int i = 0; i != LOOP; ++i) {
//            test_constructor();
////            test_setters();
////            test_comparator();
////            test_hash();
//	//   }
//	} catch (std::exception& e) {
//		cout << "caught standard exception \n\t"<< e.what() << endl;
//		exit (1);
//	} catch (...) {
//		cout << "unknown error" << endl;
//		exit (1);
//	}
//cout << "\n----------------------------------------------\n";
//	cout << endl;
//	cout << "\t ***  ***" << endl;
//    cout << "\t  o    o" << endl;
//	cout << "\t    ||  " <<endl;
//	cout << "\t    /\\  " <<endl;
//	cout << "\t    \\/  " <<endl;
//	cout << endl;
//	cout << "\t   bravo! " << endl;
//cout << "\n----------------------------------------------\n";
//	return EXIT_SUCCESS;
//}


//int main() {
//    return runAllTest();
//}
