#!/bin/bash

echo "=== Edge Case Testing on FlexClone Files ==="

# Move to the mount point where the binaries and files live
cd /flexclone_mtpt

echo "--> Cleaning up old test files..."
rm -f par child friend log

echo "--> Creating parent file (10 blocks) and cloning to child..."
./binaries/make_file_size par 10
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:9:3

echo "--> Testing: Calling only START_TRANSACTION on parent file..."
# Assuming test_edge_cases is compiled and available in the binaries folder
./binaries/edge_case_testing --only-start par
echo

echo "--> Testing: Calling only END_TRANSACTION on parent file..."
./binaries/edge_case_testing --only-end par
echo

echo "--> Testing: Calling multiple-starts on parent file..."
./binaries/edge_case_testing --multiple-starts par
echo

echo "--> Testing: Calling concurrent-starts on parent file..."
./binaries/edge_case_testing --concurrent-starts par
echo

echo "--> Cleaning up test files..."
rm -f par child friend log

echo "=== Tests Complete ==="

