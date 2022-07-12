#include "util.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <string>

namespace benchmark {
namespace util {

unsigned int GenerateRandomNumber(int lower_bound, int upper_bound) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(lower_bound, upper_bound);

    return dist(rng);
}

Permutation FisherYatesShuffle(size_t permutation_size) {
    Permutation perm(permutation_size);
    std::iota(perm.begin(), perm.end(), 1);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(perm.begin(), perm.end(), rng);

    return perm;
}

std::vector<Permutation> GenerateNRandomPermutations(unsigned int n, size_t permutation_size) {
    std::vector<Permutation> permutations{};

    #ifdef DEBUG
    std::cout << "generating " << n << " random permutations of size " << permutation_size << '\n';
    #endif

    for (unsigned int i = 0; i < n; i++)
        permutations.emplace_back(FisherYatesShuffle(permutation_size));

    return permutations;
}

void print_progress(int p, int total, int width, size_t perm_size) {
    std::string total_str = std::to_string(total);
    std::string p_str = std::to_string(p);
    int bar_width = width - total_str.size() * 2 - 4;

    std::cout << '\r';
    std::cout << "running simulation for perm_size: " << perm_size << " " << std::flush;
    
    if (bar_width > 1) {
        int fill_width = bar_width * p / total;
        std::cout << "[";

        for (int i = 0; i < bar_width; ++i) {
            char c = ' ';
            if (i < fill_width || p == total) c = '-';
            else if (i == fill_width) c = '>';
            std::cout << c;
        }

        std::cout << "]";
    }

    /*
    std::cout << std::setfill(' ') << std::setw(total_str.size()) << std::fixed << std::setprecision(1)
              << "(" << (p / total) * 100 << "%)" << std::flush;*/

    std::cout << std::setfill(' ') << std::setw(total_str.size()) << " "
        << p_str << "/" << total_str << std::flush;

    if (p == total) std::cout << std::endl;
}

} // namespace util
} // namespace benchmark

