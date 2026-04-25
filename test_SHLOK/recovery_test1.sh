#!/bin/bash

# Define terminal colors
GREEN='\033[1;32m'
BLUE='\033[1;34m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
MAGENTA='\033[1;35m'
NC='\033[0m' # No Color

echo -e "${BLUE}=======================================================${NC}"
echo -e "${GREEN}=== Starting Test 1: Advanced Offline Modifications ===${NC}"
echo -e "${BLUE}=======================================================${NC}\n"

echo -e "${YELLOW}--> 1. Setting up files and testing initial read (ext4-module)...${NC}"
cd /flexclone_mtpt
rm -f child friend log par

./binaries/make_file_size par 10
./binaries/setxattr_generic -p par -c child -f friend -l log -r 0:9:3

echo -e "\n${MAGENTA}  [+] Running a Valid Baseline Transaction...${NC}"
./binaries/Transactions_test par 2 > /dev/null
./binaries/update_chilfs child > /dev/null

echo -e "\n${CYAN}>> [par content (Baseline)]:${NC}"
cat par 

echo -e "\n${CYAN}>> [child content (Baseline)]:${NC}"
cat child 
echo

echo -e "${YELLOW}--> 2. Remounting as standard native ext4...${NC}"
cd /home/saksham/Port_Flexclone
sudo umount /flexclone_mtpt
sudo mount -t ext4 /dev/vda4 /flexclone_mtpt/

echo -e "${YELLOW}--> 3. Executing Offline Appends (Bypassing transaction log)...${NC}"
cd /flexclone_mtpt

echo -e "${MAGENTA}  [+] Type A: Standard Block Append (Block 10)${NC}"
./binaries/write_offset par 49512 "---Standard_Offline_Append---"

echo -e "${MAGENTA}  [+] Type B: Unaligned Byte Append (Offset 40995)${NC}"
./binaries/write_offset par 49560 "_Unaligned_Injection_"

echo -e "${MAGENTA}  [+] Type C: Sparse Hole Append (Block 20)${NC}"
./binaries/write_offset par 81920 "---Sparse_Offline_Append---"

echo -e "\n${CYAN}>> [par content (After Offline Appends)]:${NC}"
cat par 
echo

echo -e "\n${CYAN}>> [Check Parent size]:${NC}"
stat par 
echo


echo -e "${YELLOW}--> 4. Remounting back to ext4-module...${NC}"
cd /home/saksham/Port_Flexclone
sudo umount /flexclone_mtpt
sudo mount -t ext4-module /dev/vda4 /flexclone_mtpt/
cd /flexclone_mtpt


echo -e "\n${CYAN}>> [par content (Final State)]:${NC}"
cat par 
echo -e "\n${CYAN}>> [child content (Final State)]:${NC}"
cat child 
echo


echo -e "\n${CYAN}>> [Check Parent size(should be truncated)]:${NC}"
stat par 
echo

echo -e "${YELLOW}--> 5. Post-Sabotage Transaction Test...${NC}"
echo -e "${MAGENTA}  [+] Running a new transaction to test module recovery...${NC}"
./binaries/Transactions_test par 3 > /dev/null
./binaries/update_chilfs child > /dev/null

echo -e "\n${CYAN}>> [par content (Final State)]:${NC}"
cat par 
echo -e "\n${CYAN}>> [child content (Final State)]:${NC}"
cat child 
echo

echo -e "${BLUE}=======================================================${NC}"
echo -e "${GREEN}=== Test 1 Complete ===${NC}"
echo -e "${BLUE}=======================================================${NC}"
