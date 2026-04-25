#!/bin/bash

echo "=== Starting Crash Recovery & Log Truncation Test ==="

cd /flexclone_mtpt

echo "--> Cleaning up existing test files..."
rm -f par log child friend

echo "--> 1. Initializing Files (5 blocks)..."
./binaries/make_file_size par 5

echo "--> Parent file..."
cat par
echo

./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:4:3

echo "--> 2. Executing Valid Transactions..."

echo "  [+] Transaction 1 (Version 1)"
./binaries/Transactions_test par 2
./binaries/update_chilfs child

echo "--> Child file..."
cat child
echo

echo "  [+] Transaction 2 (Version 2)"
./binaries/Transactions_test par 3
./binaries/update_chilfs child

echo "--> Child file..."
cat child
echo

echo "-------------------------------------------------------"
echo "--> 3. Valid State Before Crash"
echo "-------------------------------------------------------"
./binaries/log_reader log 0 6
echo

echo "--> 4. Simulating Hard Crash (Mounting as standard ext4 for offline sabotage)..."
cd /home/saksham/Port_Flexclone
sudo umount /flexclone_mtpt
sleep 1
sudo mount -t ext4 /dev/vda4 /flexclone_mtpt/

cd /flexclone_mtpt
# Injecting fake records for Version 3 behind the custom module's back
./binaries/append_fake_log_auto log 3 4 10 1
./binaries/append_fake_log_auto log 3 5 11 1

echo "-------------------------------------------------------"
echo "--> 5. Corrupted State Before Remount"
echo "(The log contains Version 3 data without a commit marker)"
echo "-------------------------------------------------------"
./binaries/log_reader log 0 12
echo

echo "--> 6. Simulating Kernel Reboot (Mounting back to ext4-module)..."
cd /home/saksham/Port_Flexclone
sudo umount /flexclone_mtpt
sleep 1
sudo mount -t ext4-module /dev/vda4 /flexclone_mtpt/

echo "-------------------------------------------------------"
echo "--> 7. Recovered State After Remount"
echo "(The filesystem should have automatically truncated the fake Version 3 records)"
echo "-------------------------------------------------------"
cd /flexclone_mtpt

echo "-------------------------------------------------------"
echo "--> 8. Final File Consistency Check"
echo "(Child file should safely read up to Version 2)"
echo "-------------------------------------------------------"
cat child

echo
./binaries/log_reader log 0 12
echo "=== Test Complete ==="
