#ifndef UTIL_H
#define UTIL_H

#include "benchmark.hpp"

#include <vector>

namespace benchmark {
namespace util {

unsigned int GenerateRandomNumber(int lower_bound, int upper_bound);

Permutation FisherYatesShuffle(size_t permutation_size);

std::vector<Permutation> GenerateNRandomPermutations(unsigned int n, size_t permutation_size);

void print_progress(int p, int total, int width, size_t perm_size);

} // namespace util
} // namespace benchmark

#endif
