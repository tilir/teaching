# Concrete mathematics

Different computability and combinatorics experiments and visualizations. Pure fun.

## Building and testing repo

Release build

```
cmake -G Ninja -B build -DCMAKE_CXX_COMPILER=g++-12 -DCMAKE_BUILD_TYPE=Release .
cmake --build build
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```

Debug build

```
cmake -G Ninja -B debug -DCMAKE_CXX_COMPILER=g++-12 -DCMAKE_BUILD_TYPE=Debug .
cmake --build debug
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build debug --target test
```

## Applications

Most of applications composes to each other nicely.

```
$ ./build/apps/randperm -seed=1
10
10 1 3 6 8 5 7 4 2 9
```

Then say we want to check if it is Baxter's

```
$ ./build/apps/randperm -seed=1 | ./build/apps/is-baxters
0
```

Some apps have batch mode. In batch mode size prepends series of entities:

Try:

```
$ echo 4 | ./build/apps/allperms | ./build/apps/is-baxters -batch -combine
```

Every app have option -help.

Below all apllications described briefly

### Brace generators
- allbraces -- spills all properly braced expressions
- randbraces -- output random correctly braced expression

### Permutation related
- allperms -- spills all permutations of 1..N
- invperm -- outputs reversed permutation
- is-baxters -- says if permutation is baxters or not
- permloops -- input permutation output loop format
- randperm -- output random permutation

### Combinations related
- allsubsets -- spills all subsets of 0..(N-1) or with any labels

### Trees related
- readbst -- application reads permutation or brace expression and outputs a tree
- readtwin -- application reads permutation and outputs twin tree or equivalent baxters
