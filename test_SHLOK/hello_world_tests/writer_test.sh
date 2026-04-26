#!/bin/bash

echo "=== Terminal 1: Setup & Multi-Transaction Writer ==="

cd /flexclone_mtpt

echo "--> Cleaning up existing files..."
rm -f par log child friend

echo "--> Initializing Parent and Child (10 blocks)..."
./binaries/make_file_size par 10
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:9:3

echo "-------------------------------------------------------"
echo "Setup complete."
echo ">> GO TO TERMINAL 2 NOW AND RUN: ./reader_test.sh <<"
echo "-------------------------------------------------------"

read -p "Press [ENTER] to start Transaction 1 (Overwriting Blocks 0-4)..."
./binaries/Transactions_slow par 0 5 2 "TXN_1"

echo "-------------------------------------------------------"
echo "Transaction 1 Committed! Look at Terminal 2. The child still shows 'Init'."
read -p "Press [ENTER] to start Transaction 2 (Overwriting Blocks 5-9)..."
./binaries/Transactions_slow par 5 5 2 "TXN_2"

echo "-------------------------------------------------------"
echo "Transaction 2 Committed! Both writes are done."
echo "You can now press 'u' in Terminal 2 to sync the child."
