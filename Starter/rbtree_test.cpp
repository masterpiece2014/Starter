


#include "bwtest/bwtest.h"
#include "rbtree.h"
#include "convert.h"
#include <map>
#include <algorithm>
#include <random>
using namespace std;

struct TT
{
    int data[100];
    std::string* strs;
    virtual void yell() {}
    TT(){strs = new std::string[100];}
    ~TT(){delete [] strs;}
    TT(const TT& other) {
        this->strs = new string[100];
    }
};

TEST(iter, 0)
{
cout << "iterator\n";
    RBTree<int, int> rbt;
    for(int i = 0; i != 16; ++i) {
        rbt.insert(rand(), rand());
    }
    cout << "go  ";
    for(auto i = rbt.begin(); i != rbt.end(); ++i) {
        cout << i->key << "   " << i->value  << "   "<< i->color_ << endl;
    }
    cout << "  finished\n";
}



TEST(heavy_load, 0)
{
    const size_t LOOP = 2 << 6;
    RBTree<int , int>    rbt;
    put_out << "start\n\n";

    rbt.insert(rand(), rand());
    rbt.erase(rbt.begin());

    for(size_t i = 0; i != LOOP; ++i) {
        rbt.insert(rand(), rand());
    }
    put_out << "insertion finished   " << rbt.size() << "\n\n";

    while (! rbt.empty()) {
        rbt.erase(rbt.begin());
        if (!rbt.size() % 1000) {
            cout  <<rbt.size() << "\n";
        }
    }
    put_out << "the other half   "<< rbt.size() << "\n\n\n";

    for(size_t i = 0; i != LOOP; ++i) {
        rbt.insert(rand(), rand());
            if (!rbt.size() % 1000) {
            cout <<rbt.size()<< " another 1000\n\n";
        }
    }

    put_out << "insertion finished  " << rbt.size() << "\n\n";
    while (! rbt.empty()) {
            if (!rbt.size() % 1000) {
            cout <<rbt.size()<< " another 1000\n\n";
        }
        rbt.erase(rbt.max());
    }

    put_out << "done   " << rbt.size() << "\n\n";
}

TEST(erase_ , 0)
{
    const size_t LOOP = 18;
    RBTree<int , double>    rbt;
    put_out << "start\n\n";
    for(size_t i = 0; i != LOOP; ++i) {
        rbt.insert(rand() / 1000000, 1.1);
    }
    put_out << "insertion finished\n\n";
    int i = 7;
    while (i--) {

    cout << "\n\n <!size  " << rbt.size();
//      rbt.down_iterate(rbt.root().node_,
//        [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;}
//        );

      //  rbt.erase(rbt.begin());
        rbt.erase(rbt.max());
    }

    while (!rbt.empty()) {

    cout << "\n\n <!size  " << rbt.size();
//      rbt.down_iterate(rbt.root().node_,
//        [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;}
//        );

       // rbt.erase(rbt.begin());
        rbt.erase(rbt.max());
    }
}




template<typename XX>
    void down_iterate(XX* node)  {

      //  put_out << "\n ( root:";
auto tmp = static_cast<std::_Rb_tree_node<std::pair<int, int> > *>(node);

        put_out << "\n ( root:";

        cout << tmp->_M_value_field.first << "  " << tmp->_M_color;

        if (node->_M_left) {

            put_out << " left: ";
            down_iterate(node->_M_left);
        }


        if (node->_M_right) {
            put_out << " right: ";
            down_iterate(node->_M_right);
        }

        put_out << ')';//<<  std::endl;

    }


TEST(compare_stl, 0)
{
    std::map<int, int> stds;
    RBTree<int, int>    mine;

    stds.insert(std::make_pair(11111,11111));
    mine.insert(11111, 11111);

    expect_eq(mine.size(), stds.size());

    for(int i = 0; i != 16; ++i) {
        int t = rand();
        stds.insert(std::make_pair(t,t ));
        mine.insert(t, t);
    }

    expect_eq(mine.size(), stds.size());
//      mine.down_iterate(mine.root().node_,
//        [](RBTree<int, int>::Node* n)->void{put_out << n->key << " " << n->color_;}
//        );

//    cout << "\n\tstandard:";
//    auto iter = stds.begin();
//    while (iter->first != 11111) { ++iter;}
//    down_iterate(iter._M_node);
///////////////////////////////////////////////////1
//    for (int i = 0; i != 7; ++i)
//    {
//            int t = rand() / 100000;
//            stds.insert(std::make_pair(t,t));
//            mine.insert(t, t);
////                cout << "\n ====mine:\n";
////      mine.down_iterate(mine.root().node_,
////        [](RBTree<int, int>::Node* n)->void{put_out << n->key << " " << n->color_;}
////        );
////                cout << "\n ====std:\n";
//          //  down_iterate(stds._M_t._M_root());
//    }



    expect_eq(mine.size(), stds.size()) << "  " << mine.size()<< "  " << stds.size();

    cout << "\n\tok, normal" << mine.size() << "\n\n";



    cout << "\n\tok, done" << mine.size() << "\n\n";
//
    while (!mine.empty()) {
        stds.erase(stds.begin());
        mine.erase(mine.begin());
    }

    cout << "\n\tok, done" << mine.size() << "\n\n";

}


TEST(lasrt_one, 0)
{
    const size_t LOOP = 2 << 12;
    RBTree<int, int> rbt;
    for(size_t i = 0; i != LOOP;++i)
    {
        rbt.insert(rand(), rand());
    }

    while(! rbt.empty()) {
   // cout << rbt.size() << endl;
        rbt.erase(rbt.begin());
    }
cout << "\n\n\tsdgdfg\n\n";

    for(size_t i = 0; i != LOOP;++i)
    {
        rbt.insert(rand(), rand());
    }
    while(! rbt.empty()) {
        rbt.erase(rbt.max());
    }
}



TEST(added_balancing_func___and_now_we_have_the_real, 0)
{
    RBTree<int, double> rbt;
    auto equal_node = [](const RBTree<int, double>::Node& a,
                         const RBTree<int, double>::Node& b)->bool{
                         return a.key == b.key;
                         };


//put_out << __LINE__ << " still fine.\n";

    rbt.insert(1, 564.464);

    rbt.insert(2, 564.464);
    rbt.insert(3, 564.464);
    rbt.insert(4, 564.464);
    rbt.insert(7, 564.464);

    rbt.insert(9, 564.464);
    rbt.insert(5, 564.464);
    rbt.insert(6, 564.464);
    rbt.insert(12, 564.464);
    rbt.insert(15, 564.464);

    rbt.insert(17, 564.464);
    rbt.insert(16, 564.464);
    rbt.insert(10, 564.464);
    rbt.insert(11, 564.464);
    rbt.insert(14, 564.464);

//    for (auto iter : rbt) {
//        cout << iter.key << endl;
//    }

    expect_true(std::is_sorted(rbt.begin(), rbt.end(), equal_node)) << "not sorted";
    expect_eq(rbt.size(), 15) << "real size"  << rbt.size();
//    put_out << "   h: " <<  RBTree<int, double>::depth(rbt.root().node_) << endl;


//    auto iroot = rbt.root();
//    auto le = RBTree<int, double>::iterator(iroot.node_->left_);
//    auto ri = RBTree<int, double>::iterator(iroot.node_->right_);
//    put_out << iroot->key << endl;
//    put_out << le->key << endl;
//    put_out << ri->key << endl;


   // rbt.down_iterate(rbt.root().node_, [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;});

//    while (not rbt.empty()) {
//        rbt.erase(rbt.max());
//    }
    //rbt.clear();
   // rbt.erase(rbt.begin());
   cout << "\n\nmax" << rbt.max().node_->color_ << endl;//<< rbt.max().node_
   // rbt.erase(rbt.max());//dead
   auto iter = rbt.detach(rbt.max());
    cout << " color  " << rbt.begin().node_->color_ << endl;
        put_out << "\n\nremove root\n\n";
    rbt.down_iterate(rbt.root().node_, [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;});

}



TEST(test_set_comparator, 0)
{
    RBTree<int ,double> rbt;
    auto keysGreater = [](const int&a,
                         const int& b)->bool{
                         return a > b;
                         };
    rbt.insert(7, 564.464);
    rbt.insert(4, 564.464);
    rbt.insert(2, 564.464);
    rbt.insert(1, 564.464);
    rbt.insert(3, 564.464);
    rbt.insert(5, 564.464);
    rbt.insert(6, 564.464);

    rbt.insert(12, 564.464);
    rbt.insert(15, 564.464);
    rbt.insert(17, 564.464);
    rbt.insert(16, 564.464);
    rbt.insert(10, 564.464);
    rbt.insert(9, 564.464);
    rbt.insert(11, 564.464);
    rbt.insert(14, 564.464);

    rbt.foreach([](const RBTree<int, double>::Node* n){cout << n->key << endl;});

    rbt.clear();
    rbt.setComparator(keysGreater);


    rbt.insert(7, 564.464);
    rbt.insert(4, 564.464);
    rbt.insert(2, 564.464);
    rbt.insert(1, 564.464);
    rbt.insert(3, 564.464);
    rbt.insert(5, 564.464);
    rbt.insert(6, 564.464);

    rbt.insert(12, 564.464);
    rbt.insert(15, 564.464);
    rbt.insert(17, 564.464);
    rbt.insert(16, 564.464);
    rbt.insert(10, 564.464);
    rbt.insert(9, 564.464);
    rbt.insert(11, 564.464);
    rbt.insert(14, 564.464);

    rbt.foreach([](const RBTree<int, double>::Node* n){cout << n->key << endl;});


}


TEST(heavy_loa_d, 0)
{
    const size_t LOOP = 2 << 6;
    RBTree<int , int>    rbt;
    put_out << "start\n\n";
    for(size_t i = 0; i != LOOP; ++i) {
        rbt.insert(rand(), rand());
    }
    put_out << "insertion finished   " << rbt.size() << "\n\n";

    while (! rbt.empty()) {
        rbt.erase(rbt.begin());
        if (!rbt.size() % 1000) {
            cout  <<rbt.size() << "\n";
        }
    }
    put_out << "the other half   "<< rbt.size() << "\n\n\n";

    for(size_t i = 0; i != LOOP; ++i) {
        rbt.insert(rand(), rand());
            if (!rbt.size() % 1000) {
            cout <<rbt.size()<< " another 1000\n\n";
        }
    }

    put_out << "insertion finished  " << rbt.size() << "\n\n";
    while (! rbt.empty()) {
            if (!rbt.size() % 1000) {
            cout <<rbt.size()<< " another 1000\n\n";
        }
        rbt.erase(rbt.max());
    }

    put_out << "done   " << rbt.size() << "\n\n";
}

TEST(eraseHUIU_ , 0)
{
    const size_t LOOP = 18;
    RBTree<int , double>    rbt;
    put_out << "start\n\n";
    for(size_t i = 0; i != LOOP; ++i) {
        rbt.insert(rand() / 1000000, 1.1);
    }
    put_out << "insertion finished\n\n";
    int i = 7;
    while (i--) {

    cout << "\n\n <!size  " << rbt.size();
//      rbt.down_iterate(rbt.root().node_,
//        [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;}
//        );

      //  rbt.erase(rbt.begin());
        rbt.erase(rbt.max());
    }

    while (!rbt.empty()) {

    cout << "\n\n <!size  " << rbt.size();
//      rbt.down_iterate(rbt.root().node_,
//        [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;}
//        );

       // rbt.erase(rbt.begin());
        rbt.erase(rbt.max());
    }
}

TEST(heavy_loasedgdfd__, 0)
{
    const size_t LOOP = 16;
    RBTree<int , double>    rbt;
    put_out << "start\n\n";
    for(size_t i = 0; i != LOOP; ++i) {
        rbt.insert(rand() / 1000000, 1.1);
    }
    put_out << "insertion finished\n\n";


        put_out << "now erase begin\n\n";


  //  rbt.down_iterate(rbt.root().node_, [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;});
    put_out << " erasing from max\n\n";

rbt.erase(rbt.max());
rbt.erase(rbt.max());
put_out << "1\n";
rbt.erase(rbt.max());
put_out << "1\n";
rbt.erase(rbt.max());
put_out << "1\n";


cout << "\n\n all \n\n";
  //  rbt.down_iterate(rbt.root().node_, [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;});
cout <<"\n max" << rbt.max()->key << endl << endl;
rbt.erase(rbt.max());///
put_out << "1\n";
rbt.erase(rbt.max());
put_out << "1\n";
put_out <<  rbt.size() << " \n";
  rbt.down_iterate(rbt.root().node_, [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;});

    int i = 6;
    while (i--) {
put_out << "\nerase all \n";

  rbt.down_iterate(rbt.root().node_, [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;});


        rbt.erase(rbt.max());
    }

  rbt.down_iterate(rbt.root().node_, [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;});

put_out << "\nsize:  " << rbt.size() << " \n";
        rbt.erase(rbt.max());

put_out << "1\n";
        rbt.erase(rbt.max());

put_out << "1\n";
        rbt.erase(rbt.max());
    put_out << "done \n\n";
}


TEST(epilog, 0)
{
        const size_t LOOP = 16;
    RBTree<int , double>    rbt;
    put_out << "start\n\n";
    for(size_t i = 0; i != LOOP; ++i) {
        rbt.insert(rand(), 1.1);
    }
    cout << "size: " << rbt.size() << " height" << rbt.height(rbt.root()) << endl;

rbt.down_iterate(rbt.root().node_, [](RBTree<int, double>::Node* n)->void{put_out << n->key << " " << n->color_;});

}

TEST(new_init, 1)
{
    RBTree<int, int> rbt;
    rbt = {make_pair(rand(), rand()), make_pair(rand(), rand()), make_pair(rand(), rand())};
    expect_eq(rbt.size(), 3);

    rbt.add(1, 2)(2, 3)(3, 4)(4, 6);
    expect_eq(rbt.size(), 7);
    cout << sizeof(rbt) << endl;
}
int main()
{
  //  bwtest::setOutput("out_put_with_balancing2.txt");
        RUN_ALL();

//  //  REPORT_ALL();

//cout  << __FILE__ <<"  " << __FUNCTION__ << "    " << __LINE__<< endl;


    return 0;
}






//  cout <<" sizeof(std::map<double, double>)  " << sizeof(std::map<double, double>) << endl;
//  cout << "sizeof(RBTree<double, double>)  " << sizeof(RBTree<double, double>) << endl;
//
//  cout << sizeof(detail_::Nodebase) << endl;
//
//  cout << sizeof(less<double>) << endl;


//
//
//
//    struct A
//    {
//  //  private:
//        string str;
//    public:
////        A(char* s) {
////            str(s);
////        }
//        A(){}
//
//            A(const char* s) {
//                cout << " ctor   const char* " << endl;
//            str = s;
//        }
//
//
//        A& operator= (const string & s) {
//                cout << " operator =  const string&" << endl;
//                str = s;
//            return *this;
//        }
//
//        A& operator= (const char* s) {
//                str = s;
//                cout << " operator =  const char*" << endl;
//            return *this;
//        }
//
//        A& operator= ( char* s) {
//
//                cout << " operator =  char*" << endl;
//                str = s;
//            return *this;
//        }
//    };
//       // A a;
//
//      //   A b = "asdfsg";
//
//     //  A b("ssdfgf");
////
//
//
////        string str("dsgsfg");
////
////        a = str;
//
//       // cout << a.str << endl;
//



