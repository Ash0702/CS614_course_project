#!/bin/bash

echo "=== Panic Testing: Phase 1 (Setup & Crash) ==="

cd /flexclone_mtpt

echo "--> Cleaning up old test files..."
rm -f par child friend log

echo "--> 1. Creating Parent (10 blocks) and Initializing..."
./binaries/make_file_size par 10
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:9:3

echo "--> 2. Executing Valid Transactions (Building a baseline)..."
echo "  [+] Transaction 1 (Overwriting Blocks 0-1)"
./binaries/Transactions_test par 2
./binaries/update_chilfs child

echo "  [+] Transaction 2 (Overwriting Blocks 2-3)"
./binaries/Transactions_test par 2
./binaries/update_chilfs child

echo "-------------------------------------------------------"
echo "--> 3. ⚠️ CRITICAL STEP: Syncing Page Cache to Disk ⚠️"
echo "Forcing the kernel to commit the baseline state before we crash it."
sync
sleep 5 
echo "-------------------------------------------------------"

echo "--> 4. Arming the Kernel Panic..."
echo 1 | sudo tee /sys/module/ext4_module/parameters/panic_point

echo "--> 5. Executing Doomed Transaction (This should crash the system)..."
echo "Running Transaction 3... Expecting sudden connection loss/reboot."

# This transaction should hit the panic point and kill the OS
./binaries/Transactions_test par 3

echo "❌ If you see this message, the panic failed to trigger!"
