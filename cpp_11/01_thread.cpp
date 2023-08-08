// build options: -O2 -std=c++11 -lpthread

#include <vector>
#include <algorithm>
#include <iostream>
#include <thread>

void local_reduction(int64_t* start, int64_t* end, int64_t init, int64_t& result) {
    for(; start != end; ++start){
        init += *start;
    }
    result = init;
}


int main() {
    size_t size = 1000000;
    constexpr int num_threads = 4;
    std::vector<int64_t> source( size, 1 );
    int64_t results[num_threads];
    int64_t* sourse_ptr = source.data();
    std::vector<std::thread> threads(num_threads);

    for(int i = 0; i < num_threads; ++i){
        threads[i] = std::thread(local_reduction, sourse_ptr + i * size / num_threads, sourse_ptr + (i + 1) * size / num_threads, 0.0, std::ref(results[i]));
    }

    for(int i = 0; i < num_threads; ++i){
        threads[i].join();
    }

    for(int i = 1; i < num_threads; ++i){
        results[0] += results[i];
    }

    std::cout << results[0] << std::endl;
}
