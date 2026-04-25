#!/bin/bash

echo "=== Transaction Phase Overhead Comparison ==="

SIZES=(1 10 50 100 500 1024)
ITERATIONS=5
CSV_FILE="microbench_compare_results.csv"

# Added "Mode" to the CSV header
echo "Mode,Size_MB,Begin_us,Write_us,End_us,Fsync_us" > $CSV_FILE

drop_caches() {
    sudo sync
    sudo bash -c 'echo 3 > /proc/sys/vm/drop_caches'
}

# ==========================================
# PHASE 1: NATIVE EXT4 BASELINE
# ==========================================
echo "--> [Phase 1] Gathering Native Ext4 Baseline..."
cd /home/saksham/Port_Flexclone
sudo umount /flexclone_mtpt 2>/dev/null
sudo mount -t ext4 /dev/vda4 /flexclone_mtpt/

cd /flexclone_mtpt
for size in "${SIZES[@]}"; do
    echo "  Profiling Native Ext4 - ${size}MB ($ITERATIONS runs)..."
    for ((i=1; i<=ITERATIONS; i++)); do
        drop_caches
        # use_txn flag = 0
        RESULT=$(./binaries/bench_txn_micro par_ext4 $size 0)
        echo "Native_Ext4,$RESULT" >> /home/saksham/final_testing/$CSV_FILE
    done
    rm -f par_ext4
done

# ==========================================
# PHASE 2: FLEXCLONE (EXT4-MODULE)
# ==========================================
echo "--> [Phase 2] Gathering Flexclone Timings..."
cd /home/saksham/Port_Flexclone
sudo umount /flexclone_mtpt 2>/dev/null
sudo mount -t ext4-module /dev/vda4 /flexclone_mtpt/

cd /flexclone_mtpt
for size in "${SIZES[@]}"; do
    echo "  Profiling Flexclone Txn - ${size}MB ($ITERATIONS runs)..."
    for ((i=1; i<=ITERATIONS; i++)); do
        drop_caches
        
        ./binaries/make_file_size par_flex 1 > /dev/null 2>&1
        ./binaries/setxattr_generic -p par_flex -c child_flex -f friend_flex -l log_flex -r 0:0:3 > /dev/null 2>&1
        
        # use_txn flag = 1
        RESULT=$(./binaries/bench_txn_micro par_flex $size 1)
        echo "Flexclone,$RESULT" >> /home/saksham/final_testing/$CSV_FILE
        
        rm -f par_flex child_flex friend_flex log_flex log
    done
done

echo "=========================================="
echo "✅ Comparison Complete!"
echo "Data saved to: ~/final_testing/$CSV_FILE"
