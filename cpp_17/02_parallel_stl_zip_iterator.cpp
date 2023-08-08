// build options: -O2 -std=c++17

#include <vector>
#include <algorithm>
#include <execution>
#include <iostream>
#include <boost/iterator/zip_iterator.hpp>

int main() {
    size_t size = 1'000'000;

    std::vector<double> source_1( size, 1 );
    std::vector<double> source_2( size, 2 );
    auto zip_begin = boost::make_zip_iterator(
        boost::make_tuple(source_1.begin(), source_2.begin())
    );
    auto zip_end = zip_begin + size;
    auto x = std::reduce( std::execution::par_unseq, zip_begin, zip_begin + size, boost::make_tuple(42.0, 42.0),
        [](const auto& lhs, const auto& rhs){
            return boost::make_tuple(
                boost::get<0>(lhs) + boost::get<0>(rhs),
                boost::get<1>(lhs) + boost::get<1>(rhs));
        }
    );

    std::cout << boost::get<0>(x) << " " << boost::get<1>(x) << std::endl;
}
