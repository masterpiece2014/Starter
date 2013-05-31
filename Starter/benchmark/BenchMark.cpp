

#include <iostream>
#include <iomanip>
using std::cout;
using std::cin;
using std::endl;
using std::setw;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sstream>
#include <string>
#include <cstring>
using std::string;
#include <cstdint>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <new>
#include <list>
#include <vector>
#include <deque>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <random>
using namespace std;

const size_t FILLER_SIZE = 1024;
class Filler
{
public:
	//virtual void greet()    {cout << " Cheers!" << endl; }
	unsigned int filler[FILLER_SIZE];
	size_t sum()const {return  std::accumulate(filler, filler + FILLER_SIZE - 1, 0);}
};
//const size_t LOOP = 65536;//1
//const size_t LOOP = 4096;
const size_t LOOP = 15;

/********************/

  //  #include "Linkedlist_test.h"
        #include "SArray_test.h"
   //   #include "Realloc_test.h"
   //   #include "PArray_test.h"
  //  #include "rallocm_test.h"
  //  #include "Miscellanea.h"

/********************/


/********************/

int main()
{
    string str;
    cout << "enter \"q\" to exit" << endl;
    cin >> str;
    if (str == "q")
        exit(0);
    cout << "go !" << endl;
    try{

  //  __throw_out_of_range("test msg");
      //          ofstream logger("BenchmarkLog.txt", ios::app);
        std::ostream &logger = cout;
        logger << std::boolalpha;

cout << "\n\t-------------  1st Test -------------\n" << endl;
/////////  STOP WATCH  ///////////////////////////////////
auto t_start = std::chrono::high_resolution_clock::now();
auto t_0 = t_start;
/////////  STOP WATCH  ///////////////////////////////////
/********************** 1st Test Filed ******************************/
         // test_LinkedList();
    //    test1_LinkedList();
    //    test2test_5_LinkedList_LinkedList();
   //   test3_LinkedList();
//logger << "isOneOf   " << Starter::IsOneOf<size_t, long unsigned int, unsigned long  int, float,
//                                    signed long int, unsigned long long int,double>::value << endl;

  //  cout << std::is_same<ty, bool>::value << endl;
   // cout << IsOneOf<size_t, bool, short, int, long, unsigned short, unsigned int, unsigned long,
    //                    unsigned long long, long long>::value << endl;
  //  cout << typeid(ty).name()<< endl;


   // cout << std::is_integral<typpp>::value << endl;


//    cout << std::is_same<typpp, std::true_type>::value << endl;
//    cout << std::is_same<typpp, std::false_type>::value << endl;
//    cout << typeid(std::true_type).name()<< endl;
//    cout << typeid(std::false_type).name()<< endl;

/************************************************************/
/////////  STOP WATCH  ///////////////////////////////////
auto t_1 = std::chrono::high_resolution_clock::now();
cout << "\n\t------------- 2st Test -------------\n" << endl;
/////////  STOP WATCH  ///////////////////////////////////
/********************** 2st Test Filed ***********************/

/************************************************************/
/////////  STOP WATCH  ///////////////////////////////////
auto t_2 = std::chrono::high_resolution_clock::now();
cout << "\n\t------------- 3st Test -------------\n" << endl;
/////////  STOP WATCH  ///////////////////////////////////
/********************** 3st Test Filed ******************************/
//
//   test_LinkedList();
//  test1_LinkedList();
//   test2_LinkedList();
//    test3_LinkedList();
//    test_4();
//       test_5_LinkedList();

    //   test_hash();

//    cout << std::is_same<size_t, unsigned int>::value << endl;
//    cout << std::numeric_limits<size_t>::max() << endl;
//    cout << endl;
//    cout << sizeof(size_t) << endl;

//    for (uint32_t mi = 15; mi != 35; ++mi) {
//        uint64_t seed = 1 << mi;
//        cout << "\nseed  "<< seed << endl;
//        cout << "diff  "<< std::numeric_limits<size_t>::max() - seed << endl;
//        cout << mi << endl << endl;
//    }
//    uint32_t mi = 31;
//    uint32_t seed = (1 << 32) - 1;
//   // seed--;
//    cout << "seed  "<< seed << endl;
//    cout << " max  "<< std::numeric_limits<size_t>::max() << endl;
//    cout << "diff  "<< uint64_t(std::numeric_limits<size_t>::max()) - seed << endl;

          //    test_SArray_1();
        //  test_erase();
          //  test_insertion();
          test_assign();
          //  test_hash();


/************************************************************/
/////////  STOP WATCH  ///////////////////////////////////
auto t_3 = std::chrono::high_resolution_clock::now();
/////////  STOP WATCH  ///////////////////////////////////
cout << "\n\t------------- Finished -------------\n" << endl;
/////////  STOP WATCH  ///////////////////////////////////
auto t_end = std::chrono::high_resolution_clock::now();
/////////  STOP WATCH  ///////////////////////////////////
//cout << "enter any thing to quit" << endl;
//cin >> str;
logger << "Test 1:\n\t";
logger << "Time:" << std::chrono::duration_cast<std::chrono::microseconds>(t_1 - t_0).count()
                        << " microseconds" << endl << endl;
logger << "Test 2:\n\t";
logger << "Time:" << std::chrono::duration_cast<std::chrono::microseconds>(t_2 - t_1 + t_3 - t_end).count()
                        << " microseconds" << endl << endl;
logger << "Test 3:\n\t";
logger << "Time:" << std::chrono::duration_cast<std::chrono::microseconds>(t_3 - t_2 + t_3 - t_end).count()
                        << " microseconds" << endl << endl;

logger << "Total:\n\t";
logger << "Time:" << std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count()
                        << endl << endl;

    //  logger.close();

cout << endl;
//nanoseconds
//microseconds
//milliseconds
//seconds
}
catch(std::exception& e)
{
cout << e.what() << endl;
exit(1);
}
catch (...)
{
cout << "unknown error" << endl;
exit(1);
}
return 0;
}
