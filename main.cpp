#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <random>
#include <stdexcept>

#define NARGS 12

namespace benchmark {

using Permutation = std::vector<unsigned int>;

struct BenchmarkOptions {
    char method_name[256];
    unsigned int min_permutation_size;
    unsigned int max_permutation_size;
    unsigned int n_permutations;
    unsigned int delta_permutation_size;
    char dump_file[256];
};
 
struct BenchmarkParameters {
    std::string method_name;
    unsigned int permutation_size;
    unsigned int n_permutations;
    unsigned int min_performed_bops;
    unsigned int max_performed_bops;
    double avg_performed_bops;
    double avg_execution_time;
};

void Usage() {
    std::cerr << "usage: benchmark <options>" << std::endl;
    exit(-1);
}

unsigned int GenerateRandomNumber(unsigned int inf, unsigned int sup) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(inf, sup);

    return dist(rng);
}

Permutation FisherYatesShuffle(size_t permutation_size) {
    Permutation perm(permutation_size);

    std::iota(perm.begin(), perm.end(), 1);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(perm.begin(), perm.end(), rng);

    std::cout << "[debug] generated random permutation: ";
    std::copy(perm.begin(), perm.end(), std::ostream_iterator<unsigned int>(std::cout, " "));
    std::cout << std::endl;

    return perm;
}

std::vector<Permutation> GenerateRandomPermutations(unsigned int n, size_t permutation_size) {
    std::vector<Permutation> permutations(n);

    for (int i = 0; i < n; i++) 
        permutations.emplace_back(FisherYatesShuffle(permutation_size));

    return permutations;
}

void ProcessBenchmarkOptions(BenchmarkOptions& bench_options, char **argv, int argc) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--method") == 0) {
            strcpy(bench_options.method_name, argv[++i]);
        } else if (strcmp(argv[i], "--min-permutation-size") == 0) {
            bench_options.min_permutation_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--max-permutation-size") == 0) {
            bench_options.max_permutation_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--n-permutations") == 0) {
            bench_options.n_permutations = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--delta-permutation-size") == 0) {
            bench_options.delta_permutation_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--dump-file") == 0) {
            strcpy(bench_options.dump_file, argv[++i]);
        } else {
            std::cerr << "Invalid parameter " << argv[i] << std::endl;
        }
    }
}

void Run(BenchmarkOptions& bench_options) {
    unsigned int n_iterations = (bench_options.max_permutation_size - bench_options.min_permutation_size)
                              / bench_options.delta_permutation_size + 1;

    std::vector<BenchmarkParameters> resulting_parameters;

    std::cout << "Running benchmark for the " << bench_options.method_name << " method with options:" << std::endl;
    std::cout << "min_permutation_size: " << bench_options.min_permutation_size << std::endl;
    std::cout << "max_permutation_size: " << bench_options.max_permutation_size << std::endl;
    std::cout << "n_permutations: " << bench_options.n_permutations << std::endl;
    std::cout << "n_iterations: " << n_iterations << std::endl;
    std::cout << "delta_permutation_size: " << bench_options.delta_permutation_size << std::endl;
    std::cout << "dump_file: " << bench_options.dump_file << std::endl;

    std::vector<Permutation> perms = GenerateRandomPermutations(15, 6);
}

}

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

