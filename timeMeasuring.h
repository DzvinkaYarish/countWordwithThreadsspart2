//
// Created by dzvinka on 28.03.17.
//

#ifndef COUNTWORDSINFILE_TIMEMEASURING_H
#define COUNTWORDSINFILE_TIMEMEASURING_H

#include <chrono>
#include <atomic>

using namespace std;

inline chrono::high_resolution_clock::time_point getCurrentTimeFenced()
{
    atomic_thread_fence(memory_order::memory_order_seq_cst);
    auto res_time = chrono::high_resolution_clock::now();
    atomic_thread_fence(memory_order::memory_order_seq_cst);
    return res_time;
}
template<class D>
inline  long long toMilliSec(const D& d)
{
    return chrono::duration_cast<std::chrono::milliseconds>(d).count();
}
template<class D>
inline  long long toMicroSec(const D& d)
{
    return chrono::duration_cast<std::chrono::microseconds>(d).count();
}

#endif //COUNTWORDSINFILE_TIMEMEASURING_H
