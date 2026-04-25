#!/bin/bash

echo "=== Starting Basic Flexclone Versioning Test ==="

# Move to the mount point where the binaries and files live
cd /flexclone_mtpt

echo "--> Cleaning up old test files..."
rm -f par child friend log

echo "--> 1. Creating parent file (10 blocks) and cloning to child..."
./binaries/make_file_size par 10
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:9:3

echo "--> Initial State (Parent and Child should match):"
echo ">> [par content]:"
cat par
echo
echo ">> [child content]:"
cat child
echo
echo "---------------------------------------------------"

echo "--> 2. Modifying parent file..."
# Writing to offset 4096 (Block 1) and 8192 (Block 2)
./binaries/write_offset par 4096 "---FIRST_MODIFICATION---"
./binaries/write_offset par 8192 "---SECOND_MODIFICATION---"

echo "--> State after parent modifications:"
echo "(Child should STILL show original data due to version isolation)"
echo ">> [par content]:"
cat par
echo
echo ">> [child content]:"
cat child
echo
echo "---------------------------------------------------"

echo "--> 3. Committing/Updating child to parent's latest version..."
./binaries/update_chilfs child

echo "--> Final State:"
echo "(Child should NOW show the modified data)"
echo ">> [par content]:"
cat par
echo
echo ">> [child content]:"
cat child
echo

echo "=== Test Complete ==="
