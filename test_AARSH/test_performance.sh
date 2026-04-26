#!/bin/bash

echo "=== Flexclone Performance Benchmarking ==="

SIZES=(1 10 50 100 500)
ITERATIONS=5
CSV_FILE="performance_results.csv"

echo "Mode,Size_MB,Time_s,Throughput_MBps" > $CSV_FILE

drop_caches() {
    sudo sync
    sudo bash -c 'echo 3 > /proc/sys/vm/drop_caches'
}

# ==========================================
# TEST PHASE 1: NATIVE EXT4 BASELINE
# ==========================================
echo "--> [Phase 1] Mounting Native Ext4 for Baseline..."
cd /home/saksham/Port_Flexclone
sudo umount /flexclone_mtpt 2>/dev/null
sudo mount -t ext4 /dev/vda4 /flexclone_mtpt/

cd /flexclone_mtpt
for size in "${SIZES[@]}"; do
    echo "  Testing Native Ext4 - ${size}MB ($ITERATIONS iterations)..."
    for ((i=1; i<=ITERATIONS; i++)); do
        drop_caches
        RESULT=$(./binaries/bench_write par_ext4 $size 0)
        echo "Native_Ext4,$RESULT" >> /home/saksham/final_testing/$CSV_FILE
    done
    rm -f par_ext4
done

# ==========================================
# TEST PHASE 2: FLEXCLONE (EXT4-MODULE)
# ==========================================
echo "--> [Phase 2] Mounting Flexclone (ext4-module)..."
cd /home/saksham/Port_Flexclone
sudo umount /flexclone_mtpt 2>/dev/null
sudo mount -t ext4-module /dev/vda4 /flexclone_mtpt/

cd /flexclone_mtpt
for size in "${SIZES[@]}"; do
    echo "  Testing Flexclone Txn - ${size}MB ($ITERATIONS iterations)..."
    for ((i=1; i<=ITERATIONS; i++)); do
        drop_caches
        
        ./binaries/make_file_size par_flex 1 > /dev/null 2>&1
        ./binaries/setxattr_generic -p par_flex -c child_flex -f friend_flex -l log_flex -r 0:0:3 > /dev/null 2>&1
        
        RESULT=$(./binaries/bench_write par_flex $size 1)
        echo "Flexclone,$RESULT" >> /home/saksham/final_testing/$CSV_FILE
        
        rm -f par_flex child_flex friend_flex log_flex log
    done
done

echo "=========================================="
echo "✅ Benchmarking Complete!"
echo "Raw results saved to: ~/final_testing/$CSV_FILE"
