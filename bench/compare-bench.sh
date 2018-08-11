#!/bin/bash

# Get the directory of this script so that paths are correct wherever it's run from.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null && pwd )"

COMPARE_BENCH_PY=/opt/google-benchmark/benchmark/tools/compare_bench.py

if [ ! -e $COMPARE_BENCH_PY ]; then
    echo "Script [compare_bench.py] not found, is google-benchmark installed?"
fi

python3.7 $COMPARE_BENCH_PY $SCRIPT_DIR/eup_benchmark_results.json $SCRIPT_DIR/eup_benchmark_results_dev.json
