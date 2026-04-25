#!/bin/bash

# Stop the script immediately if any command fails
set -e 

echo "=== Panic Testing: Phase 2 (Recovery Verification) ==="

# 1. Clear any OS auto-mounts that happened during reboot
echo "--> Ensuring device is free..."
sudo umount /dev/vda4 2>/dev/null || true
sudo umount /flexclone_mtpt 2>/dev/null || true

# 2. Reload the kernel module (RAM was wiped during panic)
echo "--> Loading ext4-module..."
cd /home/saksham/Port_Flexclone
# Remove it first just in case it's in a weird state
sudo rmmod ext4_module 2>/dev/null || true 
sudo insmod fs/ext4-module/ext4-module.ko
# 3. Mount the filesystem
echo "--> Mounting the filesystem post-crash..."
sudo mount -t ext4-module /dev/vda4 /flexclone_mtpt/

cd /flexclone_mtpt

echo "-------------------------------------------------------"
echo "--> Reading Parent File (Should be recovered/consistent)"
echo "-------------------------------------------------------"
cat par
echo

echo "-------------------------------------------------------"
echo "--> Reading Child File (Should perfectly match the baseline)"
echo "-------------------------------------------------------"
cat child
echo

echo "-------------------------------------------------------"
echo "--> Dumping the Binary Log"
echo "-------------------------------------------------------"
# Turn off 'set -e' temporarily because log_reader hitting EOF might return a non-zero exit code
set +e 
./binaries/log_reader log 0 6
set -e

echo "=== Panic Verification Complete ==="
