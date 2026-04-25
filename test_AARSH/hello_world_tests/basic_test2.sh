#!/bin/bash

echo "=== Starting Multi-Version Isolation Test ==="

cd /flexclone_mtpt

echo "--> Cleaning up existing test files..."
rm -f par log child1 child2 child3 child4 friend1 friend2 friend3 friend4

echo "--> 1. Initializing Parent File (10 blocks)..."
./binaries/make_file_size par 10

echo "--> 2. Creating Child Dependencies (Base Version)..."
./binaries/setxattr_generic -p par -c child1 -f friend1 -l log -r 0:9:3
./binaries/setxattr_generic -p par -c child2 -f friend2 -l log -r 0:9:3
./binaries/setxattr_generic -p par -c child3 -f friend3 -l log -r 0:9:3
./binaries/setxattr_generic -p par -c child4 -f friend4 -l log -r 0:9:3

echo "--> 3. Executing Sequential Transactions and Updating Children..."

echo "  [+] Transaction 1: Modifying Block 1"
./binaries/write_offset par 4096 "---DATA_V1---"
./binaries/update_chilfs child1

echo "  [+] Transaction 2: Modifying Block 2"
./binaries/write_offset par 8192 "---DATA_V2---"
./binaries/update_chilfs child2

echo "  [+] Transaction 3: Modifying Block 3"
./binaries/write_offset par 12288 "---DATA_V3---"
./binaries/update_chilfs child3

echo "  [+] Transaction 4: Modifying Block 4"
./binaries/write_offset par 16384 "---DATA_V4---"
./binaries/update_chilfs child4

echo "-------------------------------------------------------"
echo "--> 4. Read Verification: See-Thru Isolation"
echo "-------------------------------------------------------"

echo ">> [child1 content (Version 1)]:"
cat child1
echo

echo ">> [child2 content (Version 2)]:"
cat child2
echo

echo ">> [child3 content (Version 3)]:"
cat child3
echo

echo ">> [child4 content (Version 4)]:"
cat child4
echo

echo ">> [par content (Current Parent State)]:"
cat par
echo

echo "-------------------------------------------------------"
echo "--> 5. Transaction Log Verification"
echo "-------------------------------------------------------"
# Reading the first 4 records from the binary log
./binaries/log_reader log 0 4

echo "=== Test Complete ==="
