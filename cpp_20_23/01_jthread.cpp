// build options: -O2 -std=c++20 -lpthread

#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

void local_reduction(int64_t* start, int64_t* end, int64_t init, std::atomic<int64_t>& result) {
    for(; start != end; ++start){
        init += *start;
    }
    
    std::atomic_fetch_add(&result, init);
}

int main() {
    size_t size = 1000000;
    constexpr int num_threads = 4;
    std::vector<int64_t> source( size, 1 );
    std::atomic<int64_t> results(0);
    int64_t* sourse_ptr = source.data();
    {
        std::vector<std::jthread> threads(num_threads);

        for(int i = 0; i < num_threads; ++i){
            threads[i] = std::jthread(local_reduction, sourse_ptr + i * size / num_threads, sourse_ptr + (i + 1) * size / num_threads, 0.0, std::ref(results));
        }
    }

    std::cout<<results<<std::endl;
}
