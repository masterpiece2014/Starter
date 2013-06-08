


#include <queue>
#include "bwtest/bwtest.h"
#include "PriorityQueue.h"
using namespace Starter;

TEST(test_push_top, 1)
{
    std::priority_queue<int>    stl;
    PrioQueue<int>              mine;
    int tmp;
    for(size_t i = 0; i != 100; ++i)  {
        tmp = rand();
        mine.push(tmp);
        stl.push(tmp);
    }
    for(size_t i = 0; i != 100; ++i)  {
        expect_eq(mine.top(), stl.top()) << "stl: " << stl.top() << "   mine " << mine.top();
        mine.pop();
        stl.pop();
    }
}

int main()
{
    RUN_ALL();
    return 0;
}
