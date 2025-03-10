//
//  ThreadPoolTest.cpp
//  MNNTests
//
//  Created by MNN on 2019/06/30.
//  Copyright © 2018, Alibaba Group Holding Limited
//

#ifdef MNN_USE_THREAD_POOL
#include "MNNDefine.h"
#include "MNNTestSuite.h"
#include "ThreadPool.hpp"

using namespace MNN;

class ThreadPoolTest : public MNNTestCase {
public:
    virtual ~ThreadPoolTest() = default;
    virtual bool run() {
        std::vector<std::thread> threads;
        MNN::ThreadPool::init(4);
        for (int i=0; i<10; ++i) {
            threads.emplace_back([]() {
                // initializer
                auto workIndex = ThreadPool::acquireWorkIndex();
                FUNC_PRINT(workIndex);
                ThreadPool::active();
                auto func = [](int index) {
                    FUNC_PRINT(index);
                    std::this_thread::yield();
                };
                ThreadPool::enqueue(std::make_pair(std::move(func), 10), workIndex);
                ThreadPool::deactive();
                ThreadPool::releaseWorkIndex(workIndex);
            });
        }
        for (auto& t : threads) {
            t.join();
        }
        MNN::ThreadPool::destroy();
        return true;
    }
};

MNNTestSuiteRegister(ThreadPoolTest, "core/threadpool");
#endif
