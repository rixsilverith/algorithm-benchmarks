[![License](https://img.shields.io/github/license/rixsilverith/algorithm-benchmarks)](https://mit-license.org/)
![Cpp Version](https://img.shields.io/badge/C%2B%2B-17-blue)

# Benchmarks for classical sorting and search algorithms

This tool performs benchmarks for some classical sorting and search algorithms taught in almost every 
algorithm analysis course. These benchmarks aim to empirically prove/demostrate the computational
complexity of those algorithms.

---

## Overview

For the empirical analysis and computational time complexity estimation of the algorithms, the benchmarks 
keep track of the number of times the basic operation (BO) of the corresponding algorithm is performed
over the given input.

The benchmarks generate a data file containing the performance measuraments of the chosen algorithm for the
provided input vector, as well as another file with the actual output of the benchmark. More precisely,
the benchmark computes the minimum, maximum and average number of performed basic operations, and the total
clock execution time of the algorithm over the given input.

---

## Usage

The benchmarking tool can be used as

```bash
$ ./benchmark <options>
```
The following is a list of the available options in the tool.

**Option** | **Description**
--- | ---
`--method <name>` | Specifies the method to be benchmarked.
`--min-permutation-size <integer>` | Minimum permutation size to be tested.
`--max-permutation-size <integer>` | Maximum permutation size to be tested.
`--n-permutations <integer>` | Number of permutations used to compute the average benchmark parameters for a fixed permutation size.
`--delta-permutation-size <integer>` | Increment in the size of the permutations in each iteration.
`--dump-file <filepath>` | File to dump the benchmark data to be analised.

---

## List of supported algorithms

The following is a list of the supported algorithms with their corresponding computational
time complexity. For the sake of completeness, the space complexity has also been included
to distinguise in-place algorithms from those that need auxiliary memory.

### Local sorting algorithms

**Algorithm** | **Benchmark name** | **Worst time complexity** | **Average time complexity** | **Best time complexity** | **Space complexity**
--- | --- | --- | --- | --- | ---
Bubblesort (naive) | `naive-bubblesort` | $\mathcal{O}(n^2)$ | $\mathcal{O}(n^2)$ | $\mathcal{O}(n^2)$ | $\mathcal{O}(1)$
Bubblesort | `bubblesort` | $\mathcal{O}(n^2)$ | $\mathcal{O}(n^2)$ | $\mathcal{O}(n)$ | $\mathcal{O}(1)$

## License

The MIT License. See [LICENSE](LICENSE) for more information.
