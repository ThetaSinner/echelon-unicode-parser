#!/bin/bash

# Get the directory of this script so that paths are correct wherever it's run from.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null && pwd )"

BENCHMARK_EXE="$SCRIPT_DIR/../build/exe/eup_benchmark/release/eup_benchmark"

if [ ! -e $BENCHMARK_EXE ]; then
    echo "Executable [eup_benchmark] does not exist, please build it first"
    exit 1
fi

$BENCHMARK_EXE --benchmark_format=json > $SCRIPT_DIR/eup_benchmark_results_dev.json
