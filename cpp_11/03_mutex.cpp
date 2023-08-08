// build options: -O2 -std=c++11 -lpthread

#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex reduction_mutex;

void local_reduction(int64_t* start, int64_t* end, int64_t init, int64_t& result) {
    for(; start != end; ++start){
        init += *start;
    }
    
    std::lock_guard<std::mutex> guard(reduction_mutex);
    result += init;
}

int main() {
    size_t size = 1000000;
    constexpr int num_threads = 4;
    std::vector<int64_t> source( size, 1 );
    int64_t results(0);
    int64_t* sourse_ptr = source.data();
    std::vector<std::thread> threads(num_threads);

    for(int i = 0; i < num_threads; ++i){
        threads[i] = std::thread(local_reduction,
            sourse_ptr + i * size / num_threads, 
            sourse_ptr + (i + 1) * size / num_threads, 
            0.0,
            std::ref(results)
        );
    }

    for(int i = 0; i < num_threads; ++i){
        threads[i].join();
    }

    std::cout << results << std::endl;
}
