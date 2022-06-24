#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <random>
#include <stdexcept>
#include <climits>
#include <chrono>

#define NARGS 12

namespace benchmark {

using Permutation = std::vector<unsigned int>;

typedef unsigned int (*benchmark_method)(Permutation, int, int);

// test algorithm
namespace mergesort {
    int merge(Permutation permutation, int index_0, int index, int index_middle) {
        Permutation aux(index + 1);

        for (int i = 0; i < index + 1; i++) aux.push_back(0);

        unsigned int performed_bops = 0;

        unsigned int i = index_0;
        unsigned int j = index_middle + 1;
        unsigned int k = index;

        while (i <= index_middle && j <= index) {
            if (permutation[i] < permutation[j]) aux[k++] = permutation[i++];
            else aux[k++] = permutation[j++];
            performed_bops++;
        }

        if (i > index_middle) while (j <= index) aux[k++] = permutation[j++];
        else if (j > index) while (i <= index_middle) aux[k++] = permutation[i++];

        for (i = index_0; i <= index; i++) permutation[i] = aux[i];

        return performed_bops;
    }

    unsigned int sort(Permutation permutation, int index_0, int index) {
        if (index_0 == index) return 0;

        int index_middle = (index + index_0) / 2;

        int performed_bops = 0;
        performed_bops += sort(permutation, index_0, index_middle);
        performed_bops += sort(permutation, index_middle + 1, index);
        performed_bops += merge(permutation, index_0, index, index_middle);

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

BenchmarkParameters ComputeBenchmarkParameters(BenchmarkOptions& bench_options, unsigned int perm_size) {
    BenchmarkParameters bench_parameters{};

    bench_parameters.permutation_size = perm_size;
    bench_parameters.min_performed_bops = INT_MAX;
    bench_parameters.max_performed_bops = 0;

    std::vector<Permutation> permutations = GenerateRandomPermutations(bench_options.n_permutations, perm_size);

    double iteration_execution_time = 0;
    unsigned int performed_bops = 0, total_performed_bops = 0, total_execution_time = 0;

    for (int i = 0; i < bench_options.n_permutations; i++) {
        auto initial_execution_time = std::chrono::high_resolution_clock::now();
        // performed_bops = bench_options.method(permutations[i], 0, perm_size - 1);
        performed_bops = 30;
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

    std::cout << "perm_size: " << bench_parameters.permutation_size << ", "
              << "avg_execution_time: " << bench_parameters.avg_execution_time << ", "
              << "avg_bops: " << bench_parameters.avg_performed_bops << ", "
              << "min_bops: " << bench_parameters.min_performed_bops << ", "
              << "max_bops: " << bench_parameters.max_performed_bops << std::endl;

    return bench_parameters;
}

void Run(BenchmarkOptions& bench_options) {
    unsigned int n_iterations = (bench_options.max_permutation_size - bench_options.min_permutation_size)
                              / bench_options.delta_permutation_size + 1;

    std::vector<BenchmarkParameters> resulting_parameters;

    std::cout << "Running benchmark for <" << bench_options.method_name << "> method with options:" << std::endl;
    std::cout << "min_permutation_size: " << bench_options.min_permutation_size << std::endl;
    std::cout << "max_permutation_size: " << bench_options.max_permutation_size << std::endl;
    std::cout << "n_permutations: " << bench_options.n_permutations << std::endl;
    std::cout << "n_iterations: " << n_iterations << std::endl;
    std::cout << "delta_permutation_size: " << bench_options.delta_permutation_size << std::endl;
    std::cout << "dump_file: " << bench_options.dump_file << std::endl;

    if (strcmp(bench_options.method_name, "mergesort") == 0)
        bench_options.method = mergesort::sort;

    std::cout << "Beginning benchmark" << std::endl;
    for (int i = 0; i < n_iterations; i++)
        resulting_parameters.push_back(ComputeBenchmarkParameters(bench_options, 
            bench_options.min_permutation_size + bench_options.delta_permutation_size * i));
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

