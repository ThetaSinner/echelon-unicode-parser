#!/bin/bash

# Get the directory of this script so that paths are correct wherever it's run from.
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null && pwd )"

if [ ! -e $SCRIPT_DIR/eup_benchmark_results_dev.json ]; then
    echo "Dev results file [eup_benchmark_results_dev.json] does not exist, cannot save updated results."
    echo 1
fi

mv $SCRIPT_DIR/eup_benchmark_results_dev.json $SCRIPT_DIR/eup_benchmark_results.json
