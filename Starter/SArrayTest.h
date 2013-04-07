




#include "SArray.h"
#include <cassert>

using namespace Starter;

struct Friendy {

    int fillerr[50];
    double mk;
    int id;
    static int ct;

    Friendy () : Friendy(0.0) {
        cout << "default Ctor " << id << "   "<< mk << endl;
    }

    Friendy(double mkk): mk(mkk){
        id = ++ ct;
        cout << " db_arg Ctor " << id << "   "<< mk << endl;
    }
    Friendy (const Friendy& other) {
        id = ++ ct;
        mk = other.mk;
        cout << " copy Ctor " << id << "   "<< mk << endl;
    }
    Friendy (Friendy&& other) {
        id = ++ ct;
        mk = other.mk;
        cout << " move Ctor " << id << "   "<< mk << endl;
    }
    Friendy&
    operator= (const Friendy& other) {
        mk = other.mk;
        cout << " default assign " << id << "   "<< mk << endl;
        return *this;
    }
    Friendy&
    operator= (Friendy&& other) {
        mk = other.mk;
        cout << " move assign " << id << "   "<< mk  << endl;
        return *this;
    }

    virtual void greet() const {
        cout << id<<"  " << mk << endl;
    }
    virtual ~Friendy() {
        cout << "Dtor  "<< "id "<< id << "  mk " << mk << endl;
    }
};
int Friendy::ct = 0;

void test_SArray_1() {
    // test lambda :capacity calculation
//    SArray<double, 5> sa;
//    auto test_grow_policy = [](size_t u)->size_t{return --u;};
//    sa.setGrowthPolicy(test_grow_policy);
//    cout << "  sizeof(double) "<< sizeof(double)<< "   new Cap  " << sa._newCapacity(4080) << endl;

    SArray<double> sa;
   // SArray<double> sa(5, 5);
//    double* ppp = sa.data();
//    ppp -= 50;
//    memset((void*)ppp, 118811, 512);
  // SArray<double> sa(10, 20);
    cout << "\n\t capa "<< sa.capacityL() << "  " << sa.capacityR() << "    size " << sa.size() << endl;
    for(int i = 0; i != 10; ++i) {
        double tmp = rand();
        cout << tmp << endl;
      //  sa.push_back(tmp);  // 9 9 0    / 9 18 12
       sa.push_front(tmp);      // 9 9 0  / 6 21 12
    }                           //        / 5 21 12
  //  cout << "\n\t capa "<< sa.capacityL() << "  " << sa.capacityR() << "    size " << sa.size() << endl;
    cout << "\niters\n";
    for(auto i : sa) {
        cout << i << endl;
    }
//    cout << "\n real \n"<< endl;
//    double* ptr = sa.data();
//    for(int i = 0; i != 27 ; i++) {
//            cout <<i << "  "<< *(ptr + i) << endl;
//    }
//    try {
//            for(int i = 0; i != 30 ; i++) {
//            cout <<i << "  "<< *(ptr + i) << endl;
//         cout << sa[i] << endl;
//    }
//    }catch(...) {
//        exit(1);
//    }

    cout << "\n   capaL "<< sa.capacityL() << "  R " << sa.capacityR() << "  capa "<< sa.capacity() <<"    size " << sa.size() << endl;

    cout << "\n\thash " << sa.hash_code() << endl;
    while(!sa.empty()) {
      //  sa.pop_back();
      sa.pop_front();
    }
    cout << "\n   capaL "<< sa.capacityL() << "  R " << sa.capacityR() << "  capa "<< sa.capacity() <<"    size " << sa.size() << endl;

}
void test_insertion() {
    SArray<double> sa1, sa2;
  //  std::deque<double> dq;
    for(int i = 0; i != 5; ++i) {
        sa1.emplace_back(rand());
        sa2.push_front(rand());
     //   dq.push_back(sa1[i]);
    }
    sa1.insert(sa1.begin() + 2, 22222);
//    dq.insert(dq.begin() + 2, 22222);
//    for(auto i : dq) {cout << i << " ";}
//    cout << "\n";
    for(auto i : sa1) {cout << i << " ";}     cout << "\n";

    for(auto i : sa2) {cout << i << " ";}    cout << "\n";

  //  sa1.insert(sa1.begin() + 2, sa2.begin(), sa2.end());
  sa1.insert(sa1.begin() + 2, {888, 999, 101010});
    for(auto i : sa1) {cout << i << " ";}    cout << "\n";
}

void test_erase() {
    SArray<Friendy> sa1, sa2;
    std::vector<Friendy> vec;
    for(int i = 0; i != 20; i ++) {
        sa1.push_back(Friendy(rand()));
        vec.push_back(sa1[i]);
    }

    for(auto& i : sa1) {
        i.greet();
    }
    cout << "\n   capaL "<< sa1.capacityL() << "  R " << sa1.capacityR() << "  capa "<< sa1.capacity() <<"    size " << sa1.size() << endl;

    auto iter = sa1.begin();
    auto viter = vec.begin();
    iter += 5;
    viter += 5;
    //iter->greet();
//    sa1.erase(iter);
    sa1.erase(iter, iter + 12);
    cout << "\n bravo! \n" << endl;
    for(auto& i : sa1) {
        i.greet();
    }
    cout << "\n   capaL "<< sa1.capacityL() << "  R " << sa1.capacityR() << "  capa "<< sa1.capacity() <<"    size " << sa1.size() << endl;

}
void test_hash() {

    SArray<uint32_t> sa1(10);//, sa2(10, 40);
    std::seed_seq seq({11, 22, 33, 44, 55});
    seq.generate(sa1.begin(), sa1.end() );

    for(auto i : sa1) {
        cout << i << endl;
    }
  //  cout << "\n   capaL "<< sa1.capacityL() << "  R " << sa1.capacityR() << "  capa "<< sa1.capacity() <<"    size " << sa1.size() << endl;

   // cout << sa1.hash_code() << endl;
  //  cout << sa2.hash_code() << endl;
  //  assert(sa1.hash_code() == sa2.hash_code());
}
void test_assign() {
//    SArray<Friendy> sa1{Friendy(1111.2222222), Friendy(3333.4444), Friendy(555.66666)};
//    SArray<Friendy> sa2 = sa1;
//    for(auto& i : sa2) {
//        i.greet();
//    }
//    cout << sa1.hash_code() << endl;
//    cout << sa2.hash_code()<< endl;
   // SArray<Friendy> sa1{Friendy(11.22), Friendy(33.44), Friendy(55.66)};
  // SArray<Friendy> sa1(3, 3);
   SArray<double> sa1(3, 93);
   for(int i = 0;  i != 100; ++i) {
       // sa1.emplace_back(Friendy( 10 / (i + 1)));
       sa1.push_back(rand());
   }
//    cout << "greet\n";
//    for(auto& i : sa1) {// id 6
//      //  i.greet();
//      cout << i << endl;
//    }
cout << "\n   capaL "<< sa1.capacityL() << "  R " << sa1.capacityR() << "  capa "<< sa1.capacity() <<"    size " << sa1.size() << endl;

            cout << "\nassign\n";
//    sa1.insert( sa1.begin() - 1,{Friendy(77.88), Friendy(99.99)});
    sa1.insert( sa1.begin() - 1, {1, 77.88, 99.99});
        cout << "\nshow\n";
        //sa1.shrink_to_fit();
        sa1.resize(0,50);
//    for(auto& i : sa1) {
//   //     i.greet();
//         cout << i << endl;
//    }
cout << "\n   capaL "<< sa1.capacityL() << "  R " << sa1.capacityR() << "  capa "<< sa1.capacity() <<"    size " << sa1.size() << endl;

}
inline size_t goodMallocSize(size_t minSize) {
  if (minSize <= 64) {
    // Choose smallest allocation to be 64 bytes - no tripping over
    // cache line boundaries, and small string optimization takes care
    // of short strings anyway.
    return 64;
  }
  if (minSize <= 512) {
    // Round up to the next multiple of 64; we don't want to trip over
    // cache line boundaries.
    return (minSize + 63) & ~size_t(63);
  }
  if (minSize <= 3840) {
    // Round up to the next multiple of 256
    return (minSize + 255) & ~size_t(255);
  }
  if (minSize <= 4072 * 1024) {
    // Round up to the next multiple of 4KB
    return (minSize + 4095) & ~size_t(4095);
  }
  // Holy Moly
  // Round up to the next multiple of 4MB
  return (minSize + 4194303) & ~size_t(4194303);
}

void test_size() {
    cout << 55 << "  " << goodMallocSize(55) << endl;
    cout << 450 << "  " << goodMallocSize(450) << endl;
    cout << 2500 << "  " << goodMallocSize(2500) << endl;
    cout << (4072 * 1024 - (4072 * 1024 % 100) ) << "  " << goodMallocSize((4072 * 1024 - (4072 * 1024 % 100) )) << endl;
  //  cout <<  << "  " << goodMallocSize() << endl;
    cout << " 4194400 " << goodMallocSize(4194400) << endl;

    uint64_t sum = 1;
    for(size_t i = 0; i != 32; ++i) {
        for(size_t j = 0; j != i; ++j) {
            sum *= 2;
        }
        cout << i << "  " << sum << endl;
        sum = 1;
    }
}



