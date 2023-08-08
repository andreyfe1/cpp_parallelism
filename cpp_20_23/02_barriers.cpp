// build options: -O2 -std=c++20 -lpthread

#include <vector>
#include <iostream>
#include <thread>
#include <barrier>

constexpr int num_threads = 8;
std::barrier task_barrier(num_threads);

void local_function(std::vector<int64_t>& source, int64_t source_start, int64_t source_end) {
    for(int i = source_start; i < source_end; ++i){
        source[i] = i;
    }

    task_barrier.arrive_and_wait();

    for(int i = source_start; i < source_end; ++i){
        if(i < source.size() / 2){
            std::swap(source[i], source[source.size() - 1 - i]);
        }
    }
}


int main() {
    size_t size = 1000;
    std::vector<int64_t> source( size);
    // scope for jthread
    {
        std::vector<std::jthread> threads(num_threads);

        for(int i = 0; i < num_threads; ++i){
            threads[i] = std::jthread(local_function,
                std::ref(source),
                i * size / num_threads,
                (i + 1) * size / num_threads);
        }
    }
    for(int i = 0; i < size; ++i){
        if(source[i] != size - 1 - i){
            std::cout << "Error:  i = " << i << ", value = " << source[i] << std::endl;
        }
    }
}
