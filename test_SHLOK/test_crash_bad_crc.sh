#!/bin/bash

# 1. Create the initial parent file
touch test_par_file

# 2. Perform initial writes at specific offsets
./write_offset test_par_file 0 1234
./write_offset test_par_file 4096 1234

# 3. Set extended attributes with the specified relationships
./setxattr_generic -p test_par_file -c test_child_file -f test_frnd_file -l test_log_file -r 0:1:3

# 4. Overwrite at offset 0
./write_offset test_par_file 0 abcd

# 5. Enable the crash trigger in the ext4 module
# Note: This requires sudo privileges
echo 1 | sudo tee /sys/module/ext4_module/parameters/scorw_crash_with_bad_crc

# 6. Final write to trigger the logic/crash
# Note: Using 'par_file_name' as requested in your snippet
./write_offset par_file_name 0 1234