[![License](https://img.shields.io/github/license/rixsilverith/algorithm-benchmarks)](https://mit-license.org/)

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

## License

The MIT License. See [LICENSE](LICENSE) for more information.
