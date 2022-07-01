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

#define NARGS 12

namespace benchmark {

using Permutation = std::vector<unsigned int>;

typedef unsigned int (*benchmark_method)(Permutation&, int, int);

namespace bubblesort {

    unsigned int sort(Permutation& vector, int index_0, int index) {
        unsigned int performed_bops = 0;
        bool swaped = true;

        /* for (size_t i = index; swaped && i >= index_0 + 1; i--) {
            swaped = false;
            for (size_t j = index_0; j <= i - 1; j++) {
                if (performed_bops++ && vector.at(j) > vector.at(j + 1)) {
                    std::swap(vector.at(j), vector.at(j + 1));
                    swaped = true;
                }
            }
        }

        return performed_bops;*/

        for (size_t i = 0; swaped && i < vector.size() - 1; i++) {
            swaped = false;
            for (size_t j = 0; j < vector.size() - i - 1; j++) {
                if (performed_bops++ && vector.at(j) > vector.at(j + 1)) {
                    std::swap(vector.at(j), vector.at(j + 1));
                    swaped = true;
                }
            }
        }
    
        return performed_bops;
    }

}

struct BenchmarkOptions {
    char method_name[256];
    benchmark_method method;
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

    #ifdef DEBUG
    std::cout << "[debug] generated random permutation: ";
    std::copy(perm.begin(), perm.end(), std::ostream_iterator<unsigned int>(std::cout, " "));
    std::cout << std::endl;
    #endif

    return perm;
}

std::vector<Permutation> GenerateRandomPermutations(unsigned int n, size_t permutation_size) {
    std::vector<Permutation> permutations{};

    #ifdef DEBUG
    std::cout << "[debug] generating " << n << " random permutations of size " << permutation_size << std::endl;
    #endif

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

BenchmarkParameters ComputeBenchmarkParameters(BenchmarkOptions& bench_options, unsigned int perm_size) {
    assert(perm_size > 0);

    BenchmarkParameters bench_parameters{};

    bench_parameters.permutation_size = perm_size;
    bench_parameters.min_performed_bops = INT_MAX;
    bench_parameters.max_performed_bops = 0;

    std::vector<Permutation> permutations = GenerateRandomPermutations(bench_options.n_permutations, perm_size);

    double iteration_execution_time = 0, total_execution_time = 0;
    unsigned int performed_bops = 0, total_performed_bops = 0;

    for (int i = 0; i < bench_options.n_permutations; i++) {
        auto initial_execution_time = std::chrono::high_resolution_clock::now();
        performed_bops = bench_options.method(permutations.at(i), 0, perm_size - 1);
        auto final_execution_time = std::chrono::high_resolution_clock::now();

        iteration_execution_time = 
            std::chrono::duration_cast<std::chrono::nanoseconds>(final_execution_time - initial_execution_time).count() * 1e-9;

        if (performed_bops < bench_parameters.min_performed_bops)
            bench_parameters.min_performed_bops = performed_bops;

        if (performed_bops > bench_parameters.max_performed_bops)
            bench_parameters.max_performed_bops = performed_bops;

        total_performed_bops += performed_bops;
        total_execution_time += iteration_execution_time;
    }

    bench_parameters.avg_performed_bops = total_performed_bops / static_cast<double>(bench_options.n_permutations);
    bench_parameters.avg_execution_time = total_execution_time / static_cast<double>(bench_options.n_permutations);

    std::cout << std::fixed << std::setprecision(9);
    std::cout << "perm_size: " << bench_parameters.permutation_size << ", "
              << "avg_execution_time (sec): " << bench_parameters.avg_execution_time << ", "
              << "avg_bops: " << std::setprecision(3) << bench_parameters.avg_performed_bops << ", "
              << "min_bops: " << bench_parameters.min_performed_bops << ", "
              << "max_bops: " << bench_parameters.max_performed_bops << std::endl;

    return bench_parameters;
}

void DumpBenchmarkParametersToFile(char *filename, std::vector<BenchmarkParameters> resulting_params) {
    std::ofstream dumpfile;
    dumpfile.open(filename);
    if (!dumpfile) {
        std::cerr << "error opening dump file" << std::endl;
        exit(-1);
    }

    for (auto bench_params : resulting_params) {
        dumpfile << bench_params.permutation_size << " "
                 << bench_params.avg_execution_time << " "
                 << bench_params.avg_performed_bops << " "
                 << bench_params.min_performed_bops << " "
                 << bench_params.max_performed_bops << std::endl;
    }

    dumpfile.close();
}

void Run(BenchmarkOptions& bench_options) {
    unsigned int n_iterations = (bench_options.max_permutation_size - bench_options.min_permutation_size)
                              / bench_options.delta_permutation_size + 1;

    std::vector<BenchmarkParameters> resulting_parameters;

    std::cout << "Running benchmark for <" << bench_options.method_name << "> method with options:" << std::endl;
    std::cout << "min_permutation_size: " << bench_options.min_permutation_size << std::endl;
    std::cout << "max_permutation_size: " << bench_options.max_permutation_size << std::endl;
    std::cout << "n_average_permutations: " << bench_options.n_permutations << std::endl;
    std::cout << "n_benchmark_iterations: " << n_iterations << std::endl;
    std::cout << "delta_permutation_size: " << bench_options.delta_permutation_size << std::endl;
    std::cout << "dump_file: " << bench_options.dump_file << std::endl;

    if (strcmp(bench_options.method_name, "bubblesort") == 0)
        bench_options.method = bubblesort::sort;

    std::cout << "Beginning benchmark" << std::endl;
    for (int i = 0; i < n_iterations; i++) {
        #ifdef DEBUG
        std::cout << "[debug] running benchmark iteration " << i << " (perm_size: "
                  << bench_options.min_permutation_size + bench_options.delta_permutation_size * i << ")" << std::endl;
        #endif

        resulting_parameters.push_back(ComputeBenchmarkParameters(bench_options, 
            bench_options.min_permutation_size + bench_options.delta_permutation_size * i));
    }

    DumpBenchmarkParametersToFile(bench_options.dump_file, resulting_parameters);
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

