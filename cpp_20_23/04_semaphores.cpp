// build options: -O2 -std=c++20 -lpthread

#include <vector>
#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

std::binary_semaphore toThreads{0}, fromThreads{0};

void local_function(std::vector<int64_t>& source, int64_t source_start, int64_t source_end) {

    // get signal from the main thread and start working
    toThreads.acquire();

    for(int i = source_start; i < source_end; ++i){
        source[i] = i;
    }

    // send signal to the main thread
    fromThreads.release();
}


int main() {
    constexpr int num_threads = 8;
    size_t size = 1000000;
    std::vector<int64_t> source(size);
    // scope for jthread
    {
        std::vector<std::jthread> threads(num_threads);

        for(int i = 0; i < num_threads; ++i){
            threads[i] = std::jthread(local_function,
                std::ref(source),
                i * size / num_threads,
                (i + 1) * size / num_threads);
        }

        auto start = std::chrono::steady_clock::now();

        // send signal to other threads to start working
        toThreads.release(num_threads);
        for(int i = 0; i < num_threads; ++i){
            // got signal that other threads stopped working
            fromThreads.acquire();
        }
         
        auto end = std::chrono::steady_clock::now();
        std::cout << "time is " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() <<std::endl;
    }

    for(int i = 0; i < size; ++i){
        if(source[i] != i){
            std::cout << "Error:  i = " << i << ", value = " << source[i] << std::endl;
        }
    }
}
