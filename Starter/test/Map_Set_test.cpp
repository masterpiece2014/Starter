/**
 * @author: CaiBowen
 * @feedback: feedback2bowen@outlook.com
 * @file: .cpp
 * @last modified:
 * @brief:
 */

#include <cstdlib>

#include "bwtest/bwtest.h"
#include "convert.h"

#include "Map.h"
#include "Set.h"

using namespace Starter;
using namespace std;

TEST(insert_erase, 1)
{
	Map<int, int> mp;
	for(int i = 0; i != (2 << 15); ++i) {
		mp.insert(rand(), rand());      // ordinary insertion
	}
	expect_eq(mp.size(), (2 << 15));
	while(! mp.empty()) {
		mp.erase(mp.begin());  // ordinary erasion
	}

	expect_true(mp.empty());
	for(int i = 0; i != (2 << 6); ++i) {
		mp.insert(rand(), rand());
	}
	expect_eq(mp.size(), 2<<6);

	mp.insert( {std::make_pair(1,2),// insert using initialzer_list
				std::make_pair(2,3),
				std::make_pair(3,4),
				std::make_pair(4,5),
				std::make_pair(5,6)
			   });

	mp.erase( {1, 2, 3, 4, 5}); // erase using initialzer_list
	expect_eq(mp.size(), (2<<6));


	mp.insert(7, 8)(8, 9)(9, 10)(10, 15)(11, 23)(12, 45); // easy insertion !

	mp.erase(7)(8)(9)(10)(11)(12);                          // east erasion!

	expect_eq(mp.size(), (2<<6));

	mp.insert(7, 8)(8, 9)(9, 10)(10, 15)(11, 23)(12, 45); // easy insertion !

	mp.erase(7, 8, 9, 10)(11, 12);                  //////////////// arbitrary erasion!!

	expect_eq(mp.size(), (2<<6));

}



TEST(clone, 1)
{
	Map<int , int> rbt;
	for(int i = 0; i != 12; ++i) {
		rbt.insert(rand(), rand());
	}
/////////////////////////////
	Map<int , int> rbt2(rbt);/////// copy construction
	expect_eq(rbt.size(), rbt2.size());
	for(auto i = rbt.begin(), j = rbt2.begin();
			i != rbt.end(); ++i, ++j) {

		cout << i->key << " " << i->value << "  " << i->color_
			 << " || " << j->key << "  " << j->value << "  " << j->color_ << endl;
	}
	rbt2.clear();

	/////////////////////////////
	rbt2 = rbt; // assign

	expect_eq(rbt.size(), rbt2.size());
	for(auto i = rbt.begin(), j = rbt2.begin();
			i != rbt.end(); ++i, ++j) {

		cout << i->key << " " << i->value << "  " << i->color_
			 << " || " << j->key << "  " << j->value << "  " << j->color_ << endl;
	}
}
TEST(set, 1)
{
    Set<string> sset;
    for(auto i = 0; i != (2<<6); ++i) {
        sset.insert(to<string>(rand()));
    }
    expect_eq(sset.size(), (2<<6));

    sset.clear();
    expect_eq(sset.size(), 0);

    sset.insert(to<string>("elem", rand()),
                to<string>("elem", rand()),
                to<string>("elem", rand()),
                to<string>("elem", rand()),
                to<string>("elem", rand()));

    expect_eq(sset.size(), 5);

}
// TEST(compatibility, 1)
//{
//    typedef std::unordered_map<int,int> std_t;
//    std_t stds;
//    for(int i = 0; i != 12; ++i) {
//        stds.insert(std::make_pair(rand(), rand()));
//    }
//
//    Map<int, int> rbt;
////    RBTree<int, double> rbt; // failed on compile
//    rbt.insert(stds.begin(), stds.end());
//
//    Map<int, int> mymap;
////    Map<int, double> mymap;
//    mymap.insert(stds.begin(), stds.end());
//}

int main()
{

	RUN_ALL();

}
