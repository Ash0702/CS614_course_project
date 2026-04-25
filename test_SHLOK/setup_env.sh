#!/bin/bash

echo "=== Flexclone Environment Setup ==="

# 1. Prevent running without sudo, which causes silent failures
if [ "$EUID" -ne 0 ]; then
  echo "Error: Please run this script with sudo."
  exit 1
fi

# 2. Check if mounted, and unmount safely if it is
if grep -qs '/flexclone_mtpt' /proc/mounts; then
    echo "--> Unmounting existing /flexclone_mtpt..."
    umount /flexclone_mtpt
    # Small sleep to let the kernel flush buffers
    sleep 1 
fi

# 3. Check if module is loaded, and remove it safely
if lsmod | grep -q "ext4_module"; then
    echo "--> Removing old kernel module (rmmod)..."
    rmmod ext4_module
    sleep 1
fi

# 4. Insert the fresh module
echo "--> Inserting fresh ext4-module..."
cd /home/saksham/Port_Flexclone/fs/ext4-module
insmod ext4-module.ko 

# 5. Mount the device
echo "--> Mounting /dev/vda4 to /flexclone_mtpt..."
mount -t ext4-module /dev/vda4 /flexclone_mtpt/

echo 0 | sudo tee /sys/module/ext4_module/parameters/panic_point

echo "=== Setup Complete! ==="
echo "You can now run your test scripts now."
