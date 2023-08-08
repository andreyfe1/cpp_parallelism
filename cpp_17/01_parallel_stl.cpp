// build options: -O2 -std=c++17

#include <vector>
#include <algorithm>
#include <execution>
#include <iostream>
#include <chrono>

void using_seq(const std::vector<double>& source) {

  double x = std::reduce( std::execution::seq, source.begin(), source.end(), 42.0 );

  std::cout<<x<<std::endl;
}

void using_par_unseq(const std::vector<double>& source) {

  double x = std::reduce( std::execution::par_unseq, source.begin(), source.end(), 42.0 );

  std::cout<<x<<std::endl;
}

int main() {
    size_t size = 1'000'000;

    std::vector<double> source_seq( size, 2 );
    auto start_seq = std::chrono::steady_clock::now();
    using_par_unseq(source_seq);
    auto end_seq = std::chrono::steady_clock::now();

    std::vector<double> source_par_unseq( size, 1 );
    auto start_par_unseq = std::chrono::steady_clock::now();
    using_seq(source_par_unseq);
    auto end_par_unseq = std::chrono::steady_clock::now();

    std::cout << "par time is " << std::chrono::duration_cast<std::chrono::microseconds>(end_par_unseq - start_par_unseq).count() <<std::endl;
    std::cout << "seq time is " << std::chrono::duration_cast<std::chrono::microseconds>(end_seq - start_seq).count() <<std::endl;
}
