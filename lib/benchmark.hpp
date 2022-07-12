#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <string>
#include <vector>

using Permutation = std::vector<unsigned int>;

typedef unsigned int (*benchmark_method)(Permutation&, int, int);

namespace benchmark {

struct BenchmarkOptions {
    char method_name[256];
    benchmark_method method;
    unsigned int min_permutation_size;
    unsigned int max_permutation_size;
    unsigned int n_avg_perms;
    unsigned int delta_permutation_size;
    char dump_file[256];
};
 
struct BenchmarkResult {
    std::string method_name;
    unsigned int permutation_size;
    unsigned int n_permutations;
    unsigned int min_performed_bops;
    unsigned int max_performed_bops;
    double avg_performed_bops;
    double avg_execution_time;
};

void Usage();

void ProcessBenchmarkOptions(BenchmarkOptions& bench_options, char **argv, int argc);

BenchmarkResult ComputeBenchmarkResults(BenchmarkOptions& bench_options, size_t perm_size);

void Run(BenchmarkOptions& options);

void DumpBenchmarkResultsToFile(const char *filename, std::vector<BenchmarkResult>& results);

}

#endif 
