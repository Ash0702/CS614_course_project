#!/bin/bash

echo "=== Flexclone: Dynamic File Growth & EOF Isolation Test ==="

cd /flexclone_mtpt
rm -f par log child friend

echo "--> 1. Initializing 10-block Parent and pinning Child..."
./binaries/make_file_size par 10
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:9:3

echo "-------------------------------------------------------"
echo "--> 2. Appending 5 New Blocks to Parent (Blocks 10-14)..."
echo "-------------------------------------------------------"
# Usage: ./Transactions_slow <file> <start_blk> <num_blks> <delay_sec> <tag>
./binaries/Transactions_slow par 10 5 0 "GROWTH_TXN"

echo "-------------------------------------------------------"
echo "--> 3. Verification: File Size and Data Isolation"
echo "-------------------------------------------------------"

echo ">> [PARENT STATE]: (Should show 15 blocks total)"
cat par
echo -e "\n"

echo ">> [CHILD STATE (Pre-Update)]: (Should strictly show only 10 blocks)"
echo "If isolation works, the cat command will stop at Block 9."
cat child
echo -e "\n"

echo "-------------------------------------------------------"
echo "--> 4. Committing Growth to Child..."
echo "-------------------------------------------------------"
./binaries/update_chilfs child

echo ">> [CHILD STATE (Post-Update)]: (Should now see all 15 blocks)"
cat child

echo "=== Dynamic Growth Test Complete ==="
