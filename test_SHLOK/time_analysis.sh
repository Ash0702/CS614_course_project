#!/bin/bash

echo "=== Starting Test2 (Transaction Time Tests) ==="


cd /flexclone_mtpt

echo "--> Cleaning up old files..."
rm -f child friend log par


./binaries/make_file_size par 262144
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:262143:3
echo

echo "========================================="
echo "[*] Without Transaction Semantics - Timings"
echo "========================================="
./binaries/time_testing --avg-no-transaction par 100
echo

echo "--> Cleaning up old files..."
rm -f child friend log par

echo "--> Creating a 1 GB file..."
./binaries/make_file_size par 262144
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:262143:3
echo

echo "========================================="
echo "[*] With Transaction Semantics - Timings"
echo "========================================="
./binaries/time_testing --avg-simple par 100
echo

echo "--> Cleaning up old files..."
rm -f child friend log par

echo "========================================="
echo "[*] With Transaction Semantics - Timings"
echo "========================================="

./binaries/make_file_size par 262144
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:262143:3
echo
./binaries/time_testing --large par
echo    
rm -f child friend log par
echo 

./binaries/make_file_size par 262144
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:262143:3
echo
./binaries/time_testing --large par
echo    
rm -f child friend log par
echo 

./binaries/make_file_size par 262144
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:262143:3
echo
./binaries/time_testing --large par
echo    
rm -f child friend log par
echo 

./binaries/make_file_size par 262144
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:262143:3
echo
./binaries/time_testing --large par
echo    
rm -f child friend log par
echo 

./binaries/make_file_size par 262144
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:262143:3
echo
./binaries/time_testing --large par
echo    
rm -f child friend log par
echo 



echo "=== Test2 Complete ==="

