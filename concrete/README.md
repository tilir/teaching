# Concrete mathematics [split-out version]

This subrepo originates from "concrete" repo of the same github.

It is snapshot, old and not fully functional.

Only purpose is to illustrate how cmake works

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build
env CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test
```
