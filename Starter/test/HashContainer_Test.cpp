
/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .cpp
 * @last modified:
 * @brief:
 */




#include <set>
#include <map>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include <cmath>
#include "bwtest/bwtest.h"
#include "convert.h"

//#include "HyMap.h"


//using namespace Starter;
using namespace std;

TEST(init, 1)
{
//    put_out <<  (Starter::detail_::PrimeLList[45] )<< endl;
//    put_out << (Starter::detail_::nextPrime(359)) << endl;
    vector<uint32_t> vec;
  //  runEratosthenesSieve(numeric_limits<uint64_t>::max() / 10 * 9, vec);
    //cout << vec.size() << endl;
    double rat = (double)18446744073709551557 / numeric_limits<uint64_t>::max();
    cout <<  rat << endl;

    cout <<18446744073709551557ul << endl;
    cout << numeric_limits<signed long>::max() << endl;
//    for(int i = 0; PrimeNumbers[i] <= numeric_limits<signed long>::max(); ++i) {
//        put_out << PrimeNumbers[i] <<',' <<'\n';
//    }
    cout <<"  " << ((pow(2, 63)) - 1) << endl;
   // cout << std::boolalpha << is_prime(18446744073709551557) << endl;
   // HyMap<int, int> hy;

//    std::vector<int> vee(100);
//    cout << "  " << Starter::hashCode(vee) << endl;


}
// < numeric_limits<uint32_t>::max()
//


int main()
{
   // bwtest::setOutput("prime_nums.txt");
    RUN_ALL();
    return 0;
}














