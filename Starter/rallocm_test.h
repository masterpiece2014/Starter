



#include "Malloc.h"


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
using namespace std;
using namespace Starter;

ofstream writer("rallocm_test_result.txt", ios::app);

void test_in_place_realloc(void* p_, size_t originalSz, size_t minSz, size_t maxSz)
{
	size_t byteIdealCap = goodMallocSize (maxSz);
	size_t byteLeastCap = goodMallocSize (minSz);
	size_t byteIdealGrowth = byteIdealCap - byteLeastCap;
	size_t byteRealGrowth;

        writer << " original bytes: " << originalSz;
        writer << "   ideal-bytes: " << byteIdealCap;
        writer << "   least-bytes: " << byteLeastCap;

	void* ptr = (void*) p_;
	if (rallocm && byteIdealCap > jemallocMinInPlaceExpandable) {

		bool result = rallocm (&ptr, &byteRealGrowth, byteLeastCap, byteIdealGrowth, ALLOCM_NO_MOVE);
		if (result == ALLOCM_SUCCESS) {
                writer << "rallocm successed!";
                writer << (byteLeastCap + byteRealGrowth);
                writer << "   growed: " << byteRealGrowth << setprecision(10) << double(byteRealGrowth) / byteIdealGrowth<< endl;
		}
		else {
                writer << "   rallocm failed" << endl;
		}
	} else {
                writer << " Cannot use rallocm    ideal bytes:" << byteIdealGrowth << "   least-bytes: " << byteLeastCap << endl;
	}
}

int test_rallocm()
{
    cout << boolalpha << rallocm << endl;
    void* ptr = ::operator new(128);
    test_in_place_realloc(ptr, 128, 4096, 8182);

    writer.close();
    return 0;
}

int test_rank() {

    cout << std::rank<int***>::value << endl;
    cout << std::rank<int[1][1]>::value << endl;
    return 0;
}












