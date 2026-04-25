#!/bin/bash

# 1. Read the contents of the parent file to stdout
cat test_par_file

# 2. Execute the log reader utility
# Standard usage: ./log_reader <file> <offset> <length/type>
if [ -f "test_log_file" ]; then
    ./log_reader test_log_file 0 3
else
    echo "Error: test_log_file not found."
    exit 1
fi