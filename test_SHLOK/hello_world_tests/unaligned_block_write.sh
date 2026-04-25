#!/bin/bash

echo "=== Edge Case 1: Unaligned Sub-Block Write ==="

cd /flexclone_mtpt
rm -f par log child friend

echo "--> 1. Creating Parent (1 block) and Child..."
./binaries/make_file_size par 1
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:0:3

echo ">> [Initial Parent State]:"
cat par

echo "-------------------------------------------------------"
echo "--> 2. Writing 10 bytes directly into the middle of Block 0 (Offset 2)..."
# Writing at byte 2, intentionally messing up the "Init_0" string
./binaries/write_offset par 2 "---INJECT---"

echo "-------------------------------------------------------"
echo ">> [Modified Parent State]:"
echo "(If successful, the new string is smoothly merged with the old block's data)"
cat par
echo

echo ">> [Child State (Should remain completely untouched)]:"
cat child

echo "-------------------------------------------------------"
echo "--> 3. Updating Child to new version..."
./binaries/update_chilfs child

echo ">> [Child State Post-Update]:"
cat child
echo "=== Sub-Block Test Complete ==="
