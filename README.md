# Benchmarks for classical sorting and search algorithms

This tool performs benchmarks for some classical sorting and search algorithms taught in almost every 
algorithm analysis course. These benchmarks aim to empirically prove/demostrate the computational
complexity of those algorithms.

---

## Overview

For the empirical analysis and computational complexity estimation of the algorithms, the benchmarks 
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
$ ./benchmark --method <name> --min-permutation-size <int> --max-permutation-size <int> --n-permutations <int>
--delta-permutation-size <int> --dump-file <filename>
```

---

## License

The MIT License. See [LICENSE](LICENSE) for more information.
