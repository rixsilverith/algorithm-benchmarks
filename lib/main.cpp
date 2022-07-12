#include <iostream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <random>
#include <stdexcept>
#include <climits>
#include <chrono>
#include <fstream>
#include <cassert>

#include "benchmark.hpp"

#define NARGS 12

int main(int argc, char **argv) {
    benchmark::BenchmarkOptions benchmark_options;

    //if (argc < NARGS) benchmark::Usage();
    benchmark::ProcessBenchmarkOptions(benchmark_options, argv, argc);

    try {
        benchmark::Run(benchmark_options);
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

