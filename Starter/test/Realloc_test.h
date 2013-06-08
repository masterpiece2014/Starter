
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





int test_in_place_realloc () {

    ofstream logger("__log.txt", std::ios::app);

    void* ptr1 = NULL;
    void* ptr2 = NULL;
	void* p_jam1 = NULL;
	void* p_jam2 = NULL;

    const uint64_t init_size_byte = 2;
          uint64_t size_byte;
		  uint64_t next_size;
            size_t min_power = 2;
    const   size_t max_power = 32;
    const   size_t init_loops = 1000;
            size_t loop = init_loops;
			size_t successed = 0;

    while (min_power != max_power) {
        min_power++;
        size_byte = init_size_byte << min_power;
        next_size = (size_byte * 3 + 1) / 2;
		loop = init_loops;
        while(--loop) {
            ptr1 = malloc(size_byte);
									p_jam1 = malloc(size_byte);
									p_jam2 = realloc(p_jam1, next_size);
            ptr2 = realloc(ptr1, next_size);
									p_jam1 = realloc(p_jam2, next_size / 2);
			if(ptr1 == NULL)
				exit(0);
			if(ptr2 == NULL)
				exit(0);
            if (ptr2 == ptr1) {
                ++successed;
            }
        //    free(ptr1);
            free(ptr2);
								free(p_jam1);
            ptr1 = NULL;
            ptr2 = NULL;
        }
		logger << setw(10) << std::setprecision(10) << (double)successed / init_loops
			<< setw(10) << "    " << size_byte<< endl;

		successed = 0;
		loop = init_loops;
    }
    logger.close();
    return 0;
}
