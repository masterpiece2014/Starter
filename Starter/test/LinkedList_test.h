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



//#include "CycList.h"

#include "CycList.h"

using namespace Starter;


//template<typename T>
////using  = CycList<T, std::allocator<T>>;
//typedef typename template CycList<T>  CycList<T>;
//
//
//typedef typename _Alloc::template rebind<_Tp>::other _Tp_alloc_type;
//
//


#include <cassert>

int test_LinkedList() {
    double db[LOOP];
    for (size_t i = 0; i != LOOP; i++) {
        db[i] = rand();
    }
    CycList<double> ls;
    for (size_t i = 0; i != LOOP; i++) {
        ls.push_back(db[i]);
        cout << db[i] << endl;
    }
    cout << "front and back   " << ls.front() << "   " << ls.back() << endl;
    auto iiiter = ls.begin();
    auto bter = iiiter;
    --iiiter; // at end() pointless node
    --iiiter; // at back()  last node
    cout << " begin   " << *bter << "  "  << *iiiter << endl;
    cout << " iiiter[3]   " << iiiter[3] << endl;

 //   iter += 3;
 //   iter -= 3;

//    while (iter != ls.end()) {
//        cout << *iter << endl;
//        iter++;
//    }
    ls.pop();
    ls.pop_front();
    ls.insert_at(4, 11111.1111);
    ls.insert_at(5, 55555);
    cout << "size  " << ls.size() << endl;
    for(const auto& item : ls) {
        cout << item << endl;
    }
    return 0;
}
struct Friendy {
    Friendy () : Friendy(0.0) {}
    Friendy(double mkk): mk(mkk){
        id = ++ ct;
     //   cout << id << "Morning!" << mk << "   ";
    }
    void greet() const {
        cout << id<<"  " << mk << endl;
    }
    ~Friendy() {
    //  cout << id<< " Dtor\n" ;
    }
    int fillerr[100];
    double mk;
    int id;
    static int ct;
};
int Friendy::ct = 0;
struct FriendyAgain :public Friendy{
    FriendyAgain(){
   //     cout << "Zhaoshanghao!" << endl;
    }
    FriendyAgain(double mkk) : Friendy(mkk){}
    void greet() {
        cout << "ChiFanMei!" << endl;
    }
    ~FriendyAgain() {
  //      cout << "WanAn!" << endl;
    }
    int filler[100];
};
#include <limits>
int test1_LinkedList() {
    CycList<Friendy> lss, laa;
  //  list<Friendy> lss, laa;
 //  vector<Friendy> lss, laa;
    Friendy* ptr = (Friendy*)::operator new (LOOP * sizeof(Friendy));

    for(size_t i = 0; i != LOOP; ++i) {
        new((void*)(ptr + i))Friendy(rand());
        lss.push_back(*(ptr + i));
    }
  //  for (auto& i : lss) {
 //       i.greet();
  //  }
  //  laa.push_back(*lss.begin());
    laa.push_back(*lss.begin());
    laa.push_back(*lss.begin());
    cout << "size and greet()" <<  laa.size() <<endl;
    (++laa.begin())->greet();
    laa = lss;
    cout << "end" << endl;
    ::operator delete(ptr);
    return 0;
}

int test2_LinkedList() {
    CycList<Friendy> lss, lbb;
 // list<Friendy> lss;
    cout << "\n\n size" << lss.size()<< "" << "" <<endl;
    lss.clear();
    cout << "\n\n after cleaning size" << lss.size()<< "" << "" <<endl;

    Friendy* ptr = (Friendy*)::operator new (LOOP * sizeof(Friendy));

    for(size_t i = 0; i != LOOP; ++i) {
        new((void*)(ptr + i))Friendy(rand());
      lss.push_back(*(ptr + i));
    }
    lss.splice(lss.end(), {Friendy(1111111.2222222), Friendy(3333333.4444), Friendy(5555.66666)});
cout << "Greetings  " << lss.size()<< endl;
    for(auto& i : lss) {
        i.greet();
    }
   // lss = lbb; // test if operator=  will call corresponding Dtor
    cout << lss.size() << " size ----- this is the end\n";
  //  cout << std::distance(lss.begin(), lss.end()) << endl;

  //  auto iter = lss.begin();
  //  auto iter6 = iter + 6;

   // cout << "\nto be erased\n";
   // iter->greet();
  //  iter6->greet();
  //  lss.pop();
  //  lss.popFront();
  //  lss.erase(iter,iter6);
  //  lss.insertAt(5, Friendy(111.333));
   // lss.insert(iter6, Friendy(111.333));
  //  cout << "\n" << lss.size() << " done\n";
    //for(auto& i : lss) {
//    for(auto& i : lss) {
//        i.greet();
//    }
//    auto xx = lss.begin();
//    ++xx;
//    ++xx;
//    cout << "\n iters\n";
//    xx->greet();
//    lss.reverse();
//    cout << "\n" << lss.size() << " reversing done\n\n";
//    for(auto& i : lss) {
//        i.greet();
//    }
//    cout << "\n iters \n";
//  //  while(xx != lss.end()) {
//        xx->greet();
//        ++xx;
//        xx->greet();
//  //  }
//    cout<< "\n\n\t TEST " << ""<<"";
//


    ::operator delete(ptr);
    return 0;
}

template<typename T>
struct Pred
{  bool operator()(T a, T b) { return true;  }
};

int test3_LinkedList() {
  //      CycList<Friendy> lss;
//
 //  std::vector<double> lss;
//   lss.push_back(1.1);
//   lss.push_back(2.2);
//    lss.push_back(3.1);
//   lss.push_back(6.2);
//   lss.push_back(1.1);
//   lss.push_back(2.2);
//    lss.push_back(3.1);
//   lss.push_back(6.2);
//   CycList<double> laa;
//   laa.transplant(laa.begin(), lss.begin(), lss.end());
//   for(auto a : laa) {
//    cout << a << endl;
//   }
//   cout << laa.end() - laa.begin() << " "  << laa.size() << endl;
//    cout << laa.begin() - laa.end() << " "  << laa.size() << endl;
  // CycList<double> ltt(lss);
  // ltt.splice(ltt.begin(), lss);
 // CycList<double> ltt{99999.9, 8888.88, 77777.77};
 // std::list<double> lss = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
//    for(auto& i : ltt) {
//        cout << i << endl;
//    }

    CycList<Friendy> laa{Friendy(1.111), Friendy(2.222), Friendy(3.333), Friendy(44.44)};

    std::vector<Friendy> vec{Friendy(55.55), Friendy(66.66), Friendy(77.77)};
   // test M_transplant
  //  std::vector<FriendyAgain> vec{FriendyAgain(44545), FriendyAgain(456), FriendyAgain(7845)};
    laa.splice(laa.begin(), {Friendy(8888.66), Friendy(99.9999)});

    laa.splice(laa.begin() + 2, vec.begin(), vec.end());
  //  laa.transplant(laa.begin() + 2, vecx.begin(), vecx.end());
  //9 8 5 6 7 1 2 3 4
  //8 9 5 6 7 1 2 3 4

  //  8 9 5 6 7 1 2 3 4
    cout << "\n size  " << laa.size() << endl;

  //  CycList<Friendy> lbb(vecx.begin(), vecx.end());
    for(auto& i : laa) {
        i.greet();
    }

    auto vecx = laa.to_array();
    cout << "\nvecx" << endl;
    for(size_t i = 0; i != laa.size(); ++i) {
        (vecx + i)->greet();
    }
    ::operator delete(vecx);
    cout << "\n laa" << endl;
    for(const auto& i : laa) {
        i.greet();
    }
    cout << "const \n";
    laa.begin()->greet();
    laa = {Friendy(8888.66), Friendy(99.9999)};
    for(const auto& i : laa) {
        i.greet();
    }
  //  laa.cBegin()->greet();
    /*
    cout << "\n laa[2].greet();" << endl;
    laa[2].greet();
  //  std::sort(laa.begin(), laa.end(), [](const Friendy& a, const Friendy& b)->bool{return a.mk > b.mk;});
    cout << "compare " << std::boolalpha;
    cout << (laa.begin() - laa.end() ) << "  " <<  (laa.end() - laa.begin() ) <<endl;
    cout<< (laa.begin() < laa.end() ) << endl;
    cout << "\n after std::sort" << endl;
    for(auto& i : laa) {
        i.greet();
    }
    static_assert(std::is_same<CycList<Friendy>::Iter, decltype(laa.begin())>::value, "" );
    int* pint= nullptr;
    uint64_t* puint_64_t = nullptr;
    cout << sizeof(puint_64_t) << endl;
    cout << typeid(puint_64_t).name() << endl;
    */
//assert(1 != 1);
  //  cout << std::is_same<CycList<Friendy>::Iter, decltype(laa.begin())>::value << endl;
//    cout << "\ntest sort" << endl;
//    CycList<double> lbb{1.1, 2.2, 4.4, 99.9, 88.88, 55.12, 19.45, 66.15};
//    for(auto& i : lbb) {
//        cout << i << endl;
//    }
//    cout << "\ntest size" << lbb.size() << endl;

//    auto yeah = [](double i)->bool{cout << i << " in lambda"<< endl; return true;};
//
//    lbb.removeIf(yeah);
//
//    std::function<bool(double, double)> dd = [](double i, double j)->bool{return true;};
//    lbb.merge(lxx, dd);
//    Pred<bool> ppp;
//    lbb.merge(lxx, ppp);


//    cout << lbb.end() - lbb.begin()  << "   size: " << lbb.size()<< endl;
//  // std::sort(lbb.begin(), lbb.end());
//   // cout << "\ntest sort" << endl;
//    for(auto& i : lbb) {
//        cout << i << endl;
//    }
//    for(int i = 0; i != LOOP; ++i) {
//      lss.push_back(Friendy(rand()));
//    }
//    for(auto& i : lss) {
//        i.greet();
//    }
//    lss.removeAt(6);
//    cout << " removeAt 6" << lss.size() << endl;
//    for(auto& i : lss) {
//        i.greet();
//    }
//    cout << " At 6" << lss.size() << endl;
//    lss[6].greet();

//typedef Friendy alis;
//Friendy ad;
//if(std::is_same<decltype(ad), alis>::value) { cout << "ok ok" << endl;}

    return 0;
}


void test_4() {
    CycList<Friendy> laa{Friendy(1.111), Friendy(2.222), Friendy(3.333), Friendy(44.44)};
    std::vector<Friendy> vec{Friendy(55.55), Friendy(66.66), Friendy(77.77)};

    laa.splice(laa.begin(), {Friendy(8888.66), Friendy(99.9999)});

    laa.splice(laa.begin() + 2, vec.begin(), vec.end());
    for(auto & i : laa) {
        i.greet();
    }
}
void test_5_LinkedList() {
//    CycList<int> l1{5, 9, 0, 1, 3};
////    for(auto & i : l1) {
////        cout << i << " ";
////    }
////    cout << endl;
//    CycList<int> l2{8, 7, 2, 16, 4};
////   // l1.merge(l2, [](int a, int b)->bool{return a > b;});
//    cout << "\nfirst" << l1.size() << endl;
//    l1.merge(l2);
////    for(auto & i : l1) {
////        cout << i << " ";
////    }
//  //  l1.splice(l1.begin(), {11, 11, 33, 33, 55, 55, 88, 88});
// //   cout << "\nfirst" << l1.size() << endl;
// //       for(auto & i : l1) {
//  //      cout << i << " ";
//  //  }
//  //  l1.unique();
//    cout << "\nunique"<< l1.size()  << endl;
//        for(auto & i : l1) {
//        cout << i << " ";
//    }
  //  CycList<int> ls{0, 1, 35, 9, 44, 51, 28};
    CycList<Friendy> ls;
    CycList<Friendy> lt;

    for(int i = 0; i != 16; ++i) {
        ls.push_back(Friendy(rand()));
        lt.push_back(Friendy(rand()));
    }
    cout << "\n\t original  " << ls.size() << endl;
    for(auto & i : ls) {
      //  cout << i << endl;
      i.greet();
    }
    cout << "\n\t sorting  " << ls.size() << endl;
    ls.sort([](const Friendy& a, const Friendy& b)->bool{return a.mk < b.mk;});
    lt = ls;
    cout << "\n\t sorted  " << lt.size() << endl;
    for(auto & i : lt) {
      //  cout << i << endl;
      i.greet();
    }

}

void test_hash() {
    CycList<Friendy> ls{Friendy(55.55), Friendy(66.66), Friendy(77.77), Friendy(8888.66), Friendy(99.9999)};
     CycList<Friendy> lt(ls);
   // CycList<double> ls;
//    for(int i = 0; i != 256; ++i) {
//        ls.push_back(Friendy(rand()));
//      //   ls.push_back(rand());
//    }
// ls.sort();
  //   ls.sort([](const Friendy& a, const Friendy& b)->bool{return a.mk < b.mk;});
//    for(auto  i : ls) {
//     //   cout << i << "  ";
//              i.greet();
//    }
    cout << "\n\n hash  " << ls.hash_code()<< endl;
        cout << " hash  " << lt.hash_code()<< endl;
 }

inline uint32_t jenkins_rev_mix32(uint32_t key) {
  key += (key << 12);  // key *= (1 + (1 << 12))
  key ^= (key >> 22);
  key += (key << 4);   // key *= (1 + (1 << 4))
  key ^= (key >> 9);
  key += (key << 10);  // key *= (1 + (1 << 10))
  key ^= (key >> 2);
  // key *= (1 + (1 << 7)) * (1 + (1 << 12))
  key += (key << 7);
  key += (key << 12);
  return key;
}

/*
 * Inverse of jenkins_rev_mix32
 *
 * Note that jenkinks_rev_unmix32 is significantly slower than
 * jenkins_rev_mix32.
 */

inline uint32_t
jenkins_rev_unmix32(uint32_t key) {
  // These are the modular multiplicative inverses (in Z_2^32) of the
  // multiplication factors in jenkins_rev_mix32, in reverse order.  They were
  // computed using the Extended Euclidean algorithm, see
  // http://en.wikipedia.org/wiki/Modular_multiplicative_inverse
  key *= 2364026753U;

  // The inverse of a ^= (a >> n) is
  // b = a
  // for (int i = n; i < 32; i += n) {
  //   b ^= (a >> i);
  // }
  key ^=
    (key >> 2) ^ (key >> 4) ^ (key >> 6) ^ (key >> 8) ^
    (key >> 10) ^ (key >> 12) ^ (key >> 14) ^ (key >> 16) ^
    (key >> 18) ^ (key >> 20) ^ (key >> 22) ^ (key >> 24) ^
    (key >> 26) ^ (key >> 28) ^ (key >> 30);
  key *= 3222273025U;
  key ^= (key >> 9) ^ (key >> 18) ^ (key >> 27);
  key *= 4042322161U;
  key ^= (key >> 22);
  key *= 16773121U;
  return key;
}

const uint32_t FNV_32_HASH_START = 216613626UL;
const uint64_t FNV_64_HASH_START = 14695981039346656037ULL;
inline uint32_t fnv32(const char* s,
                      uint32_t hash = FNV_32_HASH_START) {
  for (; *s; ++s) {
    hash += (hash << 1) + (hash << 4) + (hash << 7) +
            (hash << 8) + (hash << 24);
    hash ^= *s;
  }
  return hash;
}

inline uint32_t fnv32_buf(const void* buf,
                          int n,
                          uint32_t hash = FNV_32_HASH_START) {
  const char* char_buf = reinterpret_cast<const char*>(buf);

  for (int i = 0; i < n; ++i) {
    hash += (hash << 1) + (hash << 4) + (hash << 7) +
            (hash << 8) + (hash << 24);
    hash ^= char_buf[i];
  }

  return hash;
}

inline uint32_t fnv32(const std::string& str,
                      uint64_t hash = FNV_32_HASH_START) {
  return fnv32_buf(str.data(), str.size(), hash);
}


int test_hash_funcs() {
    uint32_t key = 20130126;
    cout << jenkins_rev_mix32(jenkins_rev_unmix32(key)) << endl;
    cout << jenkins_rev_unmix32(key) << endl;
    cout << jenkins_rev_unmix32 ( jenkins_rev_mix32(key) ) << endl;
    std::string str = "20130126xsdnfndaklhvopekwpqowdajsfqw";
    std::hash<std::string> stringHasher;
    cout << str << endl;

  // assert( reinterpret_cast<size_t>(str.data()) == stringHasher(str) );
  cout << fnv32(str) << endl;
    cout << stringHasher(str) << endl;
    return 0;
}

//    Friendy* ptr = (Friendy*)::operator new (LOOP * sizeof(Friendy));
//    Friendy* ptr1 = ptr + 1;
//    new((void*)ptr1) Friendy();
//    ptr1->~Friendy();
//    ::operator delete(ptr);
//    ptr = (Friendy*)::operator new (128 * sizeof(Friendy));     ::operator delete(ptr);
//    ptr = (Friendy*)::operator new (128 * sizeof(Friendy));     ::operator delete(ptr);
//    ptr = (Friendy*)::operator new (128 * sizeof(Friendy));     ::operator delete(ptr);
//    ptr1->greet();






