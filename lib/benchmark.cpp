#include "benchmark.hpp"
#include "util.hpp"
#include "methods/bubblesort.hpp"

#include <iostream>
#include <iomanip>
#include <climits>
#include <cstring>
#include <chrono>
#include <fstream>

namespace benchmark {

void Usage() {
    std::cerr << "usage: benchmark <options>" << std::endl;
    exit(-1);
}

void ProcessBenchmarkOptions(BenchmarkOptions& bench_options, char **argv, int argc) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--method") == 0) {
            strcpy(bench_options.method_name, argv[++i]);
        } else if (strcmp(argv[i], "--min-permutation-size") == 0) {
            bench_options.min_permutation_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--max-permutation-size") == 0) {
            bench_options.max_permutation_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--n-avg-perms") == 0) {
            bench_options.n_avg_perms = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--delta") == 0) {
            bench_options.delta_permutation_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--dump-file") == 0) {
            strcpy(bench_options.dump_file, argv[++i]);
        } else {
            std::cerr << "Invalid parameter " << argv[i] << std::endl;
        }
    }
}

/* 'ComputeBenchmarkResults' - Computes the average clock execution time, average,
 * minimum and maximum basic operations the chosen method performs over a random
 * permutation of size `perm_size`. The `n_avg_perms` benchmark options determines
 * the number of permutations of size `perm_size` used to compute the average values.
 */
BenchmarkResult ComputeBenchmarkResults(BenchmarkOptions& bench_options, size_t perm_size) {
    BenchmarkResult bench_results{};

    bench_results.permutation_size = perm_size;
    bench_results.min_performed_bops = INT_MAX;
    bench_results.max_performed_bops = 0;

    std::vector<Permutation> permutations = util::GenerateNRandomPermutations(bench_options.n_avg_perms, perm_size);

    double iteration_execution_time = 0, total_execution_time = 0;
    unsigned int iteration_performed_bops = 0, total_performed_bops = 0;

    for (unsigned int i = 0; i < bench_options.n_avg_perms; i++) {
        auto initial_execution_time = std::chrono::high_resolution_clock::now();
        iteration_performed_bops = bench_options.method(permutations.at(i), 0, perm_size - 1);
        auto final_execution_time = std::chrono::high_resolution_clock::now();

        util::print_progress(i + 1, bench_options.n_avg_perms, 70, perm_size);

        iteration_execution_time = 
            std::chrono::duration_cast<std::chrono::nanoseconds>(final_execution_time 
            - initial_execution_time).count() * 1e-9;

        if (iteration_performed_bops < bench_results.min_performed_bops)
            bench_results.min_performed_bops = iteration_performed_bops;

        if (iteration_performed_bops > bench_results.max_performed_bops)
            bench_results.max_performed_bops = iteration_performed_bops;

        total_performed_bops += iteration_performed_bops;
        total_execution_time += iteration_execution_time;
    }

    bench_results.avg_performed_bops = total_performed_bops / static_cast<double>(bench_options.n_avg_perms);
    bench_results.avg_execution_time = total_execution_time / static_cast<double>(bench_options.n_avg_perms);

    std::cout << std::fixed << std::setprecision(9)
              << "avg_execution_time (sec): " << bench_results.avg_execution_time << ", "
              << std::setprecision(2) 
              << "avg_bops: " << bench_results.avg_performed_bops << ", "
              << "min_bops: " << bench_results.min_performed_bops << ", "
              << "max_bops: " << bench_results.max_performed_bops << '\n';

    return bench_results;
}

void Run(BenchmarkOptions& options) {
    unsigned int n_iterations = (options.max_permutation_size - options.min_permutation_size)
                              / options.delta_permutation_size + 1;

    std::vector<BenchmarkResult> bench_results;

    std::cout << "Running benchmark for <" << options.method_name << "> method with options:" << '\n'
              << "min_permutation_size: " << options.min_permutation_size << '\n'
              << "max_permutation_size: " << options.max_permutation_size << '\n'
              << "n_average_permutations: " << options.n_avg_perms << '\n'
              << "n_iterations: " << n_iterations << '\n'
              << "delta_permutation_size: " << options.delta_permutation_size << '\n'
              << "dump_file: " << options.dump_file << '\n';

    if (strcmp(options.method_name, "bubblesort") == 0)
        options.method = methods::bubblesort::sort;

    std::cout << "Beginning benchmark" << '\n';

    auto initial_benchmark_time = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < n_iterations; i++) {
        unsigned int iter_batch_size = options.min_permutation_size + options.delta_permutation_size * i;
        #ifdef DEBUG
        std::cout << "running benchmark iteration " << i << "(perm_size: " << iter_batch_size << ")\n"
        #endif

        bench_results.push_back(ComputeBenchmarkResults(options, iter_batch_size));
    }

    auto end_benchmark_time = std::chrono::high_resolution_clock::now();

    double total_benchmark_execution_time = 
        std::chrono::duration_cast<std::chrono::nanoseconds>(end_benchmark_time -
        initial_benchmark_time).count() * 1e-9;

    std::cout << "benchmark finished in " << total_benchmark_execution_time << " seconds\n";

    DumpBenchmarkResultsToFile(options.dump_file, bench_results);
    std::cout << "benchmark results dumped to file " << options.dump_file << '\n';
}

void DumpBenchmarkResultsToFile(const char *filename, std::vector<BenchmarkResult>& results) {
    std::ofstream dumpfile;
    dumpfile.open(filename);
    if (!dumpfile) {
        std::cerr << "error opening dump file\n";
        exit(-1);
    }

    for (auto bench_result : results) {
        dumpfile << bench_result.permutation_size << " "
                 << bench_result.avg_execution_time << " "
                 << bench_result.min_performed_bops << " "
                 << bench_result.max_performed_bops << '\n';
    }

    dumpfile.close();
}

} // namespace benchmark
