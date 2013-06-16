/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .cpp
 * @last modified:
 * @brief:
 */


#include "bwtest/bwtest.h"
#include "convert.h"

#include "TreeMap.h"
#include "TreeSet.h"
#include <set>
#include <map>
#include <vector>
#include <random>
#include <ctime>
using namespace Starter;
using namespace std;


TEST(unique, 1)
{
    map<double, int> smp;
    TreeMap<double, int> mp;
    std::vector<double> vec;
//    std::random_device randd;
//    std::mt19937 gen(randd());
//   // uniform_real_distribution<> distri(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
//
//    uniform_real_distribution<> distri(0, 1);
    srand((unsigned)time(0));

    unsigned long long int NN = 2<<15;
    vec.reserve(NN);
    for(size_t i = 0; i != (NN); ++i) {
        double t =  rand() * ((double)(rand() >> 31)) / RAND_MAX;
        smp.insert(make_pair(t,1));
        mp.insert(t, 1);
        vec.push_back(t);
    }

    expect_eq(smp.size(), mp.size()) << " std " << smp.size() << "  mine " << mp.size();

    std::sort(vec.begin(), vec.end());
    auto it = unique(vec.begin(), vec.end());
    vec.resize(it - vec.begin());
    expect_eq(vec.size(), smp.size());
}

TEST(map_insert_erase, 1)
{
    map<double, string> smp;
    TreeMap<double, string> mp;
    string strval("uni val");

    for(auto i = 0; i != (2<<6); ++i) {
        double t = rand(); // / rand();
        smp.insert(make_pair(t, strval));
        mp.insert(t, strval);
        expect_eq(1, std::distance(smp.equal_range(t).first, smp.equal_range(t).second));

        expect_eq(1, std::distance(mp.equal_range(t).first, mp.equal_range(t).second));
    }

    expect_eq(smp.size(), mp.size()) << " std " << smp.size() << "  mine " << mp.size();

    while(!mp.empty()) {
        mp.erase(mp.begin());
    }

	mp.insert( {std::make_pair(1,strval),// insert using initialzer_list
				std::make_pair(2,strval),
				std::make_pair(3,strval),
				std::make_pair(4,strval),
				std::make_pair(5,strval)
			   });

	mp.erase( {1, 2, 3, 4, 5}); // erase using initialzer_list
	expect_eq(mp.size(), 0);

    // easy insertion !
	mp.insert(7, strval)(8, strval)(9, strval)(10, strval)(11, strval)(12, strval);

    // arbitrary erasion!!
	mp.erase(7, 8, 9, 10, 11, 12);

	expect_eq(mp.size(), 0);

	mp.insert(7, strval)(8, strval)(8, strval)(8, strval);

	expect_eq(mp.size(), 2);

	MultiTreeMap<int, int> mmp;
    for(auto i = 0; i != (2<<10); ++i) {
        mmp.insert(rand(), rand());
    }

    expect_eq(mmp.size(), (2<<10));

}



TEST(clone, 1)
{
	TreeMap<int , int> rbt;
	for(int i = 0; i != 12; ++i) {
		rbt.insert(rand(), rand());
		expect_true(rbt.data().root().is_black());
	}
/////////////////////////////
	TreeMap<int , int> rbt2(rbt);/////// copy construction

	expect_eq(rbt.size(), rbt2.size());
	for(auto i = rbt.begin(), j = rbt2.begin();
			i != rbt.end(); ++i, ++j) {

        expect_eq( i->key,  j->key);
        expect_eq( i->value,  j->value);
        expect_eq( i->color_,  j->color_);
	}
	rbt2.clear();

	/////////////////////////////
	rbt2 = rbt; // assign
	expect_eq(rbt.size(), rbt2.size());

	for(auto i = rbt.begin(), j = rbt2.begin();
			i != rbt.end(); ++i, ++j) {
        expect_eq( i->key,  j->key);
        expect_eq( i->value,  j->value);
        expect_eq( i->color_,  j->color_);
	}

	 expect_true(rbt <= rbt2);
	 expect_true(rbt >= rbt2);
}


TEST(set_insert_erase, 1)
{
    set<double> smp;
    TreeSet<double> mp;

    srand((unsigned)time(0));

    for(auto i = 0; i != (2<<15); ++i) {
        double t =  rand() * ((double)(rand() >> 31)) / RAND_MAX;
        smp.insert(t);
        mp.insert(t);
    }

    expect_eq(smp.size(), mp.size()) << " std " << smp.size() << "  mine " << mp.size();

    while(!mp.empty()) {
        mp.erase(mp.begin());
    }

	mp.insert( {1, 2, 3, 4, 5});

	mp.erase( {1, 2, 3, 4, 5}); // erase using initialzer_list
	expect_eq(mp.size(), 0);

    // easy insertion !
	mp.insert(7)(8)(9)(10)(11)(12);

    // arbitrary erasion!!
	mp.erase(7, 8, 9, 10, 11, 12);

	expect_eq(mp.size(), 0);

	mp.insert(7)(8)(8)(8);

	expect_eq(mp.size(), 2);
	mp.clear();

	mp.insert(45, 46, 456, 4651, 132);
	expect_eq(mp.size(), 5);

    TreeSet<double> mp1;
    mp1 = mp;
    expect_eq(mp1, mp);
    expect_false(mp < mp);
    expect_false(mp > mp1);



	MultiTreeSet<int> mmp;
    for(auto i = 0; i != (2<<10); ++i) {
        mmp.insert(rand());
    }

    expect_eq(mmp.size(), (2<<10));

}

// due to GCC bug, currently unavailable
// TEST(compatibility, 1)
//{
//    typedef std::unordered_map<int,int> std_t;
//    std_t stds;
//    for(int i = 0; i != 12; ++i) {
//        stds.insert(std::make_pair(rand(), rand()));
//    }
//
//    TreeMap<int, int> rbt;
////    RBTree<int, double> rbt; // failed on compile
//    rbt.insert(stds.begin(), stds.end());
//
//    TreeMap<int, int> mymap;
////    TreeMap<int, double> mymap;
//    mymap.insert(stds.begin(), stds.end());
//}
//
//int main()
//{
//	RUN_ALL();
//}
